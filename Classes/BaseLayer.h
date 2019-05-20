#ifndef __BASE_LAYER_H__
#define __BASE_LAYER_H__

#pragma once

#include <cocos2d.h>

#include "CommonData.h"
#include "GameManager.h"

class BaseLayer : public cocos2d::Layer
{
public:
	BaseLayer() {  }
	virtual ~BaseLayer() {  }

protected:
	unsigned int m_listenerIdList[EVENT_TYPE_MAX];
};

#endif