#include "TitleScene.h"

#include "EventManager.h"
#include "SystemData.h"

#include "InGameScene.h"

#include "InputLayer.h"
#include "TitleLayer.h"

USING_NS_CC;

Scene* TitleScene::createScene()
{
	return TitleScene::create();
}

// on "init" you need to initialize your instance
bool TitleScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	if (GameManager::GetInstance())
	{
		GameManager::GetInstance()->SetCurScene(SCENE_NAME_TITLE);
	}

	if (EventManager::GetInstance())
	{
		m_listenerIdList[EVENT_TYPE_INPUT] = EventManager::GetInstance()->AddEventListener(
			EVENT_TYPE_INPUT, std::bind(&TitleScene::EventInput, this, std::placeholders::_1));
	}

	return true;
}

void TitleScene::onEnter()
{
	// super
	Scene::onEnter();

	// layer create
	AttachLayer();

	scheduleUpdate();
}

void TitleScene::onExit()
{
	// super
	Scene::onExit();

	if (EventManager::GetInstance())
	{
		EventManager::GetInstance()->RemoveEventListener(EVENT_TYPE_INPUT, m_listenerIdList[EVENT_TYPE_INPUT]);
	}
}

bool TitleScene::AttachLayer()
{
	if (SystemData::GetInstance() == false)
		return false;

	InputLayer *pInputLayer = (InputLayer *)InputLayer::CreateLayer();
	if (pInputLayer)
	{
		unsigned int tag = SystemData::GetInstance()->GetInputLayerTag();
		pInputLayer->setTag(tag);
		this->addChild(pInputLayer, 4);
	}

	TitleLayer *pTitleLayer = (TitleLayer *)TitleLayer::CreateLayer();
	if (pTitleLayer)
	{
		unsigned int tag = SystemData::GetInstance()->GetTitleLayerTag();
		pTitleLayer->setTag(tag);
		this->addChild(pTitleLayer, 5);
	}

	return true;
}

bool TitleScene::DetachLayer()
{
	this->removeAllChildren();

	return true;
}

void TitleScene::EventInput(EventInputMsg msg)
{
	if (msg.type != EVENT_TYPE_INPUT)
		return;

	if (Director::getInstance() && GameManager::GetInstance())
	{
		eScene curScene = GameManager::GetInstance()->GetCurScene();
		if (curScene == SCENE_NAME_TITLE)
		{
			Scene *pScene = InGameScene::createScene();
			if (pScene)
			{
				Director::getInstance()->replaceScene(pScene);
			}
		}
	}
}