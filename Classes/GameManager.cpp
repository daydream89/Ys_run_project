#include "GameManager.h"

#include "EventManager.h"
#include "SystemData.h"

namespace GameManagerPrivate
{
	static int s_ObjectIdGenerator = RESERVED_MAP_INDEX + 1;
	int GenerateObjectID()
	{
		return s_ObjectIdGenerator++;
	}

	struct BorderPosition
	{
		double up, down, left, right;
		BorderPosition() : up(0.0), down(0.0), left(0.0), right(0.0) { }
	};

	struct BorderCollision
	{
		BorderPosition topCollision, bottomCollision, leftCollision, rightCollision;
	};

	void CreateBorderCollision(BorderCollision &objectCollision, const cocos2d::Vec2 &objectPos, const cocos2d::Size &objectSize)
	{
		int thickness = SystemData::GetInstance() ? SystemData::GetInstance()->GetCollisionThickness() : 0;

		// calculate top border collision
		objectCollision.topCollision.up		= objectPos.y + (objectSize.height / 2);
		objectCollision.topCollision.down	= objectCollision.topCollision.up - thickness;
		objectCollision.topCollision.left	= objectPos.x - (objectSize.width / 2);
		objectCollision.topCollision.right	= objectPos.x + (objectSize.width / 2);

		// calculate bottom border collision
		objectCollision.bottomCollision.down	= objectPos.y - (objectSize.height / 2);
		objectCollision.bottomCollision.up		= objectCollision.bottomCollision.down + thickness;
		objectCollision.bottomCollision.left	= objectCollision.topCollision.left;
		objectCollision.bottomCollision.right	= objectCollision.topCollision.right;

		// calculate left border collision
		objectCollision.leftCollision.up	= objectCollision.topCollision.down;
		objectCollision.leftCollision.down	= objectCollision.bottomCollision.up;
		objectCollision.leftCollision.left	= objectCollision.topCollision.left;
		objectCollision.leftCollision.right	= objectCollision.topCollision.left + thickness;

		// calculate left border collision
		objectCollision.rightCollision.up = objectCollision.topCollision.down;
		objectCollision.rightCollision.down = objectCollision.bottomCollision.up;
		objectCollision.rightCollision.left = objectCollision.topCollision.right;
		objectCollision.rightCollision.right = objectCollision.topCollision.right - thickness;
	}

	// offset : distance of collision detection allowed from sprite border
	// todo. offset apply
	bool DetectCollision(BorderPosition object1Border, BorderPosition object2Border, float offset = 0.f)
	{
		// for object1
		if (object1Border.up >= object2Border.down && object1Border.down <= object2Border.down)
		{
			if (object1Border.right >= object2Border.left && object1Border.left <= object2Border.left)
				return true;
			else if (object1Border.left <= object2Border.right && object1Border.right >= object2Border.right)
				return true;
		}
		else if (object1Border.up >= object2Border.up && object1Border.down <= object2Border.up)
		{
			if (object1Border.right >= object2Border.left && object1Border.left <= object2Border.left)
				return true;
			else if (object1Border.left <= object2Border.right && object1Border.right >= object2Border.right)
				return true;
		}

		// for object2
		if (object2Border.up >= object1Border.down && object2Border.down <= object1Border.down)
		{
			if (object2Border.right >= object1Border.left && object2Border.left <= object1Border.left)
				return true;
			else if (object2Border.left <= object1Border.right && object2Border.right >= object1Border.right)
				return true;
		}
		else if (object2Border.up >= object1Border.up && object2Border.down <= object1Border.up)
		{
			if (object2Border.right >= object1Border.left && object2Border.left <= object1Border.left)
				return true;
			else if (object2Border.left <= object1Border.right && object2Border.right >= object1Border.right)
				return true;
		}

		return false;
	}

	eCollisionSubDir SetCollisionSubDirection(const cocos2d::Vec2 &pos1, const cocos2d::Vec2 &pos2)
	{
		float gradient = (pos1.y - pos2.y) / (pos1.x - pos2.x);

		if (gradient < -2.f || 2.f < gradient)
			return COLL_SUB_DIR_ZERO;
		else if (gradient < 0.f)
			return COLL_SUB_DIR_NEGATIVE;
		else if (0.f <= gradient)
			return COLL_SUB_DIR_POSITIVE;
		else
			return COLL_SUB_DIR_NONE;
	}
}

