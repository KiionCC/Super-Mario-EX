#include "SimpleAudioEngine.h"

#include "AnimationManager.h"
#include "Data_Global.h"
#include "Resource_Item.h"
#include "Resource_Map.h"
#include "Resource_Role.h"
#include "Scene_Game.h"
#include "Scene_Menu.h"
#include "Scene_Select.h"

float GameLayer::mapMaxH;

GameLayer::GameLayer() :
	mainMap(NULL),
	mario(NULL)
{
	mapMaxH = 0.0f;
	mapBeginPos = ccp(0.0f, 96.0f);

	birthPoint.x = 180.0f;
	birthPoint.y = 32.0f;

	isKeyDownA = false;
	isKeyDownD = false;
	isSky = false;
	marioFireable = true;

	moveOffset = 0.0f;
	moveDelta = 0.0f;
	JumpOffset = 0.0f;

	ccMoveDelta = 0.05f;
	ccMoveOffset = 2.0f;
	ccJumpOffset = 0.3f;

	currentPos.x = 0.0f;
	currentPos.y = 0.0f;

	Size winsize = Director::getInstance()->getWinSize();
	marioAnchor = ccp(winsize.width / 2 - 80, winsize.height / 2);

	backKeyPos = ccp(84, 48);
	leftKeyPos = ccp(40, 48);
	rightKeyPos = ccp(128, 48);
	jumpKeyPos = ccp(432, 35);
	fireKeyPos = ccp(353, 35);
	MSetKeyPos = ccp(260, 33);

	FireBallPos = ccp(winsize.width - 70, winsize.height - 20);
	arrowPos = ccp(winsize.width - 30, winsize.height - 20);

	mainLayer = Layer::create();
	mainLayer->retain();

	isPass = false;

	isLeftKeyDown = false;
	isRightKeyDown = false;
	isJumpKeyDown = false;
	isFireKeyDown = false;
}

GameLayer::~GameLayer()
{
	this->unscheduleUpdate();
	this->unscheduleAllSelectors();
}

void GameLayer::onEnter()
{
	Layer::onEnter();
	UserDefault *defaults = UserDefault::getInstance();
	if(!defaults->getBoolForKey(MUSIC_KEY))
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("OnLand.wav",true);
}

void GameLayer::onExit()
{
	Layer::onExit();
}

bool GameLayer::init()
{
	this->initMarioAndMap();

	this->initcontrolUI();

	this->initBulletUI();

	this->initRect();

	this->initSetMenu();

	//后面检查
	auto touchMore = EventListenerTouchAllAtOnce::create();
	touchMore->onTouchesBegan = CC_CALLBACK_2(GameLayer::onTouchesBegan, this);
	touchMore->onTouchesMoved = CC_CALLBACK_2(GameLayer::onTouchesMoved, this);
	touchMore->onTouchesEnded = CC_CALLBACK_2(GameLayer::onTouchesEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchMore, this);

	//键盘事件
	auto keyboardListen = EventListenerKeyboard::create();
	keyboardListen->onKeyPressed = CC_CALLBACK_2(GameLayer::onKeyPressed, this);
	keyboardListen->onKeyReleased = CC_CALLBACK_2(GameLayer::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListen, this);

	this->setTouchEnabled(true);
	this->scheduleUpdate();

	return true;
}

void GameLayer::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_A:
		isLeftKeyDown = true;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_D:
		isRightKeyDown = true;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_J:
		isJumpKeyDown = true;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_K:
		isFireKeyDown = true;
		break;
	default:
		break;
	}
}

void GameLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_A:
		isLeftKeyDown = false;
		isKeyDownA = false;
		moveOffset = 0.0f;
		moveDelta = 0.0f;
		mario->setMarioState(eNormalLeft);
		pBackKeyImage->setDisplayFrame(pBackKeyNormal);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_D:
		isRightKeyDown = false;
		isKeyDownD = false;
		moveOffset = 0.0f;
		moveDelta = 0.0f;
		mario->setMarioState(eNormalRight);
		pBackKeyImage->setDisplayFrame(pBackKeyNormal);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_J:
		isJumpKeyDown = false;
		pJumpImage->setDisplayFrame(pAB_Normal);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_K:
		isFireKeyDown = false;
		pFireImage->setDisplayFrame(pAB_Normal);
		break;
	default:
		break;
	}
}

