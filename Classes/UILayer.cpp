#include "UILayer.h"

#include "EventManager.h"

USING_NS_CC;

// todo. need menu UI & game pause

UILayer::UILayer()
{
	
}

UILayer::~UILayer()
{

}

bool UILayer::init()
{
	if (EventManager::GetInstance())
	{
		m_listenerIdList[EVENT_TYPE_GAMEOVER] = EventManager::GetInstance()->AddEventListener(
			EVENT_TYPE_GAMEOVER, std::bind(&UILayer::EventGameOver, this, std::placeholders::_1));

		m_listenerIdList[EVENT_TYPE_NEXT_STAGE] = EventManager::GetInstance()->AddEventListener(
			EVENT_TYPE_NEXT_STAGE, std::bind(&UILayer::EventNextStage, this, std::placeholders::_1));

		m_listenerIdList[EVENT_TYPE_INPUT] = EventManager::GetInstance()->AddEventListener(
			EVENT_TYPE_INPUT, std::bind(&UILayer::EventInput, this, std::placeholders::_1));
	}

	scheduleUpdate();

	return true;
}

void UILayer::onEnter()
{
	// super
	Layer::onEnter();

	m_isGameOver = false;
	m_isRestart = false;

	m_isShownStageAlertUI = false;
	m_curStageAlertUIDelay = 0.f;

	m_curKillCount = 0;

	CreateMenuButton();
	CreateTimerUI();
	CreateKillCountUI();
}

void UILayer::onExit()
{
	// super
	Layer::onExit();

	if (EventManager::GetInstance())
	{
		EventManager::GetInstance()->RemoveEventListener(EVENT_TYPE_GAMEOVER, m_listenerIdList[EVENT_TYPE_GAMEOVER]);
		EventManager::GetInstance()->RemoveEventListener(EVENT_TYPE_NEXT_STAGE, m_listenerIdList[EVENT_TYPE_NEXT_STAGE]);
		EventManager::GetInstance()->RemoveEventListener(EVENT_TYPE_INPUT, m_listenerIdList[EVENT_TYPE_INPUT]);
	}
}

void UILayer::update(float delta)
{
	if(m_isGameOver == false)
		UpdateTimerUI();

	if (m_isShownStageAlertUI == true)
	{
		m_curStageAlertUIDelay += delta;
		if (1.f <= m_curStageAlertUIDelay)
		{
			RemoveNextStageUI();
			m_curStageAlertUIDelay = 0.f;
		}
	}

	// event listener function에서 바로 호출하면 문제 생김.
	// todo. event handler가 queueing을 통해 다음 프레임에 처리할 수 있도록 처리 필요.
	if (m_isRestart == true)
	{
		EventSystemMsg msg;
		msg.type = EVENT_TYPE_RESTART;
		EventManager::GetInstance()->NotifyEvent(msg);

		m_isRestart = false;
	}

	if (GameManager::GetInstance())
	{
		if (GameManager::GetInstance()->GetKillCount() != m_curKillCount)
			UpdateKillCountUI();
	}
}

Layer *UILayer::CreateLayer()
{
	return UILayer::create();
}

void UILayer::CreateMenuButton()
{
	Vec2 origin = Director::getInstance() ? Director::getInstance()->getVisibleOrigin() : Vec2();
	Size visibleSize = Director::getInstance() ? Director::getInstance()->getVisibleSize() : Size(0.f, 0.f);

	MenuItemImage *pCloseItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png", CC_CALLBACK_1(UILayer::MenuBtnCallback, this));
	if (pCloseItem == nullptr)
		return;

	pCloseItem->setPosition(Vec2(origin.x + visibleSize.width - pCloseItem->getContentSize().width / 2,
		origin.y + visibleSize.height - pCloseItem->getContentSize().height / 2 - 30));

	Menu *pMenu = Menu::create(pCloseItem, NULL);
	if (pMenu)
	{
		pMenu->setPosition(Vec2::ZERO);
		this->addChild(pMenu);
	}
}

void UILayer::SetGameOverUI()
{
	Label *pLabel = Label::createWithTTF("Game Over !", "fonts\\arial.ttf", 32);
	if (pLabel == nullptr)
		return;

	if (Director::getInstance() == nullptr)
		return;

	const Size &visibleSize = Director::getInstance()->getVisibleSize();

	pLabel->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	pLabel->setColor(Color3B::RED);
	pLabel->enableShadow();
	
	this->addChild(pLabel);

	float curPlayTime = GameManager::GetInstance()->GetCurPlayTime();
	unsigned int totalScore = (m_curKillCount * 2) + static_cast<int>(curPlayTime);
	std::string totalScoreString = "Total Score : " + std::to_string(totalScore);

	Label *pScoreLabel = Label::createWithTTF(totalScoreString, "fonts\\arial.ttf", 32);
	if (pScoreLabel == nullptr)
		return;

	pScoreLabel->setPosition(visibleSize.width / 2, (visibleSize.height / 2) - (pLabel->getContentSize().height));
	pScoreLabel->setColor(Color3B::RED);
	pScoreLabel->enableShadow();

	this->addChild(pScoreLabel);
}

