#include <math.h>

#include "AnimationManager.h"
#include "Data_Global.h"
#include "Resource_Enemy.h"
#include "Resource_Map.h"
#include "Resource_Role.h"
#include "Scene_Game.h"

// ****************** Enemy ******************** //
Enemy::Enemy()
{
	startFace = eLeft; // 初始朝向
	moveOffset = 0.0f;
	ccMoveOffset = 0.6f;

	jumpOffset = 0.0f;
	ccJumpOffset = 0.3f;

	enemyState = eEnemyState_nonactive;
}

Enemy::~Enemy()
{

}

void Enemy::setTileCoord(Point _tileCoord)
{
	tileCoord = _tileCoord;
}
Point Enemy::getTileCoord()
{
	return tileCoord;
}

void Enemy::setEnemyPos(Point _enemyPos)
{
	enemyPos = _enemyPos;
}
Point Enemy::getEnemyPos()
{
	return enemyPos;
}

void Enemy::setBodySize(Size _size)
{
	bodySize = _size;
}
Size Enemy::getBodySize()
{
	return bodySize;
}

void Enemy::setMoveOffset(float _moveoffset)
{
	moveOffset = _moveoffset;
}
float Enemy::getMoveOffset()
{
	return moveOffset;
}

void Enemy::setccMoveOffset(float _ccmoveoffset)
{
	ccMoveOffset = _ccmoveoffset;
}
float Enemy::getccMoveOffset()
{
	return ccMoveOffset;
}

EnemyType Enemy::getEnemyType()
{
	return enemyType;
}

Rect Enemy::getEnemyRect()
{
	Point pos = this->getPosition();
	return CCRectMake(pos.x - bodySize.width / 2 + 2, pos.y + 2, bodySize.width - 4, bodySize.height - 4);
}

void Enemy::setEnemyState(EnemyState _state)
{
	enemyState = _state;
	switch (enemyState)
	{
	case eEnemyState_over:
	{
		this->enemyBody->stopAllActions();
		this->unscheduleUpdate();
		this->setVisible(false);
		break;
	}
	default:
		break;
	}
}
EnemyState Enemy::getEnemyState()
{
	return enemyState;
}
void Enemy::checkState()
{
	Size winSize = Director::getInstance()->getWinSize();
	float tempMaxH = GameLayer::getMapMaxH();
	Point pos = this->getPosition();

	if ((pos.x + bodySize.width / 2 - tempMaxH >= 0) &&
		(pos.x - bodySize.width / 2 - tempMaxH) <= winSize.width)
	{
		enemyState = eEnemyState_active;
	}
	else
	{
		if (pos.x + bodySize.width / 2 - tempMaxH < 0)
		{
			this->setEnemyState(eEnemyState_over);
		}
		else
		{
			enemyState = eEnemyState_nonactive;
		}
	}
}

void Enemy::stopEnemyUpdate()
{
	enemyBody->stopAllActions();
}

// 怪物的水平方向碰撞检测
void Enemy::enemyCollistionH()
{
	Point currentPos = this->getPosition();
	Size enemySize = this->getContentSize();
	Point leftCollistion = ccp(currentPos.x - enemySize.width / 2, currentPos.y);
	Point leftTilecoord = GameMap::getGameMap()->positionToTileCoord(leftCollistion);
	Point leftPos = GameMap::getGameMap()->tilecoordToPosition(leftTilecoord);
	leftPos = ccp(leftPos.x + bodySize.width / 2 + GameMap::getGameMap()->tileSize.width, currentPos.y);

	TileType tileType;
	// 左侧
	tileType = GameMap::getGameMap()->tileTypeforPos(leftTilecoord);
	switch (tileType)
	{
	case eTile_Pipe:
	case eTile_Block:
		this->setPosition(leftPos);
		moveOffset *= -1;
		break;
	default:
		break;
	}
	// 右侧
	Point rightCollistion = ccp(currentPos.x + bodySize.width / 2, currentPos.y);
	Point rightTilecoord = GameMap::getGameMap()->positionToTileCoord(rightCollistion);
	Point rightPos = GameMap::getGameMap()->tilecoordToPosition(rightTilecoord);
	rightPos = ccp(rightPos.x - bodySize.width / 2, currentPos.y);

	tileType = GameMap::getGameMap()->tileTypeforPos(rightTilecoord);
	switch (tileType)
	{
	case eTile_Pipe:
	case eTile_Block:
		this->setPosition(rightPos);
		moveOffset *= -1;
		break;
	default:
		break;
	}
}

// 怪物的竖直方向碰撞检测
void Enemy::enemyCollistionV()
{
	Point currentPos = this->getPosition();
	Point downCollision = currentPos;
	Point downTilecoord = GameMap::getGameMap()->positionToTileCoord(downCollision);
	downTilecoord.y += 1;

	Point downPos = GameMap::getGameMap()->tilecoordToPosition(downTilecoord);
	downPos = ccp(currentPos.x, downPos.y + GameMap::getGameMap()->getTileSize().height);

	TileType tileType = GameMap::getGameMap()->tileTypeforPos(downTilecoord);
	bool downFlag = false;
	switch (tileType)
	{
	case eTile_Land:
	case eTile_Pipe:
	case eTile_Block:
	{
		downFlag = true;
		jumpOffset = 0.0f;
		this->setPosition(downPos);
		break;
	}
	case eTile_Trap:
	{
		this->setEnemyState(eEnemyState_over);
		break;
	}
	}

	if (downFlag)
	{
		return;
	}

	jumpOffset -= ccJumpOffset;
}

EnemyVSMario Enemy::checkCollisionWithMario()
{
	EnemyVSMario ret = eVS_nonKilled;

	Point marioPos = Mario::getMarioInstance()->getPosition();
	Size marioSize = Mario::getMarioInstance()->getContentSize();
	Rect marioRect = CCRectMake(marioPos.x - marioSize.width / 2 + 2, marioPos.y + 3,
		marioSize.width - 4, marioSize.height - 4);

	Rect marioRectVS = CCRectMake(marioPos.x - marioSize.width / 2 - 3, marioPos.y,
		marioSize.width - 6, 2);

	Point enemyPos = this->getPosition();
	Rect enemyRect = CCRectMake(enemyPos.x - bodySize.width / 2 + 1, enemyPos.y,
		bodySize.width - 2, bodySize.height - 4);

	Rect enemyRectVS = CCRectMake(enemyPos.x - bodySize.width / 2 - 2, enemyPos.y + bodySize.height - 4,
		bodySize.width - 4, 4);

	if (marioRectVS.intersectsRect(enemyRectVS))
	{
		ret = eVS_enemyKilled;
		return ret;
	}

	if (marioRect.intersectsRect(enemyRect))
	{
		ret = eVS_marioKilled;
		return ret;
	}

	return ret;
}