void GameLayer::initSetMenu()
{
	pColor = LayerColor::create(ccc4(0, 0, 0, 100));
	this->addChild(pColor, this->getChildrenCount());
	pColor->setVisible(false);

	pSetMenu = Sprite::create("Set_Menu.png");
	pSetMenu->setAnchorPoint(ccp(129.0 / 248, 71.0 / 132));
	Point origin = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	pSetMenu->setPosition(ccp(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	this->addChild(pSetMenu, this->getChildrenCount() + 1);
	pSetMenu->setVisible(false);

	pResume = MenuItemImage::create("resume_n.png", "resume_s.png",
		this, menu_selector(GameLayer::menuResume));
	pResume->setPosition(ccp(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 40));
	pResume->setVisible(false);
	pResume->setEnabled(false);

	pReStart = MenuItemImage::create("restart_n.png", "restart_s.png",
		this, menu_selector(GameLayer::menuReStart));
	pReStart->setPosition(ccp(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	pReStart->setVisible(false);
	pReStart->setEnabled(false);

	pSelectMenu = MenuItemImage::create("select_n.png", "select_s.png",
		this, menu_selector(GameLayer::menuSelectMenu));
	pSelectMenu->setPosition(ccp(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 40));
	pSelectMenu->setVisible(false);
	pSelectMenu->setEnabled(false);

	pMenu->addChild(pResume, pMenu->getChildrenCount());
	pMenu->addChild(pReStart, pMenu->getChildrenCount());
	pMenu->addChild(pSelectMenu, pMenu->getChildrenCount());
}

void GameLayer::menuMSet(Ref *pSender)
{
	this->pauseGameLayer();
}

void GameLayer::menuResume(Ref *pSender)
{
	this->resumeGameLayer();
}

void GameLayer::menuReStart(Ref *pSender)
{
	Scene *pScene = Scene::create();
	pScene->addChild(GameLayer::create());
	Director::getInstance()->replaceScene(pScene);
}

void GameLayer::menuSelectMenu(Ref *pSender)
{
	Scene *pScene = Scene::create();
	pScene->addChild(SelectLevel::create());
	Director::getInstance()->replaceScene(pScene);
}

void GameLayer::initRect()
{
	leftKeyRect = CCRectMake(leftKeyPos.x - pLeftKey->getContentSize().width / 2,
		leftKeyPos.y - pLeftKey->getContentSize().height / 2,
		pLeftKey->getContentSize().width,
		pLeftKey->getContentSize().height);

	rightKeyRect = CCRectMake(rightKeyPos.x - pRightKey->getContentSize().width / 2,
		rightKeyPos.y - pRightKey->getContentSize().height / 2,
		pRightKey->getContentSize().width,
		pRightKey->getContentSize().height);

	jumpKeyRect = CCRectMake(jumpKeyPos.x - pJump->getContentSize().width / 2,
		jumpKeyPos.y - pJump->getContentSize().height / 2,
		pJump->getContentSize().width,
		pJump->getContentSize().height);

	fireKeyRect = CCRectMake(fireKeyPos.x - pFire->getContentSize().width / 2,
		fireKeyPos.y - pFire->getContentSize().height / 2,
		pFire->getContentSize().width,
		pFire->getContentSize().height);
}

void GameLayer::initMarioAndMap()
{
	char temp[20] = { 0 };
	sprintf(temp, "MarioMap%d.tmx", Global::getGlobalInstance()->getCurrentLevel());
	mainMap = GameMap::create(temp);

	mapSize = CCSizeMake(mainMap->getMapSize().width * mainMap->getTileSize().width,
		mainMap->getMapSize().height * mainMap->getTileSize().height);
	mainMap->setPosition(ccp(0, 0));
	mainLayer->addChild(mainMap);
	mario = Mario::create();
	mario->setBodyType(Global::getGlobalInstance()->currentMarioType);
	mario->setAnchorPoint(ccp(0.5f, 0.0f));
	mario->setPosition(mainMap->getMarioBirthPos());
	marioSize = mario->getCurrentSize();
	mainLayer->addChild(mario);
	mainLayer->setPosition(mapBeginPos);
	this->addChild(mainLayer);
}

void GameLayer::loadMap()
{

}

void GameLayer::initcontrolUI()
{
	controlUI = Sprite::create("controlUI.png");
	controlUI->setAnchorPoint(ccp(0, 0));
	this->addChild(controlUI);

	Size winSize = Director::getInstance()->getWinSize();

	pGameOverBack = Sprite::create("gameover.png");
	pGameOverBack->setPosition(ccp(winSize.width / 2, winSize.height / 2 + 50));
	this->addChild(pGameOverBack);
	pGameOverBack->setVisible(false);

	pPassFailure = Sprite::create("PassFailure.png");
	pPassFailure->setPosition(ccp(winSize.width / 2, winSize.height / 2 + 50));
	this->addChild(pPassFailure);
	pPassFailure->setVisible(false);

	pPassSuccess = Sprite::create("PassSuccess.png");
	pPassSuccess->setPosition(ccp(winSize.width / 2, winSize.height / 2 + 50));
	this->addChild(pPassSuccess);
	pPassSuccess->setVisible(false);


	winSize = Director::getInstance()->getWinSize();

	pBackKeyImage = Sprite::create("backKeyImage.png");
	pBackKeyImage->setPosition(backKeyPos);
	this->addChild(pBackKeyImage);

	pBackKeyNormal = SpriteFrame::create("backKeyImage.png", CCRectMake(0, 0, 72, 72));
	pBackKeyNormal->retain();
	pBackKeyLeft = SpriteFrame::create("backKeyLeft.png", CCRectMake(0, 0, 72, 72));
	pBackKeyLeft->retain();
	pBackKeyRight = SpriteFrame::create("backKeyRight.png", CCRectMake(0, 0, 72, 72));
	pBackKeyRight->retain();

	pAB_Normal = SpriteFrame::create("AB_normal.png", CCRectMake(0, 0, 72, 50));
	pAB_Normal->retain();
	pAB_Selected = SpriteFrame::create("AB_select.png", CCRectMake(0, 0, 72, 50));
	pAB_Selected->retain();

	pJumpImage = Sprite::createWithSpriteFrame(pAB_Normal);
	pJumpImage->setPosition(jumpKeyPos);
	this->addChild(pJumpImage, 3);

	pFireImage = Sprite::createWithSpriteFrame(pAB_Normal);
	pFireImage->setPosition(fireKeyPos);
	this->addChild(pFireImage, 3);

	pLeftKey = MenuItemImage::create("leftright.png", "leftright.png",
		this, menu_selector(GameLayer::menuCallBackLeftKey));
	pRightKey = MenuItemImage::create("leftright.png", "leftright.png",
		this, menu_selector(GameLayer::menuCallBackRightKey));
	pJump = MenuItemImage::create("AB_normal.png", "AB_select.png",
		this, menu_selector(GameLayer::menuCallBackJumpKey));
	pFire = MenuItemImage::create("AB_normal.png", "AB_select.png",
		this, menu_selector(GameLayer::menuCallBackFireKey));
	pMSet = MenuItemImage::create("M_n.png", "M_s.png",
		this, menu_selector(GameLayer::menuMSet));

	pBackToMenu = MenuItemImage::create("backToMenu.png", "backToMenu.png",
		this, menu_selector(GameLayer::menuCallBackBackToMenu));
	pBackToMenu->setEnabled(false);
	pBackToMenu->setVisible(false);

	pNext = MenuItemImage::create("nextlevel_normal.png", "nextlevel_select.png",
		this, menu_selector(GameLayer::menuNext));
	pNext->setPosition(ccp(winSize.width / 2, winSize.height / 2 + 40));
	pNext->setVisible(false);
	pNext->setEnabled(false);

	pRetry = MenuItemImage::create("retry_normal.png", "retry_select.png",
		this, menu_selector(GameLayer::menuRetry));
	pRetry->setPosition(ccp(winSize.width / 2, winSize.height / 2));
	pRetry->setVisible(false);
	pRetry->setEnabled(false);

	pLeftKey->setPosition(leftKeyPos);
	pRightKey->setPosition(rightKeyPos);
	pJump->setPosition(jumpKeyPos);
	pFire->setPosition(fireKeyPos);
	pMSet->setPosition(MSetKeyPos);
	pBackToMenu->setPosition(ccp(winSize.width / 2, winSize.height / 2 + 20));

	pMenu = Menu::create(pNext, pRetry, pMSet, NULL);
	pMenu->setAnchorPoint(Vec2(0, 0));
	pMenu->setPosition(ccp(0, 0));

	this->addChild(pMenu, 100);
}

void GameLayer::initBulletUI()
{
	pBulletBorderArrow = Sprite::create("bulletBorder.png");
	pBulletBorderArrow->setPosition(arrowPos);
	//pBulletBorderArrow->setScale(2.0f);

	pBulletBorderFireBall = Sprite::create("bulletBorder.png");
	pBulletBorderFireBall->setPosition(FireBallPos);

	Sprite *pArrow = Sprite::create("arrowBullet.png");

	pMenuArrow = MenuItemSprite::create(pArrow, pArrow, pArrow,
		this, menu_selector(GameLayer::menuCallBackArrow));
	pMenuArrow->setPosition(arrowPos);
	pMenu->addChild(pMenuArrow);

	Sprite *pFireBall = Sprite::create("fireBall.png");
	//pFireBall->setScale(2.0f);
	pMenuFireBall = MenuItemSprite::create(pFireBall, pFireBall, pFireBall,
		this, menu_selector(GameLayer::menuCallBackFireBall));
	pMenuFireBall->setPosition(FireBallPos);
	pMenu->addChild(pMenuFireBall);
}

void GameLayer::menuCallBackLeftKey(Ref *pSender)
{
	isKeyDownA = false;
	moveOffset = 0.0f;
	moveDelta = 0.0f;
	mario->setMarioState(eNormalLeft);
	pBackKeyImage->setDisplayFrame(pBackKeyNormal);
}

void GameLayer::menuCallBackRightKey(Ref *pSender)
{
	isKeyDownD = false;
	moveOffset = 0.0f;
	moveDelta = 0.0f;
	mario->setMarioState(eNormalRight);
	pBackKeyImage->setDisplayFrame(pBackKeyNormal);
}

void GameLayer::menuCallBackJumpKey(Ref *pSender)
{

}

void GameLayer::menuCallBackFireKey(Ref *pSender)
{

}

void GameLayer::menuCallBackArrow(Ref *pSender)
{
	if (mario->isBulletable())
	{
		Global::getGlobalInstance()->setCurrentBulletType(eBullet_arrow);
		mario->setBulletType(eBullet_arrow);
	}
}

void GameLayer::menuCallBackFireBall(Ref *pSender)
{
	if (mario->isBulletable())
	{
		Global::getGlobalInstance()->setCurrentBulletType(eBullet_common);
		mario->setBulletType(eBullet_common);
	}
}

void GameLayer::menuCallBackBackToMenu(Ref *pSender)
{
	this->toMainMenu();
}

void GameLayer::onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event)
{

}

void GameLayer::onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event)
{

}

void GameLayer::onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event)
{

}

void GameLayer::onTouchesCancelled(const std::vector<Touch*>&touches, Event *unused_event)
{

}

void GameLayer::menuCloseCallBack(Ref *pSender)
{
	this->toMainMenu();
}

void GameLayer::updateControl()
{
	if (!mario->isMarioDied())
	{
		if (isLeftKeyDown)
		{
			isKeyDownA = true;
			moveOffset = -ccMoveOffset;
			moveDelta = -ccMoveDelta;
			mario->setMarioState(eLeft);
			pBackKeyImage->setDisplayFrame(pBackKeyLeft);
		}
		else if (isRightKeyDown)
		{
			isKeyDownD = true;
			moveOffset = ccMoveOffset;
			moveDelta = ccMoveDelta;
			mario->setMarioState(eRight);
			pBackKeyImage->setDisplayFrame(pBackKeyRight);
		}
		if (isJumpKeyDown)
		{
			mario->ladderable = false;
			if (!isSky)
			{
				if (!UserDefault::getInstance()->getBoolForKey(EFFECT_KEY))
				   CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("Jump.wav");
				JumpOffset = 6.0f;
				isSky = true;
				mario->isFlying = true;
			}
			pJumpImage->setDisplayFrame(pAB_Selected);
		}
		if (isFireKeyDown)
		{
			// 发射子弹
			if (mario->isBulletable())
			{
				if (marioFireable)
				{
					if (!UserDefault::getInstance()->getBoolForKey(EFFECT_KEY))
					   CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("RengHuoQiu.wav");
					mainMap->createNewBullet();
					mario->fireAction();
					marioFireable = false;
					DelayTime *pDelay = DelayTime::create(0.5f);
					this->runAction(Sequence::create(pDelay,
						CallFunc::create(this, callfunc_selector(GameLayer::reSetMarioFireable)), NULL));

				}
			}
			pFireImage->setDisplayFrame(pAB_Selected);
		}
	}

}

void GameLayer::toMainMenu()
{
	Scene *pScene = Scene::create();
	pScene->addChild(MenuLayer::create());
	Director::getInstance()->replaceScene(pScene);
}

void GameLayer::stopForPassFailure()
{
	if (Global::getGlobalInstance()->getCurrentLifeNum() == 0)
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
		if (!UserDefault::getInstance()->getBoolForKey(EFFECT_KEY))
		   CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("GameOver.wav");
		pGameOverBack->setVisible(true);
	}
	else
	{
		pPassFailure->setVisible(true);
	}

	mainMap->stopUpdateForMarioDie();
	this->unscheduleUpdate();
	this->reSetKeyNo();

	DelayTime *pDelay = DelayTime::create(1);
	this->runAction(Sequence::create(pDelay,
		CallFunc::create(this, callfunc_selector(GameLayer::reShowPassFailure)), NULL));
}

