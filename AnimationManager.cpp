#include <string>

#include "SimpleAudioEngine.h"

#include "AnimationManager.h"
using namespace std;

AnimationManager* AnimationManager::_instance = NULL;

Animation* AnimationManager::createAnimationByAnimationType(AnimationType key)
{
	Vector<SpriteFrame*> pArray;
	SpriteFrame *frame = NULL;
	Texture2D *pTexture;
	Animation *pAnimation = NULL;

	switch (key)
	{
	case eAniRight:
		{
			pTexture = TextureCache::getInstance()->addImage("walkRight.png");
			for (int i = 0; i < 10; ++i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(18*i, 0, 18, 32));
				pArray.pushBack(frame);
			}
			pAnimation = Animation::createWithSpriteFrames(pArray, 0.02f);
			break;
		}
	case eAniLeft:
		{
			pTexture = TextureCache::getInstance()->addImage("walkLeft.png");
			for (int i = 9; i >= 0; --i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(18*i, 0, 18, 32));
				pArray.pushBack(frame);
			}
			pAnimation = Animation::createWithSpriteFrames(pArray, 0.02f);
			break;
		}
	case eAniLeftSmall:
		{
			pTexture = TextureCache::getInstance()->addImage("smallWalkLeft.png");
			for (int i = 9; i >= 0; --i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(14*i, 0, 14, 16));
				pArray.pushBack(frame);
			}
			pAnimation = Animation::createWithSpriteFrames(pArray, 0.02f);
			break;
		}
	case eAniRightSmall:
		{
			pTexture = TextureCache::getInstance()->addImage("smallWalkRight.png");
			for (int i = 0; i < 10; ++i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(14*i, 0, 14, 16));
				pArray.pushBack(frame);
			}
			pAnimation = Animation::createWithSpriteFrames(pArray, 0.02f);
			break;
		}
	case eAniLeftFire:
		{
			pTexture = TextureCache::getInstance()->addImage("WalkLeft_fire.png");
			for (int i = 9; i >= 0; --i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(18*i, 0, 18, 32));
				pArray.pushBack(frame);
			}
			pAnimation = Animation::createWithSpriteFrames(pArray, 0.02f);
			break;
		}
	case eAniRightFire:
		{
			pTexture = TextureCache::getInstance()->addImage("WalkRight_fire.png");
			for (int i = 0; i < 10; ++i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(18*i, 0, 18, 32));
				pArray.pushBack(frame);
			}
			pAnimation = Animation::createWithSpriteFrames(pArray, 0.02f);
			break;
		}
	case eAniBlinkCoin:
		{
			pTexture = TextureCache::getInstance()->addImage("coinani.png");
			for (int i = 0; i < 4; ++i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(8*i, 0, 8, 14));
				pArray.pushBack(frame);
				pArray.pushBack(frame);
			}
			pAnimation = Animation::createWithSpriteFrames(pArray, 0.02f);
			break;
		}
	case eAniMushroom:
		{
			pTexture = TextureCache::getInstance()->addImage("Mushroom0.png");
			for (int i = 0; i < 2; ++i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(16*i, 0, 16, 16));
				pArray.pushBack(frame);
			}
			pAnimation = Animation::createWithSpriteFrames(pArray, 0.3f);
			break;
		}
	case eAniflower:
		{
			pTexture = TextureCache::getInstance()->addImage("flower0.png");
			for (int i = 0; i < 2; ++i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(16*i, 0, 16, 24));
				pArray.pushBack(frame);
			}
			pAnimation = Animation::createWithSpriteFrames(pArray, 0.3f);
			break;
		}
	case eAniTortoiseLeft:
		{
			pTexture = TextureCache::getInstance()->addImage("tortoise0.png");
			for (int i = 2; i < 4; ++i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(18*i, 0, 18, 24));
				pArray.pushBack(frame);
			}
			pAnimation = Animation::createWithSpriteFrames(pArray, 0.3f);
			break;
		}
	case eAniTortoiseRight:
		{
			pTexture = TextureCache::getInstance()->addImage("tortoise0.png");
			for (int i = 4; i < 6; ++i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(18*i, 0, 18, 24));
				pArray.pushBack(frame);
			}
			pAnimation = Animation::createWithSpriteFrames(pArray, 0.3f);
			break;
		}
	case eAniTortoiseFly:
		{
			pTexture = TextureCache::getInstance()->addImage("tortoise0.png");
			for (int i = 0; i < 2; ++i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(18*i, 0, 18, 24));
				pArray.pushBack(frame);
			}
			pAnimation = Animation::createWithSpriteFrames(pArray, 0.3f);
			break;
		}
	case eAniRotatedFireBall:
		{
			pTexture = TextureCache::getInstance()->addImage("fireRight.png");
			for (int i = 0; i < 8; ++i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(10*i, 3, 10, 10));
				pArray.pushBack(frame);
			}
			pAnimation = Animation::createWithSpriteFrames(pArray, 0.01f);
			break;
		}
	case eAniBoomedFireBall:
		{
			pTexture = TextureCache::getInstance()->addImage("fireRight.png");

			frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(80, 3, 10, 10));
			pArray.pushBack(frame);
			frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(90, 0, 14, 16));
			pArray.pushBack(frame);
			frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(105, 0, 16, 16));
			pArray.pushBack(frame);

			pAnimation = Animation::createWithSpriteFrames(pArray, 0.05f);
			break;
		}
	case eAniFireActionR:
		{
			pTexture = TextureCache::getInstance()->addImage("fireActionR.png");
			for (int i = 0; i < 6; ++i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(25*i, 0, 25, 32));
				pArray.pushBack(frame);
			}
			pAnimation = Animation::createWithSpriteFrames(pArray, 0.02f);
			break;
		}
	case eAniArrowActionR:
		{
			pTexture = TextureCache::getInstance()->addImage("arrowActionR.png");
			for (int i = 0; i < 6; ++i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(25*i, 0, 25, 32));
				pArray.pushBack(frame);
			}
			pAnimation = Animation::createWithSpriteFrames(pArray, 0.02f);
			break;
		}
	case eAniFireActionL:
		{
			pTexture = TextureCache::getInstance()->addImage("fireActionL.png");
			for (int i = 5; i >= 0; --i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(25*i, 0, 25, 32));
				pArray.pushBack(frame);
			}
			pAnimation = Animation::createWithSpriteFrames(pArray, 0.02f);
			break;
		}
	case eAniArrowActionL:
		{
			pTexture = TextureCache::getInstance()->addImage("arrowActionL.png");
			for (int i = 5; i >= 0; --i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(25*i, 0, 25, 32));
				pArray.pushBack(frame);
			}
			pAnimation = Animation::createWithSpriteFrames(pArray, 0.02f);
			break;
		}
	case eAniFlyFishR:
		{
			pTexture = TextureCache::getInstance()->addImage("flyFishRight.png");
			frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(16*4, 0, 16, 16));
			pArray.pushBack(frame);
			frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(16*5, 0, 16, 16));
			pArray.pushBack(frame);

			pAnimation = Animation::createWithSpriteFrames(pArray, 0.05f);
			break;
		}
	case eAniFlyFishL:
		{
			pTexture = TextureCache::getInstance()->addImage("flyFishLeft.png");
			frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(16*4, 0, 16, 16));
			pArray.pushBack(frame);
			frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(16*5, 0, 16, 16));
			pArray.pushBack(frame);

			pAnimation = Animation::createWithSpriteFrames(pArray, 0.05f);
			break;
		}
	case eAniArrowBroken:
		{
			pTexture = TextureCache::getInstance()->addImage("arrowBroken.png");
			for (int i = 0; i < 3; ++i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(16*i, 0, 16, 16));
				pArray.pushBack(frame);
			}

			pAnimation = Animation::createWithSpriteFrames(pArray, 0.05f);
			break;
		}
	case eAniSmallDie:
		{
			pTexture = TextureCache::getInstance()->addImage("small_die.png");
			for (int i = 0; i < 7; ++i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(16*i, 0, 16, 18));
				pArray.pushBack(frame);
				pArray.pushBack(frame);
			}

			pAnimation = Animation::createWithSpriteFrames(pArray, 0.1f);
			break;
		}
	case eAniNormalDie:
		{
			pTexture = TextureCache::getInstance()->addImage("normal_die.png");
			for (int i = 0; i < 7; ++i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(24*i, 0, 24, 34));
				pArray.pushBack(frame);
				pArray.pushBack(frame);
			}

			pAnimation = Animation::createWithSpriteFrames(pArray, 0.1f);
			break;
		}
	case eAniFireDie:
		{
			pTexture = TextureCache::getInstance()->addImage("fire_die.png");
			for (int i = 0; i < 7; ++i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(24*i, 0, 24, 34));
				pArray.pushBack(frame);
				pArray.pushBack(frame);
			}

			pAnimation = Animation::createWithSpriteFrames(pArray, 0.1f);
			break;
		}
	case eAniBossMoveLeft:
		{
			pTexture = TextureCache::getInstance()->addImage("boss.png");
			for (int i = 3; i >=0; --i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(32*i, 0, 32, 32));
				pArray.pushBack(frame);
			}

			pAnimation = Animation::createWithSpriteFrames(pArray, 0.2f);
			break;
		}
	case eAniBossMoveRight:
		{
			pTexture = TextureCache::getInstance()->addImage("boss.png");
			for (int i = 4; i < 8; ++i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(32*i, 0, 32, 32));
				pArray.pushBack(frame);
			}

			pAnimation = Animation::createWithSpriteFrames(pArray, 0.2f);
			break;
		}
	case eAniBossFireLeft:
		{
			pTexture = TextureCache::getInstance()->addImage("bossBullet.png");
			for (int i = 0; i < 2; ++i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(24*i, 0, 24, 8));
				pArray.pushBack(frame);
			}

			pAnimation = Animation::createWithSpriteFrames(pArray, 0.1f);
			break;
		}
	case eAniBossFireRight:
		{
			pTexture = TextureCache::getInstance()->addImage("bossBullet.png");
			for (int i = 3; i >= 2; --i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(24*i, 0, 24, 8));
				pArray.pushBack(frame);
			}

			pAnimation = Animation::createWithSpriteFrames(pArray, 0.1f);
			break;
		}
	case eAniBatteryBoom:
		{
			frame = SpriteFrame::create("batteryBoom1.png", CCRectMake(0, 0, 32, 27));
			pArray.pushBack(frame);
			frame = SpriteFrame::create("batteryBoom2.png", CCRectMake(0, 0, 32, 18));
			pArray.pushBack(frame);
			frame = SpriteFrame::create("batteryBoom3.png", CCRectMake(0, 0, 32, 20));
			pArray.pushBack(frame);

			pAnimation = Animation::createWithSpriteFrames(pArray, 0.1f);
			break;
		}
	case eAniLightning:
		{
			pTexture = TextureCache::getInstance()->addImage("lightning.png");

			frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(0, 0, 16, 32));
			pArray.pushBack(frame);
			frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(16, 0, 16, 32));
			pArray.pushBack(frame);

			pAnimation = Animation::createWithSpriteFrames(pArray, 0.1f);
			break;
		}
	case eAniArrowLeft:
		{
			pTexture = TextureCache::getInstance()->addImage("allow_walkLeft.png");
			for (int i = 9; i >= 0; --i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(18*i, 0, 18, 32));
				pArray.pushBack(frame);
			}
			pAnimation = Animation::createWithSpriteFrames(pArray, 0.02f);
			break;
		}
	case eAniArrowRight:
		{
			pTexture = TextureCache::getInstance()->addImage("allow_walkRight.png");
			for (int i = 0; i < 10; ++i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(18*i, 0, 18, 32));
				pArray.pushBack(frame);
			}
			pAnimation = Animation::createWithSpriteFrames(pArray, 0.02f);
			break;
		}
	case eAniArrowDie:
		{
			pTexture = TextureCache::getInstance()->addImage("arrow_die.png");
			for (int i = 0; i < 7; ++i)
			{
				frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(24*i, 0, 24, 34));
				pArray.pushBack(frame);
				pArray.pushBack(frame);
			}
			pAnimation = Animation::createWithSpriteFrames(pArray, 0.1f);
			break;
		}
	default:
		break;
	}
	
