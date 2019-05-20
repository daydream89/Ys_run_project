#include "OpponentLayer.h"

#include "EventManager.h"
#include "SystemData.h"

USING_NS_CC;

namespace OpponentLayerPrivate
{
	int FindTagFromID(int id, eOpponentType type)
	{
		unsigned int tag = 0;
		if (type == OPPONENT_TYPE_ENEMY)
			tag = SystemData::GetInstance() ? SystemData::GetInstance()->GetEnemyStartTag() : 0;
		else if (type == OPPONENT_TYPE_WALL)
			tag = SystemData::GetInstance() ? SystemData::GetInstance()->GetWallStartTag() : 0;
		else
			return -1;

		return tag + static_cast<int>(id);;
	}

	int FindIDFromTag(int tag, eOpponentType type)
	{
		unsigned int startTag = 0;
		if (type == OPPONENT_TYPE_ENEMY)
			startTag = SystemData::GetInstance() ? SystemData::GetInstance()->GetEnemyStartTag() : 0;
		else if (type == OPPONENT_TYPE_WALL)
			startTag = SystemData::GetInstance() ? SystemData::GetInstance()->GetWallStartTag() : 0;
		else
			return -1;

		return tag - startTag;
	}

	bool CheckMoveEndPosition(cocos2d::Vec2 &startingPos, const cocos2d::Size &spriteSize, const cocos2d::Size &visibleSize)
	{
		// position check
		if (GameManager::GetInstance())
		{
			if (GameManager::GetInstance()->IsCreatablePos(startingPos, spriteSize.width, spriteSize.height) == false)
			{
				if ((visibleSize.width / 2) <= startingPos.x)
					startingPos.x = (visibleSize.width / 2) - (startingPos.x - (visibleSize.width / 2));
				else
					startingPos.x = (visibleSize.width / 2) + ((visibleSize.width / 2) - startingPos.x);

				if (GameManager::GetInstance()->IsCreatablePos(startingPos, spriteSize.width, spriteSize.height) == false)
					return false;
			}
		}

		return true;
	}

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

	std::string ConvertWideStringToString(const std::wstring &wcstr)
	{
		char pOpponentPath[MAX_STR_LEN];
		size_t destSize = 0;
		wcstombs_s(&destSize, pOpponentPath, MAX_STR_LEN, wcstr.c_str(), wcstr.size());

		std::string retStr = pOpponentPath;
		
		return retStr;
	}
}

OpponentLayer::OpponentLayer()
{
	
}

OpponentLayer::~OpponentLayer()
{

}

bool OpponentLayer::init()
{
	if (EventManager::GetInstance())
	{
		m_listenerIdList[EVENT_TYPE_COLLISION] = EventManager::GetInstance()->AddEventListener(
			EVENT_TYPE_COLLISION, std::bind(&OpponentLayer::EventCollision, this, std::placeholders::_1));

		m_listenerIdList[EVENT_TYPE_NEXT_STAGE] = EventManager::GetInstance()->AddEventListener(
			EVENT_TYPE_NEXT_STAGE, std::bind(&OpponentLayer::EventNextStage, this, std::placeholders::_1));

		m_listenerIdList[EVENT_TYPE_OPPONENT_CREATE] = EventManager::GetInstance()->AddEventListener(
			EVENT_TYPE_OPPONENT_CREATE, std::bind(&OpponentLayer::EventOpponentCreate, this, std::placeholders::_1));
	}

	scheduleUpdate();

	return true;
}

void OpponentLayer::onEnter()
{
	// super
	Layer::onEnter();
}

void OpponentLayer::onExit()
{
	// super
	Layer::onExit();

	if (EventManager::GetInstance())
	{
		EventManager::GetInstance()->RemoveEventListener(EVENT_TYPE_COLLISION, m_listenerIdList[EVENT_TYPE_COLLISION]);
		EventManager::GetInstance()->RemoveEventListener(EVENT_TYPE_NEXT_STAGE, m_listenerIdList[EVENT_TYPE_NEXT_STAGE]);
		EventManager::GetInstance()->RemoveEventListener(EVENT_TYPE_OPPONENT_CREATE, m_listenerIdList[EVENT_TYPE_OPPONENT_CREATE]);
	}
}