void GameLayer::reSetKeyNo()
{
	pMenuFireBall->setEnabled(false);
	pMenuArrow->setEnabled(false);
}

void GameLayer::reShowPassFailure()
{
	if (Global::getGlobalInstance()->getCurrentLifeNum() == 0)
	{
		this->toMainMenu();
	}
	else
	{
		pRetry->setVisible(true);
		pRetry->setEnabled(true);
	}
}

void GameLayer::stopForPassSuccess()
{
	int level = Global::getGlobalInstance()->getCurrentLevel();
	CCString *str = CCString::createWithFormat("Level%d", level + 1);
	UserDefault::sharedUserDefault()->setStringForKey(str->getCString(), "yes");


	mainMap->stopUpdateForMarioDie();
	this->unscheduleUpdate();

	DelayTime *pDelay = DelayTime::create(1);
	this->runAction(Sequence::create(pDelay,
		CallFunc::create(this, callfunc_selector(GameLayer::reShowPassSuccess)), NULL));

}

void GameLayer::reShowPassSuccess()
{
	pPassSuccess->setVisible(true);
	
	if (Global::getGlobalInstance()->getCurrentLevel() == Global::getGlobalInstance()->getTotalLevels())
	{
		this->showPassAll();
	}
	else
	{
		pNext->setVisible(true);
		pNext->setEnabled(true);
	}


}