//	pArray->release();
	
	return pAnimation;
}

void AnimationManager::preLoadEffectAndMusic()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("OnLand.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("DingChuMoGuHuoHua.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("AddLife.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("EatMushroomOrFlower.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("SuoXiao.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("BossDiaoLuoQiaoXia.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("BossDie.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("BossHuoQiu.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("CaiSiGuaiWu.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("DiaoRuXianJingSi.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("DingPoZhuan.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("DingYingZhuanKuai.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("EatCoin.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("GameOver.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("HuoQiuDaDaoGuaiWu.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("Jump.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("QiZiLuoXia.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("YuDaoGuaiWuSi.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("RengHuoQiu.wav");
}

void AnimationManager::initAnimationMap()
{
	this->preLoadEffectAndMusic();

	this->setSelectLevel();

	this->setMusicSwitch();

	char temp[20] = { 0 };

	sprintf(temp, "%d", eAniRight);	
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniRight), temp);

	sprintf(temp, "%d", eAniLeft);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniLeft), temp);

	sprintf(temp, "%d", eAniLeftSmall);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniLeftSmall), temp);

	sprintf(temp, "%d", eAniRightSmall);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniRightSmall), temp);

	sprintf(temp, "%d", eAniBlinkCoin);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniBlinkCoin), temp);

	sprintf(temp, "%d", eAniMushroom);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniMushroom), temp);

	sprintf(temp, "%d", eAniflower);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniflower), temp);

	sprintf(temp, "%d", eAniTortoiseLeft);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniTortoiseLeft), temp);

	sprintf(temp, "%d", eAniTortoiseRight);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniTortoiseRight), temp);

	sprintf(temp, "%d", eAniTortoiseFly);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniTortoiseFly), temp);

	sprintf(temp, "%d", eAniRightFire);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniRightFire),  temp);

	sprintf(temp, "%d", eAniLeftFire);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniLeftFire), temp);

	sprintf(temp, "%d", eAniRotatedFireBall);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniRotatedFireBall), temp);

	sprintf(temp, "%d", eAniBoomedFireBall);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniBoomedFireBall), temp);

	sprintf(temp, "%d", eAniFireActionR);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniFireActionR), temp);

	sprintf(temp, "%d", eAniFireActionL);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniFireActionL), temp);

	sprintf(temp, "%d", eAniFlyFishR);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniFlyFishR), temp);

	sprintf(temp, "%d", eAniFlyFishL);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniFlyFishL), temp);

	sprintf(temp, "%d", eAniArrowBroken);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniArrowBroken), temp);

	sprintf(temp, "%d", eAniSmallDie);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniSmallDie), temp);

	sprintf(temp, "%d", eAniNormalDie);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniNormalDie), temp);

	sprintf(temp, "%d", eAniFireDie);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniFireDie), temp);

	sprintf(temp, "%d", eAniBossMoveLeft);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniBossMoveLeft), temp);

	sprintf(temp, "%d", eAniBossMoveRight);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniBossMoveRight), temp);

	sprintf(temp, "%d", eAniBossFireLeft);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniBossFireLeft), temp);

	sprintf(temp, "%d", eAniBossFireRight);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniBossFireRight), temp);

	sprintf(temp, "%d", eAniBatteryBoom);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniBatteryBoom), temp);

	sprintf(temp, "%d", eAniLightning);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniLightning), temp);

	sprintf(temp, "%d", eAniArrowLeft);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniArrowLeft), temp);

	sprintf(temp, "%d", eAniArrowRight);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniArrowRight), temp);

	sprintf(temp, "%d", eAniArrowDie);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniArrowDie), temp);

	sprintf(temp, "%d", eAniArrowActionL);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniArrowActionL), temp);

	sprintf(temp, "%d", eAniArrowActionR);
	AnimationCache::getInstance()->addAnimation(
		createAnimationByAnimationType(eAniArrowActionR), temp);
}

