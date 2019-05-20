#include "InGameScene.h"

#include "EventManager.h"

#include "SystemData.h"

#include "BackGroundLayer.h"
#include "InputLayer.h"
#include "OpponentLayer.h"
#include "PlayerLayer.h"
#include "UILayer.h"

USING_NS_CC;

Scene* InGameScene::createScene()
{
    return InGameScene::create();
}

InGameScene::InGameScene()
{

}

InGameScene::~InGameScene()
{

}

// on "init" you need to initialize your instance
bool InGameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

	if (GameManager::GetInstance())
	{
		GameManager::GetInstance()->SetCurScene(SCENE_NAME_INGAME);
	}

	if (EventManager::GetInstance())
	{
		m_listenerIdList[EVENT_TYPE_RESTART] = EventManager::GetInstance()->AddEventListener(
			EVENT_TYPE_RESTART, std::bind(&InGameScene::EventRestart, this, std::placeholders::_1));
	}

    return true;
}

void InGameScene::onEnter()
{
	// super
	Scene::onEnter();

	// layer create
	AttachLayer();

	scheduleUpdate();
}

void InGameScene::onExit()
{
	// super
	Scene::onExit();

	if (EventManager::GetInstance())
	{
		EventManager::GetInstance()->RemoveEventListener(EVENT_TYPE_RESTART, m_listenerIdList[EVENT_TYPE_RESTART]);
	}
}

// for game manager update
void InGameScene::update(float delta)
{
	// super
	Scene::update(delta);

	if(GameManager::GetInstance())
		GameManager::GetInstance()->Update(delta);
}

bool InGameScene::AttachLayer()
{
	if (SystemData::GetInstance() == false)
		return false;

	BackGroundLayer *bgLayer = (BackGroundLayer *)BackGroundLayer::CreateLayer();
	if (bgLayer)
	{
		unsigned int tag = SystemData::GetInstance()->GetBackgroundLayerTag();
		bgLayer->setTag(tag);
		this->addChild(bgLayer, 0);
	}

	PlayerLayer *pPlayerLayer = (PlayerLayer *)PlayerLayer::CreateLayer();
	if (pPlayerLayer)
	{
		unsigned int tag = SystemData::GetInstance()->GetPlayerLayerTag();
		pPlayerLayer->setTag(tag);
		this->addChild(pPlayerLayer, 1);

		// todo. need to move another position.
		// GameManager sends event game start is ready. then received part call this function.
		pPlayerLayer->CreatePlayer();
	}

	OpponentLayer *pOpponentLayer = (OpponentLayer *)OpponentLayer::CreateLayer();
	if (pOpponentLayer)
	{
		unsigned int tag = SystemData::GetInstance()->GetOpponentLayerTag();
		pOpponentLayer->setTag(tag);
		this->addChild(pOpponentLayer, 2);
	}

	UILayer *pUILayer = (UILayer *)UILayer::CreateLayer();
	if (pUILayer)
	{
		unsigned int tag = SystemData::GetInstance()->GetUILayerTag();
		pUILayer->setTag(tag);
		this->addChild(pUILayer, 3);
	}

	InputLayer *pInputLayer = (InputLayer *)InputLayer::CreateLayer();
	if (pInputLayer)
	{
		unsigned int tag = SystemData::GetInstance()->GetInputLayerTag();
		pInputLayer->setTag(tag);
		this->addChild(pInputLayer, 4);
	}

	return true;
}

bool InGameScene::DetachLayer()
{
	this->removeAllChildren();

	return true;
}

void InGameScene::EventRestart(EventSystemMsg msg)
{
	if (msg.type != EVENT_TYPE_RESTART)
		return;

	if (Director::getInstance())
	{
		Scene *pScene = Director::getInstance()->getRunningScene();
		if (pScene)
		{
			((InGameScene *)pScene)->DetachLayer();
			((InGameScene *)pScene)->AttachLayer();
		}
	}
}