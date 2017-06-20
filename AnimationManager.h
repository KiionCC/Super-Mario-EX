#ifndef _ANIMATION_MANAGER_H_
#define _ANIMATION_MANAGER_H_

#include "cocos2d.h"

#include "Data_Enum.h"
using namespace cocos2d;

class AnimationManager
{
public:
	void initAnimationMap();
	void preLoadEffectAndMusic(); // 加载音效和背景音乐

	Animation* getAnimation(AnimationType key);
	Animate* createAnimate(const char *key);
	Animate* createAnimate(AnimationType key);

	void setSelectLevel();
	void setMusicSwitch();

	static AnimationManager* getInstance();
	static AnimationManager *_instance;
private:
	Animation* createAnimationByAnimationType(AnimationType key);
};

#define sAnimationMgr AnimationManager::getInstance()
#endif