void GameLayer::showMarioJump()
{
	mario->reSetForSuccess();
	CCJumpTo *pJump = CCJumpTo::create(1.0f, mainMap->finalPoint, 32, 3);
	mario->runAction(pJump);
}

void GameLayer::update(float dt)
{
	// 是否死亡判断
	if (mario->isMarioDied())
	{
		this->stopForPassFailure();
		return;
	}
	// 是否通关判断
	if (isPass)
	{
		this->stopForPassSuccess();
		return;
	}

	this->updateControl();

	currentPos = mario->getPosition();
	marioSize = mario->getContentSize();

	if (mario->isLadderable())
	{
		currentPos.x += moveOffset + mainMap->marioInLadder->getMoveOffset();
		currentPos.y += JumpOffset + mainMap->marioInLadder->getJumpOffset();
	}
	else
	{
		currentPos.x += moveOffset;
		currentPos.y += JumpOffset;
	}
	if (isSky)
	{
		switch (mario->face)
		{
		case eLeft:
			mario->setMarioState(eJumpLeft);
			break;
		case eRight:
			mario->setMarioState(eJumpRight);
			break;
		default:
			break;
		}
	}

	mario->setPosition(currentPos);
	setSceneScrollPosition();
	collistionV();
	collistionH();
}

void GameLayer::updateContent(float dt)
{
	currentPos = mario->getPosition();
	currentPos.x = currentPos.x + moveOffset + moveDelta;
	mario->setPosition(currentPos);
	setSceneScrollPosition();
	collistionH();
}

