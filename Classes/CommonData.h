#pragma once

#include <vector>
#include <map>
#include <list>

/* map에서 찾는 결과가 없는 경우, 넘겨줄 invalid data를 0번에 저장.
   find에 실패한 경우, local data를 넘기는 것보다는 깔끔할듯 하여 사용. */
#define RESERVED_MAP_INDEX		0

#define MAX_STR_LEN 256

enum eFileType
{
	FILE_TYPE_NONE = 0,

	FILE_TYPE_PATH,
	FILE_TYPE_SYSTEM,
	FILE_TYPE_TAG,

	FILE_TYPE_MAX,
};

enum eInputKey
{
	INPUT_KEY_NONE = -1,

	INPUT_KEY_ARROW_UP = 0,
	INPUT_KEY_ARROW_DOWN,
	INPUT_KEY_ARROW_LEFT,
	INPUT_KEY_ARROW_RIGHT,

	INPUT_KEY_ESC,
	INPUT_KEY_ENTER,
	INPUT_KEY_BACKSPACE,
	INPUT_KEY_TAB,
	INPUT_KEY_SHIFT,
	INPUT_KEY_CTRL,
	INPUT_KEY_ALT,
	INPUT_KEY_SPACE,

	INPUT_KEY_0,
	INPUT_KEY_1,
	INPUT_KEY_2,
	INPUT_KEY_3,
	INPUT_KEY_4,
	INPUT_KEY_5,
	INPUT_KEY_6,
	INPUT_KEY_7,
	INPUT_KEY_8,
	INPUT_KEY_9,

	INPUT_KEY_A,
	INPUT_KEY_B,
	INPUT_KEY_C,
	INPUT_KEY_D,
	INPUT_KEY_E,
	INPUT_KEY_F,
	INPUT_KEY_G,
	INPUT_KEY_H,
	INPUT_KEY_I,
	INPUT_KEY_J,
	INPUT_KEY_K,
	INPUT_KEY_L,
	INPUT_KEY_M,
	INPUT_KEY_N,
	INPUT_KEY_O,
	INPUT_KEY_P,
	INPUT_KEY_Q,
	INPUT_KEY_R,
	INPUT_KEY_S,
	INPUT_KEY_T,
	INPUT_KEY_U,
	INPUT_KEY_V,
	INPUT_KEY_W,
	INPUT_KEY_X,
	INPUT_KEY_Y,
	INPUT_KEY_Z,

	INPUT_KEY_MAX,
};

enum eActionTag
{
	ACTION_TAG_PLAYER_MOVE_LEFT = 1,
	ACTION_TAG_PLAYER_MOVE_RIGHT,

	ACTION_TAG_END,
};

enum eAnimationTag
{
	ANIMATION_TAG_MOVE = 1,
	ANIMATION_TAG_KNOCK_BACK,

	ANIMATION_TAG_END,
};

enum eCollisionDirection
{
	COLL_DIRECTION_NONE = 0,
	COLL_DIRECTION_LEFT,
	COLL_DIRECTION_RIGHT,
	COLL_DIRECTION_DOWN,
	COLL_DIRECTION_UP,
	
	COLL_DIRECTION_MAX,
};

enum eCollisionSubDir
{
	COLL_SUB_DIR_NONE = 0,

	COLL_SUB_DIR_POSITIVE,
	COLL_SUB_DIR_ZERO,
	COLL_SUB_DIR_NEGATIVE,

	COLL_SUB_DIR_MAX,
};

enum eOpponentType
{
	OPPONENT_TYPE_NONE = 0,
	OPPONENT_TYPE_ENEMY,
	OPPONENT_TYPE_WALL,

	OPPONENT_TYPE_MAX,
	OPPONENT_TYPE_ALL,
};

struct CollisionData
{
	int id;
	eCollisionDirection dir;
	eCollisionSubDir subDir;

	CollisionData()
		: id(0)
		, dir(COLL_DIRECTION_NONE)
		, subDir(COLL_SUB_DIR_NONE)
	{ }
};

enum eEventListenerType
{
	EVENT_TYPE_NONE = 0,

	EVENT_TYPE_COLLISION,
	EVENT_TYPE_INPUT,
	
	// system
	EVENT_TYPE_GAMEOVER,
	EVENT_TYPE_NEXT_STAGE,
	EVENT_TYPE_RESTART,
	EVENT_TYPE_OPPONENT_CREATE,

	EVENT_TYPE_MAX,
};

enum eScene
{
	SCENE_NAME_NONE = 0,
	SCENE_NAME_TITLE,
	SCENE_NAME_INGAME,
};

struct EventCollisionMsg
{
	eEventListenerType type;
	int objectID;
	eOpponentType objectType;
	eCollisionDirection collDir;
	eCollisionSubDir collSubDir;

	EventCollisionMsg()
		: type(EVENT_TYPE_COLLISION)
		, objectID(0)
		, objectType(OPPONENT_TYPE_NONE)
		, collDir(COLL_DIRECTION_NONE)
		, collSubDir(COLL_SUB_DIR_NONE)
	{ }
};

struct EventInputMsg
{
	eEventListenerType type;

	eInputKey inputState;
	bool isPressed;

	EventInputMsg()
		: type(EVENT_TYPE_INPUT)
		, inputState(INPUT_KEY_NONE)
		, isPressed(false)
	{ }
};

struct EventSystemMsg
{
	eEventListenerType type;

	int value;

	EventSystemMsg()
		: type(EVENT_TYPE_NONE)
		, value(0)
	{ }
};