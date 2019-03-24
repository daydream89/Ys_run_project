#ifndef __BACKGROUND_LAYER_H__
#define __BACKGROUND_LAYER_H__

#pragma once

#include "BaseLayer.h"

class BackGroundLayer : public BaseLayer
{
public:
	enum eBackGroundOffset
	{
		BACKGROUND_OFFSET_BASIC = 0,
		BACKGROUND_OFFSET_NEXT,
		BACKGROUND_OFFSET_MOVING,

		BACKGROUND_OFFSET_MAX,
	};

public:
	static cocos2d::Layer *CreateLayer();
	
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();

	void CreateBackGround(int offset);
	void RemoveBackGround(Node *pBackGroundSprite);
	void BackGroundMoveEndCallback(Node *pBackGroundSprite);

	void StartBackGroundMove(int offset, const cocos2d::Vec2 startingPos, cocos2d::Sprite *pSprite);

	void Pause();
	
	void ChangeSprite(cocos2d::__String path);
	void ChangeSpeed();

	void EventNextStage(EventSystemMsg msg);

	CREATE_FUNC(BackGroundLayer);

private:
	int m_BackGroundIdGenerator;
};

#endif