#ifndef __INPUT_LAYER_H__
#define __INPUT_LAYER_H__

#pragma once

#include "BaseLayer.h"

class InputLayer : public BaseLayer
{
public:
	static cocos2d::Layer *CreateLayer();

	virtual bool init();

	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

	CREATE_FUNC(InputLayer);

private:
	void SendKeyEvent(eInputKey inputKey, bool isPressed);
};

#endif	//__INPUT_LAYER_H__
