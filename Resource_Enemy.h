#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "cocos2d.h"

#include "Data_Enum.h"
using namespace cocos2d;

class Enemy : public Node
{
public:
	Enemy();
	virtual ~Enemy();

	void setTileCoord(Point _tileCoord);
	Point getTileCoord();
	void setEnemyPos(Point _enemyPos);
	Point getEnemyPos();
	void setBodySize(Size _size);
	Size getBodySize();

	void setMoveOffset(float _moveoffset);
	float getMoveOffset();

	void setccMoveOffset(float _ccmoveoffset);
	float getccMoveOffset();

	EnemyType getEnemyType();

	virtual Rect getEnemyRect();

	// 显示怪物
	virtual void launchEnemy() = 0;
	// 怪物水平方向碰撞检测（Horizontal）
	virtual void enemyCollistionH();
	// 怪物竖直方向碰撞检测（Vertical）
	virtual void enemyCollistionV();

	// 和怪物状态控制相关的
	void setEnemyState(EnemyState _state);
	EnemyState getEnemyState();
	void checkState();
	virtual void stopEnemyUpdate();

	// 判断当前怪物是否与主角相碰
	virtual EnemyVSMario checkCollisionWithMario();

	// 怪物被踩死时调用的函数
	virtual void KilledByMario();
	void setNonVisibleForKilledByMario();
	// 怪物被子弹打死调用的函数
	virtual void KilledByBullet();
	void setNonVisibleForKilledByBullet();

protected:
	EnemyType enemyType;  // 怪物类型
	Sprite *enemyBody;  // 怪物的精灵
	Size bodySize;      // 怪物尺寸
	SpriteFrame *enemyLifeOver;  // 怪物死时的精灵帧
	SpriteFrame *overByArrow;    // 怪物被箭射死的精灵帧

	Point tileCoord;   // 怪物出生点的地图坐标系下的
	Point enemyPos;    // GL坐标系下的，可以直接设置怪物的位置

	marioDirection startFace;  // 初始怪物朝向

							   // 水平方向移动控制变量
	float moveOffset;
	float ccMoveOffset;
	// 竖直方向上移动控制变量
	float jumpOffset;
	float ccJumpOffset;

	// 标识怪物的当前状态
	// 屏幕中的怪物属于活跃状态，屏幕之外的为非活跃状态，被杀后属于死亡状态
	EnemyState enemyState;

};


// 不同类型怪物

// 蘑菇
class E_Mushroom : public Enemy
{
public:
	E_Mushroom();
	~E_Mushroom();

	void launchEnemy();

	void update(float dt);
};

// 食人花
class E_Flower : public Enemy
{
public:
	E_Flower();
	~E_Flower();

	void launchEnemy();

	void update(float dt);
	// 重写食人花与角色的碰撞检测函数
	EnemyVSMario checkCollisionWithMario();

	// 重写获取矩形函数
	Rect getEnemyRect();

protected:
	Point startPos;  // 标识食人花刚出生时的位置
};

// 普通乌龟
class E_Tortoise : public Enemy
{
public:
	E_Tortoise(int _startface);
	~E_Tortoise();

	void launchEnemy();
	// 重写水平方向碰撞检测，因为乌龟有在水平方向上的调头
	void enemyCollistionH();
	void update(float dt);

private:
	SpriteFrame *leftFace;
	SpriteFrame *rightFace;
};

// 往返乌龟
class E_Tortoise_Return : public Enemy
{
public:
	E_Tortoise_Return(float dis);
	~E_Tortoise_Return();

	void launchEnemy();

	void update(float dt);

private:
	float roundDis;  // 水平方向上往复的距离

	void reRight();
	void reLeft();
};

// 飞行乌龟
class E_Tortoise_Fly : public Enemy
{
public:
	E_Tortoise_Fly(float dis);
	~E_Tortoise_Fly();

	void launchEnemy();

	void update(float dt);

	void setFlyDis(float dis);
	float getFlyDis();

private:
	float flyDis;
};

// 旋转火链
class E_FireString : public Enemy
{
public:
	E_FireString(float _begAngle, float _time);
	~E_FireString();

	// 
	void launchEnemy();

	// 重写父类的停止怪物更新
	void stopEnemyUpdate();

	// 重写与主角的碰撞检测有很大不同
	EnemyVSMario checkCollisionWithMario();

	// 清空被杀函数，设为无敌
	void KilledByBullet();
	void KilledByMario();

