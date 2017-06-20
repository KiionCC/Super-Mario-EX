#ifndef _ROLE_H_
#define _ROLE_H_

#include "cocos2d.h"

#include "Data_Enum.h"
using namespace cocos2d;

class Mario : public Node
{
public:
	Sprite *mainBody;  // 主要的马里奥精灵
	Sprite *mainTemp;  // 马里奥的替身，主要用在变大变小的动画期间
	Size norBodySize;  // 正常情况下的身体大小(大号身材)
	Size smallSize;    // 小号身材
	Size currentSize;  // 当前的身材
	BodyType bodyType;   // 当前的身体状态

	void reSetForSuccess();  // 在小旗子下落的同时，马里奥要执行的换状态然后跳跃到在最后的点

	SpriteFrame *_normalRight;    // 正常状态下马里奥面向右
	SpriteFrame *_normalLeft;     // 正常情况下马里奥面向左
	SpriteFrame *_jumpLeft;  // 左跳在空中停留状态下，主角对应的纹理
	SpriteFrame *_jumpRight; // 右跳同理

	SpriteFrame *_smallRight;  // 对应着小号状态下的
	SpriteFrame *_smallLeft;
	SpriteFrame *_smallJumpLeft;
	SpriteFrame *_smallJumpRight;

	SpriteFrame *_normalRightFire;  // 正常中的子弹状态向右
	SpriteFrame *_normalLeftFire;   // 正常中子弹向左
	SpriteFrame *_jumpRightFire;    // 跳跃中子弹向右
	SpriteFrame *_jumpLeftFire;     // 跳跃中子弹向右

	SpriteFrame *_normalRightArrow;  // 发射弓箭的皮肤
	SpriteFrame *_normalLeftArrow;
	SpriteFrame *_jumpRightArrow;
	SpriteFrame *_jumpLeftArrow;

	SpriteFrame *_lifeOverSmall;  // 小型马里奥死亡状态
	SpriteFrame *_lifeOverNormal;
	SpriteFrame *_lifeOverFire;

	bool isDied;   // 是否已经死亡
	void setMarioDie(bool _die);
	bool isMarioDied();

	//void dieForEnemy();  // 马里奥碰到怪物后的执行的函数，这个只是换个死状的精灵帧
	void dieForTrap();   // 马里奥掉入陷阱里要执行一段上移和下移的动画后消失掉
	void reSetNonVisible();

	marioDirection state;  // 标示当前主角的状态
	marioDirection statePre;  // 标识主角的上一个状态
	marioDirection face;   // 马里奥的朝向
	bool isFlying;   // 马里奥是否在空中飞行状态

	bool bulletable;  // 表明是否可以发射子弹，这算是正常体型中的一个特例
	bool isBulletable();

	BulletType currentBulletType;  // 当前使用的子弹类型
	void setBulletType(BulletType _type);
	BulletType getBulletType();

	// 是否站在梯子等小物件上
	bool ladderable;
	void setLadderable(bool _ladderable);
	bool isLadderable();

	void fireAction();
	void reSetStateForFired();

	// 用来表示奖励一个人的UP字样
	CCLabelTTF *pLabelUp;

	static Mario* create();

	void setMarioState(marioDirection state);
	marioDirection getMarioState();

	Mario();
	~Mario();

	void onEnter();
	void onExit();

	void changeForGotAddLifeMushroom();
	void changeForGotMushroom();
	void changeForGotEnemy();

	void setBodyType(BodyType _bodytype);
	Size getCurrentSize();
	BodyType getCurrentBodyType();

	void setSafeTime(bool _issafe);
	bool getIsSafeTime();

	static Mario *_marioInstance;
	static Mario* getMarioInstance();
private:
	bool marioInit();
	void clearLabelUp();
	void setMarioTypeForSmall();
	void setMarioTypeForNormal();

	bool isSafeTime;  // 标识主角是否为安全时间内，受伤变小后5秒或是3秒内不受怪物碰撞检测
	void reSetSafeTime();
};
#endif
