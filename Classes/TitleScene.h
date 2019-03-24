#ifndef __TITLE_SCENE_H__
#define __TITLE_SCENE_H__

#pragma once

#include "BaseScene.h"

class TitleScene : public BaseScene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	//virtual void update(float delta);

	bool AttachLayer();
	bool DetachLayer();

	void EventInput(EventInputMsg msg);

	CREATE_FUNC(TitleScene);

private:
	unsigned int m_listenerIdList[EVENT_TYPE_MAX];
};

#endif // __TITLE_SCENE_H__

