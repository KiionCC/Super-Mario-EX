#include "SimpleAudioEngine.h"

#include "AnimationManager.h"
#include "Data_Global.h"
#include "Resource_Bullet.h"
#include "Resource_Enemy.h"
#include "Resource_Item.h"
#include "Resource_Map.h"
#include "Resource_Role.h"
#include "Scene_Game.h"

GameMap* GameMap::_gameMap;
GameMap* GameMap::getGameMap()
{
	return _gameMap;
}

GameMap::GameMap()
{
	CCTexture2D *pTexture = TextureCache::getInstance()->addImage("superMarioMap.png");
	_brokenCoin = SpriteFrame::createWithTexture(pTexture, CCRectMake(1, 18, 16, 16));
	_brokenCoin->retain();

	pItemCoordArray = PointArray::create(100);
	pItemCoordArray->retain();

	pSpriteArray = Array::createWithCapacity(4);
	pSpriteArray->retain();

	pMushroomPointArray = PointArray::create(100);
	pMushroomPointArray->retain();

	pEnemyArray = Array::create();
	pEnemyArray->retain();

	pBulletArray = Array::create();
	pBulletArray->retain();

	pLadderArray = Array::create();
	pLadderArray->retain();

	pMushSprite = NULL;
	pAddLifeMushroom = NULL;
	pItem = NULL;
	marioInLadder = NULL;

	isBossMap = false;

	_gameMap = this;

	enemyTilePos = Vec2(0, 0);
	pRandomEnemy = NULL;

	// 专为Boss地图准备的
	bridgeTileStartPos = Vec2(0, 0);
	bridgeTileNums = 0;
	pBossEnemy = NULL;
	pPrincess = NULL;
}

GameMap::~GameMap()
{
	this->unscheduleAllSelectors();
}

GameMap* GameMap::create(const char *tmxFile)
{
	GameMap *pGameMap = new GameMap();
	if (pGameMap && pGameMap->initWithTMXFile(tmxFile))
	{
		pGameMap->extraInit();
		pGameMap->autorelease();
		return pGameMap;
	}
	CC_SAFE_DELETE(pGameMap);
	return NULL;
}

void GameMap::extraInit()
{
	tileSize = this->getTileSize();
	mapSize = this->getMapSize();

	cloudLayer = this->layerNamed("cloud");
	blockLayer = this->layerNamed("block");
	pipeLayer = this->layerNamed("pipe");
	landLayer = this->layerNamed("land");
	trapLayer = this->layerNamed("trap");
	objectLayer = this->objectGroupNamed("objects");
	coinLayer = this->layerNamed("coin");
	flagpoleLayer = this->layerNamed("flagpole");

	this->initObjects();

	// 下面这些工作都是应该在初始化完对象层之后才能做的
	// 在初始化完旗子摆放点后，将其放在地图上
	// 首先判断该地图是不是BossMap
	if (isBossMap)
	{
		pFlag = Sprite::create("axe.png");
		pFlag->retain();
		pPrincess = Sprite::create("princess.png");
		pPrincess->setAnchorPoint(ccp(0.5f, 0.0f));
		pPrincess->setPosition(ccp(finalPoint.x + 16, finalPoint.y));
		this->addChild(pPrincess, this->getChildrenCount());
	}
	else
	{
		pFlag = Sprite::create("flag.png");
		pFlag->retain();
	}

	pFlag->setAnchorPoint(ccp(0.5f, 0));
	pFlag->setPosition(flagPoint);
	this->addChild(pFlag, this->getChildrenCount());

	this->launchEnemyInMap();

	this->launchLadderInMap();

	this->scheduleUpdate();
}

void GameMap::showFlagMove()
{
	if (isBossMap)
	{
		// 在Boss关，顺利通关动画应该是桥掉落下去，Boss掉下去
		this->initBridgeArray();
	}
	else
	{
		if (!UserDefault::getInstance()->getBoolForKey(EFFECT_KEY))
		    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("QiZiLuoXia.wav");
		MoveBy *pMoveBy = MoveBy::create(2.0f, ccp(0, -8 * 16));
		pFlag->runAction(pMoveBy);
	}
}