void OpponentLayer::update(float delta)
{

}

Layer *OpponentLayer::CreateLayer()
{
	return OpponentLayer::create();
}

bool OpponentLayer::CreateOpponent(eOpponentType type)
{
	if (SystemData::GetInstance() == false)
		return false;

	std::wstring wcsPath;
	if (type == OPPONENT_TYPE_ENEMY)
		wcsPath = SystemData::GetInstance()->GetEnemySpritePath();
	else if (type == OPPONENT_TYPE_WALL)
		wcsPath = SystemData::GetInstance()->GetWallSpritePath();
	
	std::string opponentPath = OpponentLayerPrivate::ConvertWideStringToString(wcsPath);
	Sprite *pOpponentSprite = Sprite::create(opponentPath);
	if (pOpponentSprite == nullptr)
		return false;

	cocos2d::Size spriteSize = pOpponentSprite->getContentSize();
	cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();

	cocos2d::Vec2 startingPos;
	startingPos.x = random(spriteSize.width / 2, visibleSize.width - spriteSize.width);
	startingPos.y = visibleSize.height + (spriteSize.height / 2);

	if (OpponentLayerPrivate::CheckMoveEndPosition(startingPos, spriteSize, visibleSize))
	{
		pOpponentSprite->setPosition(startingPos);
		StartOpponentMove(type, startingPos, pOpponentSprite);

		int enemyID = -1;
		if (GameManager::GetInstance())
		{
			if (type == OPPONENT_TYPE_ENEMY)
				enemyID = GameManager::GetInstance()->RegisterEnemy(pOpponentSprite);
			else if (type == OPPONENT_TYPE_WALL)
				enemyID = GameManager::GetInstance()->RegisterWall(pOpponentSprite);
		}

		if (enemyID != -1)
		{
			pOpponentSprite->setTag(OpponentLayerPrivate::FindTagFromID(enemyID, type));
			this->addChild(pOpponentSprite);

			if (type == OPPONENT_TYPE_ENEMY)
				StartEnemyAnimation(wcsPath, pOpponentSprite);
		}
	}
	else
		return false;

	return true;
}

void OpponentLayer::OpponentMoveEndCallback(Node *pEnemySprite, int type)
{
	if (pEnemySprite == nullptr)
		return;

	RemoveOpponent(pEnemySprite, (eOpponentType)type);
}

void OpponentLayer::RemoveOpponent(Node *pOpponentSprite, eOpponentType type)
{
	if (pOpponentSprite == nullptr)
		return;

	int opponentTag = pOpponentSprite->getTag();
	int opponentID = OpponentLayerPrivate::FindIDFromTag(opponentTag, type);
	if (GameManager::GetInstance() && opponentID != -1)
		GameManager::GetInstance()->UnregisterEnemy(static_cast<int>(opponentID));

	removeChildByTag(opponentTag);
}

bool OpponentLayer::StartOpponentMove(eOpponentType type, const Vec2 startingPos, Sprite *pOpponentSprite, bool isChange /*= false*/)
{
	cocos2d::Size spriteSize = pOpponentSprite->getContentSize();
	double scrollEndPosY = SystemData::GetInstance()->GetScrollingEndPosY();
	double duration = OpponentLayerPrivate::CalculateMovingDuration(startingPos);

	MoveTo *pMove = MoveTo::create(duration, Vec2(startingPos.x, scrollEndPosY + (spriteSize.height / 2)));
	if (pMove)
	{
		CallFuncN *pCallFuncN = CallFuncN::create(CC_CALLBACK_1(OpponentLayer::OpponentMoveEndCallback, this, static_cast<int>(type)));
		if (pCallFuncN)
		{
			Sequence *pSequence = Sequence::create(pMove, pCallFuncN, NULL);
			if (pSequence)
			{
				pSequence->setTag(ANIMATION_TAG_MOVE);
				pOpponentSprite->runAction(pSequence);
			}
		}
	}

	return true;
}

