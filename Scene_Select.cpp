#include <string>

#include "Data_Global.h"
#include "Scene_Game.h"
#include "Scene_Menu.h"
#include "Scene_Select.h"
using namespace std;


bool SelectLevel::init()
{
	bool bRet = false;

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();
	do
	{
		CC_BREAK_IF( !Layer::init() );

		m_nCurPage = 1;

		pLayer = Layer::create();
		CCString *seleteName = NULL;
		for (int i = 1; i <= Global::getGlobalInstance()->getTotalLevels(); ++i)
		{
			seleteName = CCString::createWithFormat("select%d.png", i);
			Sprite *pSprite = Sprite::create(seleteName->getCString());
			pSprite->setPosition(ccp(visibleSize.width * (i-0.5f), visibleSize.height/2 + 10));
			pLayer->addChild(pSprite, 0, i);
		}

		pLayer->setContentSize(CCSizeMake(480*8, 320));
		this->addChild(pLayer, 1);

		Sprite *pBg = Sprite::create("bg2.png");
		pBg->setPosition(ccp(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
		this->addChild(pBg, 0);

		pNewGame = MenuItemImage::create("newgameA.png", "newgameB.png",
			this, menu_selector(SelectLevel::menuBegin));
		pNewGame->setPosition(ccp(origin.x + visibleSize.width/2, 30));

		MenuItemImage *pBack = MenuItemImage::create("backA.png", "backB.png",
			this, menu_selector(SelectLevel::menuBack));
		pBack->setPosition(ccp(origin.x + 20, origin.y + visibleSize.height - 20));

		Menu *pMenu = Menu::create(pNewGame, pBack, NULL);
		pMenu->setAnchorPoint(Vec2(0,0));
		pMenu->setPosition(Vec2(0,0));
		this->addChild(pMenu, 2);

		m_pLevel = CCLabelTTF::create("Level: 1", "Arial", 20);
		m_pLevel->setPosition(ccp(origin.x + visibleSize.width/2, origin.y + visibleSize.height - 20));
		this->addChild(m_pLevel, 3);

		bRet = true;
	}while(0);
	
	return bRet;

}

void SelectLevel::menuCloseCallback(Ref* pSender)
{

}

void SelectLevel::onEnter()
{
	Layer::onEnter();

	CCString *ccStr = NULL;
	string str;
	Sprite *pSp = NULL;
	for (int i = 2; i <= 8; ++i)
	{
		ccStr = CCString::createWithFormat("Level%d", i);
		str = UserDefault::sharedUserDefault()->getStringForKey(ccStr->getCString());
		if (str == "no")
		{
			LayerColor *pColor = LayerColor::create(ccc4(0, 0, 0, 200));
			pSp = (Sprite*)pLayer->getChildByTag(i);
			pColor->ignoreAnchorPointForPosition(false);
			pColor->setAnchorPoint(ccp(0.5, 0.5));
			pColor->setPosition(pSp->getPosition());
			pColor->setContentSize(pSp->getContentSize());
			pLayer->addChild(pColor, pSp->getZOrder() + 1);
		}
	}
}

void SelectLevel::onExit()
{
	Layer::onExit();
}


bool SelectLevel::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	m_touchPoint = Director::getInstance()->convertToGL(pTouch->getLocation());
	return true;
}

void SelectLevel::onTouchMoved(Touch *pTouch, Event *pEvent)
{

}

void SelectLevel::onTouchEnded(Touch *pTouch, Event *pEvent)
{
	Point endPoint = Director::getInstance()->convertToGL(pTouch->getLocation());
	float distance = endPoint.x - m_touchPoint.x;
	if(fabs(distance) > 5)
	{
		adjustView(distance);
	}
}

void SelectLevel::onTouchCancelled(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	Point endPoint = Director::getInstance()->convertToGL(pTouch->getLocation());
	float distance = endPoint.x - m_touchPoint.x;
	if(fabs(distance) > 5)
	{
		adjustView(distance);
	}
}

void SelectLevel::adjustView(float offset)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	if (offset<0)
	{
		++m_nCurPage;
	}else
	{
		--m_nCurPage;
	}

	if (m_nCurPage <1)
	{
		m_nCurPage = 1;
	}
	if (m_nCurPage > 8)
	{
		m_nCurPage = 8;
	}

	Point adjustPos = ccp(origin.x - visibleSize.width * (m_nCurPage-1), 0);
	pLayer->runAction(CCMoveTo::create(0.2f, adjustPos));


	CCString *ccStr = CCString::createWithFormat("Level: %d", m_nCurPage);
	m_pLevel->setString(ccStr->getCString());
	ccStr = CCString::createWithFormat("Level%d", m_nCurPage);
	string str = UserDefault::sharedUserDefault()->getStringForKey(ccStr->getCString());
	if (str == "no")
	{
		pNewGame->setEnabled(false);
	}else
	{
		pNewGame->setEnabled(true);
	}
	    
}

void SelectLevel::menuBegin( Ref *pSender ) // 进入游戏界面
{
	Global::getGlobalInstance()->setCurrentLevel(m_nCurPage);
	GameLayer *pGameLayer = GameLayer::create();

	Scene *pScene = Scene::create();
	pScene->addChild(pGameLayer);
	Director::getInstance()->replaceScene(pScene);
}

void SelectLevel::menuBack( Ref *pSender ) //返回开始界面
{
	MenuLayer *pMainMenu = MenuLayer::create();
	Scene *pScene = Scene::create();
	pScene->addChild(pMainMenu);
	Director::getInstance()->replaceScene(pScene);
}