void GameMap::initObjects()
{
	ValueVector tempArray = objectLayer->getObjects();

	float x, y, w, h;
	Value objPointMap;
	for each(objPointMap in tempArray)
	{
		ValueMap objPoint = objPointMap.asValueMap();
		int posX = objPoint.at("x").asFloat();
		int posY = objPoint.at("y").asFloat();
		posY -= this->getTileSize().height;
		Point tileXY = this->positionToTileCoord(ccp(posX, posY));
		std::string name = objPoint.at("name").asString();
		std::string type = objPoint.at("type").asString();

		// 进行怪物的初始化，先根据名字来判断是不是enemy，再细分enemy类型		
		if (name == "enemy")
		{
			Enemy *pEnemy = NULL;
			if (type == "mushroom")
			{
				pEnemy = new E_Mushroom();
			}
			else if (type == "flower")
			{
				pEnemy = new E_Flower();
			}
			else if (type == "tortoise")
			{
				// 地图上预先标记好的乌龟都是向左的
				pEnemy = new E_Tortoise(0);
			}
			else if (type == "tortoise_round")
			{
				std::string dis = objPoint.at("roundDis").asString();
				int roundDis = atof(dis.c_str());
				pEnemy = new E_Tortoise_Return(roundDis);
			}
			else if (type == "tortoise_fly")
			{
				std::string dis = objPoint.at("flyDis").asString();
				int flyDis = atof(dis.c_str());
				pEnemy = new E_Tortoise_Fly(flyDis);
			}
			else if (type == "fire_string")
			{
				std::string str = objPoint.at("begAngle").asString();
				float begAngle = atof(str.c_str());
				str = objPoint.at("time").asString();
				float time = atof(str.c_str());
				pEnemy = new E_FireString(begAngle, time);
			}
			else if (type == "flyfish")
			{
				std::string str = objPoint.at("offsetH").asString();
				float offsetH = atof(str.c_str());
				str = objPoint.at("offsetV").asString();
				float offsetV = atof(str.c_str());
				str = objPoint.at("duration").asString();
				float duration = atof(str.c_str());

				pEnemy = new E_FlyingFish(offsetH, offsetV, duration);
			}
			else if (type == "boss")
			{
				isBossMap = true;
				pEnemy = new E_Boss();
				pBossEnemy = pEnemy;
			}
			else if (type == "addmushroom")
			{
				std::string str = objPoint.at("nums").asString();
				int nums = atoi(str.c_str());
				pEnemy = new E_Mushroom_Hidden(nums);
			}
			else if (type == "battery")
			{
				float delay = objPoint.at("delay").asFloat();
				pEnemy = new E_Battery(delay);
			}
			else if (type == "darkcloud")
			{
				float delay = objPoint.at("delay").asFloat();
				int style = objPoint.at("style").asFloat();
				pEnemy = new E_Cloud(delay, style);
			}

			if (pEnemy != NULL)
			{
				pEnemy->setTileCoord(tileXY);
				pEnemy->setEnemyPos(ccp(posX, posY));
				pEnemyArray->addObject(pEnemy);
			}
		}
		else if (name == "ladder")
		{
			float dis = objPoint.at("ladderDis").asFloat();
			int val;
			SuspensionLadder *pLadder = NULL;
			if (type == "ladderLR")
			{
				// 左右移动的梯子
				pLadder = new SL_LeftRight(dis);
				val = objPoint.at("LorR").asInt();
				pLadder->setStartFace(val);
			}
			else if (type == "ladderUD")
			{
				// 上下移动的梯子
				pLadder = new SL_UpDown(dis);
				val = objPoint.at("UorD").asInt();
				pLadder->setStartFace(val);
			}

			if (pLadder != NULL)
			{
				pLadder->setStartPos(ccp(posX, posY));
				pLadderArray->addObject(pLadder);
			}
		}
		else if (name == "mushroom")
		{
			if (type == "MushroomReward")
			{
				// 吃了变大或是子弹蘑菇点
				pMushroomPointArray->addControlPoint(tileXY);
			}
			else if (type == "MushroomAddLife")
			{
				// 地图中仅有的一个奖人蘑菇点
				addLifePoint = tileXY;
			}
		}
		else if (name == "others")
		{
			if (type == "BirthPoint")
			{
				// 马里奥在当前地图上的出生点
				marioBirthPos = this->tilecoordToPosition(tileXY);
				marioBirthPos.x += this->getTileSize().width / 2;
			}
			else if (type == "flagpoint")
			{
				flagPoint = ccp(posX, posY);
			}
			else if (type == "finalpoint")
			{
				finalPoint = ccp(posX, posY);
			}
			else if (type == "bridgestartpos")
			{
				bridgeTileStartPos = tileXY;
			}
		}
	}

}

void GameMap::launchEnemyInMap()
{
	Enemy *tempEnemy = NULL;
	unsigned int enemyCount = pEnemyArray->count();
	for (unsigned int idx = 0; idx < enemyCount; ++idx)
	{
		tempEnemy = (Enemy *)pEnemyArray->objectAtIndex(idx);
		tempEnemy->setPosition(tempEnemy->getEnemyPos());
		switch (tempEnemy->getEnemyType())
		{
		case eEnemy_flower:
		case eEnemy_AddMushroom:
			this->addChild(tempEnemy, 3);
			break;
		default:
			this->addChild(tempEnemy, 7);
			break;
		}
		tempEnemy->launchEnemy();
	}
}

