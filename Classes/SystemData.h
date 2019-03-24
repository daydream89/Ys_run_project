#pragma once

#include "CommonData.h"

class SystemData
{
private:
	enum ePathType
	{
		PATH_TYPE_NONE = 0,
		PATH_TYPE_ENEMY_SPRITE,
		PATH_TYPE_WALL_SPRITE,
		PATH_TYPE_PLAYER_SPRITE,
		PATH_TYPE_BACKGROUND_SPRITE,
		PATH_TYPE_TITLE_IMAGE,

		PATH_TYPE_MAX,
	};

	enum TagType
	{
		TAG_TYPE_NONE = 0,
		
		TAG_TYPE_BACKGROUND_LAYER,
		TAG_TYPE_PLAYER_LAYER,
		TAG_TYPE_OPPONENT_LAYER,
		TAG_TYPE_UI_LAYER,
		TAG_TYPE_INPUT_LAYER,
		TAG_TYPE_TITLE_LAYER,
		
		TAG_TYPE_PLAYER_SPRITE,
		TAG_TYPE_ENEMY_START,
		TAG_TYPE_WALL_START,

		TAG_TYPE_MAX,
	};

private:
	static SystemData *m_pSystemData;
public:
	static SystemData *GetInstance();

public:
	void Init();

	bool SetData(eFileType fileType, const std::wstring &key, const std::wstring &value);

	const std::wstring &GetEnemySpritePath() const { return m_pathList[PATH_TYPE_ENEMY_SPRITE]; }
	const std::wstring &GetWallSpritePath() const { return m_pathList[PATH_TYPE_WALL_SPRITE]; }
	const std::wstring &GetPlayerSpritePath() const { return m_pathList[PATH_TYPE_PLAYER_SPRITE]; }
	const std::wstring &GetBackGroundSpritePath() const { return m_pathList[PATH_TYPE_BACKGROUND_SPRITE]; }
	const std::wstring &GetTitleImagePath() const { return m_pathList[PATH_TYPE_TITLE_IMAGE]; }
	
	const double &GetPlayerSpeed() const { return m_PlayerSpeed; }
	const double &GetBGStartingYBasic() const { return m_BGStartingYBasic; }
	const double &GetBGStartingYNext() const { return m_BGStartingYNext; }
	const double &GetOpponentStartingY() const { return m_OpponentStartingY; }
	const double &GetScrollingVelocity() const { return m_ScrollingVelocity; }
	const double &GetScrollingWeight() const { return m_ScrollingWeight; }
	const double &GetScrollingEndPosY() const { return m_ScrollingEndPosY; }
	const double &GetEnemyCreateDelay() const { return m_EnemyCreateDelay; }
	const double &GetWallCreateDelay() const { return m_WallCreateDelay; }
	const double &GetAdditionalCreateDelay() const { return m_AdditionalCreateDelay; }
	const unsigned int &GetDefaultEnemyHp() const { return m_DefaultEnemyHp; }
	const double &GetKnockBackCoefficient() const { return m_KnockBackCoefficient; }
	const int &GetCollisionThickness() const { return m_CollisionThickness; }
	const double &GetStageTimeLengthSec() const { return m_StageTimeLengthSec; }

	const int &GetSpriteAnimationFrame() const { return m_SpriteAnimationFrame; }
	
	const unsigned int &GetBackgroundLayerTag() const { return m_tagList[TAG_TYPE_BACKGROUND_LAYER]; }
	const unsigned int &GetPlayerLayerTag() const { return m_tagList[TAG_TYPE_PLAYER_LAYER]; }
	const unsigned int &GetOpponentLayerTag() const { return m_tagList[TAG_TYPE_OPPONENT_LAYER]; }
	const unsigned int &GetUILayerTag() const { return m_tagList[TAG_TYPE_UI_LAYER]; }
	const unsigned int &GetInputLayerTag() const { return m_tagList[TAG_TYPE_INPUT_LAYER]; }
	const unsigned int &GetTitleLayerTag() const { return m_tagList[TAG_TYPE_TITLE_LAYER]; }
	const unsigned int &GetPlayerTag() const { return m_tagList[TAG_TYPE_PLAYER_SPRITE]; }
	const unsigned int &GetEnemyStartTag() const { return m_tagList[TAG_TYPE_ENEMY_START]; }
	const unsigned int &GetWallStartTag() const { return m_tagList[TAG_TYPE_WALL_START]; }

private:
	// Path
	std::wstring m_pathList[PATH_TYPE_MAX];

	// System
	double m_PlayerSpeed = 0;
	double m_BGStartingYBasic = 0;
	double m_BGStartingYNext = 0;
	double m_OpponentStartingY = 0;
	double m_ScrollingVelocity = 0;		// pixel per second
	double m_ScrollingWeight = 0;
	double m_ScrollingEndPosY = 0;
	double m_EnemyCreateDelay = 0;
	double m_WallCreateDelay = 0;
	double m_AdditionalCreateDelay = 0;
	unsigned int m_DefaultEnemyHp = 0;
	double m_KnockBackCoefficient = 0;
	int m_CollisionThickness = 0;
	double m_StageTimeLengthSec = 0.f;

	const int m_SpriteAnimationFrame = 4;

	// Tag
	unsigned int m_tagList[TAG_TYPE_MAX];
};