#include "Data_Global.h"

Global* Global::_globalInstance;
Global* Global::getGlobalInstance()
{
	if (_globalInstance == 0)
	{
		_globalInstance = new Global();
	}
	return _globalInstance;
}

Global::Global()
{
	currentLevel = 1;

	totalLevels = 8;
	
	currentMarioType = eBody_Small;

	currentBulletType = eBullet_common;

	lifeNum = 3;
}

Global::~Global()
{

}

void Global::currentLevelPlusOne()
{
	++currentLevel;
}

void Global::reSetLevel()
{
	currentLevel = 1;
}

int Global::getCurrentLevel()
{
	return currentLevel;
}

int Global::getTotalLevels()
{
	return totalLevels;
}

void Global::setCurrentMarioType(BodyType _type)
{
	currentMarioType = _type;
}

BodyType Global::getCurrentMarioType()
{
	return currentMarioType;
}

void Global::setCurrentBulletType(BulletType _type)
{
	currentBulletType = _type;
}

BulletType Global::getCurrentBulletType()
{
	return currentBulletType;
}

void Global::lifeNumPlusOne()
{
	++lifeNum;
}

void Global::lifeNumCutOne()
{
	--lifeNum;
}

void Global::setCurrentLifeNum(int num)
{
	lifeNum = num;
}

int Global::getCurrentLifeNum()
{
	return lifeNum;
}

void Global::setCurrentLevel(int level)
{
	currentLevel = level;
}