void Enemy::KilledByMario()
{
	enemyState = eEnemyState_over;
	enemyBody->stopAllActions();
	this->stopAllActions();
	this->unscheduleUpdate();
	enemyBody->setDisplayFrame(enemyLifeOver);
	ActionInterval *pDelay = DelayTime::create(1.0f);
	this->runAction(Sequence::create(pDelay,
		CallFunc::create(this, callfunc_selector(Enemy::setNonVisibleForKilledByMario)), NULL));
}

void Enemy::setNonVisibleForKilledByMario()
{
	this->setVisible(false);
}

void Enemy::KilledByBullet()
{
	enemyState = eEnemyState_over;
	enemyBody->stopAllActions();
	this->unscheduleUpdate();

	MoveBy *pMoveBy = NULL;
	JumpBy *pJumpBy = NULL;

	switch (Global::getGlobalInstance()->getCurrentBulletType())
	{
	case eBullet_common:
	{
		if (enemyType == eEnemy_mushroom || enemyType == eEnemy_AddMushroom)
		{
			enemyBody->setDisplayFrame(overByArrow);
		}
		else
		{
			enemyBody->setDisplayFrame(enemyLifeOver);
		}

		switch (Mario::getMarioInstance()->face)
		{
		case eRight:
			pJumpBy = JumpBy::create(0.3f, ccp(bodySize.width * 2, 0), bodySize.height, 1);
			break;
		case eLeft:
			pJumpBy = JumpBy::create(0.3f, ccp(-bodySize.width * 2, 0), bodySize.height, 1);
			break;
		default:
			break;
		}

		break;
	}
	case eBullet_arrow:
	{
		enemyBody->setDisplayFrame(overByArrow);
		Sprite *arrow = Sprite::create("arrow.png");
		arrow->setPosition(ccp(bodySize.width / 2, bodySize.height / 2));
		this->addChild(arrow);

		switch (Mario::getMarioInstance()->face)
		{
		case eRight:
			pMoveBy = MoveBy::create(0.1f, ccp(2 * bodySize.width, 0));
			break;
		case eLeft:
			pMoveBy = MoveBy::create(0.1f, ccp(-2 * bodySize.width, 0));
			arrow->runAction(CCFlipX::create(true));
			break;
		default:
			break;
		}

		break;
		break;
	}
	default:
		break;
	}

	// 先判断如果是食人花的话，就是原地停留一段时间然后消失掉
	if (enemyType == eEnemy_flower)
	{
		DelayTime *pDelay = DelayTime::create(0.2f);
		this->runAction(Sequence::create(pDelay,
			CallFunc::create(this, callfunc_selector(Enemy::setNonVisibleForKilledByBullet)), NULL));
		return;
	}

	if (pJumpBy)
	{
		this->runAction(Sequence::create(pJumpBy,
			CallFunc::create(this, callfunc_selector(Enemy::setNonVisibleForKilledByBullet)), NULL));
	}
	else
	{
		this->runAction(Sequence::create(pMoveBy,
			CallFunc::create(this, callfunc_selector(Enemy::setNonVisibleForKilledByBullet)), NULL));
	}
}

void Enemy::setNonVisibleForKilledByBullet()
{
	enemyState = eEnemyState_over;
	this->setVisible(false);
}


// ******************** E_Mushroom ***************** //
E_Mushroom::E_Mushroom()
{
	enemyType = eEnemy_mushroom;
	bodySize = CCSizeMake(16.0f, 16.0f);
	enemyBody = Sprite::create("Mushroom0.png", CCRectMake(0, 0, 16, 16));
	enemyBody->setAnchorPoint(ccp(0, 0));
	this->setContentSize(bodySize);
	this->addChild(enemyBody);
	this->setAnchorPoint(ccp(0.5f, 0));

	enemyLifeOver = SpriteFrame::create("Mushroom0.png", CCRectMake(32, 0, 16, 16));
	enemyLifeOver->retain();

	overByArrow = SpriteFrame::create("Mushroom0.png", CCRectMake(48, 0, 16, 16));
	overByArrow->retain();

	moveOffset = -ccMoveOffset;
}

E_Mushroom::~E_Mushroom()
{
	this->unscheduleAllSelectors();
}

void E_Mushroom::launchEnemy()
{
	enemyBody->runAction(RepeatForever::create(sAnimationMgr->createAnimate(eAniMushroom)));
	this->scheduleUpdate();
}



void E_Mushroom::update(float dt)
{
	this->checkState();

	if (enemyState == eEnemyState_active)
	{
		Point currentPos = this->getPosition();
		currentPos.x += moveOffset;
		currentPos.y += jumpOffset;
		this->setPosition(currentPos);

		this->enemyCollistionH();
		this->enemyCollistionV();
	}
}


// ********************** E_Flower ****************** //
E_Flower::E_Flower()
{
	enemyType = eEnemy_flower;
	bodySize = CCSizeMake(16, 24);
	enemyBody = Sprite::create("flower0.png", CCRectMake(0, 0, 16, 24));
	enemyBody->setAnchorPoint(ccp(0, 0));
	this->setContentSize(bodySize);
	this->addChild(enemyBody);
	this->setAnchorPoint(ccp(0.5f, 0));

	// 食人花只能被子弹打死
	enemyLifeOver = SpriteFrame::create("flower0.png", CCRectMake(0, 0, 16, 24));
	enemyLifeOver->retain();

	overByArrow = enemyLifeOver;
}

E_Flower::~E_Flower()
{
	this->unscheduleAllSelectors();
}

