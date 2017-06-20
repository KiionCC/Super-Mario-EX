#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#define MUSIC_KEY "music_key"
#define EFFECT_KEY "effect_key"

#include "Data_Enum.h"

// �洢��Ϸ�Ľ�������Ӧ���ݣ�����Ϸ�����н��и��������
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
	
	BodyType currentMarioType;  // ��ǰ���ǵ�����״̬
	void setCurrentMarioType(BodyType _type);
	BodyType getCurrentMarioType();

	BulletType currentBulletType;  // ��ǰ����ʹ�õ��ӵ�����
	void setCurrentBulletType(BulletType _type);
	BulletType getCurrentBulletType();

	// �����һ������������
	int lifeNum;
	void lifeNumPlusOne();
	void lifeNumCutOne();
	void setCurrentLifeNum(int num);
	int getCurrentLifeNum();

	static Global  *_globalInstance;
	static Global* getGlobalInstance();
};
#endif