void GameMap::launchLadderInMap()
{
	SuspensionLadder *tempLadder = NULL;
	unsigned int ladderCount = pLadderArray->count();
	for (unsigned int idx = 0; idx < ladderCount; ++idx)
	{
		tempLadder = (SuspensionLadder *)pLadderArray->objectAtIndex(idx);
		tempLadder->setPosition(tempLadder->getStartPos());
		this->addChild(tempLadder, 3);
		tempLadder->launchLadder();
	}
}

void GameMap::enemyVSMario()
{
	Enemy *tempEnemy = NULL;
	EnemyVSMario vsRet;
	unsigned int enemyCount = pEnemyArray->count();
	for (unsigned int idx = 0; idx < enemyCount; ++idx)
	{
		tempEnemy = (Enemy *)pEnemyArray->objectAtIndex(idx);
		if (tempEnemy->getEnemyState() == eEnemyState_active)
		{
			vsRet = tempEnemy->checkCollisionWithMario();
			switch (vsRet)
			{
			case eVS_marioKilled:
			{
				if (!Mario::getMarioInstance()->getIsSafeTime())
				{
					Mario::getMarioInstance()->changeForGotEnemy();
				}
				break;
			}
			case eVS_enemyKilled:
			{
				tempEnemy->KilledByMario();
				if (!UserDefault::getInstance()->getBoolForKey(EFFECT_KEY))
			    	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("CaiSiGuaiWu.wav");
				break;
			}
			default:
				break;
			}
		}
	}
}

void GameMap::update(float dt)
{
	this->enemyVSMario();
	this->bulletVSEnemy();
}

void GameMap::stopUpdateForMarioDie()
{
	// 停止怪物的更新
	unsigned int enemyCount = pEnemyArray->count();
	Enemy *pEnemy = NULL;
	for (unsigned int idx = 0; idx < enemyCount; ++idx)
	{
		pEnemy = (Enemy *)pEnemyArray->objectAtIndex(idx);

		if (pEnemy->getEnemyState() == eEnemyState_active)
		{
			pEnemy->stopEnemyUpdate();
			switch (pEnemy->getEnemyType())
			{
			case eEnemy_mushroom:
			case eEnemy_tortoise:
				break;
			case eEnemy_flower:
			case eEnemy_flyFish:
			case eEnemy_tortoiseRound:
			case eEnemy_tortoiseFly:
			case eEnemy_fireString:
				//case eEnemy_Boss:
				pEnemy->stopAllActions();
				break;
			}

			pEnemy->unscheduleUpdate();
		}
	}

	// 停止子弹的更新
	unsigned int bulletCount = pBulletArray->count();
	Bullet *pBullet = NULL;
	for (unsigned int idx = 0; idx < bulletCount; ++idx)
	{
		pBullet = (Bullet *)pBulletArray->objectAtIndex(idx);
		if (pBullet->getBulletState() == eBulletState_active)
		{
			pBullet->unscheduleUpdate();
		}
	}

	// 停止悬浮梯的更新
	unsigned int ladderCount = pLadderArray->count();
	SuspensionLadder *pLadder = NULL;
	for (unsigned int idx = 0; idx < ladderCount; ++idx)
	{
		pLadder = (SuspensionLadder *)pLadderArray->objectAtIndex(idx);
		pLadder->stopAllActions();
		pLadder->unscheduleUpdate();
	}

	this->unscheduleUpdate();
}

CCTMXLayer* GameMap::getCloudLayer() const
{
	return cloudLayer;
}

CCTMXLayer* GameMap::getLandLayer()const
{
	return landLayer;
}

CCTMXLayer* GameMap::getPipeLayer()const
{
	return pipeLayer;
}

CCTMXLayer* GameMap::getBlockLayer()const
{
	return blockLayer;
}

CCTMXLayer* GameMap::getTrapLayer()const
{
	return trapLayer;
}

CCTMXObjectGroup* GameMap::getObjectLayer()const
{
	return objectLayer;
}

CCTMXLayer* GameMap::getCoinLayer()const
{
	return coinLayer;
}

CCTMXLayer* GameMap::getFlagpoleLayer()const
{
	return flagpoleLayer;
}

Point GameMap::getMarioBirthPos()
{
	return marioBirthPos;
}