void GameLayer::updateDelta(float dt)
{

}

// 水平方向碰撞检测
void GameLayer::collistionH()
{
	Point currentPos = mario->getPosition();
	// 做判断不让主角移除屏幕的左侧
	if ((currentPos.x - marioSize.width / 2 - mapMaxH) <= 0)
	{
		Point pp = ccp(mapMaxH + marioSize.width / 2, currentPos.y);
		mario->setPosition(pp);
		return;
	}

	bool flag = false;
	// 右侧判断
	Point rightCollision = ccp(currentPos.x + marioSize.width / 2, currentPos.y /* + marioSize.height/2*/);
	Point rightTileCoord = mainMap->positionToTileCoord(rightCollision);
	if (mainMap->isMarioEatMushroom(rightTileCoord))
	{
		mario->changeForGotMushroom();
	}
	if (mainMap->isMarioEatAddLifeMushroom(rightTileCoord))
	{
		mario->changeForGotAddLifeMushroom();
	}
	Point rightPos = mainMap->tilecoordToPosition(rightTileCoord);
	rightPos = ccp(rightPos.x - marioSize.width / 2, currentPos.y);

	TileType tileType = mainMap->tileTypeforPos(rightTileCoord);
	switch (tileType)
	{
	case eTile_Block:
	case eTile_Pipe:
	case eTile_Land:
		mario->setPosition(rightPos);
		flag = true;
		break;
	case eTile_Flagpole:
	{
		isPass = true;
		mainMap->showFlagMove();
		this->showMarioJump();
		return;
		break;
	}
	case eTile_Coin:
		if (!UserDefault::getInstance()->getBoolForKey(EFFECT_KEY))
		   CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("EatCoin.wav");
		mainMap->getCoinLayer()->removeTileAt(rightTileCoord);
		break;
	default:
		break;
	}

	// 主角的左侧碰撞检测点
	Point leftCollision = ccp(currentPos.x - marioSize.width / 2, currentPos.y);
	Point leftTileCoord = mainMap->positionToTileCoord(leftCollision);
	if (mainMap->isMarioEatMushroom(leftTileCoord))
	{
		mario->changeForGotMushroom();
	}
	if (mainMap->isMarioEatAddLifeMushroom(leftTileCoord))
	{
		mario->changeForGotAddLifeMushroom();
	}
	Point leftPos = mainMap->tilecoordToPosition(leftTileCoord);
	leftPos = ccp(leftPos.x + marioSize.width / 2 + mainMap->getTileSize().width, currentPos.y);
	tileType = mainMap->tileTypeforPos(leftTileCoord);
	switch (tileType)
	{
	case eTile_Block:
	case eTile_Pipe:
	case eTile_Land:
		mario->setPosition(leftPos);
		flag = true;
		break;
	case eTile_Coin:
		if (!UserDefault::getInstance()->getBoolForKey(EFFECT_KEY))
	    	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("EatCoin.wav");
		mainMap->getCoinLayer()->removeTileAt(leftTileCoord);
		break;
	case eTile_Flagpole:
	{
		isPass = true;
		mainMap->showFlagMove();
		this->showMarioJump();
		return;
		break;
	}
	default:
		break;
	}
}