void E_Flower::launchEnemy()
{
	enemyBody->runAction(RepeatForever::create(sAnimationMgr->createAnimate(eAniflower)));
	Point pos = this->getPosition();
	pos.y -= bodySize.height;
	startPos = pos;
	this->runAction(Place::create(pos));

	ActionInterval *pMoveBy = MoveBy::create(1.0f, ccp(0.0f, bodySize.height));
	ActionInterval *pDelay = DelayTime::create(1.0f);
	ActionInterval *pMoveByBack = pMoveBy->reverse();
	ActionInterval *pDelay2 = DelayTime::create(2.0f);
	this->runAction(RepeatForever::create(
		(ActionInterval*)Sequence::create(pMoveBy, pDelay, pMoveByBack, pDelay2, NULL)));

	this->scheduleUpdate();
}

void E_Flower::update(float dt)
{
	// 食人花的帧更新函数中不做位置控制类代码
	this->checkState();
}

EnemyVSMario E_Flower::checkCollisionWithMario()
{
	EnemyVSMario ret = eVS_nonKilled;

	Point marioPos = Mario::getMarioInstance()->getPosition();
	Size marioSize = Mario::getMarioInstance()->currentSize;
	Rect marioRect = CCRectMake(marioPos.x - marioSize.width / 2 + 2, marioPos.y + 2,
		marioSize.width - 4, marioSize.height - 4);

	Point enemyPos = this->getPosition();
	Rect enemyRect = CCRectMake(enemyPos.x - bodySize.width / 2 + 2, enemyPos.y + bodySize.height - (enemyPos.y - startPos.y),
		bodySize.width - 4, enemyPos.y - startPos.y);


	if (marioRect.intersectsRect(enemyRect))
	{
		ret = eVS_marioKilled;
	}

	return ret;
}

Rect E_Flower::getEnemyRect()
{
	Point enemyPos = this->getPosition();
	Rect enemyRect = CCRectMake(enemyPos.x - bodySize.width / 2 + 2, enemyPos.y + bodySize.height - (enemyPos.y - startPos.y),
		bodySize.width - 4, enemyPos.y - startPos.y);
	return enemyRect;
}

// ********************** E_Tortoise ****************** //
E_Tortoise::E_Tortoise(int _startface)
{
	// 地上行走的乌龟在初始化时，要先判断起始朝向
	// 0：左             1：右
	switch (_startface)
	{
	case 0:
		startFace = eLeft;
		enemyBody = Sprite::create("tortoise0.png", CCRectMake(18 * 2, 0, 18, 24));
		leftFace = SpriteFrame::create("tortoise0.png", CCRectMake(18 * 2, 0, 18, 24));
		leftFace->retain();
		moveOffset = -ccMoveOffset;
		break;
	case 1:
		startFace = eRight;
		enemyBody = Sprite::create("tortoise0.png", CCRectMake(18 * 5, 0, 18, 24));
		rightFace = SpriteFrame::create("tortoise0.png", CCRectMake(18 * 5, 0, 18, 24));
		rightFace->retain();
		moveOffset = ccMoveOffset;
		break;
	default:
		break;
	}

	enemyType = eEnemy_tortoise;
	bodySize = CCSizeMake(18.0f, 24.0f);
	enemyBody->setAnchorPoint(ccp(0, 0));
	this->setContentSize(bodySize);
	this->addChild(enemyBody);
	this->setAnchorPoint(ccp(0.5f, 0.0f));

	enemyLifeOver = SpriteFrame::create("tortoise0.png", CCRectMake(18 * 9, 0, 18, 24));
	enemyLifeOver->retain();

	overByArrow = SpriteFrame::create("tortoise0.png", CCRectMake(18 * 8, 0, 18, 24));
	overByArrow->retain();
}

E_Tortoise::~E_Tortoise()
{
	this->unscheduleAllSelectors();
}

void E_Tortoise::launchEnemy()
{
	switch (startFace)
	{
	case eLeft:
		enemyBody->runAction(RepeatForever::create(sAnimationMgr->createAnimate(eAniTortoiseLeft)));
		break;
	case eRight:
		enemyBody->runAction(RepeatForever::create(sAnimationMgr->createAnimate(eAniTortoiseRight)));
		break;
	default:
		break;
	}
	this->scheduleUpdate();
}

void E_Tortoise::enemyCollistionH()
{
	Point currentPos = this->getPosition();
	Size enemySize = this->getContentSize();
	Point leftCollistion = ccp(currentPos.x - enemySize.width / 2, currentPos.y);
	Point leftTilecoord = GameMap::getGameMap()->positionToTileCoord(leftCollistion);
	Point leftPos = GameMap::getGameMap()->tilecoordToPosition(leftTilecoord);
	leftPos = ccp(leftPos.x + bodySize.width / 2 + GameMap::getGameMap()->tileSize.width, currentPos.y);

	TileType tileType;
	// 左侧检测
	tileType = GameMap::getGameMap()->tileTypeforPos(leftTilecoord);
	switch (tileType)
	{
	case eTile_Pipe:
	case eTile_Block:
		this->setPosition(leftPos);
		moveOffset *= -1;

		enemyBody->stopAllActions();
		enemyBody->runAction(RepeatForever::create(sAnimationMgr->createAnimate(eAniTortoiseRight)));
		break;
	default:
		break;
	}
	// 右侧检测
	Point rightCollistion = ccp(currentPos.x + bodySize.width / 2, currentPos.y);
	Point rightTilecoord = GameMap::getGameMap()->positionToTileCoord(rightCollistion);
	Point rightPos = GameMap::getGameMap()->tilecoordToPosition(rightTilecoord);
	rightPos = ccp(rightPos.x - bodySize.width / 2, currentPos.y);

	tileType = GameMap::getGameMap()->tileTypeforPos(rightTilecoord);
	switch (tileType)
	{
	case eTile_Pipe:
	case eTile_Block:
		this->setPosition(rightPos);
		moveOffset *= -1;

		enemyBody->stopAllActions();
		enemyBody->runAction(RepeatForever::create(sAnimationMgr->createAnimate(eAniTortoiseLeft)));
		break;
	default:
		break;
	}
}