// 接受一个Tile坐标系下的点，给出该点对应在图中的图块元素类型
TileType GameMap::tileTypeforPos(Point tileCoord)
{
	int GID = pipeLayer->tileGIDAt(tileCoord);
	if (GID > 0)
	{
		return eTile_Pipe;
	}
	GID = blockLayer->tileGIDAt(tileCoord);
	if (GID > 0)
	{
		return eTile_Block;
	}
	GID = landLayer->tileGIDAt(tileCoord);
	if (GID > 0)
	{
		return eTile_Land;
	}
	GID = trapLayer->tileGIDAt(tileCoord);
	if (GID > 0)
	{
		return eTile_Trap;
	}
	GID = coinLayer->tileGIDAt(tileCoord);
	if (GID > 0)
	{
		return eTile_Coin;
	}
	GID = flagpoleLayer->tileGIDAt(tileCoord);
	if (GID > 0)
	{
		return eTile_Flagpole;
	}
	return eTile_NoneH;
}

// 在block层检测 哪些block砖块是可有被顶碎的？
// 哪些砖块是闪烁的问号，头顶一下会弹出一个金币来
// 在地图的图素属性中，用blockType来表示
// 1: 闪烁的问号
// 2: 普通的一顶即碎的砖块
void GameMap::breakBlock(Point tileCoord, BodyType _bodyType)
{
	int gID = blockLayer->tileGIDAt(tileCoord);

	Value pD;
	pD = this->propertiesForGID(gID);

	if (pD.isNull() == false)
	{
		int value = pD.asValueMap().at("blockType").asInt();
		if (value)
		{
			int blockType = value;
			if (blockType == 2)
			{
				switch (_bodyType)
				{
				case eBody_Normal:
				{
					// 大号马里奥:砖块被顶碎成四块，然后消失掉
					this->showBlockBroken(tileCoord);
					blockLayer->removeTileAt(tileCoord);
				}
				break;
				case eBody_Small:
				{
					// 假如是小号马里奥：砖块只是向上移动一下然后再恢复位置
					this->showBlockJump(tileCoord);
					if (!UserDefault::getInstance()->getBoolForKey(EFFECT_KEY))
				    	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("DingYingZhuanKuai.wav");
				}
				break;
				default:
					break;
				}
			}
			else if (blockType == 1)  // 闪闪金币砖块
			{
				if (itemCoordArrayContains(tileCoord) == false)
				{
					// 假如当前顶的闪闪的问号没有被顶过							
					pItemCoordArray->addControlPoint(tileCoord);

					if (mushroomPointContains(tileCoord))
					{
						// 如果是蘑菇那么金币就不展示跳跃动作
						resetCoinPoint = tileCoord;
						this->resetCoinBlockTexture();
						this->showNewMushroom(tileCoord, _bodyType);
						this->deleteOneMushPointFromArray(tileCoord);
					}
					else
					{
						// 顶砖块随机出现金币或不同种类怪物
						if (CCRANDOM_0_1() > 0.4f)
						{
							if (!UserDefault::getInstance()->getBoolForKey(EFFECT_KEY))
						    	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("EatCoin.wav");
							this->showJumpUpBlinkCoin(tileCoord);
							// 如果是普通的金币，那么展示一个跳跃动作
							this->showCoinJump(tileCoord);
						}
						else
						{
							this->showCoinJump(tileCoord);
							// 停顿0.2秒
							enemyTilePos = tileCoord;
							this->runAction(Sequence::create(DelayTime::create(0.2f),
								CallFunc::create(this, callfunc_selector(GameMap::randomShowEnemy)), NULL));
						}

					}
				}
				else
				{
					if (!UserDefault::getInstance()->getBoolForKey(EFFECT_KEY))
				    	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("DingYingZhuanKuai.wav");
				}
			}
			else if (blockType == 3)
			{
				if (itemCoordArrayContains(tileCoord) == false)
				{
					pItemCoordArray->addControlPoint(tileCoord);
					// 隐藏的砖块，顶开会有奖励蘑菇
					Sprite *pSprite = blockLayer->tileAt(tileCoord);
					pSprite->setDisplayFrame(_brokenCoin);
					this->showAddLifeMushroom(tileCoord);
				}
			}
		}
	}
}

