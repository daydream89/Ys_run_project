#include "PlayerLayer.h"

#include "EventManager.h"
#include "SystemData.h"

USING_NS_CC;

namespace PlayerLayerPrivate
{
	bool CheckPlayerMovableLeft(Node *pPlayerSprite)
	{
		if (pPlayerSprite == nullptr)
			return false;

		Vec2 position = pPlayerSprite->getPosition();
		Vec2 contentSize = pPlayerSprite->getContentSize();
		if (position.x - (contentSize.x / 2) < 0)
			return false;

		return true;
	}

	bool CheckPlayerMovableRight(Node *pPlayerSprite)
	{
		if (pPlayerSprite == nullptr)
			return false;

		Size screenSize = Director::getInstance()->getVisibleSize();
		Vec2 position = pPlayerSprite->getPosition();
		Vec2 contentSize = pPlayerSprite->getContentSize();
		if (screenSize.width <= position.x + (contentSize.x / 2))
			return false;

		return true;
	}
}

bool PlayerLayer::init()
{
	if (EventManager::GetInstance())
	{
		m_listenerIdList[EVENT_TYPE_GAMEOVER] = EventManager::GetInstance()->AddEventListener(
			EVENT_TYPE_GAMEOVER, std::bind(&PlayerLayer::EventGameOver, this, std::placeholders::_1));

		m_listenerIdList[EVENT_TYPE_INPUT] = EventManager::GetInstance()->AddEventListener(
			EVENT_TYPE_INPUT, std::bind(&PlayerLayer::EventInput, this, std::placeholders::_1));

	}

	scheduleUpdate();

	return true;
}

void PlayerLayer::onEnter()
{
	// super
	Layer::onEnter();

	m_isPlayerMoving = false;
	m_isPlayerExist = false;

	m_curInputKey = INPUT_KEY_NONE;
}

void PlayerLayer::onExit()
{
	// super
	Layer::onExit();

	if (EventManager::GetInstance())
	{
		EventManager::GetInstance()->RemoveEventListener(EVENT_TYPE_GAMEOVER, m_listenerIdList[EVENT_TYPE_GAMEOVER]);
		EventManager::GetInstance()->RemoveEventListener(EVENT_TYPE_INPUT, m_listenerIdList[EVENT_TYPE_INPUT]);
	}
}

void PlayerLayer::update(float delta)
{
	delta;

	if(m_isPlayerExist)
		if (m_curInputKey != INPUT_KEY_NONE)
			MovePlayer(m_curInputKey);
}

Layer *PlayerLayer::CreateLayer()
{
	return PlayerLayer::create();
}

bool PlayerLayer::CreatePlayer()
{
	if (SystemData::GetInstance() == false)
		return false;

	std::wstring wcsPath = SystemData::GetInstance()->GetPlayerSpritePath();
	size_t destSize = 0;
	char pPlayerPath[MAX_STR_LEN];
	if (pPlayerPath == NULL)
		return false;
	wcstombs_s(&destSize, pPlayerPath, MAX_STR_LEN, wcsPath.c_str(), wcsPath.size());

	Sprite *pPlayerSprite = Sprite::create(pPlayerPath);
	if (pPlayerSprite == nullptr)
		return false;

	cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
	pPlayerSprite->setPosition(visibleSize.width/2, 40);
	
	unsigned int tag = SystemData::GetInstance()->GetPlayerLayerTag();
	pPlayerSprite->setTag(tag);
	this->addChild(pPlayerSprite);

	// set animation
	StartPlayerAnimation(wcsPath, pPlayerSprite);

	if (GameManager::GetInstance())
		GameManager::GetInstance()->RegisterPlayer(pPlayerSprite);

	m_isPlayerExist = true;

	return true;
}

void PlayerLayer::MovePlayer(eInputKey inputState)
{
	unsigned int tag = SystemData::GetInstance() ? SystemData::GetInstance()->GetPlayerLayerTag() : 0;
	Node *pPlayerSprite = this->getChildByTag(tag);
	if (pPlayerSprite == nullptr)
		return;

	if (inputState == INPUT_KEY_ARROW_LEFT)
	{
		if (PlayerLayerPrivate::CheckPlayerMovableLeft(pPlayerSprite) == false)
			return;

		Vec2 playerPos = pPlayerSprite->getPosition();
		Vec2 playerSpriteSize = pPlayerSprite->getContentSize();
		double playerSpeed = SystemData::GetInstance() ? SystemData::GetInstance()->GetPlayerSpeed() : 1;
		pPlayerSprite->setPosition(playerPos.x - playerSpeed, playerPos.y);
	}
	else if (inputState == INPUT_KEY_ARROW_RIGHT)
	{
		if (PlayerLayerPrivate::CheckPlayerMovableRight(pPlayerSprite) == false)
			return;

		Vec2 playerPos = pPlayerSprite->getPosition();
		Vec2 playerSpriteSize = pPlayerSprite->getContentSize();
		double playerSpeed = SystemData::GetInstance() ? SystemData::GetInstance()->GetPlayerSpeed() : 1;
		pPlayerSprite->setPosition(playerPos.x + playerSpeed, playerPos.y);
	}
}

void PlayerLayer::RemovePlayer()
{
	unsigned int tag = SystemData::GetInstance() ? SystemData::GetInstance()->GetPlayerLayerTag() : 0;
	removeChildByTag(tag);

	m_isPlayerExist = false;
}

bool PlayerLayer::StartPlayerAnimation(const std::wstring &idleName, Sprite *pPlayerSprite)
{
	Animation *pAnimation = Animation::create();
	if (pAnimation)
	{
		pAnimation->setDelayPerUnit(0.2f);
		int spriteFrame = SystemData::GetInstance()->GetSpriteAnimationFrame();

		// todo. tokenizing part. move public function. usable another class.
		std::wstring wcsAniPath = idleName;
		int splitPos = wcsAniPath.find(L'.');
		wcsAniPath = wcsAniPath.substr(0, splitPos);
		wcsAniPath.append(L"_");
		for (int i = 1; i <= spriteFrame; i++)
		{
			splitPos = wcsAniPath.find(L'_');
			wcsAniPath = wcsAniPath.substr(0, splitPos);
			wcsAniPath.append(L"_" + std::to_wstring(i) + L".png");
			size_t destSizeOfAni = 0;
			char pAniPath[MAX_STR_LEN];
			if (pAniPath == NULL)
				return false;
			wcstombs_s(&destSizeOfAni, pAniPath, MAX_STR_LEN, wcsAniPath.c_str(), wcsAniPath.size());

			pAnimation->addSpriteFrameWithFile(pAniPath);
		}

		Animate *pAnimate = Animate::create(pAnimation);
		if (pAnimate)
		{
			RepeatForever *pRepeat = RepeatForever::create(pAnimate);
			if (pRepeat)
				pPlayerSprite->runAction(pRepeat);
		}
	}

	return true;
}

void PlayerLayer::EventGameOver(EventSystemMsg msg)
{
	if (msg.type != EVENT_TYPE_GAMEOVER)
		return;

	RemovePlayer();
}

void PlayerLayer::EventInput(EventInputMsg msg)
{
	if (msg.type != EVENT_TYPE_INPUT)
		return;

	switch (msg.inputState)
	{
		case INPUT_KEY_ARROW_LEFT:
		case INPUT_KEY_ARROW_RIGHT:	
		{
			if(msg.isPressed)
				m_curInputKey = msg.inputState;
			else
			{
				if (msg.inputState == m_curInputKey)
					m_curInputKey = INPUT_KEY_NONE;
			}
		}
	}
}