void E_Tortoise::update(float dt)
{
	this->checkState();

	if (enemyState == eEnemyState_active)
	{
		Point currentPos = this->getPosition();
		currentPos.x += moveOffset;
		currentPos.y += jumpOffset;
		this->setPosition(currentPos);

		this->enemyCollistionH();
		this->enemyCollistionV();
	}
}


// ********************** E_Tortoise_Return ****************** //
E_Tortoise_Return::E_Tortoise_Return(float dis)
{
	enemyType = eEnemy_tortoiseRound;
	bodySize = CCSizeMake(18.0f, 24.0f);
	enemyBody = Sprite::create("tortoise0.png", CCRectMake(18 * 2, 0, 18, 24));
	enemyBody->setAnchorPoint(ccp(0, 0));
	this->setContentSize(bodySize);
	this->addChild(enemyBody);
	this->setAnchorPoint(ccp(0.5f, 0.0f));

	enemyLifeOver = SpriteFrame::create("tortoise0.png", CCRectMake(18 * 9, 0, 18, 24));
	enemyLifeOver->retain();

	overByArrow = SpriteFrame::create("tortoise0.png", CCRectMake(18 * 8, 0, 18, 24));
	overByArrow->retain();

	roundDis = dis;
}
E_Tortoise_Return::~E_Tortoise_Return()
{
	this->unscheduleAllSelectors();
}

void E_Tortoise_Return::launchEnemy()
{
	enemyState = eEnemyState_active;
	enemyBody->runAction(RepeatForever::create(sAnimationMgr->createAnimate(eAniTortoiseLeft)));
	ActionInterval *pMoveLeft = MoveBy::create(2.0f, ccp(-roundDis, 0.0f));
	ActionInterval *pMoveRight = MoveBy::create(2.0f, ccp(roundDis, 0.0f));
	DelayTime *pDelay = DelayTime::create(0.2f);

	this->runAction(RepeatForever::create((ActionInterval*)Sequence::create(pMoveLeft,
		CallFunc::create(this, callfunc_selector(E_Tortoise_Return::reRight)),
		pMoveRight,
		CallFunc::create(this, callfunc_selector(E_Tortoise_Return::reLeft)),
		NULL)));
}

void E_Tortoise_Return::update(float dt)
{
	this->checkState();
}

void E_Tortoise_Return::reRight()
{
	enemyBody->stopAllActions();
	enemyBody->runAction(RepeatForever::create(sAnimationMgr->createAnimate(eAniTortoiseRight)));
}

void E_Tortoise_Return::reLeft()
{
	enemyBody->stopAllActions();
	enemyBody->runAction(RepeatForever::create(sAnimationMgr->createAnimate(eAniTortoiseLeft)));
}


// ********************** E_Tortoise_Fly ****************** //

E_Tortoise_Fly::E_Tortoise_Fly(float dis)
{
	enemyType = eEnemy_tortoiseFly;
	bodySize = CCSizeMake(18.0f, 24.0f);
	enemyBody = Sprite::create("tortoise0.png", CCRectMake(0, 0, 18, 24));
	enemyBody->setAnchorPoint(ccp(0, 0));
	this->setContentSize(bodySize);
	this->addChild(enemyBody);
	this->setAnchorPoint(ccp(0.5f, 0.0f));

	enemyLifeOver = SpriteFrame::create("tortoise0.png", CCRectMake(18 * 9, 0, 18, 24));
	enemyLifeOver->retain();

	overByArrow = SpriteFrame::create("tortoise0.png", CCRectMake(18 * 8, 0, 18, 24));
	overByArrow->retain();

	flyDis = dis;
}

E_Tortoise_Fly::~E_Tortoise_Fly()
{
	this->unscheduleAllSelectors();
}

void E_Tortoise_Fly::launchEnemy()
{
	enemyState = eEnemyState_active;
	enemyBody->runAction(RepeatForever::create(sAnimationMgr->createAnimate(eAniTortoiseFly)));
	ActionInterval *pMoveDown = MoveBy::create(2.0f, ccp(0, -flyDis));
	ActionInterval *pMoveUp = MoveBy::create(2.0f, ccp(0, flyDis));

	this->runAction(RepeatForever::create(
		(ActionInterval*)Sequence::create(pMoveDown, pMoveUp, NULL)));
}

void E_Tortoise_Fly::update(float dt)
{
	this->checkState();
}

void E_Tortoise_Fly::setFlyDis(float dis)
{
	flyDis = dis;
}

float E_Tortoise_Fly::getFlyDis()
{
	return flyDis;
}

// ********************** E_FireString ****************** //
E_FireString::E_FireString(float _begAngle, float _time)
{
	enemyType = eEnemy_fireString;

	pArrayFire = Array::createWithCapacity(3);
	pArrayFire->retain();

	enemyBody = Sprite::create("fireBall.png");
	pArrayFire->addObject(enemyBody);
	enemyBody2 = Sprite::create("fireBall.png");
	pArrayFire->addObject(enemyBody2);
	enemyBody3 = Sprite::create("fireBall.png");
	pArrayFire->addObject(enemyBody3);
	fireSize = CCSizeMake(8.0f, 8.0f);

	enemyBody->setPosition(ccp(8, 8));
	this->addChild(enemyBody);
	enemyBody2->setPosition(ccp(24, 8));
	this->addChild(enemyBody2);
	enemyBody3->setPosition(ccp(40, 8));
	this->addChild(enemyBody3);

	bodySize = CCSizeMake(48, 16);
	this->setContentSize(bodySize);
	this->setAnchorPoint(ccp(0.0f, 0.5f));

	begAngle = _begAngle;
	time = _time;

	angle = begAngle;
	PI = 3.1415926;

	// 火链不会死亡，不会用到这两帧
	enemyLifeOver = NULL;
	overByArrow = NULL;
}

E_FireString::~E_FireString()
{
	this->unscheduleAllSelectors();
}

void E_FireString::launchEnemy()
{
	enemyState = eEnemyState_active;
	enemyBody->runAction(RepeatForever::create(sAnimationMgr->createAnimate(eAniRotatedFireBall)));
	enemyBody2->runAction(RepeatForever::create(sAnimationMgr->createAnimate(eAniRotatedFireBall)));
	enemyBody3->runAction(RepeatForever::create(sAnimationMgr->createAnimate(eAniRotatedFireBall)));

	this->runAction(Sequence::create(CCRotateBy::create(1.0f, -begAngle),
		CallFunc::create(this, callfunc_selector(E_FireString::launchFireString)), NULL));
}

