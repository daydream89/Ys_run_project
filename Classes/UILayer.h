#ifndef __UI_LAYER_H__
#define __UI_LAYER_H__

#pragma once

#include "BaseLayer.h"

class UILayer : public BaseLayer
{
public:
	static cocos2d::Layer *CreateLayer();

	UILayer();
	virtual ~UILayer();

	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	virtual void update(float delta);

	void CreateMenuButton();
	void SetGameOverUI();

	void CreateRestartButton();

	void ShowNextStageUI(unsigned int curStageNum);
	void RemoveNextStageUI();

	void CreateTimerUI();
	void UpdateTimerUI();

	void CreateKillCountUI();
	void UpdateKillCountUI();

	void MenuBtnCallback(Ref *pSender);
	void RestartBtnCallback(Ref *pSender);

	void EventGameOver(EventSystemMsg msg);
	void EventNextStage(EventSystemMsg msg);
	void EventInput(EventInputMsg msg);

	CREATE_FUNC(UILayer);
	
private:
	bool m_isGameOver;
	bool m_isRestart;
	
	bool m_isShownStageAlertUI;
	float m_curStageAlertUIDelay;

	unsigned int m_curKillCount;
};

#endif		// __UI_LAYER_H__