GameManager *GameManager::m_pGameManager = nullptr;
GameManager *GameManager::GetInstance()
{
	if (m_pGameManager == nullptr)
		m_pGameManager = new GameManager;
	return m_pGameManager;
}

void GameManager::Init(const cocos2d::Size &visibleSize)
{
	if (EventManager::GetInstance())
	{
		m_listenerIdList[EVENT_TYPE_RESTART] = EventManager::GetInstance()->AddEventListener(
			EVENT_TYPE_RESTART, std::bind(&GameManager::EventRestart, this, std::placeholders::_1));
	}

	m_visibleSize = visibleSize;

	m_curScene = SCENE_NAME_NONE;

	Initialize();
}

void GameManager::Initialize()
{
	m_isGameOver = false;
	m_curStage = 1;

	m_curPlayTime = 0.f;

	m_killCount = 0;

	m_curEnemyCreateDelay = SystemData::GetInstance() ? SystemData::GetInstance()->GetEnemyCreateDelay() : 0;
	m_curWallCreateDelay = SystemData::GetInstance() ? SystemData::GetInstance()->GetWallCreateDelay() : 0;

	MapInitializer<GameManager::OpponentData>(m_opponentMap);

	m_collisionVec.clear();

	m_playerSprite = nullptr;
}

// updated by InGameScene::update()
void GameManager::Update(float delta /*= 0.f*/)
{
	m_curPlayTime += delta;
	if (SystemData::GetInstance())
	{
		float StageTimeLength = SystemData::GetInstance()->GetStageTimeLengthSec();
		if (StageTimeLength * m_curStage <= m_curPlayTime)
		{
			m_curStage++;

			if (EventManager::GetInstance())
			{
				EventSystemMsg msg;
				msg.type = EVENT_TYPE_NEXT_STAGE;
				msg.value = m_curStage;
				EventManager::GetInstance()->NotifyEvent(msg);
			}
		}
	}

	// wall & enemy create process
	// creation time of wall & enemy are relatively prime
	OpponentCreatableProcess(delta);

	if (m_isGameOver)
	{

	}
	else
	{
		// collision check process
		CollisionCheckProcess();
	}
}

void GameManager::Release()
{
	if (EventManager::GetInstance())
	{
		EventManager::GetInstance()->RemoveEventListener(EVENT_TYPE_RESTART, m_listenerIdList[EVENT_TYPE_RESTART]);
	}

	m_curScene = SCENE_NAME_NONE;

	m_opponentMap.clear();
	m_collisionVec.clear();

	m_playerSprite = nullptr;
}

// todo. move public function. usable another class.
template <typename Value>
void GameManager::MapInitializer(std::map<int, Value> &mapData)
{
	mapData.clear();

	Value value;
	mapData.insert(std::make_pair(RESERVED_MAP_INDEX, value));
}

void GameManager::OpponentCreatableProcess(float delta)
{
	// todo. need modify. create time affect stage number. about 10%?
	m_curWallCreateDelay -= delta;
	if (m_curWallCreateDelay <= 0.f)
	{
		SendEventOpponentCreate(OPPONENT_TYPE_WALL);
		m_curWallCreateDelay = SystemData::GetInstance() ? SystemData::GetInstance()->GetWallCreateDelay() : 0;
	}

	m_curEnemyCreateDelay -= delta;
	if (m_curEnemyCreateDelay <= 0.f)
	{
		SendEventOpponentCreate(OPPONENT_TYPE_ENEMY);
		m_curEnemyCreateDelay = SystemData::GetInstance() ? SystemData::GetInstance()->GetEnemyCreateDelay() : 0;
	}
}

void GameManager::SetOpponentCreateDelayed(eOpponentType type)
{
	double createDelay = SystemData::GetInstance() ? SystemData::GetInstance()->GetAdditionalCreateDelay() : 0;
	if (type = OPPONENT_TYPE_ENEMY)
		m_curEnemyCreateDelay += createDelay;
	else if (type = OPPONENT_TYPE_WALL)
		m_curWallCreateDelay += createDelay;
}