void E_FireString::stopEnemyUpdate()
{
	enemyBody->stopAllActions();
	enemyBody2->stopAllActions();
	enemyBody3->stopAllActions();
}

EnemyVSMario E_FireString::checkCollisionWithMario()
{
	EnemyVSMario ret = eVS_nonKilled;

	Point marioPos = Mario::getMarioInstance()->getPosition();
	Size marioSize = Mario::getMarioInstance()->getContentSize();
	Rect marioRect = CCRectMake(marioPos.x - marioSize.width / 2 + 2, marioPos.y + 2,
		marioSize.width - 4, marioSize.height - 4);

	Point thisPos = this->getPosition();

	double angleTemp = 2 * PI*angle / 360.0f;
	for (int i = 0; i < 3; ++i)
	{
		Point firePos = ccp(thisPos.x + (2 * i * 8 + 8)*cos(angleTemp), thisPos.y + (2 * i * 8 + 8)*sin(angleTemp));
		Rect fireRect = CCRectMake(firePos.x - fireSize.width / 2, firePos.y - fireSize.height / 2,
			fireSize.width, fireSize.height);
		if (marioRect.intersectsRect(fireRect))
		{
			ret = eVS_marioKilled;
			break;
		}
	}

	return ret;
}

void E_FireString::KilledByBullet()
{

}

void E_FireString::KilledByMario()
{

}

void E_FireString::launchFireString()
{
	this->runAction(RepeatForever::create(
		CCRotateBy::create(time, -360.0f)));
	this->scheduleUpdate();
}

void E_FireString::update(float dt)
{
	angle += (6.0 / time);
	if (angle >= 360)
	{
		angle -= 360.0f;
	}
}


// ********************** E_FlyingFish ****************** //
E_FlyingFish::E_FlyingFish(float _offsetH, float _offsetV, float _duration)
{
	enemyType = eEnemy_flyFish;
	enemyState = eEnemyState_active;
	enemyBody = Sprite::create("flyFishRight.png", CCRectMake(16 * 4, 0, 16, 16));
	bodySize = CCSizeMake(16, 16);
	enemyBody->setAnchorPoint(ccp(0, 0));
	this->setContentSize(bodySize);
	this->addChild(enemyBody);
	this->setAnchorPoint(ccp(0.5f, 0.0f));

	enemyLifeOver = SpriteFrame::create("flyFishRight.png", CCRectMake(16 * 4, 0, 16, 16));
	enemyLifeOver->retain();
	overByArrow = enemyLifeOver;

	offsetH = _offsetH;
	offsetV = _offsetV;
	offsetDuration = _duration;
	isFlying = false;
}

E_FlyingFish::~E_FlyingFish()
{
	this->unscheduleAllSelectors();
}

void E_FlyingFish::launchEnemy()
{
	this->setVisible(false);
	enemyBody->runAction(RepeatForever::create(sAnimationMgr->createAnimate(eAniFlyFishR)));
	this->scheduleUpdate();
}

EnemyVSMario E_FlyingFish::checkCollisionWithMario()
{
	EnemyVSMario ret = eVS_nonKilled;

	Point marioPos = Mario::getMarioInstance()->getPosition();
	Size marioSize = Mario::getMarioInstance()->getContentSize();
	Rect marioRect = CCRectMake(marioPos.x - marioSize.width / 2 + 2, marioPos.y + 2,
		marioSize.width - 4, marioSize.height - 4);

	Point enemyPos = this->getPosition();
	Rect enemyRect = CCRectMake(enemyPos.x - bodySize.width / 2 + 2, enemyPos.y + 2,
		bodySize.width - 4, bodySize.height - 4);

	if (marioRect.intersectsRect(enemyRect))
	{
		ret = eVS_marioKilled;
	}

	return ret;
}

void E_FlyingFish::checkState()
{

}

void E_FlyingFish::update(float dt)
{
	if (!isFlying)
	{
		Point marioPos = Mario::getMarioInstance()->getPosition();
		if (fabs(marioPos.x - enemyPos.x) < bodySize.width)
		{
			this->flyInSky();
		}
	}
}

void E_FlyingFish::flyInSky()
{
	isFlying = true;
	this->setVisible(true);
	ActionInterval *pMoveBy = MoveBy::create(offsetDuration, ccp(offsetH, offsetV));
	this->runAction(Sequence::create(pMoveBy,
		CallFunc::create(this, callfunc_selector(E_FlyingFish::reSetNotInSky)), NULL));
}

void E_FlyingFish::reSetNotInSky()
{
	this->setVisible(false);
	this->runAction(Place::create(enemyPos));
	isFlying = false;
}

void E_FlyingFish::KilledByMario()
{

}


// ********************** E_Boss ****************** //
E_Boss::E_Boss()
{
	enemyType = eEnemy_Boss;
	enemyState = eEnemyState_nonactive;

	enemyBody = Sprite::create("boss.png", CCRectMake(0, 0, 32, 32));
	enemyBody->setAnchorPoint(ccp(0, 0));
	bodySize = CCSizeMake(32, 32);
	this->setContentSize(bodySize);
	this->addChild(enemyBody);
	this->setAnchorPoint(ccp(0.5f, 0.0f));

	enemyLifeOver = SpriteFrame::create("boss.png", CCRectMake(0, 0, 32, 32));
	enemyLifeOver->retain();
	overByArrow = enemyLifeOver;
	overByArrow->retain();

	ccMoveOffset = 0.5f;
	moveOffset = -ccMoveOffset;
}

E_Boss::~E_Boss()
{
	this->unscheduleAllSelectors();
}

void E_Boss::launchEnemy()
{
	leftSide = enemyPos.x - 32;
	rightSide = enemyPos.x + 32;

	this->scheduleUpdate();
}

