#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#define MUSIC_KEY "music_key"
#define EFFECT_KEY "effect_key"

#include "Data_Enum.h"

// 存储游戏的进度与相应数据，在游戏过程中进行更新与加载
class Global
{
public:
	Global();
	~Global();

	int currentLevel;
	void setCurrentLevel(int level);
	void reSetLevel();
	int getCurrentLevel();
	void currentLevelPlusOne();

	int totalLevels;
	int getTotalLevels();
	
	BodyType currentMarioType;  // 当前主角的身型状态
	void setCurrentMarioType(BodyType _type);
	BodyType getCurrentMarioType();

	BulletType currentBulletType;  // 当前主角使用的子弹类型
	void setCurrentBulletType(BulletType _type);
	BulletType getCurrentBulletType();

	// 马里奥一共的生命条数
	int lifeNum;
	void lifeNumPlusOne();
	void lifeNumCutOne();
	void setCurrentLifeNum(int num);
	int getCurrentLifeNum();

	static Global  *_globalInstance;
	static Global* getGlobalInstance();
};
#endif