void GameMap::randomShowEnemy()
{
	if (!UserDefault::getInstance()->getBoolForKey(EFFECT_KEY))
    	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("DingChuMoGuHua.wav");

	// 顶出的怪种类也做一次随机
	if (CCRANDOM_0_1() > 0.5f)
	{
		pRandomEnemy = new E_Mushroom();
		if (CCRANDOM_0_1() > 0.5f)
		{
			// 设置初始方向向右
			pRandomEnemy->setMoveOffset(pRandomEnemy->getccMoveOffset());
		}
	}
	else
	{
		int val = 1;
		if (CCRANDOM_0_1() < 0.5f)
		{
			val = 1;
		}
		pRandomEnemy = new E_Tortoise(val);
	}

	Point pos = this->tilecoordToPosition(enemyTilePos);
	pos.x += tileSize.width / 2;

	pRandomEnemy->setPosition(pos);
	this->addChild(pRandomEnemy, blockLayer->getZOrder() - 1);

	pRandomEnemy->runAction(Sequence::create(JumpBy::create(0.2f, ccp(0, 16), 1, 20),
		CallFunc::create(this, callfunc_selector(GameMap::randomLaunchEnemy)), NULL));


}

void GameMap::randomLaunchEnemy()
{
	pEnemyArray->addObject(pRandomEnemy);
	//pRandomEnemy->setEnemyPos(pos);
	pRandomEnemy->setZOrder(7);
	pRandomEnemy->launchEnemy();
}

bool GameMap::itemCoordArrayContains(Point tileCoord)
{
	Point temp;
	bool flag = false;
	for (unsigned int idx = 0; idx < pItemCoordArray->count(); ++idx)
	{
		temp = pItemCoordArray->getControlPointAtIndex(idx);
		if (temp.x == tileCoord.x && temp.y == tileCoord.y)
		{
			flag = true;
			break;
		}
	}
	return flag;
}

bool GameMap::mushroomPointContains(Point tileCoord)
{
	Point temp;
	bool flag = false;
	for (unsigned int idx = 0; idx < pMushroomPointArray->count(); ++idx)
	{
		temp = pMushroomPointArray->getControlPointAtIndex(idx);
		if (temp.x == tileCoord.x && temp.y == tileCoord.y)
		{
			flag = true;
			break;
		}
	}
	return flag;
}

void GameMap::deleteOneMushPointFromArray(Point tileCoord)
{
	Point temp;
	for (unsigned int idx = 0; idx < pMushroomPointArray->count(); ++idx)
	{
		temp = pMushroomPointArray->getControlPointAtIndex(idx);
		if (temp.x == tileCoord.x && temp.y == tileCoord.y)
		{
			pMushroomPointArray->removeControlPointAtIndex(idx);
			break;
		}
	}
}

void GameMap::showBlockBroken(Point tileCoord)
{
	if (!UserDefault::getInstance()->getBoolForKey(EFFECT_KEY))
    	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("DingPoZhuan.wav");

	CCTexture2D *pTexture = TextureCache::getInstance()->addImage("singleblock.png");
	SpriteFrame *frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(0, 0, 8, 8));
	Point pos = this->tilecoordToPosition(tileCoord);
	pos.x += this->getTileSize().width / 2;
	pos.y += this->getTileSize().height / 2;

	Sprite *pSprite1 = Sprite::createWithSpriteFrame(frame);
	pSprite1->setPosition(pos);
	pSpriteArray->addObject(pSprite1);
	Sprite *pSprite2 = Sprite::createWithSpriteFrame(frame);
	pSprite2->setPosition(pos);
	pSpriteArray->addObject(pSprite2);
	Sprite *pSprite3 = Sprite::createWithSpriteFrame(frame);
	pSprite3->setPosition(pos);
	pSpriteArray->addObject(pSprite3);
	Sprite *pSprite4 = Sprite::createWithSpriteFrame(frame);
	pSprite4->setPosition(pos);
	pSpriteArray->addObject(pSprite4);

	this->addChild(pSprite1);
	this->addChild(pSprite2);
	this->addChild(pSprite3);
	this->addChild(pSprite4);

	ActionInterval *pLeftUp = JumpBy::create(0.2f,
		ccp(-this->getTileSize().width * 2, this->getTileSize().height), 10, 1);
	ActionInterval *pRightUp = JumpBy::create(0.2f,
		ccp(this->getTileSize().width * 2, this->getTileSize().height), 10, 1);
	ActionInterval *pLeftDown = JumpBy::create(0.2f,
		ccp(-this->getTileSize().width * 3, -this->getTileSize().height), 5, 1);
	ActionInterval *pRightDown = JumpBy::create(0.2f,
		ccp(this->getTileSize().width * 3, -this->getTileSize().height), 5, 1);

	pSprite1->runAction(pLeftUp);
	pSprite2->runAction(pRightUp);
	pSprite3->runAction(pLeftDown);
	pSprite4->runAction(Sequence::create(pRightDown,
		CallFunc::create(this, callfunc_selector(GameMap::clearSpriteArray)), NULL));
}

