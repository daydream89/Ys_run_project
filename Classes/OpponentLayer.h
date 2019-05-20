#ifndef __OPPONENT_LAYER_H__
#define __OPPONENT_LAYER_H__

#pragma once

#include "BaseLayer.h"

class OpponentLayer : public BaseLayer
{
public:
	static cocos2d::Layer *CreateLayer();

	OpponentLayer();
	virtual ~OpponentLayer();

	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	virtual void update(float delta);

	bool CreateOpponent(eOpponentType type);
	void RemoveOpponent(Node *pOpponentSprite, eOpponentType type);
	void OpponentMoveEndCallback(Node *pEnemySprite, int type);

	bool StartOpponentMove(eOpponentType type, const cocos2d::Vec2 startingPos, cocos2d::Sprite *pOpponentSprite, bool isChange = false);
	bool StartEnemyAnimation(const std::wstring &idleName, cocos2d::Sprite *pEnemySprite);
	void KnockBackEnemy(Node *pEnemySprite, eCollisionSubDir subDir);

	void EventCollision(EventCollisionMsg msg);
	void EventNextStage(EventSystemMsg msg);
	void EventOpponentCreate(EventSystemMsg msg);

	CREATE_FUNC(OpponentLayer);
};

#endif		// __OPPONENT_LAYER_H__