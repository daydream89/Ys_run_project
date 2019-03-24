#pragma once

#include <cocos2d.h>

#include "CommonData.h"

class GameManager
{
public:
	struct OpponentData
	{
		eOpponentType type;
		cocos2d::Sprite *pSprite;
		uint32_t hp;	// only for enemy

		OpponentData()
			: type(OPPONENT_TYPE_NONE)
			, pSprite(nullptr)
			, hp(0)
		{ }
	};

private:
	static GameManager *m_pGameManager;
public:
	static GameManager *GetInstance();

	void Init(const cocos2d::Size &visibleSize);
	void Update(float delta = 0.f);
	void Release();

	void Initialize();

	void SetCurScene(eScene scene) { m_curScene = scene; }
	eScene GetCurScene() const { return m_curScene; }

	bool IsGameOver() { return m_isGameOver; }
	unsigned int GetCurStage() { return m_curStage; }

	float GetCurPlayTime() { return m_curPlayTime; }

	void OpponentCreatableProcess(float delta);
	void SetOpponentCreateDelayed(eOpponentType type);
	void SendEventOpponentCreate(eOpponentType type);

	int RegisterEnemy(cocos2d::Sprite *pEnemy);
	void UnregisterEnemy(int enemyID);
	int RegisterWall(cocos2d::Sprite *pEnemy);
	void UnregisterWall(int wallID);
	const OpponentData& FindOpponentData(int id);
	unsigned int GetOpponentDataAllbyType(eOpponentType type, std::map<int, OpponentData> &opponentDataList);
	int GetOpponentHP(int id);

	unsigned int GetKillCount() { return m_killCount; }

	void RegisterPlayer(cocos2d::Sprite *pPlayer);

	bool IsCreatablePos(cocos2d::Vec2 pos, float width, float height);
	void CollisionCheckProcess();
	bool CollisionCheckWithPlayer();
	const std::vector<CollisionData> &GetCollisionList() { return m_collisionVec; }

	void EventRestart(EventSystemMsg msg);

private:
	template <typename Value>
	void MapInitializer(std::map<int, Value> &mapData);

private:
	unsigned int m_listenerIdList[EVENT_TYPE_MAX];

	eScene m_curScene;

	bool m_isGameOver;
	unsigned int m_curStage;

	float m_curPlayTime;

	cocos2d::Size m_visibleSize;

	float m_curEnemyCreateDelay;
	float m_curWallCreateDelay;

	typedef std::map<int, OpponentData> OpponentMap;
	OpponentMap m_opponentMap;

	uint64_t m_killCount;

	std::vector<CollisionData> m_collisionVec;

	cocos2d::Sprite *m_playerSprite;
};