void GameMap::showJumpUpBlinkCoin(Point tileCoord)
{
	pItem = Item::create(eBlinkCoin);
	Point pos = this->tilecoordToPosition(tileCoord);
	pos.x += this->getTileSize().width / 2;
	pos.y += this->getTileSize().height;
	pItem->setPosition(pos);
	pItem->setVisible(true);
	this->addChild(pItem);

	ActionInterval *pJump = JumpBy::create(0.16f, ccp(0, this->getTileSize().height),
		this->getTileSize().height*1.5, 1);

	pItem->itemBody->runAction(sAnimationMgr->createAnimate(eAniBlinkCoin));
	pItem->runAction(Sequence::create(pJump, CallFunc::create(
		this, callfunc_selector(GameMap::clearItem)), NULL));
}

void GameMap::showBlockJump(Point tileCorrd)
{
	Sprite *tempSprite = blockLayer->tileAt(tileCorrd);
	ActionInterval *pJumpBy = JumpBy::create(0.2f, Vec2(0, 0),
		this->getTileSize().height*0.5, 1);
	tempSprite->runAction(pJumpBy);
}

void GameMap::showCoinJump(Point tileCoord)
{
	Sprite *tempSprite = blockLayer->tileAt(tileCoord);
	ActionInterval *pJumpBy = JumpBy::create(0.2f, Vec2(0, 0),
		this->getTileSize().height*0.5, 1);

	// 通过类中的成员变量来传递参数，可以简化函数中有关传参的代价
	resetCoinPoint = tileCoord;

	tempSprite->runAction(Sequence::create(pJumpBy,
		CallFunc::create(this, callfunc_selector(GameMap::resetCoinBlockTexture)), NULL));
}

void GameMap::resetCoinBlockTexture()
{
	Sprite *coinTile = blockLayer->tileAt(resetCoinPoint);
	coinTile->setDisplayFrame(_brokenCoin);
}



void GameMap::showNewMushroom(Point tileCoord, BodyType _bodyType)
{
	if (!UserDefault::getInstance()->getBoolForKey(EFFECT_KEY))
	   CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("DingChuMoGuHuoHua.wav");
	// 设置当前蘑菇出生点
	mushTileCoord = ccp(tileCoord.x, tileCoord.y - 1);

	Point pos = this->tilecoordToPosition(tileCoord);
	pos.x += this->getTileSize().width / 2;
	pos.y += this->getTileSize().height / 2;

	switch (_bodyType)
	{
	case eBody_Small:
	{
		pMushSprite = Sprite::create("rewardMushroomSet.png", CCRectMake(0, 0, 16, 16));
	}
	break;
	case eBody_Normal:
	{
		pMushSprite = Sprite::create("Tools.png", CCRectMake(0, 0, 18, 18));
	}
	break;
	default:
		break;
	}

	pMushSprite->setPosition(pos);
	this->addChild(pMushSprite);

	MoveBy *pMushJump = MoveBy::create(0.4f, ccp(0, this->getTileSize().height));
	pMushSprite->runAction(pMushJump);
}

void GameMap::showAddLifeMushroom(Point tileCoord)
{
	if (!UserDefault::getInstance()->getBoolForKey(EFFECT_KEY))
	   CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("DingChuMoGuHua.wav");

	// 再一次的设置奖励蘑菇出生点
	addLifePoint = ccp(tileCoord.x, tileCoord.y - 1);

	Point pos = this->tilecoordToPosition(tileCoord);
	pos.x += this->getTileSize().width / 2;
	pos.y += this->getTileSize().height / 2;

	pAddLifeMushroom = Sprite::create("rewardMushroomSet.png", CCRectMake(16, 0, 16, 16));

	pAddLifeMushroom->setPosition(pos);
	this->addChild(pAddLifeMushroom);

	MoveBy *pMushJump = MoveBy::create(0.4f, ccp(0, this->getTileSize().height));
	pAddLifeMushroom->runAction(pMushJump);
}


void GameMap::clearItem()
{
	pItem->removeFromParentAndCleanup(true);
	pItem = NULL;
}

void GameMap::clearSpriteArray()
{
	Sprite *pS = NULL;
	for (unsigned int idx = 0; idx < pSpriteArray->count(); ++idx)
	{
		pS = (Sprite *)pSpriteArray->objectAtIndex(idx);
		pS->removeFromParentAndCleanup(true);
	}
}

bool GameMap::isMarioEatMushroom(Point tileCoord)
{
	if (pMushSprite == NULL)
	{
		return false;
	}
	if (tileCoord.x == mushTileCoord.x && tileCoord.y == mushTileCoord.y)
	{
		pMushSprite->removeFromParentAndCleanup(true);
		mushTileCoord = Vec2(0, 0);
		pMushSprite = NULL;
		return true;
	}
	else
	{
		return false;
	}
}

