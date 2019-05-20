#include "TitleLayer.h"

#include "SystemData.h"

USING_NS_CC;

#define OPACITY_CHANGE 2
#define OPACITY_MIN 0
#define OPACITY_MAX 255

TitleLayer::TitleLayer()
{
	
}

TitleLayer::~TitleLayer()
{

}

bool TitleLayer::init()
{
	scheduleUpdate();

	return true;
}

void TitleLayer::onEnter()
{
	// super
	Layer::onEnter();

	CreateTitle();
	CreatePressAnyKey();

	m_isTextVanishing = true;
}

void TitleLayer::onExit()
{
	// super
	Layer::onExit();
}

void TitleLayer::update(float delta)
{
	UpdatePressAnyKey(delta);
}

Layer *TitleLayer::CreateLayer()
{
	return TitleLayer::create();
}

void TitleLayer::CreateTitle()
{
	if (SystemData::GetInstance() == false)
		return;

	std::wstring wcsPath = SystemData::GetInstance()->GetTitleImagePath();
	char pTitlePath[MAX_STR_LEN];
	size_t destSize = 0;
	wcstombs_s(&destSize, pTitlePath, MAX_STR_LEN, wcsPath.c_str(), wcsPath.size());

	Sprite *pTitle = Sprite::create(pTitlePath);
	if (pTitle)
	{
		pTitle->setAnchorPoint(Point(Point::ZERO));
		this->addChild(pTitle);
	}
}

void TitleLayer::CreatePressAnyKey()
{
	Label *pLabel = Label::createWithTTF("Press Arrow Key", "fonts\\arial.ttf", 20);
	if (pLabel == nullptr)
		return;

	if (Director::getInstance() == nullptr)
		return;

	const Size &visibleSize = Director::getInstance()->getVisibleSize();

	pLabel->setPosition(visibleSize.width / 2, (visibleSize.height / 2) - 40);
	pLabel->setColor(Color3B::YELLOW);
	pLabel->setName("blinkText");

	this->addChild(pLabel);

	m_isTextVanishing = true;
}

void TitleLayer::UpdatePressAnyKey(float elapsedTime)
{
	Node *pNode = this->getChildByName("blinkText");
	if (pNode == nullptr)
		return;

	GLubyte curOpacity = pNode->getOpacity();
	if (m_isTextVanishing)
	{
		if (curOpacity - OPACITY_CHANGE <= OPACITY_MIN)
		{
			curOpacity = OPACITY_MIN;
			m_isTextVanishing = false;
		}
		else
			curOpacity -= OPACITY_CHANGE;
	}
	else
	{
		if (OPACITY_MAX <= curOpacity + OPACITY_CHANGE)
		{
			curOpacity = OPACITY_MAX;
			m_isTextVanishing = true;
		}
		else
			curOpacity += OPACITY_CHANGE;
	}

	pNode->setOpacity(curOpacity);
}