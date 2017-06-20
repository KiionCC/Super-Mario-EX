#include "SimpleAudioEngine.h"

#include "AnimationManager.h"
#include "Data_Global.h"
#include "Resource_Role.h"

Mario* Mario::_marioInstance;
Mario* Mario::getMarioInstance()
{
	return _marioInstance;
}

Mario::Mario() :
	mainBody(NULL),
	mainTemp(NULL),
	_jumpLeft(NULL),
	_jumpRight(NULL),
	_lifeOverSmall(NULL),
	_lifeOverNormal(NULL),
	_lifeOverFire(NULL),
	_normalLeft(NULL),
	_normalRight(NULL),
	_smallJumpLeft(NULL),
	_smallJumpRight(NULL),
	_smallLeft(NULL),
	_smallRight(NULL),
	pLabelUp(NULL)
{
	norBodySize = CCSizeMake(18, 32);
	smallSize = CCSizeMake(14, 16);
	currentSize = smallSize;
	state = eNormalRight;
	statePre = eNormalRight;
	face = eRight;
	isFlying = false;
	bodyType = eBody_Small;

	CCTexture2D *pTexture = TextureCache::getInstance()->addImage("walkLeft.png");
	_jumpLeft = SpriteFrame::createWithTexture(pTexture, CCRectMake(180, 0, 18, 32));
	_jumpLeft->retain();
	_normalLeft = SpriteFrame::createWithTexture(pTexture, CCRectMake(18 * 9, 0, 18, 32));
	_normalLeft->retain();

	pTexture = TextureCache::getInstance()->addImage("WalkLeft_fire.png");
	_jumpLeftFire = SpriteFrame::createWithTexture(pTexture, CCRectMake(180, 0, 18, 32));
	_jumpLeftFire->retain();
	_normalLeftFire = SpriteFrame::createWithTexture(pTexture, CCRectMake(18 * 9, 0, 18, 32));
	_normalLeftFire->retain();

	pTexture = TextureCache::getInstance()->addImage("WalkRight_fire.png");
	_jumpRightFire = SpriteFrame::createWithTexture(pTexture, CCRectMake(180, 0, 18, 32));
	_jumpRightFire->retain();
	_normalRightFire = SpriteFrame::createWithTexture(pTexture, CCRectMake(0, 0, 18, 32));
	_normalRightFire->retain();

	pTexture = TextureCache::getInstance()->addImage("walkRight.png");
	_jumpRight = SpriteFrame::createWithTexture(pTexture, CCRectMake(180, 0, 18, 32));
	_jumpRight->retain();
	_normalRight = SpriteFrame::createWithTexture(pTexture, CCRectMake(0, 0, 18, 32));
	_normalRight->retain();

	pTexture = TextureCache::getInstance()->addImage("smallWalkRight.png");
	_smallRight = SpriteFrame::createWithTexture(pTexture, CCRectMake(0, 0, 14, 16));
	_smallRight->retain();
	_smallJumpRight = SpriteFrame::createWithTexture(pTexture, CCRectMake(140, 0, 14, 16));
	_smallJumpRight->retain();

	pTexture = TextureCache::getInstance()->addImage("smallWalkLeft.png");
	_smallLeft = SpriteFrame::createWithTexture(pTexture, CCRectMake(14 * 9, 0, 14, 16));
	_smallLeft->retain();
	_smallJumpLeft = SpriteFrame::createWithTexture(pTexture, CCRectMake(140, 0, 14, 16));
	_smallJumpLeft->retain();

	pTexture = TextureCache::getInstance()->addImage("small_die.png");
	_lifeOverSmall = SpriteFrame::createWithTexture(pTexture, CCRectMake(16, 0, 16, 18));
	_lifeOverSmall->retain();

	pTexture = TextureCache::getInstance()->addImage("normal_die.png");
	_lifeOverNormal = SpriteFrame::createWithTexture(pTexture, CCRectMake(24, 0, 24, 34));
	_lifeOverNormal->retain();

	pTexture = TextureCache::getInstance()->addImage("fire_die.png");
	_lifeOverFire = SpriteFrame::createWithTexture(pTexture, CCRectMake(24, 0, 24, 34));
	_lifeOverFire->retain();

	pTexture = TextureCache::getInstance()->addImage("allow_walkLeft.png");
	_normalLeftArrow = SpriteFrame::createWithTexture(pTexture, CCRectMake(0, 0, 18, 32));
	_normalLeftArrow->retain();
	_jumpLeftArrow = SpriteFrame::createWithTexture(pTexture, CCRectMake(180, 0, 18, 32));
	_jumpLeftArrow->retain();

	pTexture = TextureCache::getInstance()->addImage("allow_walkRight.png");
	_normalRightArrow = SpriteFrame::createWithTexture(pTexture, CCRectMake(0, 0, 18, 32));
	_normalRightArrow->retain();
	_jumpRightArrow = SpriteFrame::createWithTexture(pTexture, CCRectMake(180, 0, 18, 32));
	_jumpRightArrow->retain();

	isDied = false;

	_marioInstance = this;

	isSafeTime = false;

	bulletable = false;

	ladderable = false;

	currentBulletType = eBullet_common;

	_marioInstance = this;
}