// 垂直方向碰撞检测
void GameLayer::collistionV()
{
	Point currentPos = mario->getPosition();

	// 下面是判断马里奥的脚底是否已经接近地图下边缘的5像素处，以此为die
	if (currentPos.y <= 0)
	{
		mario->setMarioDie(true);
		mario->setPosition(ccp(currentPos.x, 1));
		mario->dieForTrap();  // 执行一段动画，表示因掉入陷阱而死
		if (!UserDefault::getInstance()->getBoolForKey(EFFECT_KEY))
		   CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("DiaoRuXianJingSi.wav");
		return;
	}

	// 下面是判断马里奥是否头顶越过游戏画面最上层
	if (currentPos.y > mapSize.height - marioSize.height - 2)
	{
		JumpOffset = 0.0f;
		mario->setPosition(ccp(currentPos.x, mapSize.height - marioSize.height - 2));
		isSky = false;

		return;
	}

	// 主角头顶的碰撞检测
	for (int marioIdx = 6; marioIdx <= marioSize.width - 6; ++marioIdx)
	{
		Point upCollision = ccp(currentPos.x - marioSize.width / 2 + marioIdx, currentPos.y + marioSize.height);
		Point upTileCoord = mainMap->positionToTileCoord(upCollision);
		// 判断是否吃到蘑菇
		if (mainMap->isMarioEatMushroom(upTileCoord))
		{
			mario->changeForGotMushroom();
		}
		if (mainMap->isMarioEatAddLifeMushroom(upTileCoord))
		{
			mario->changeForGotAddLifeMushroom();
		}
		Point upPos = mainMap->tilecoordToPosition(upTileCoord);
		upPos = ccp(currentPos.x, upPos.y - marioSize.height);
		TileType tileType = mainMap->tileTypeforPos(upTileCoord);
		bool flagUp = false;
		switch (tileType)
		{
		case eTile_Block:
		case eTile_Land:
			if (JumpOffset > 0)
			{
				mainMap->breakBlock(upTileCoord, mario->getCurrentBodyType());
				JumpOffset = 0.0f;
				mario->setPosition(upPos);
				flagUp = true;
			}
			break;
		case eTile_Coin:
			if (!UserDefault::getInstance()->getBoolForKey(EFFECT_KEY))
		     	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("EatCoin.wav");
			mainMap->getCoinLayer()->removeTileAt(upTileCoord);
			break;
		default:
			break;
		}
		if (flagUp)
		{
			JumpOffset -= ccJumpOffset;
			return;
		}
	}

	float marioLeftSide = currentPos.x - marioSize.width / 2;

	for (int marioIdx = 4; marioIdx <= marioSize.width - 4; ++marioIdx)
	{
		Point downCollision = ccp(marioLeftSide + marioIdx, currentPos.y);
		Point downTileCoord = mainMap->positionToTileCoord(downCollision);
		if (mainMap->isMarioEatMushroom(downTileCoord))
		{
			mario->changeForGotMushroom();
		}
		if (mainMap->isMarioEatAddLifeMushroom(downTileCoord))
		{
			mario->changeForGotAddLifeMushroom();
		}
		downTileCoord.y += 1;
		Point downPos = mainMap->tilecoordToPosition(downTileCoord);
		downPos = ccp(currentPos.x, downPos.y + mainMap->getTileSize().height);

		TileType tileType = mainMap->tileTypeforPos(downTileCoord);
		bool flagDown = false;
		switch (tileType)
		{
		case eTile_Flagpole:
		{
			isPass = true;
			mainMap->showFlagMove();
			this->showMarioJump();
			return;
			break;
		}
		case eTile_Coin:
			if (!UserDefault::getInstance()->getBoolForKey(EFFECT_KEY))
		    	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("EatCoin.wav");
			mainMap->getCoinLayer()->removeTileAt(downTileCoord);
			break;
		case eTile_Land:
		case eTile_Pipe:
		case eTile_Block:
		{
			if (JumpOffset < 0)
			{
				mario->setLadderable(false);
				JumpOffset = 0.0f;
				mario->setPosition(downPos);
				isSky = false;
				mario->isFlying = false;
				switch (mario->face)
				{
				case eLeft:
					if (isKeyDownA)
					{
						mario->setMarioState(eLeft);
					}
					else
					{
						mario->setMarioState(eNormalLeft);
					}
					break;
				case eRight:
					if (isKeyDownD)
					{
						mario->setMarioState(eRight);
					}
					else
					{
						mario->setMarioState(eNormalRight);
					}
					break;
				default:
					break;
				}
			}
			flagDown = true;
		}
		break;
		default:
			break;
		}
		if (flagDown)
		{
			return;
		}

		// 检测主角是否站在梯子上
		float ladderLevel = 0.0f;
		if (mainMap->isMarioInLadder(downCollision, ladderLevel))
		{
			JumpOffset = 0.0f;
			downPos = ccp(currentPos.x, ladderLevel);
			mario->setPosition(downPos);
			mario->setLadderable(true);
			isSky = false;
			mario->isFlying = false;
			switch (mario->face)
			{
			case eLeft:
				if (isKeyDownA)
				{
					mario->setMarioState(eLeft);
				}
				else
				{
					mario->setMarioState(eNormalLeft);
				}
				break;
			case eRight:
				if (isKeyDownD)
				{
					mario->setMarioState(eRight);
				}
				else
				{
					mario->setMarioState(eNormalRight);
				}
				break;
			default:
				break;
			}
			return;
		}
		else
		{
			mario->setLadderable(false);
		}
	}

	JumpOffset -= ccJumpOffset;
}