bool OpponentLayer::StartEnemyAnimation(const std::wstring &idleName, Sprite *pEnemySprite)
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
			std::string aniPath = OpponentLayerPrivate::ConvertWideStringToString(wcsAniPath);
			
			pAnimation->addSpriteFrameWithFile(aniPath);
		}

		Animate *pAnimate = Animate::create(pAnimation);
		if (pAnimate)
		{
			RepeatForever *pRepeat = RepeatForever::create(pAnimate);
			if (pRepeat)
				pEnemySprite->runAction(pRepeat);
		}
	}

	return true;
}

void OpponentLayer::KnockBackEnemy(Node *pEnemySprite, eCollisionSubDir subDir)
{
	if (pEnemySprite == nullptr)
		return;

	cocos2d::Vec2 curPos = pEnemySprite->getPosition();
	cocos2d::Vec2 nextPos = curPos;
	cocos2d::Size contentSize = pEnemySprite->getContentSize();

	double coefficient = SystemData::GetInstance() ? SystemData::GetInstance()->GetKnockBackCoefficient() : 0;
	nextPos.y += (contentSize.height * coefficient);
	if (subDir == COLL_SUB_DIR_POSITIVE)
		nextPos.x += contentSize.width;
	else if (subDir == COLL_SUB_DIR_NEGATIVE)
		nextPos.x -= contentSize.width;

	pEnemySprite->setPosition(nextPos);
}

void OpponentLayer::EventCollision(EventCollisionMsg msg)
{
	if (msg.type != EVENT_TYPE_COLLISION)
		return;

	if (msg.objectType == OPPONENT_TYPE_ENEMY)
	{
		cocos2d::Sprite *pSprite = (cocos2d::Sprite *)getChildByTag(OpponentLayerPrivate::FindTagFromID(msg.objectID, msg.objectType));
		if (pSprite == nullptr)
			return;

		if (!GameManager::GetInstance())
			return;

		//enemy knock-back
		int opponentHP = GameManager::GetInstance()->GetOpponentHP(msg.objectID);
		if (opponentHP > 0)
			KnockBackEnemy(pSprite, msg.collSubDir);
		else if (opponentHP == 0)
			RemoveOpponent(pSprite, msg.objectType);
	}
}

void OpponentLayer::EventNextStage(EventSystemMsg msg)
{
	if (msg.type != EVENT_TYPE_NEXT_STAGE)
		return;

	// enemy speed change
	if (GameManager::GetInstance() == nullptr)
		return;

	std::map<int, GameManager::OpponentData> opponentDataList;
	GameManager::GetInstance()->GetOpponentDataAllbyType(OPPONENT_TYPE_ALL, opponentDataList);
	for (const std::pair<int, GameManager::OpponentData> &data : opponentDataList)
	{
		if (data.second.type == OPPONENT_TYPE_NONE)
			continue;

		cocos2d::Sprite *pSprite = (cocos2d::Sprite *)getChildByTag(OpponentLayerPrivate::FindTagFromID(data.first, data.second.type));
		if (pSprite)
		{
			if(pSprite->getActionByTag(ANIMATION_TAG_MOVE) != nullptr)
				pSprite->stopActionByTag(ANIMATION_TAG_MOVE);
			StartOpponentMove(data.second.type, pSprite->getPosition(), pSprite, true);
		}
	}
}

void OpponentLayer::EventOpponentCreate(EventSystemMsg msg)
{
	if (msg.type != EVENT_TYPE_OPPONENT_CREATE)
		return;

	if (GameManager::GetInstance() == nullptr)
		return;

	eOpponentType opponentType = static_cast<eOpponentType>(msg.value);
	if (opponentType != OPPONENT_TYPE_NONE)
	{
		if (CreateOpponent(opponentType) == false)
			GameManager::GetInstance()->SetOpponentCreateDelayed(opponentType);
	}
}