Mario::~Mario()
{
	this->unscheduleAllSelectors();
}

void Mario::setMarioState(marioDirection _state)
{
	if (isDied)
	{
		return;
	}
	if (state == _state)
	{
		return;
	}
	statePre = state;
	state = _state;

	mainBody->stopAllActions();
	switch (_state)
	{
	case eFireTheHole:
	{

		break;
	}
	case eNormalRight:
	{
		if (bodyType == eBody_Normal)
		{
			if (bulletable)
			{
				switch (Global::getGlobalInstance()->getCurrentBulletType())
				{
				case eBullet_common:
					mainBody->setDisplayFrame(_normalRightFire);
					break;
				case eBullet_arrow:
					mainBody->setDisplayFrame(_normalRightArrow);
					break;
				}
			}
			else
			{
				mainBody->setDisplayFrame(_normalRight);
			}
		}
		else
		{
			mainBody->setDisplayFrame(_smallRight);
		}
		face = eRight;
		break;
	}
	case eNormalLeft:
	{
		if (bodyType == eBody_Normal)
		{
			if (bulletable)
			{
				switch (Global::getGlobalInstance()->getCurrentBulletType())
				{
				case eBullet_common:
					mainBody->setDisplayFrame(_normalLeftFire);
					break;
				case eBullet_arrow:
					mainBody->setDisplayFrame(_normalLeftArrow);
					break;
				}
			}
			else
			{
				mainBody->setDisplayFrame(_normalLeft);
			}
		}
		else
		{
			mainBody->setDisplayFrame(_smallLeft);
		}
		face = eLeft;
		break;
	}
	case eRight:
	{
		if (!isFlying)
		{
			if (bodyType == eBody_Normal)
			{
				if (bulletable)
				{
					switch (Global::getGlobalInstance()->getCurrentBulletType())
					{
					case eBullet_common:
						mainBody->runAction(RepeatForever::create(
							sAnimationMgr->createAnimate(eAniRightFire)));
						break;
					case eBullet_arrow:
						mainBody->runAction(RepeatForever::create(
							sAnimationMgr->createAnimate(eAniArrowRight)));
						break;
					}
				}
				else
				{
					mainBody->runAction(RepeatForever::create(
						sAnimationMgr->createAnimate(eAniRight)));
				}
			}
			else
			{
				mainBody->runAction(RepeatForever::create(
					sAnimationMgr->createAnimate(eAniRightSmall)));
			}
		}
		face = eRight;
		break;
	}
	case eLeft:
	{
		if (!isFlying)
		{
			if (bodyType == eBody_Normal)
			{
				if (bulletable)
				{
					switch (Global::getGlobalInstance()->getCurrentBulletType())
					{
					case eBullet_common:
						mainBody->runAction(RepeatForever::create(
							sAnimationMgr->createAnimate(eAniLeftFire)));
						break;
					case eBullet_arrow:
						mainBody->runAction(RepeatForever::create(
							sAnimationMgr->createAnimate(eAniArrowLeft)));
						break;
					}
				}
				else
				{
					mainBody->runAction(RepeatForever::create(
						sAnimationMgr->createAnimate(eAniLeft)));
				}
			}
			else
			{
				mainBody->runAction(RepeatForever::create(
					sAnimationMgr->createAnimate(eAniLeftSmall)));
			}

		}
		face = eLeft;
		break;
	}
	case eJumpLeft:
	{
		if (bodyType == eBody_Normal)
		{
			if (bulletable)
			{
				switch (Global::getGlobalInstance()->getCurrentBulletType())
				{
				case eBullet_common:
					mainBody->setDisplayFrame(_jumpLeftFire);
					break;
				case eBullet_arrow:
					mainBody->setDisplayFrame(_jumpLeftArrow);
					break;
				}
			}
			else
			{
				mainBody->setDisplayFrame(_jumpLeft);
			}
		}
		else
		{
			mainBody->setDisplayFrame(_smallJumpLeft);
		}
		face = eLeft;
		break;
	}
	case eJumpRight:
	{
		if (bodyType == eBody_Normal)
		{
			if (bulletable)
			{
				switch (Global::getGlobalInstance()->getCurrentBulletType())
				{
				case eBullet_common:
					mainBody->setDisplayFrame(_jumpRightFire);
					break;
				case eBullet_arrow:
					mainBody->setDisplayFrame(_jumpRightArrow);
					break;
				}
			}
			else
			{
				mainBody->setDisplayFrame(_jumpRight);
			}
		}
		else
		{
			mainBody->setDisplayFrame(_smallJumpRight);
		}
		face = eRight;
		break;
	}
	default:
		break;
	}
}