void GameManager::SendEventOpponentCreate(eOpponentType type)
{
	if (EventManager::GetInstance())
	{
		EventSystemMsg msg;
		msg.type = EVENT_TYPE_OPPONENT_CREATE;
		msg.value = static_cast<int>(type);

		EventManager::GetInstance()->NotifyEvent(msg);
	}
}

int GameManager::RegisterEnemy(cocos2d::Sprite *pEnemy)
{
	if (pEnemy == nullptr)
		return -1;

	int newID = GameManagerPrivate::GenerateObjectID();
	int defaultHP = SystemData::GetInstance() ? SystemData::GetInstance()->GetDefaultEnemyHp() : 1;

	OpponentData enemyData;
	enemyData.type = OPPONENT_TYPE_ENEMY;
	enemyData.pSprite = pEnemy;
	enemyData.hp = m_curStage * defaultHP;

	m_opponentMap.insert(std::make_pair(newID, enemyData));

	return newID;
}

void GameManager::UnregisterEnemy(int enemyID)
{
	OpponentMap::iterator iter = m_opponentMap.find(enemyID);
	if (iter != m_opponentMap.end())
		m_opponentMap.erase(iter);
}

int GameManager::RegisterWall(cocos2d::Sprite *pWall)
{
	if (pWall == nullptr)
		return -1;

	int newID = GameManagerPrivate::GenerateObjectID();

	OpponentData wallData;
	wallData.type = OPPONENT_TYPE_WALL;
	wallData.pSprite = pWall;

	m_opponentMap.insert(std::make_pair(newID, wallData));

	return newID;
}

void GameManager::UnregisterWall(int wallID)
{
	OpponentMap::iterator iter = m_opponentMap.find(wallID);
	if (iter != m_opponentMap.end())
		m_opponentMap.erase(iter);
}

const GameManager::OpponentData& GameManager::FindOpponentData(int id)
{
	OpponentMap::iterator iter = m_opponentMap.find(id);
	if (iter == m_opponentMap.end())
		return m_opponentMap.at(RESERVED_MAP_INDEX);

	return iter->second;
}

unsigned int GameManager::GetOpponentDataAllbyType(eOpponentType type, std::map<int, OpponentData> &opponentDataList)
{
	if (type == OPPONENT_TYPE_ALL)
	{
		for (const std::pair<int, OpponentData> &data : m_opponentMap)
			opponentDataList.insert(std::make_pair(data.first, data.second));

		return opponentDataList.size();
	}

	for (const std::pair<int, OpponentData> &data : m_opponentMap)
	{
		if (data.second.type == type)
			opponentDataList.insert(std::make_pair(data.first, data.second));
	}

	return opponentDataList.size();
}

int GameManager::GetOpponentHP(int id)
{
	OpponentMap::iterator iter = m_opponentMap.find(id);
	if (iter == m_opponentMap.end())
		return 0;

	return iter->second.hp;
}

void GameManager::RegisterPlayer(cocos2d::Sprite *pPlayer)
{
	if (pPlayer)
		m_playerSprite = pPlayer;
}

void GameManager::CollisionCheckProcess()
{
	if (CollisionCheckWithPlayer())
	{
		for (const CollisionData &collision : m_collisionVec)
		{
			OpponentMap::iterator iter = m_opponentMap.find(collision.id);
			if (iter == m_opponentMap.end())
				continue;

			if (iter->second.type == OPPONENT_TYPE_ENEMY)
			{
				if (collision.dir == COLL_DIRECTION_UP)
				{
					iter->second.hp--;
					if (iter->second.hp == 0)
						m_killCount++;

					if (EventManager::GetInstance())
					{
						EventCollisionMsg msg;
						msg.type = EVENT_TYPE_COLLISION;
						msg.objectID = iter->first;
						msg.objectType = iter->second.type;
						msg.collDir = collision.dir;
						msg.collSubDir = collision.subDir;
						EventManager::GetInstance()->NotifyEvent(msg);
					}
				}
				else
				{
					m_isGameOver = true;

					if (EventManager::GetInstance())
					{
						EventSystemMsg msg;
						msg.type = EVENT_TYPE_GAMEOVER;
						EventManager::GetInstance()->NotifyEvent(msg);
					}
				}
			}
			else if (iter->second.type == OPPONENT_TYPE_WALL)
			{
				m_isGameOver = true;

				if (EventManager::GetInstance())
				{
					EventSystemMsg msg;
					msg.type = EVENT_TYPE_GAMEOVER;
					EventManager::GetInstance()->NotifyEvent(msg);
				}
			}
		}
	}
}