EnemyVSMario E_Boss::checkCollisionWithMario()
{
	EnemyVSMario ret = eVS_nonKilled;

	Point marioPos = Mario::getMarioInstance()->getPosition();
	Size marioSize = Mario::getMarioInstance()->getContentSize();
	Rect marioRect = CCRectMake(marioPos.x - marioSize.width / 2 + 2, marioPos.y + 2,
		marioSize.width - 4, marioSize.height - 4);

	Point enemyPos = this->getPosition();
	Rect enemyRect = CCRectMake(enemyPos.x - bodySize.width / 2 + 2, enemyPos.y + 2,
		bodySize.width - 4, bodySize.height - 4);

	if (marioRect.intersectsRect(enemyRect))
	{
		ret = eVS_marioKilled;
	}

	return ret;
}

void E_Boss::KilledByBullet()
{
	if (Global::getGlobalInstance()->getCurrentBulletType() == eBullet_arrow)
	{
		this->runAction(Sequence::create(MoveBy::create(0.1f, ccp(8, 0)),
			MoveBy::create(0.1f, ccp(-8, 0)), NULL));

		static int num = 0;
		++num;
		if (num == 5)
		{
			enemyState = eEnemyState_over;
			enemyBody->stopAllActions();
			this->stopAllActions();
			this->unscheduleUpdate();
			this->setVisible(false);
		}
	}
}

void E_Boss::KilledByMario()
{

}

void E_Boss::moveLeft()
{
	enemyBody->stopAllActions();
	enemyBody->runAction(RepeatForever::create(sAnimationMgr->createAnimate(eAniBossMoveLeft)));
}

void E_Boss::moveRight()
{
	enemyBody->stopAllActions();
	enemyBody->runAction(RepeatForever::create(sAnimationMgr->createAnimate(eAniBossMoveRight)));
}

void E_Boss::update(float dt)
{
	this->checkState();

	if (enemyState == eEnemyState_active)
	{
		Point currentPos = this->getPosition();
		currentPos.x += moveOffset;
		this->setPosition(currentPos);

		this->enemyCollistionH();
	}
}

void E_Boss::enemyCollistionH()
{
	Point pos = this->getPosition();

	float leftCheck = pos.x - bodySize.width / 2;

	if (leftCheck - leftSide <= 0.5f)
	{
		if (enemyState == eEnemyState_active)
		{
			if (!UserDefault::getInstance()->getBoolForKey(EFFECT_KEY))
			    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("BossHuoQiu.wav");
			Point tempPos = ccp(pos.x - bodySize.width / 4, pos.y + 3 * (bodySize.height) / 4);
			GameMap::getGameMap()->createNewBulletForBoss(tempPos, eEnemy_BossFire);
		}
	}

	if (leftCheck <= leftSide)
	{
		moveOffset *= -1;
		this->moveRight();
		return;
	}

	float rightCheck = pos.x + bodySize.width / 2;
	if (rightCheck >= rightSide)
	{
		moveOffset *= -1;
		this->moveLeft();
	}
}


// ********************** E_BossFire ****************** //
E_BossFire::E_BossFire()
{
	enemyState = eEnemyState_active;
	enemyType = eEnemy_BossFire;

	enemyBody = Sprite::create("bossBullet.png", CCRectMake(0, 0, 24, 8));
	enemyBody->setAnchorPoint(ccp(0, 0));
	bodySize = CCSizeMake(24, 8);
	this->setContentSize(bodySize);
	this->addChild(enemyBody);
	this->setAnchorPoint(ccp(0.5f, 0.0f));

	enemyLifeOver = SpriteFrame::create("bossBullet.png", CCRectMake(0, 0, 24, 8));
	enemyLifeOver->retain();
	overByArrow = enemyLifeOver;
	overByArrow->retain();

	moveOffset = -3.0f;

}

E_BossFire::~E_BossFire()
{
	this->unscheduleAllSelectors();
}

void E_BossFire::launchEnemy()
{
	enemyBody->runAction(RepeatForever::create(sAnimationMgr->createAnimate(eAniBossFireLeft)));
	this->scheduleUpdate();
}

EnemyVSMario E_BossFire::checkCollisionWithMario()
{
	EnemyVSMario ret = eVS_nonKilled;

	Point marioPos = Mario::getMarioInstance()->getPosition();
	Size marioSize = Mario::getMarioInstance()->getContentSize();
	Rect marioRect = CCRectMake(marioPos.x - marioSize.width / 2 + 2, marioPos.y + 2,
		marioSize.width - 4, marioSize.height - 4);

	Point enemyPos = this->getPosition();
	Rect enemyRect = CCRectMake(enemyPos.x - bodySize.width / 2, enemyPos.y,
		bodySize.width, bodySize.height);

	if (marioRect.intersectsRect(enemyRect))
	{
		ret = eVS_marioKilled;
	}

	return ret;
}

void E_BossFire::KilledByBullet()
{

}

void E_BossFire::KilledByMario()
{

}

void E_BossFire::update(float dt)
{
	this->checkState();

	if (enemyState == eEnemyState_active)
	{
		Point currentPos = this->getPosition();
		currentPos.x += moveOffset;
		this->setPosition(currentPos);
	}
}


// ********************** E_Mushroom_Hidden ****************** //
E_Mushroom_Hidden::E_Mushroom_Hidden(int _addnum)
{
	enemyType = eEnemy_AddMushroom;
	enemyState = eEnemyState_nonactive;

	enemyBody = Sprite::create("Mushroom0.png", CCRectMake(0, 0, 16, 16));
	enemyBody->setAnchorPoint(ccp(0.0f, 0.0f));
	bodySize = CCSizeMake(16, 16);
	this->setContentSize(bodySize);
	this->addChild(enemyBody);
	this->setAnchorPoint(ccp(0.5f, 0.0f));

	enemyLifeOver = SpriteFrame::create("Mushroom0.png", CCRectMake(0, 0, 16, 16));
	enemyLifeOver->retain();
	overByArrow = SpriteFrame::create("Mushroom0.png", CCRectMake(16 * 3, 0, 16, 16));
	overByArrow->retain();

	addNums = _addnum;
	isAddable = true;
}

E_Mushroom_Hidden::~E_Mushroom_Hidden()
{
	this->unscheduleAllSelectors();
}

void E_Mushroom_Hidden::launchEnemy()
{
	this->scheduleUpdate();
}