marioDirection Mario::getMarioState()
{
	return state;
}

void Mario::onEnter()
{
	Node::onEnter();
}

void Mario::onExit()
{
	Node::onExit();
}

bool Mario::marioInit()
{
	this->setContentSize(smallSize);
	// 最初加载小型马里奥
	mainBody = Sprite::create("smallWalkRight.png", CCRectMake(0, 0, 14, 16));
	mainBody->setAnchorPoint(ccp(0, 0));
	this->addChild(mainBody);
	state = eNormalRight;
	return true;
}

Mario* Mario::create()
{
	Mario *pMario = new Mario();
	if (pMario && pMario->marioInit())
	{
		pMario->autorelease();
		return pMario;
	}
	CC_SAFE_DELETE(pMario);
	return NULL;
}

void Mario::setBodyType(BodyType _bodytype)
{
	bodyType = _bodytype;
	switch (_bodytype)
	{
	case eBody_Normal:
		currentSize = norBodySize;
		mainBody->setDisplayFrame(_normalRight);
		break;
	case eBody_Small:
		currentSize = smallSize;
		mainBody->setDisplayFrame(_smallRight);
		break;
	case eBody_Fireable:
	{
		bodyType = eBody_Normal;
		currentSize = norBodySize;
		bulletable = true;
		switch (Global::getGlobalInstance()->getCurrentBulletType())
		{
		case eBullet_arrow:
			mainBody->setDisplayFrame(_normalRightArrow);
			break;
		case eBullet_common:
			mainBody->setDisplayFrame(_normalRightFire);
			break;
		}

		break;
	}
	default:
		break;
	}
	this->setContentSize(currentSize);
}

Size Mario::getCurrentSize()
{
	return currentSize;
}

BodyType Mario::getCurrentBodyType()
{
	return bodyType;
}

void Mario::setSafeTime(bool _issafe)
{
	isSafeTime = _issafe;
}

bool Mario::getIsSafeTime()
{
	return isSafeTime;
}

void Mario::changeForGotAddLifeMushroom()
{
	if (!UserDefault::getInstance()->getBoolForKey(EFFECT_KEY))
    	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("AddLife.wav");

	Global::getGlobalInstance()->lifeNumPlusOne();

	pLabelUp = CCLabelTTF::create("UP1!", "Arial", 20);
	pLabelUp->setPosition(Vec2(0, 0));
	this->addChild(pLabelUp);
	JumpBy *pJump = JumpBy::create(0.5f, ccp(0, this->getContentSize().height / 2),
		this->getContentSize().height, 1);
	pLabelUp->runAction(Sequence::create(pJump,
		CallFunc::create(this, callfunc_selector(Mario::clearLabelUp)), NULL));
}

