#ifndef __INGAME_SCENE_H__
#define __INGAME_SCENE_H__

#pragma once

#include "BaseScene.h"

class InGameScene : public BaseScene
{
public:
    static cocos2d::Scene* createScene();

	InGameScene();
	virtual ~InGameScene();

    virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	virtual void update(float delta);

	virtual bool AttachLayer();
	virtual bool DetachLayer();

	void EventRestart(EventSystemMsg msg);

	CREATE_FUNC(InGameScene);
};

#endif // __INGAME_SCENE_H__
