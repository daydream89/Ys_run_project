#include "InputLayer.h"

#include "EventManager.h"

USING_NS_CC;

namespace InputLayerPrivate
{
	// todo. move public function. usable another class.
	eInputKey ConvertInputState(EventKeyboard::KeyCode keyCode)
	{
		switch (keyCode)
		{
			case EventKeyboard::KeyCode::KEY_LEFT_ARROW:	return INPUT_KEY_ARROW_LEFT;
			case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:	return INPUT_KEY_ARROW_RIGHT;
			case EventKeyboard::KeyCode::KEY_UP_ARROW:		return INPUT_KEY_ARROW_UP;
			case EventKeyboard::KeyCode::KEY_DOWN_ARROW:	return INPUT_KEY_ARROW_DOWN;
			
			case EventKeyboard::KeyCode::KEY_ESCAPE:		return INPUT_KEY_ESC;
			case EventKeyboard::KeyCode::KEY_ENTER:			return INPUT_KEY_ENTER;
			case EventKeyboard::KeyCode::KEY_BACKSPACE:		return INPUT_KEY_BACKSPACE;
			case EventKeyboard::KeyCode::KEY_TAB:			return INPUT_KEY_TAB;
			case EventKeyboard::KeyCode::KEY_LEFT_SHIFT:
			case EventKeyboard::KeyCode::KEY_RIGHT_SHIFT:	return INPUT_KEY_SHIFT;
			case EventKeyboard::KeyCode::KEY_LEFT_CTRL:
			case EventKeyboard::KeyCode::KEY_RIGHT_CTRL:	return INPUT_KEY_CTRL;
			case EventKeyboard::KeyCode::KEY_LEFT_ALT:
			case EventKeyboard::KeyCode::KEY_RIGHT_ALT:		return INPUT_KEY_ALT;
			case EventKeyboard::KeyCode::KEY_SPACE:			return INPUT_KEY_SPACE;

			case EventKeyboard::KeyCode::KEY_0:				return INPUT_KEY_0;
			case EventKeyboard::KeyCode::KEY_1:				return INPUT_KEY_1;
			case EventKeyboard::KeyCode::KEY_2:				return INPUT_KEY_2;
			case EventKeyboard::KeyCode::KEY_3:				return INPUT_KEY_3;
			case EventKeyboard::KeyCode::KEY_4:				return INPUT_KEY_4;
			case EventKeyboard::KeyCode::KEY_5:				return INPUT_KEY_5;
			case EventKeyboard::KeyCode::KEY_6:				return INPUT_KEY_6;
			case EventKeyboard::KeyCode::KEY_7:				return INPUT_KEY_7;
			case EventKeyboard::KeyCode::KEY_8:				return INPUT_KEY_8;
			case EventKeyboard::KeyCode::KEY_9:				return INPUT_KEY_9;

			case EventKeyboard::KeyCode::KEY_A:				return INPUT_KEY_A;
			case EventKeyboard::KeyCode::KEY_B:				return INPUT_KEY_B;
			case EventKeyboard::KeyCode::KEY_C:				return INPUT_KEY_C;
			case EventKeyboard::KeyCode::KEY_D:				return INPUT_KEY_D;
			case EventKeyboard::KeyCode::KEY_E:				return INPUT_KEY_E;
			case EventKeyboard::KeyCode::KEY_F:				return INPUT_KEY_F;
			case EventKeyboard::KeyCode::KEY_G:				return INPUT_KEY_G;
			case EventKeyboard::KeyCode::KEY_H:				return INPUT_KEY_H;
			case EventKeyboard::KeyCode::KEY_I:				return INPUT_KEY_I;
			case EventKeyboard::KeyCode::KEY_J:				return INPUT_KEY_J;
			case EventKeyboard::KeyCode::KEY_K:				return INPUT_KEY_K;
			case EventKeyboard::KeyCode::KEY_L:				return INPUT_KEY_L;
			case EventKeyboard::KeyCode::KEY_M:				return INPUT_KEY_M;
			case EventKeyboard::KeyCode::KEY_N:				return INPUT_KEY_N;
			case EventKeyboard::KeyCode::KEY_O:				return INPUT_KEY_O;
			case EventKeyboard::KeyCode::KEY_P:				return INPUT_KEY_P;
			case EventKeyboard::KeyCode::KEY_Q:				return INPUT_KEY_Q;
			case EventKeyboard::KeyCode::KEY_R:				return INPUT_KEY_R;
			case EventKeyboard::KeyCode::KEY_S:				return INPUT_KEY_S;
			case EventKeyboard::KeyCode::KEY_T:				return INPUT_KEY_T;
			case EventKeyboard::KeyCode::KEY_U:				return INPUT_KEY_U;
			case EventKeyboard::KeyCode::KEY_V:				return INPUT_KEY_V;
			case EventKeyboard::KeyCode::KEY_W:				return INPUT_KEY_W;
			case EventKeyboard::KeyCode::KEY_X:				return INPUT_KEY_X;
			case EventKeyboard::KeyCode::KEY_Y:				return INPUT_KEY_Y;
			case EventKeyboard::KeyCode::KEY_Z:				return INPUT_KEY_Z;

			default:										return INPUT_KEY_NONE;
		}
	}
}

Layer *InputLayer::CreateLayer()
{
	return InputLayer::create();
}

InputLayer::InputLayer()
{
	
}

InputLayer::~InputLayer()
{

}

bool InputLayer::init()
{
	EventListenerKeyboard *pKeylistener = EventListenerKeyboard::create(); 
	pKeylistener->onKeyPressed = CC_CALLBACK_2(InputLayer::onKeyPressed, this);
	pKeylistener->onKeyReleased = CC_CALLBACK_2(InputLayer::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pKeylistener, this);

	return true;
}

void InputLayer::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	eInputKey inputKey = InputLayerPrivate::ConvertInputState(keyCode);
	if (inputKey != INPUT_KEY_NONE)
		SendKeyEvent(inputKey, true);
}

void InputLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	eInputKey inputKey = InputLayerPrivate::ConvertInputState(keyCode);
	if (inputKey != INPUT_KEY_NONE)
		SendKeyEvent(inputKey, false);
}

void InputLayer::SendKeyEvent(eInputKey inputKey, bool isPressed)
{
	if (EventManager::GetInstance())
	{
		EventInputMsg msg;
		msg.type = EVENT_TYPE_INPUT;
		msg.inputState = inputKey;
		msg.isPressed = isPressed;

		EventManager::GetInstance()->NotifyEvent(msg);
	}
}