bool GameMap::isMarioEatAddLifeMushroom(Point tileCoord)
{
	if (pAddLifeMushroom == NULL)
	{
		return false;
	}
	if (tileCoord.x == addLifePoint.x && tileCoord.y == addLifePoint.y)
	{
		pAddLifeMushroom->removeFromParentAndCleanup(true);
		mushTileCoord = Vec2(0, 0);
		pAddLifeMushroom = NULL;
		return true;
	}
	else
	{
		return false;
	}
}

Point GameMap::positionToTileCoord(Point pos)
{
	int x = pos.x / this->getTileSize().width;
	int y = (this->getMapSize().height - 1) - pos.y / this->getTileSize().height;
	return ccp(x, y);
}

Point GameMap::tilecoordToPosition(Point tileCoord)
{
	float x = tileCoord.x * this->getTileSize().width;
	float y = (this->getMapSize().height - 1 - tileCoord.y) * this->getTileSize().height;
	return ccp(x, y);
}

void GameMap::createNewBullet()
{
	Bullet *pBullet = NULL;
	switch (Global::getGlobalInstance()->getCurrentBulletType())
	{
	case eBullet_common:
		pBullet = new B_Common();
		break;
	case eBullet_arrow:
		pBullet = new B_Arrow();
		break;
	default:
		break;
	}

	pBulletArray->addObject(pBullet);

	pBullet->setPosition(pBullet->getStartPos());
	this->addChild(pBullet, 7);
	pBullet->launchBullet();
}

void GameMap::createNewBulletForBoss(Point pos, EnemyType _enemyType)
{
	Enemy *pEnemy = NULL;

	switch (_enemyType)
	{
	case eEnemy_BossFire:
		pEnemy = new E_BossFire();
		break;
	case eEnemy_mushroom:
		pEnemy = new E_Mushroom();
		break;
	case eEnemy_BatteryBullet:
		pEnemy = new E_BatteryBullet();
		break;
	case eEnemy_Lightning:
		pEnemy = new E_Lightning();
		break;
	default:
		break;
	}

	if (pEnemy != NULL)
	{
		pEnemyArray->addObject(pEnemy);
		pEnemy->setEnemyPos(pos);
		pEnemy->setPosition(pos);
		this->addChild(pEnemy, 7);
		pEnemy->launchEnemy();
	}
}

void GameMap::bulletVSEnemy()
{
	unsigned int bulletCount = pBulletArray->count();
	unsigned int enemyCount = pEnemyArray->count();
	Bullet *pBullet = NULL;
	Enemy *pEnemy = NULL;
	Array *delBullet = Array::create();
	delBullet->retain();
	Array *delEnemy = NULL;
	Rect bulletRect;
	Rect enemyRect;

	for (unsigned int idxBullet = 0; idxBullet < bulletCount; ++idxBullet)
	{
		pBullet = (Bullet *)pBulletArray->objectAtIndex(idxBullet);
		if (pBullet->getBulletState() == eBulletState_nonactive)
		{
			delBullet->addObject(pBullet);
			continue;
		}
		bulletRect = pBullet->getBulletRect();

		for (unsigned int idxEnemy = 0; idxEnemy < enemyCount; ++idxEnemy)
		{
			pEnemy = (Enemy *)pEnemyArray->objectAtIndex(idxEnemy);
			switch (pEnemy->getEnemyType())
			{
				// 火链是不能被任何子弹打掉的
				// 炮台发射的子弹不会被任何子弹打掉
			case eEnemy_BatteryBullet:
			case eEnemy_fireString:
			case eEnemy_Lightning:
			case eEnemy_DarkCloud:
				continue;
				break;
			}
			if (pBullet->getBulletType() == eBullet_common &&
				pEnemy->getEnemyType() == eEnemy_Boss)
			{
				// 普通的子弹对Boss没有作用
				continue;
			}
			if (pBullet->getBulletType() == eBullet_common &&
				pEnemy->getEnemyType() == eEnemy_BossFire)
			{
				// 普通子弹对Boss发出的火球也是没有作用的
				continue;
			}

			if (pEnemy->getEnemyState() == eEnemyState_active)
			{
				enemyRect = pEnemy->getEnemyRect();

				if (bulletRect.intersectsRect(enemyRect))
				{
					pBullet->forKilledEnemy();
					if (!UserDefault::getInstance()->getBoolForKey(EFFECT_KEY))
					   CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("HuoQiuDaDaoGuaiWu.wav");
					pEnemy->KilledByBullet();
				}
			}
		}
	}

	unsigned int delCount = delBullet->count();
	for (unsigned int idxDel = 0; idxDel < delCount; ++idxDel)
	{
		pBullet = (Bullet *)delBullet->objectAtIndex(idxDel);
		pBulletArray->removeObject(pBullet, true);
		this->removeChild(pBullet, true);
	}
	delBullet->release();
}