Animation* AnimationManager::getAnimation(AnimationType key)
{
	char temp[20] = { 0 };
	sprintf(temp, "%d", key);
	
	return AnimationCache::getInstance()->animationByName(temp);
}

Animate* AnimationManager::createAnimate(const char *key)
{
	Animation *anim = AnimationCache::getInstance()->animationByName(key);
	if (anim)
	{
		return Animate::create(anim);
	}
	return NULL;
}

Animate* AnimationManager::createAnimate(AnimationType key)
{
	Animation *anim = getAnimation(key);
	if (anim)
	{
		return Animate::create(anim);
	}
	return NULL;
}

AnimationManager* AnimationManager::getInstance()
{
	if (!_instance)
	{
		_instance = new AnimationManager();
	}
	return _instance;
}

void AnimationManager::setSelectLevel()
{
	CCString *str = NULL;
	for (int i = 2; i <= 8; ++i)
	{
		 str= CCString::createWithFormat("Level%d", i);
		UserDefault::sharedUserDefault()->setStringForKey(str->getCString(), "no");
	}
}

void AnimationManager::setMusicSwitch()
{
	UserDefault::sharedUserDefault()->setStringForKey("Music", "on");
	UserDefault::sharedUserDefault()->setStringForKey("SoundEffect", "on");
}