void Mario::changeForGotMushroom()
{
	if (!UserDefault::getInstance()->getBoolForKey(EFFECT_KEY))
    	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("EatMushroomOrFlower.wav");

	switch (bodyType)
	{
	case eBody_Small:
	{
		// 播放一段变大闪烁的动画
		Global::getGlobalInstance()->currentMarioType = eBody_Normal;
		this->setMarioTypeForNormal();
		ActionInterval *pBlink = CCBlink::create(1, 5);
		this->runAction(pBlink);
	}
	break;
	case eBody_Normal:
		// 展示一个up，显示奖励一个人，从主角脚底上升到头顶然后消失
	{
		bulletable = true;
		Global::getGlobalInstance()->currentMarioType = eBody_Fireable;
		if (!bulletable)
		{
			switch (face)
			{
			case eRight:
				mainBody->setDisplayFrame(_normalRightFire);
				break;
			case eLeft:
				mainBody->setDisplayFrame(_normalLeftFire);
				break;
			default:
				break;
			}
		}
	}
	break;
	default:
		break;
	}
}

void Mario::changeForGotEnemy()
{
	isSafeTime = true;
	ActionInterval *pDelay = DelayTime::create(3.0f);
	this->runAction(Sequence::create(pDelay,
		CallFunc::create(this, callfunc_selector(Mario::reSetSafeTime)), NULL));

	switch (bodyType)
	{
	case eBody_Normal:
	{
		if (!UserDefault::getInstance()->getBoolForKey(EFFECT_KEY))
	    	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("SuoXiao.wav");
		Global::getGlobalInstance()->currentMarioType = eBody_Small;
		this->setMarioTypeForSmall();
		ActionInterval *pBlink = CCBlink::create(3, 15);
		this->runAction(pBlink);
		break;
	}
	case eBody_Small:
	{
		if (!UserDefault::getInstance()->getBoolForKey(EFFECT_KEY))
	    	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("YuDaoGuaiWuSi.wav");
		mainBody->stopAllActions();
		mainBody->setDisplayFrame(_lifeOverSmall);
		this->setMarioDie(true);
		break;
	}
	default:
		break;
	}
}

void Mario::setMarioDie(bool _die)
{
	isDied = _die;
	Global::getGlobalInstance()->currentMarioType = eBody_Small;
	Global::getGlobalInstance()->lifeNumCutOne();
}
bool Mario::isMarioDied()
{
	return isDied;
}

void Mario::dieForTrap()
{
	mainBody->stopAllActions();

	switch (bodyType)
	{
	case eBody_Small:
		mainBody->setDisplayFrame(_lifeOverSmall);
		mainBody->runAction(sAnimationMgr->createAnimate(eAniSmallDie));
		break;
	case eBody_Normal:
		if (bulletable)
		{
			switch (Global::getGlobalInstance()->getCurrentBulletType())
			{
			case eBullet_common:
				mainBody->setDisplayFrame(_lifeOverFire);
				mainBody->runAction(sAnimationMgr->createAnimate(eAniFireDie));
				break;
			case eBullet_arrow:
				_lifeOverFire = SpriteFrame::create("arrow_die.png", CCRectMake(24, 0, 24, 32));
				mainBody->setDisplayFrame(_lifeOverFire);
				mainBody->runAction(sAnimationMgr->createAnimate(eAniArrowDie));
			}
		}
		else
		{
			mainBody->setDisplayFrame(_lifeOverNormal);
			mainBody->runAction(sAnimationMgr->createAnimate(eAniNormalDie));
		}
		break;
	default:
		break;
	}

	ActionInterval *pMoveUp = MoveBy::create(0.6f, ccp(0, 32));
	ActionInterval *pMoveDown = MoveBy::create(0.6f, ccp(0, -32));
	ActionInterval *pDeley = DelayTime::create(0.2f);

	this->runAction(Sequence::create(pMoveUp, pDeley, pMoveDown,
		CallFunc::create(this, callfunc_selector(Mario::reSetNonVisible)), NULL));
}