void UILayer::CreateRestartButton()
{
	Vec2 origin = Director::getInstance() ? Director::getInstance()->getVisibleOrigin() : Vec2();
	Size visibleSize = Director::getInstance() ? Director::getInstance()->getVisibleSize() : Size(0.f, 0.f);

	MenuItemImage *pRestartImg = MenuItemImage::create("restart_normal.png", "restart_press.png", CC_CALLBACK_1(UILayer::RestartBtnCallback, this));
	if (pRestartImg == nullptr)
		return;

	pRestartImg->setPosition(Vec2((visibleSize.width / 2), (visibleSize.height / 2) - 100));

	Menu *pMenu = Menu::create(pRestartImg, NULL);
	if (pMenu)
	{
		pMenu->setPosition(Vec2::ZERO);
		this->addChild(pMenu);
	}
}

void UILayer::ShowNextStageUI(unsigned int curStageNum)
{
	std::string str = "Stage " + std::to_string(curStageNum);
	Label *pLabel = Label::createWithTTF(str, "fonts\\arial.ttf", 32);
	if (pLabel == nullptr)
		return;

	if (Director::getInstance() == nullptr)
		return;

	const Size &visibleSize = Director::getInstance()->getVisibleSize();

	pLabel->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	pLabel->setColor(Color3B::BLUE);
	pLabel->enableShadow();
	pLabel->setName("NextStageUI");

	this->addChild(pLabel);

	m_isShownStageAlertUI = true;
}

void UILayer::RemoveNextStageUI()
{
	this->removeChildByName("NextStageUI");
	m_isShownStageAlertUI = false;
}

void UILayer::CreateTimerUI()
{
	std::string timerString = "00:00";

	Label *pLabel = Label::createWithTTF(timerString.c_str(), "fonts\\arial.ttf", 20);
	if (pLabel == nullptr)
		return;

	if (Director::getInstance() == nullptr)
		return;

	const Size &visibleSize = Director::getInstance()->getVisibleSize();
	float positionX = 10 + pLabel->getContentSize().width / 2;
	float positionY = visibleSize.height - (pLabel->getContentSize().height / 2) - 10;

	pLabel->setPosition(positionX, positionY);
	pLabel->setColor(Color3B::RED);
	pLabel->enableShadow();
	pLabel->setName("TimerUI");

	this->addChild(pLabel);
}

void UILayer::UpdateTimerUI()
{
	Node *pTimerUI = this->getChildByName("TimerUI");
	if (pTimerUI == nullptr)
		return;

	if (GameManager::GetInstance() == nullptr)
		return;

	// todo. need refactoring clearly
	float curPlayTime = GameManager::GetInstance()->GetCurPlayTime();
	int playSecond = static_cast<int>(curPlayTime);
	curPlayTime -= static_cast<float>(playSecond);
	int playMS = static_cast<int>(curPlayTime * 100);
	std::string timerString = std::to_string(playSecond) + ":" + std::to_string(playMS);

	((Label *)pTimerUI)->setString(timerString);
}

void UILayer::CreateKillCountUI()
{
	std::string killCountString = "KILL : 0";

	Label *pLabel = Label::createWithTTF(killCountString.c_str(), "fonts\\arial.ttf", 20);
	if (pLabel == nullptr)
		return;

	if (Director::getInstance() == nullptr)
		return;

	const Size &visibleSize = Director::getInstance()->getVisibleSize();
	float positionX = visibleSize.width - pLabel->getContentSize().width / 2 - 10;
	float positionY = visibleSize.height - (pLabel->getContentSize().height / 2) - 10;

	pLabel->setPosition(positionX, positionY);
	pLabel->setColor(Color3B::RED);
	pLabel->enableShadow();
	pLabel->setName("KillCountUI");

	this->addChild(pLabel);
}

void UILayer::UpdateKillCountUI()
{
	Node *pKillCountUI = this->getChildByName("KillCountUI");
	if (pKillCountUI == nullptr)
		return;

	if (GameManager::GetInstance() == nullptr)
		return;

	m_curKillCount = GameManager::GetInstance()->GetKillCount();
	std::string killCountString = "KILL : " + std::to_string(m_curKillCount);
	((Label *)pKillCountUI)->setString(killCountString);
}

void UILayer::MenuBtnCallback(Ref* pSender)
{
	if (Director::getInstance())
		Director::getInstance()->end();
}

void UILayer::RestartBtnCallback(Ref *pSender)
{
	pSender;

	EventSystemMsg msg;
	msg.type = EVENT_TYPE_RESTART;
	EventManager::GetInstance()->NotifyEvent(msg);
}

void UILayer::EventGameOver(EventSystemMsg msg)
{
	if (msg.type != EVENT_TYPE_GAMEOVER)
		return;

	m_isGameOver = true;

	SetGameOverUI();
	CreateRestartButton();
}

void UILayer::EventNextStage(EventSystemMsg msg)
{
	if (msg.type != EVENT_TYPE_NEXT_STAGE)
		return;

	unsigned int curStage = static_cast<unsigned int>(msg.value);
	ShowNextStageUI(curStage);
}

void UILayer::EventInput(EventInputMsg msg)
{
	if (msg.type != EVENT_TYPE_INPUT)
		return;

	switch(msg.inputState)
	{
		case INPUT_KEY_ENTER:
		{
			if (m_isGameOver == true && msg.isPressed == true)
			{
				m_isRestart = true;
			}
		}break;
		case INPUT_KEY_ESC:
		{
			if (Director::getInstance())
				Director::getInstance()->end();
		}break;
	}
}