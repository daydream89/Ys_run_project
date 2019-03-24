#include "SystemData.h"

SystemData *SystemData::m_pSystemData = nullptr;
SystemData *SystemData::GetInstance()
{
	if (m_pSystemData == nullptr)
		m_pSystemData = new SystemData;
	return m_pSystemData;
}

void SystemData::Init()
{
	for (int i = PATH_TYPE_NONE; i < PATH_TYPE_MAX; ++i)
		m_pathList[i] = L"";

	for (int i = TAG_TYPE_NONE; i < TAG_TYPE_MAX; ++i)
		m_tagList[i] = 0;
}

bool SystemData::SetData(eFileType fileType, const std::wstring &key, const std::wstring &value)
{
	if (fileType == FILE_TYPE_PATH)
	{
		if (key == L"EnemySpritePath")
			m_pathList[PATH_TYPE_ENEMY_SPRITE] = value;
		else if (key == L"WallSpritePath")
			m_pathList[PATH_TYPE_WALL_SPRITE] = value;
		else if (key == L"PlayerSpritePath")
			m_pathList[PATH_TYPE_PLAYER_SPRITE] = value;
		else if (key == L"BackgroundSpritePath")
			m_pathList[PATH_TYPE_BACKGROUND_SPRITE] = value;
		else if (key == L"TitleImagePath")
			m_pathList[PATH_TYPE_TITLE_IMAGE] = value;
	}
	else if (fileType == FILE_TYPE_SYSTEM)
	{
		if (key == L"PlayerSpeed")
			m_PlayerSpeed = _wtof(value.c_str());
		else if (key == L"BGStartingYBasic")
			m_BGStartingYBasic = _wtof(value.c_str());
		else if (key == L"BGStartingYNext")
			m_BGStartingYNext = _wtof(value.c_str());
		else if (key == L"OpponentStartingY")
			m_OpponentStartingY = _wtof(value.c_str());
		else if (key == L"ScrollingVelocity")
			m_ScrollingVelocity = _wtof(value.c_str());
		else if (key == L"ScrollingWeight")
			m_ScrollingWeight = _wtof(value.c_str());
		else if (key == L"ScrollingEndPosY")
			m_ScrollingEndPosY = _wtof(value.c_str());
		else if (key == L"EnemyCreateDelay")
			m_EnemyCreateDelay = _wtof(value.c_str());
		else if (key == L"WallCreateDelay")
			m_WallCreateDelay = _wtof(value.c_str());
		else if (key == L"AdditionalCreateDelay")
			m_AdditionalCreateDelay = _wtof(value.c_str());
		else if (key == L"DefaultEnemyHp")
			m_DefaultEnemyHp = static_cast<unsigned int>(_wtoi(value.c_str()));
		else if (key == L"KnockBackCoefficient")
			m_KnockBackCoefficient = _wtof(value.c_str());
		else if (key == L"CollisionThickness")
			m_CollisionThickness = _wtoi(value.c_str());
		else if (key == L"StageTimeLengthSec")
			m_StageTimeLengthSec = _wtof(value.c_str());
	}
	else if (fileType == FILE_TYPE_TAG)
	{
		if (key == L"BackgroundLayerTag")
			m_tagList[TAG_TYPE_BACKGROUND_LAYER] = _wtoi(value.c_str());
		else if (key == L"PlayerLayerTag")
			m_tagList[TAG_TYPE_PLAYER_LAYER] = _wtoi(value.c_str());
		else if (key == L"OpponentLayerTag")
			m_tagList[TAG_TYPE_OPPONENT_LAYER] = _wtoi(value.c_str());
		else if (key == L"UILayerTag")
			m_tagList[TAG_TYPE_UI_LAYER] = _wtoi(value.c_str());
		else if (key == L"InputLayerTag")
			m_tagList[TAG_TYPE_INPUT_LAYER] = _wtoi(value.c_str());
		else if (key == L"TitleLayerTag")
			m_tagList[TAG_TYPE_TITLE_LAYER] = _wtoi(value.c_str());
		else if (key == L"PlayerTag")
			m_tagList[TAG_TYPE_PLAYER_SPRITE] = _wtoi(value.c_str());
		else if (key == L"EnemyStartTag")
			m_tagList[TAG_TYPE_ENEMY_START] = _wtoi(value.c_str());
		else if (key == L"WallStartTag")
			m_tagList[TAG_TYPE_WALL_START] = _wtoi(value.c_str());
	}
	else
		return false;

	return true;
}