#ifndef __TITLE_LAYER_H__
#define __TITLE_LAYER_H__

#pragma once

#include "BaseLayer.h"

class TitleLayer : public BaseLayer
{
public:
	static cocos2d::Layer *CreateLayer();

	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	virtual void update(float delta);

	void CreateTitle();

	void CreatePressAnyKey();
	void UpdatePressAnyKey(float elapsedTime);

	CREATE_FUNC(TitleLayer);

private:
	bool m_isTextVanishing;
};

#endif		// __TITLE_LAYER_H__
