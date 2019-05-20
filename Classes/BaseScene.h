#ifndef __BASE_SCENE_H__
#define __BASE_SCENE_H__

#pragma once

#include "cocos2d.h"

#include "GameManager.h"

class BaseScene : public cocos2d::Scene
{
public:
	BaseScene() {  }
	virtual ~BaseScene() {  }

	virtual bool AttachLayer() = 0;
	virtual bool DetachLayer() = 0;

protected:
	unsigned int m_listenerIdList[EVENT_TYPE_MAX];
};

#endif // __BASE_SCENE_H__