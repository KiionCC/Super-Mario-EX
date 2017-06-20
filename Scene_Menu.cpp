#include "Data_Global.h"
#include "Scene_AudioSet.h"
#include "Scene_Game.h"
#include "Scene_Menu.h"
#include "Scene_Select.h"

MenuLayer::MenuLayer()
{
	Size winSize = Director::getInstance()->getWinSize();

	Sprite *pBackground = Sprite::create("bg_Menu.png");
	pBackground->setPosition(ccp(winSize.width/2, winSize.height/2));
	this->addChild(pBackground);
	
	pStart = MenuItemImage::create("start_origin.png", "start_touch.png",
		this, menu_selector(MenuLayer::menuCallBackForStartGame));
	pStart->setPosition(ccp(winSize.width/2 - 100, winSize.height/2 - 40));

	pSetting = MenuItemImage::create("setting_origin.png", "setting_touch.png",
		this, menu_selector(MenuLayer::menuSetting));
	pSetting->setPosition(ccp(winSize.width/2 - 100, winSize.height/2 - 80));

	pQuit = MenuItemImage::create("quit_origin.png", "quit_touch.png",
		this, menu_selector(MenuLayer::menuQuit));
	pQuit->setPosition(ccp(winSize.width/2 - 100, winSize.height/2 - 120));

	pMenu = Menu::create(pStart, pSetting, pQuit, NULL);
	pMenu->setPosition(ccp(0, 0));
	this->addChild(pMenu, 0);
}

MenuLayer::~MenuLayer()
{
	this->unscheduleAllSelectors();
}

bool MenuLayer::init()
{
	this->setTouchEnabled(true);
	auto listen = EventListenerTouchOneByOne::create();
	listen->onTouchBegan = CC_CALLBACK_2(MenuLayer::onTouchBegan, this);
	listen->onTouchMoved = CC_CALLBACK_2(MenuLayer::onTouchMoved, this);
	listen->onTouchEnded = CC_CALLBACK_2(MenuLayer::onTouchEnded, this);
	listen->onTouchCancelled = CC_CALLBACK_2(MenuLayer::onTouchCancelled, this);
	listen->setSwallowTouches(true);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listen, this);

	return true;
}


void MenuLayer::menuCallBackForStartGame(Ref *pSender)
{
	Global::getGlobalInstance()->setCurrentLifeNum(3);

	SelectLevel *pSelectMenu = SelectLevel::create();
	Scene *pScene = Scene::create();
	pScene->addChild(pSelectMenu);
	Director::getInstance()->replaceScene(pScene);
}

bool MenuLayer::onTouchBegan(Touch *touch, Event * pEvent)
{
	return true;
}

void MenuLayer::onTouchEnded(Touch *touch, Event * pEvent)
{

}

void MenuLayer::onTouchCancelled(Touch *touch, Event *pEvent)
{

}

void MenuLayer::onTouchMoved(Touch *touch, Event *pEvent)
{

}

void MenuLayer::menuSetting( Ref *pSender )
{
	Scene *pScene = Scene::create();
	pScene->addChild(AudioSet::create());
	Director::getInstance()->replaceScene(pScene);
}

void MenuLayer::menuQuit( Ref *pSender )
{
	Director::getInstance()->end();
}