EnemyVSMario E_Mushroom_Hidden::checkCollisionWithMario()
{
	EnemyVSMario ret = eVS_nonKilled;

	Point marioPos = Mario::getMarioInstance()->getPosition();
	Size marioSize = Mario::getMarioInstance()->getContentSize();
	Rect marioRect = CCRectMake(marioPos.x - marioSize.width / 2 + 2, marioPos.y + 2,
		marioSize.width - 4, marioSize.height - 4);

	Point enemyPos = this->getPosition();
	Rect enemyRect = CCRectMake(enemyPos.x - bodySize.width / 2 + 2, enemyPos.y + 2,
		bodySize.width - 4, bodySize.height - 4);

	if (marioRect.intersectsRect(enemyRect))
	{
		ret = eVS_marioKilled;
	}

	return ret;
}

void E_Mushroom_Hidden::KilledByMario()
{

}

void E_Mushroom_Hidden::update(float dt)
{
	this->checkState();

	if (enemyState == eEnemyState_active)
	{
		if (addNums)
		{
			if (isAddable)
			{
				isAddable = false;
				this->runAction(Sequence::create(MoveBy::create(0.5f, ccp(0, 16)),
					CallFunc::create(this, callfunc_selector(E_Mushroom_Hidden::addMushroom)), NULL));

				this->runAction(Sequence::create(DelayTime::create(2.0f),
					CallFunc::create(this, callfunc_selector(E_Mushroom_Hidden::reSetNonAddable)), NULL));

			}
		}
		else
		{
			enemyState = eEnemyState_over;
			enemyBody->stopAllActions();
			this->stopAllActions();
			this->unscheduleUpdate();
			this->setVisible(false);
		}
	}
}

void E_Mushroom_Hidden::addMushroom()
{
	--addNums;
	GameMap::getGameMap()->createNewBulletForBoss(this->getPosition(), eEnemy_mushroom);
	this->runAction(MoveBy::create(0.5f, ccp(0, -16)));
}

void E_Mushroom_Hidden::reSetNonAddable()
{
	isAddable = true;
}


// ********************** E_Battery ****************** //
E_Battery::E_Battery(float delay)
{
	enemyType = eEnemy_Battery;
	enemyState = eEnemyState_nonactive;

	enemyBody = Sprite::create("battery.png");
	enemyBody->setAnchorPoint(ccp(0, 0));
	bodySize = CCSizeMake(32, 32);
	this->setContentSize(bodySize);
	this->addChild(enemyBody);
	this->setAnchorPoint(ccp(0.5f, 0.0f));

	enemyLifeOver = NULL;
	overByArrow = NULL;

	isFireable = true;

	fireDelay = delay;
}

E_Battery::~E_Battery()
{
	this->unscheduleAllSelectors();
}

void E_Battery::launchEnemy()
{
	firePos = ccp(enemyPos.x - bodySize.width / 2, enemyPos.y + bodySize.height / 2);

	this->scheduleUpdate();
}

void E_Battery::update(float dt)
{
	this->checkState();

	if (enemyState == eEnemyState_active)
	{
		if (isFireable)
		{
			isFireable = false;

			this->addBatteryBullet();

			this->runAction(Sequence::create(DelayTime::create(fireDelay),
				CallFunc::create(this, callfunc_selector(E_Battery::reSetNonFireable)), NULL));
		}
	}
}

EnemyVSMario E_Battery::checkCollisionWithMario()
{
	EnemyVSMario ret = eVS_nonKilled;

	Point marioPos = Mario::getMarioInstance()->getPosition();
	Size marioSize = Mario::getMarioInstance()->getContentSize();
	Rect marioRect = CCRectMake(marioPos.x - marioSize.width / 2 + 2, marioPos.y + 2,
		marioSize.width - 4, marioSize.height - 4);

	Point enemyPos = this->getPosition();
	Rect enemyRect = CCRectMake(enemyPos.x - bodySize.width / 2 + 2, enemyPos.y + 2,
		bodySize.width - 4, bodySize.height - 4);

	if (marioRect.intersectsRect(enemyRect))
	{
		ret = eVS_marioKilled;
	}

	return ret;
}

void E_Battery::KilledByBullet()
{
	// 炮台只能被火球打死，不能被箭射死
	switch (Global::getGlobalInstance()->getCurrentBulletType())
	{
	case eBullet_common:
	{
		enemyBody->runAction(Sequence::create(sAnimationMgr->createAnimate(eAniBatteryBoom),
			CallFunc::create(this, callfunc_selector(E_Battery::stopAndClear)), NULL));

		break;
	}
	default:
		break;
	}
}

void E_Battery::stopAndClear()
{
	enemyState = eEnemyState_over;

	enemyBody->stopAllActions();
	this->stopAllActions();
	this->unscheduleUpdate();
	this->setVisible(false);
}

void E_Battery::KilledByMario()
{

}

void E_Battery::addBatteryBullet()
{
	GameMap::getGameMap()->createNewBulletForBoss(firePos, eEnemy_BatteryBullet);
}

void E_Battery::reSetNonFireable()
{
	isFireable = true;
}


// ********************** E_BatteryBullet ****************** //
E_BatteryBullet::E_BatteryBullet()
{
	enemyType = eEnemy_BatteryBullet;
	enemyState = eEnemyState_active;

	enemyBody = Sprite::create("batteryBullet.png");
	enemyBody->setAnchorPoint(ccp(0, 0));
	bodySize = CCSizeMake(4, 4);
	this->setContentSize(bodySize);
	this->addChild(enemyBody);
	this->setAnchorPoint(ccp(0.5f, 0.5f));

	ccMoveOffset = -1.5f;
	moveOffset = ccMoveOffset;
}

E_BatteryBullet::~E_BatteryBullet()
{
	this->unscheduleAllSelectors();
}

void E_BatteryBullet::launchEnemy()
{
	this->scheduleUpdate();
}

EnemyVSMario E_BatteryBullet::checkCollisionWithMario()
{
	EnemyVSMario ret = eVS_nonKilled;

	Point marioPos = Mario::getMarioInstance()->getPosition();
	Size marioSize = Mario::getMarioInstance()->getContentSize();
	Rect marioRect = CCRectMake(marioPos.x - marioSize.width / 2 + 2, marioPos.y + 2,
		marioSize.width - 4, marioSize.height - 4);

	Point enemyPos = this->getPosition();

	// 用主角矩形是否包含这个小白点子弹的中心为准，判定是否与主角接触
	if (marioRect.containsPoint(enemyPos))
	{
		ret = eVS_marioKilled;
	}

	return ret;
}