void GameLayer::setSceneScrollPosition()
{
	Point pos = mario->getPosition();
	Size winsize = Director::getInstance()->getWinSize();

	float x = MAX(pos.x, marioAnchor.x);
	float y = MAX(pos.y, marioAnchor.y);

	x = MIN(x, mapSize.width - winsize.width / 2 - 80);
	y = MIN(y, mapSize.height - winsize.height / 2);
	Point actualPosition = ccp(x, y);

	Point viewPoint = ccpSub(marioAnchor, actualPosition);

	// 使得地图不能倒退回去
	if (fabsf(viewPoint.x) <= mapMaxH)
	{
		return;
	}
	else
	{
		mainLayer->setPosition(viewPoint);
		mapMaxH = fabsf(mainLayer->getPosition().x);
	}
}

float GameLayer::getMapMaxH()
{
	return mapMaxH;
}

void GameLayer::reSetMarioFireable()
{
	marioFireable = true;
}

void GameLayer::menuRetry(Ref *pSender)
{
	Scene *pScene = Scene::create();
	pScene->addChild(GameLayer::create());
	Director::getInstance()->replaceScene(pScene);
}

void GameLayer::menuNext(Ref *pSender)
{
	Global::getGlobalInstance()->currentLevelPlusOne();
	Scene *pScene = Scene::create();
	pScene->addChild(GameLayer::create());
	Director::getInstance()->replaceScene(pScene);
}