bool GameMap::isMarioInLadder(Point marioPos, float &ladderLevel)
{
	bool ret = false;
	SuspensionLadder *tempLadder = NULL;
	unsigned int ladderCount = pLadderArray->count();
	Rect ladderRect;
	for (unsigned int idx = 0; idx < ladderCount; ++idx)
	{
		tempLadder = (SuspensionLadder *)pLadderArray->objectAtIndex(idx);
		if (tempLadder->getLadderState() == eLadderState_active)
		{
			ladderRect = tempLadder->getLadderRect();
			if (ladderRect.containsPoint(marioPos))
			{
				ladderLevel = tempLadder->getPosition().y + tempLadder->getLadderSize().height;
				ret = true;
				this->marioInLadder = tempLadder;
				Mario::getMarioInstance()->setLadderable(true);
				break;
			}
		}
	}
	return ret;
}

void GameMap::initBridgeArray()
{
	bridgeTileNums = 13;

	Sprite *pS = NULL;
	Point bossPos = pBossEnemy->getPosition();
	Point pos;
	for (int i = 0; i < bridgeTileNums; ++i)
	{
		Point tilePos = bridgeTileStartPos;
		tilePos.x += i;

		pS = landLayer->tileAt(tilePos);
		pS->runAction(MoveBy::create(1.0f, ccp(0, -60)));

		pos = this->tilecoordToPosition(tilePos);
		if (pos.x >= bossPos.x)
		{
			if (pBossEnemy->getEnemyState() == eEnemyState_active)
			{
				if (!UserDefault::getInstance()->getBoolForKey(EFFECT_KEY))
			     	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("BossDiaoLuoQiaoXia.wav");
				pBossEnemy->runAction(MoveBy::create(1.0f, ccp(0, -80)));
			}
		}
	}

	// 将那把斧子设置为不可见
	pFlag->setVisible(false);
}

void GameMap::pauseGameMap()
{
	// 停止怪物的更新
	unsigned int enemyCount = pEnemyArray->count();
	Enemy *pEnemy = NULL;
	for (unsigned int idx = 0; idx < enemyCount; ++idx)
	{
		pEnemy = (Enemy *)pEnemyArray->objectAtIndex(idx);

		if (pEnemy->getEnemyState() == eEnemyState_active)
		{
			pEnemy->unscheduleUpdate();
		}
	}

	// 停止子弹的更新
	unsigned int bulletCount = pBulletArray->count();
	Bullet *pBullet = NULL;
	for (unsigned int idx = 0; idx < bulletCount; ++idx)
	{
		pBullet = (Bullet *)pBulletArray->objectAtIndex(idx);
		if (pBullet->getBulletState() == eBulletState_active)
		{
			pBullet->unscheduleUpdate();
		}
	}

	// 停止悬浮梯的更新
	unsigned int ladderCount = pLadderArray->count();
	SuspensionLadder *pLadder = NULL;
	for (unsigned int idx = 0; idx < ladderCount; ++idx)
	{
		pLadder = (SuspensionLadder *)pLadderArray->objectAtIndex(idx);
		pLadder->unscheduleUpdate();
	}

	this->unscheduleUpdate();
}

void GameMap::resumeGameMap()
{
	// 停止怪物的更新
	unsigned int enemyCount = pEnemyArray->count();
	Enemy *pEnemy = NULL;
	for (unsigned int idx = 0; idx < enemyCount; ++idx)
	{
		pEnemy = (Enemy *)pEnemyArray->objectAtIndex(idx);

		if (pEnemy->getEnemyState() == eEnemyState_active)
		{
			pEnemy->scheduleUpdate();
		}
	}

	// 停止子弹的更新
	unsigned int bulletCount = pBulletArray->count();
	Bullet *pBullet = NULL;
	for (unsigned int idx = 0; idx < bulletCount; ++idx)
	{
		pBullet = (Bullet *)pBulletArray->objectAtIndex(idx);
		if (pBullet->getBulletState() == eBulletState_active)
		{
			pBullet->scheduleUpdate();
		}
	}

	// 停止悬浮梯的更新
	unsigned int ladderCount = pLadderArray->count();
	SuspensionLadder *pLadder = NULL;
	for (unsigned int idx = 0; idx < ladderCount; ++idx)
	{
		pLadder = (SuspensionLadder *)pLadderArray->objectAtIndex(idx);
		pLadder->scheduleUpdate();
	}

	this->scheduleUpdate();
}
