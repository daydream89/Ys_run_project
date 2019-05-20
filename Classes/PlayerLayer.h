#ifndef __PLAYER_LAYER_H__
#define __PLAYER_LAYER_H__

#pragma once

#include "BaseLayer.h"

class PlayerLayer : public BaseLayer
{
public:
	static cocos2d::Layer *CreateLayer();

	PlayerLayer();
	virtual ~PlayerLayer();

	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	virtual void update(float delta);

	bool CreatePlayer();
	void MovePlayer(eInputKey direction);
	void RemovePlayer();

	bool StartPlayerAnimation(const std::wstring &idleName, cocos2d::Sprite *pPlayerSprite);

	void EventGameOver(EventSystemMsg msg);
	void EventInput(EventInputMsg msg);
	
	CREATE_FUNC(PlayerLayer);

private:
	bool m_isPlayerMoving;
	bool m_isPlayerExist;

	eInputKey m_curInputKey;
};

#endif