void GameLayer::showPassAll()
{
	CCLabelTTF *pPassAll = CCLabelTTF::create("You Pass All", "Arial", 40);
	Point origin = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();

	pPassAll->setPosition(ccp(origin.x + visibleSize.width / 2, origin.y + visibleSize.height));
	this->addChild(pPassAll, this->getChildrenCount());
	CCMoveTo *pTo = CCMoveTo::create(0.3f, ccp(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	DelayTime *pDelay = DelayTime::create(2.0f);
	pPassAll->runAction(Sequence::create(pTo, pDelay,
		CallFunc::create(this, callfunc_selector(GameLayer::toMainMenu)), NULL));
}

void GameLayer::pauseGameLayer()
{
	mainMap->pauseGameMap();
	this->unscheduleUpdate();

	pMenuArrow->setEnabled(false);
	pMenuFireBall->setEnabled(false);

	pColor->setVisible(true);
	pSetMenu->setVisible(true);
	pResume->setVisible(true);
	pResume->setEnabled(true);
	pReStart->setVisible(true);
	pReStart->setEnabled(true);
	pSelectMenu->setVisible(true);
	pSelectMenu->setEnabled(true);
}

void GameLayer::resumeGameLayer()
{
	mainMap->resumeGameMap();
	this->scheduleUpdate();

	pMenuArrow->setEnabled(true);
	pMenuFireBall->setEnabled(true);

	pColor->setVisible(false);
	pSetMenu->setVisible(false);
	pResume->setVisible(false);
	pResume->setEnabled(false);
	pReStart->setVisible(false);
	pReStart->setEnabled(false);
	pSelectMenu->setVisible(false);
	pSelectMenu->setEnabled(false);
}