	void update(float dt);

private:
	Sprite *enemyBody2;  // 另外两个火球精灵
	Sprite *enemyBody3;

	Size fireSize;  // 单个火球的尺寸，在于主角的碰撞检测中这个是起主要检测作用的
	Array *pArrayFire;  // 三个火球的数组

	float begAngle;  // 火链初始角度
	float time;      // 火链转动周期

	double angle;  // 火链角度
	double PI;  // 圆周率π

	void launchFireString();
};

// 飞鱼
class E_FlyingFish : public Enemy
{
public:
	E_FlyingFish(float _offsetH, float _offsetV, float _duration);
	~E_FlyingFish();

	void launchEnemy();
	void update(float dt);
	void checkState();

	// 重写碰撞
	EnemyVSMario checkCollisionWithMario();

	// 清空被主角踩死的函数
	void KilledByMario();

private:
	float offsetH;  // 飞鱼水平方向上偏移
	float offsetV;  // 竖直方向上偏移
	float offsetDuration;  // 偏移持续时间
	bool isFlying;    // 是否是飞行状态，防止一个地方无休止的发射

	void flyInSky();
	void reSetNotInSky();
};

// Boss
class E_Boss : public Enemy
{
public:
	E_Boss();
	~E_Boss();

	void launchEnemy();

	EnemyVSMario checkCollisionWithMario();

	void KilledByBullet();
	void KilledByMario();

	void enemyCollistionH();

	void update(float dt);

private:

	float leftSide;
	float rightSide;

	void moveLeft();
	void moveRight();
};

// Boss发出的子弹
class E_BossFire : public Enemy
{
public:
	E_BossFire();
	~E_BossFire();

	void launchEnemy();

	EnemyVSMario checkCollisionWithMario();

	void KilledByBullet();
	void KilledByMario();

	void update(float dt);
private:

};


// 隐藏的蘑菇怪
class E_Mushroom_Hidden : public Enemy
{
public:
	E_Mushroom_Hidden(int _addnum);
	~E_Mushroom_Hidden();

	void launchEnemy();

	EnemyVSMario checkCollisionWithMario();

	void KilledByMario();

	void update(float dt);
private:
	int addNums;  // 一共要增加蘑菇的个数
	bool isAddable;  // 怪物出现速度

	void addMushroom();
	void reSetNonAddable();  // 增加完蘑菇后重新设置为可以增加
};


// 炮台
class E_Battery : public Enemy
{
public:
	E_Battery(float delay);
	~E_Battery();

	void launchEnemy();

	EnemyVSMario checkCollisionWithMario();

	void KilledByBullet();  // 炮台只能被火球打爆
	void KilledByMario();  // 炮台不能被主角踩死

	void update(float dt);

private:

	bool isFireable;  // 用于控制发射子弹的频度
	float fireDelay;  // 两发子弹之间的时间间隔

	void addBatteryBullet();
	void reSetNonFireable();  // 重新设置为不可发射子弹

	void stopAndClear();

	Point firePos;   // 发射出来的子弹的初始位置
};


// 炮台子弹
class E_BatteryBullet : public Enemy
{
public:
	E_BatteryBullet();
	~E_BatteryBullet();

	void launchEnemy();

	EnemyVSMario checkCollisionWithMario();

	void KilledByBullet();
	void KilledByMario();

	void update(float dt);
private:

};


// 乌云怪物，可以在主角靠近乌云时落雷
class E_Cloud : public Enemy
{
public:
	E_Cloud(float _delay, int _type);
	~E_Cloud();

	void launchEnemy();

	void update(float dt);

	void KilledByBullet();
	void KilledByMario();

private:
	float dropRegion;  // 落雷的区间
	float leftSide;    // 掉落的左边界
	float rightSide;   // 右边界

	bool isDropable;   // 控制是否可以落雷

	float delay;       // 两个闪电之间的时间间隔

	void addLightning();
	void reSetDropable();

	int type;   // 乌云的大小 0 1 2分别代表小中大

	SpriteFrame *normal; // 正常的云
	SpriteFrame *dark;   // 落雷时的云
	void reSetNormal();
};


// 闪电
class E_Lightning : public Enemy
{
public:
	E_Lightning();
	~E_Lightning();

	EnemyVSMario checkCollisionWithMario();

	void launchEnemy();

	void checkState();

	void update(float dt);

	void KilledByMario();
	void KilledByBullet();

private:

};
#endif