void Mario::reSetNonVisible()
{
	mainBody->stopAllActions();
	this->setVisible(false);
}

void Mario::clearLabelUp()
{
	pLabelUp->removeFromParentAndCleanup(true);
	pLabelUp = NULL;
}

void Mario::setMarioTypeForSmall()
{
	this->setBodyType(eBody_Small);
	bulletable = false;
	switch (face)
	{
	case eRight:
		mainBody->setDisplayFrame(_smallRight);
		break;
	case eLeft:
		mainBody->setDisplayFrame(_smallLeft);
		break;
	default:
		break;
	}
}

void Mario::setMarioTypeForNormal()
{
	this->setBodyType(eBody_Normal);
	switch (face)
	{
	case eRight:
		mainBody->setDisplayFrame(_normalRight);
		break;
	case eLeft:
		mainBody->setDisplayFrame(_normalLeft);
		break;
	default:
		break;
	}
}

void Mario::reSetSafeTime()
{
	isSafeTime = false;
}

bool Mario::isBulletable()
{
	return bulletable;
}

void Mario::setLadderable(bool _gadgetable)
{
	ladderable = _gadgetable;
}
bool Mario::isLadderable()
{
	return ladderable;
}

void Mario::fireAction()
{
	ActionInterval *pAction = NULL;
	this->setMarioState(eFireTheHole);
	switch (face)
	{
	case eRight:
	{
		switch (Global::getGlobalInstance()->getCurrentBulletType())
		{
		case eBullet_common:
			pAction = sAnimationMgr->createAnimate(eAniFireActionR);
			break;
		case eBullet_arrow:
			pAction = sAnimationMgr->createAnimate(eAniArrowActionR);
			break;
		}
		break;
	}
	case eLeft:
	{
		switch (Global::getGlobalInstance()->getCurrentBulletType())
		{
		case eBullet_common:
			pAction = sAnimationMgr->createAnimate(eAniFireActionL);
			break;
		case eBullet_arrow:
			pAction = sAnimationMgr->createAnimate(eAniArrowActionL);
			break;
		}
		break;
	}
	default:
		break;
	}
	mainBody->runAction(Sequence::create(pAction,
		CallFunc::create(this, callfunc_selector(Mario::reSetStateForFired)), NULL));
}

void Mario::reSetStateForFired()
{
	this->setMarioState(statePre);
}

void Mario::setBulletType(BulletType _type)
{
	if (currentBulletType != _type)
	{
		currentBulletType = _type;
		switch (face)
		{
		case eRight:
		{
			switch (currentBulletType)
			{
			case eBullet_common:
				mainBody->setDisplayFrame(_normalRightFire);
				break;
			case eBullet_arrow:
				mainBody->setDisplayFrame(_normalRightArrow);
				break;
			}
			break;
		}
		case eLeft:
		{
			switch (currentBulletType)
			{
			case eBullet_common:
				mainBody->setDisplayFrame(_normalLeftFire);
				break;
			case eBullet_arrow:
				mainBody->setDisplayFrame(_normalLeftArrow);
				break;
			}
			break;
		}
		}
	}
}

BulletType Mario::getBulletType()
{
	return currentBulletType;
}

void Mario::reSetForSuccess()
{
	switch (bodyType)
	{
	case eBody_Small:
		mainBody->setDisplayFrame(_smallRight);
		break;
	case eBody_Normal:
		if (bulletable)
		{
			switch (Global::getGlobalInstance()->getCurrentBulletType())
			{
			case eBullet_common:
				mainBody->setDisplayFrame(_normalRightFire);
				break;
			case eBullet_arrow:
				mainBody->setDisplayFrame(_normalRightArrow);
				break;
			}
		}
		else
		{
			mainBody->setDisplayFrame(_normalRight);
		}
		break;
	default:
		break;
	}
}
