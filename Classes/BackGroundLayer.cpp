#include "BackGroundLayer.h"

#include "EventManager.h"
#include "SystemData.h"

USING_NS_CC;

namespace BackGroundLayerPrivate
{
	// todo. same function exist in OpponentLayer. need combine.
	double CalculateMovingDuration(const Vec2 &startPos)
	{
		if (SystemData::GetInstance() == nullptr)
			return 0.f;

		unsigned int curStage = GameManager::GetInstance() ? GameManager::GetInstance()->GetCurStage() : 1;

		double velocity = SystemData::GetInstance()->GetScrollingVelocity();
		double velWeight = SystemData::GetInstance()->GetScrollingWeight();
		double finalVelocity = velocity + (velWeight * static_cast<double>(curStage));

		double endPosY = SystemData::GetInstance()->GetScrollingEndPosY();
		double distance = startPos.y - endPosY;

		return (distance / finalVelocity);
	}
}

Layer *BackGroundLayer::CreateLayer()
{
	return BackGroundLayer::create();
}

BackGroundLayer::BackGroundLayer()
{
	
}

BackGroundLayer::~BackGroundLayer()
{

}

bool BackGroundLayer::init()
{
	if (EventManager::GetInstance())
	{
		m_listenerIdList[EVENT_TYPE_NEXT_STAGE] = EventManager::GetInstance()->AddEventListener(
			EVENT_TYPE_NEXT_STAGE, std::bind(&BackGroundLayer::EventNextStage, this, std::placeholders::_1));
	}

	m_BackGroundIdGenerator = 0;

	return true;
}

void BackGroundLayer::onEnter()
{
	// super
	Layer::onEnter();

	if (SystemData::GetInstance() == false)
		return;

	CreateBackGround(BACKGROUND_OFFSET_BASIC);
	CreateBackGround(BACKGROUND_OFFSET_NEXT);
}

void BackGroundLayer::onExit()
{
	// super
	Layer::onExit();

	if (EventManager::GetInstance())
	{
		EventManager::GetInstance()->RemoveEventListener(EVENT_TYPE_NEXT_STAGE, m_listenerIdList[EVENT_TYPE_NEXT_STAGE]);
	}
}

void BackGroundLayer::CreateBackGround(int offset)
{
	std::wstring wcsPath = SystemData::GetInstance()->GetBackGroundSpritePath();
	char pBgPath[MAX_STR_LEN];
	size_t destSize = 0;
	wcstombs_s(&destSize, pBgPath, MAX_STR_LEN, wcsPath.c_str(), wcsPath.size());

	Sprite *pBackGround = Sprite::create(pBgPath);
	if (pBackGround)
	{
		Size spriteSize = pBackGround->getContentSize();
		Vec2 startPos;
		startPos.x = 0.f;
		if (offset == BACKGROUND_OFFSET_BASIC)
			startPos.y =  0.f;
		else if (offset == BACKGROUND_OFFSET_NEXT)
			startPos.y = spriteSize.height / 2;
		else
			startPos.y = 0.f;

		pBackGround->setAnchorPoint(Point(Point::ZERO));
		pBackGround->setPosition(startPos);
		pBackGround->setTag(m_BackGroundIdGenerator++);
		this->addChild(pBackGround);

		StartBackGroundMove(offset, startPos, pBackGround);
	}
}

void BackGroundLayer::RemoveBackGround(Node *pBackGroundSprite)
{
	if (pBackGroundSprite == nullptr)
		return;

	int opponentTag = pBackGroundSprite->getTag();

	removeChildByTag(opponentTag);
}

void BackGroundLayer::BackGroundMoveEndCallback(Node *pBackGroundSprite)
{
	if (pBackGroundSprite == nullptr)
		return;

	RemoveBackGround(pBackGroundSprite);

	CreateBackGround(BACKGROUND_OFFSET_NEXT);
}

void BackGroundLayer::StartBackGroundMove(int offset, const cocos2d::Vec2 startingPos, cocos2d::Sprite *pSprite)
{
	double duration = BackGroundLayerPrivate::CalculateMovingDuration(startingPos);
	MoveTo *pMove = MoveTo::create(duration, Vec2(0.f, SystemData::GetInstance()->GetScrollingEndPosY()));
	if (pMove)
	{
		CallFuncN *pCallFuncN = CallFuncN::create(CC_CALLBACK_1(BackGroundLayer::BackGroundMoveEndCallback, this));
		if (pCallFuncN)
		{
			Sequence *pSequence = Sequence::create(pMove, pCallFuncN, NULL);
			if (pSequence)
			{
				pSequence->setTag(ANIMATION_TAG_MOVE);
				pSprite->runAction(pSequence);
			}
		}
	}
}

void BackGroundLayer::Pause()		// todo. pause
{

}

void BackGroundLayer::ChangeSprite(cocos2d::__String path)		// todo. bg sprite change
{

}

void BackGroundLayer::ChangeSpeed()
{
	if (this->getActionByTag(ANIMATION_TAG_MOVE) != nullptr)
		this->stopActionByTag(ANIMATION_TAG_MOVE);

	for (int i = 2; 0 < 0; --i)
	{
		cocos2d::Sprite *pSprite = (cocos2d::Sprite *)getChildByTag(m_BackGroundIdGenerator - i);
		if (pSprite)
		{
			if (pSprite->getActionByTag(ANIMATION_TAG_MOVE) != nullptr)
				pSprite->stopActionByTag(ANIMATION_TAG_MOVE);

			StartBackGroundMove(BACKGROUND_OFFSET_MOVING, pSprite->getPosition(), pSprite);
		}
	}
}

void BackGroundLayer::EventNextStage(EventSystemMsg msg)
{
	if (msg.type != EVENT_TYPE_NEXT_STAGE)
		return;

	ChangeSpeed();
}