bool GameManager::CollisionCheckWithPlayer()
{
	m_collisionVec.clear();

	if (m_playerSprite == nullptr)
		return false;

	cocos2d::Vec2 playerPos = m_playerSprite->getPosition();
	cocos2d::Size playerOffset = m_playerSprite->getContentSize();

	GameManagerPrivate::BorderCollision playerCollision;
	GameManagerPrivate::CreateBorderCollision(playerCollision, playerPos, playerOffset);

	for (const std::pair<int, OpponentData> &data : m_opponentMap)
	{
		if (data.second.pSprite == nullptr)
			continue;

		// for operation reduce. not check up to half line.
		cocos2d::Vec2 opponentPos = data.second.pSprite->getPosition();
		if ((m_visibleSize.height / 2) <= opponentPos.y)
			continue;

		cocos2d::Size opponentOffset = data.second.pSprite->getContentSize();

		GameManagerPrivate::BorderPosition opponentBorder;
		opponentBorder.up = static_cast<double>(opponentPos.y + (opponentOffset.height / 2));
		opponentBorder.down = static_cast<double>(opponentPos.y - (opponentOffset.height / 2));
		opponentBorder.left = static_cast<double>(opponentPos.x - (opponentOffset.width / 2));
		opponentBorder.right = static_cast<double>(opponentPos.x + (opponentOffset.width / 2));

		CollisionData collData;
		collData.id = data.first;
		collData.dir = COLL_DIRECTION_NONE;

		if (GameManagerPrivate::DetectCollision(playerCollision.leftCollision, opponentBorder))
			collData.dir = COLL_DIRECTION_LEFT;
		else if (GameManagerPrivate::DetectCollision(playerCollision.rightCollision, opponentBorder))
			collData.dir = COLL_DIRECTION_RIGHT;
		else if (GameManagerPrivate::DetectCollision(playerCollision.topCollision, opponentBorder))
			collData.dir = COLL_DIRECTION_UP;
		else if (GameManagerPrivate::DetectCollision(playerCollision.bottomCollision, opponentBorder))
			collData.dir = COLL_DIRECTION_DOWN;

		if (collData.dir != COLL_DIRECTION_NONE)
		{
			// calculate sub direction only if there is collision detect actually.
			collData.subDir = GameManagerPrivate::SetCollisionSubDirection(playerPos, opponentPos);

			m_collisionVec.push_back(collData);
			return true;
		}
	}

	return false;
}

bool GameManager::IsCreatablePos(cocos2d::Vec2 pos, float width, float height)
{
	// each direction is based on screen
	float offsetY = height / 2;
	float offsetX = width / 2;

	GameManagerPrivate::BorderPosition border;
	border.up = pos.y + offsetY;
	border.down = pos.y - offsetY;
	border.left = pos.x - offsetX;
	border.right = pos.x + offsetX;

	for (const std::pair<int, OpponentData> &opponent : m_opponentMap)
	{
		if (opponent.second.pSprite == nullptr)
			continue;

		const cocos2d::Vec2 &opponentPos = opponent.second.pSprite->getPosition();
		cocos2d::Size opponentOffset = opponent.second.pSprite->getContentSize();
		opponentOffset.height = opponentOffset.height / 2;
		opponentOffset.width = opponentOffset.width / 2;

		GameManagerPrivate::BorderPosition opponentBorder;
		opponentBorder.up = opponentPos.y + opponentOffset.height;
		opponentBorder.down = opponentPos.y - opponentOffset.height;
		if (border.up < opponentBorder.down || opponentBorder.up < border.down)
			continue;

		opponentBorder.left = opponentPos.x - opponentOffset.width;
		opponentBorder.right = opponentPos.x + opponentOffset.width;
		if (border.right < opponentBorder.left || opponentBorder.right < border.left)
			continue;

		return false;
	}

	return true;
}

void GameManager::EventRestart(EventSystemMsg msg)
{
	if (msg.type != EVENT_TYPE_RESTART)
		return;

	Initialize();
}