void E_BatteryBullet::KilledByBullet()
{

}

void E_BatteryBullet::KilledByMario()
{

}

void E_BatteryBullet::update(float dt)
{
	this->checkState();

	if (enemyState == eEnemyState_active)
	{
		Point currentPos = this->getPosition();
		currentPos.x += moveOffset;
		this->setPosition(currentPos);
	}
}



// ********************** E_Cloud ****************** //
E_Cloud::E_Cloud(float _delay, int _type)
{
	enemyType = eEnemy_DarkCloud;
	enemyState = eEnemyState_nonactive;

	type = _type;
	switch (type)
	{
	case 0:
		enemyBody = Sprite::create("cloud.png", CCRectMake(114, 0, 32, 24));
		normal = SpriteFrame::create("cloud.png", CCRectMake(114, 0, 32, 24));
		bodySize = CCSizeMake(32, 32);
		dark = SpriteFrame::create("darkCloud.png", CCRectMake(114, 0, 32, 24));
		break;
	case 1:
		enemyBody = Sprite::create("cloud.png", CCRectMake(0, 0, 48, 24));
		normal = SpriteFrame::create("cloud.png", CCRectMake(0, 0, 48, 24));
		bodySize = CCSizeMake(64, 32);
		dark = SpriteFrame::create("darkCloud.png", CCRectMake(0, 0, 48, 24));
		break;
	case 2:
		enemyBody = Sprite::create("cloud.png", CCRectMake(49, 0, 64, 24));
		normal = SpriteFrame::create("cloud.png", CCRectMake(49, 0, 64, 24));
		bodySize = CCSizeMake(32, 32);
		dark = SpriteFrame::create("darkCloud.png", CCRectMake(49, 0, 64, 24));
		break;
	default:
		break;
	}

	dark->retain();
	normal->retain();

	enemyBody->setAnchorPoint(ccp(0, 0));
	this->setContentSize(bodySize);
	this->addChild(enemyBody);
	this->setAnchorPoint(ccp(0.5f, 0.0f));

	enemyLifeOver = NULL;
	overByArrow = NULL;

	delay = _delay;
	dropRegion = 64.0f;

	isDropable = true;
}

E_Cloud::~E_Cloud()
{
	this->unscheduleAllSelectors();
}

void E_Cloud::launchEnemy()
{
	leftSide = enemyPos.x - dropRegion;
	rightSide = enemyPos.x + dropRegion;

	this->scheduleUpdate();
}

void E_Cloud::update(float dt)
{
	this->checkState();

	Point marioPos = Mario::getMarioInstance()->getPosition();

	if (enemyState == eEnemyState_active)
	{
		if (leftSide <= marioPos.x && marioPos.x <= rightSide)
		{
			if (isDropable)
			{
				isDropable = false;

				this->addLightning();

				this->runAction(Sequence::create(DelayTime::create(delay),
					CallFunc::create(this, callfunc_selector(E_Cloud::reSetDropable)), NULL));
			}
		}
	}
}

void E_Cloud::addLightning()
{
	enemyBody->setDisplayFrame(dark);
	GameMap::getGameMap()->createNewBulletForBoss(enemyPos, eEnemy_Lightning);
	this->runAction(Sequence::create(DelayTime::create(0.3f),
		CallFunc::create(this, callfunc_selector(E_Cloud::reSetNormal)), NULL));
}

void E_Cloud::reSetNormal()
{
	enemyBody->setDisplayFrame(normal);
}

void E_Cloud::reSetDropable()
{
	isDropable = true;
}

void E_Cloud::KilledByMario()
{

}

void E_Cloud::KilledByBullet()
{

}


// ********************** E_Lightning ****************** //
E_Lightning::E_Lightning()
{
	enemyType = eEnemy_Lightning;
	enemyState = eEnemyState_active;

	enemyBody = Sprite::create("lightning.png", CCRectMake(0, 0, 16, 32));
	enemyBody->setAnchorPoint(ccp(0, 0));
	bodySize = CCSizeMake(16, 32);
	this->setContentSize(bodySize);
	this->addChild(enemyBody);
	this->setAnchorPoint(ccp(0.5f, 0.0f));

	enemyLifeOver = NULL;
	overByArrow = NULL;

	ccJumpOffset = 2.0f;
	jumpOffset = -ccJumpOffset;
}

E_Lightning::~E_Lightning()
{
	this->unscheduleAllSelectors();
}

void E_Lightning::launchEnemy()
{
	enemyBody->runAction(RepeatForever::create(sAnimationMgr->createAnimate(eAniLightning)));

	this->scheduleUpdate();
}

void E_Lightning::update(float dt)
{
	this->checkState();

	if (enemyState == eEnemyState_active)
	{
		Point currentPos = this->getPosition();
		currentPos.y += jumpOffset;
		this->setPosition(currentPos);
	}
}

void E_Lightning::checkState()
{
	Point pos = this->getPosition();

	if (pos.y <= 2)
	{
		enemyState = eEnemyState_over;
		enemyBody->stopAllActions();
		this->stopAllActions();
		this->unscheduleUpdate();
		this->setVisible(false);
	}
}

EnemyVSMario E_Lightning::checkCollisionWithMario()
{
	EnemyVSMario ret = eVS_nonKilled;

	Point marioPos = Mario::getMarioInstance()->getPosition();
	Size marioSize = Mario::getMarioInstance()->getContentSize();
	Rect marioRect = CCRectMake(marioPos.x - marioSize.width / 2 + 2, marioPos.y + 2,
		marioSize.width - 4, marioSize.height - 4);

	Point enemyPos = this->getPosition();
	Rect enemyRect = CCRectMake(enemyPos.x - bodySize.width / 2 + 2, enemyPos.y + 2,
		bodySize.width - 4, bodySize.height - 4);

	if (marioRect.intersectsRect(enemyRect))
	{
		ret = eVS_marioKilled;
	}

	return ret;
}

void E_Lightning::KilledByBullet()
{

}

void E_Lightning::KilledByMario()
{

}
