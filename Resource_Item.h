#ifndef _ITEM_H_
#define _ITEM_H_

#include "cocos2d.h"

#include "Data_Enum.h"
using namespace cocos2d;

class Item : public Node
{
public:
	Item(ItemType _itemtype);
	~Item();
	Sprite *itemBody;   // 主要是block层的item对应的精灵
	ItemType itemType;    // Item的类型

	static Item* create(ItemType _itemtype);

private:
	bool itemInit();
};

// 悬浮梯
class SuspensionLadder : public Node
{
public:
	SuspensionLadder();
	virtual ~SuspensionLadder();

	// 下面两个方向上的更新，可能因人而异
	virtual void ladderUpdateH();
	virtual void ladderUpdateV();

	// 纯虚函数
	virtual void launchLadder() = 0;

	float getMoveOffset();
	float getJumpOffset();

	void setStartPos(Point pos);
	Point getStartPos();

	void setStartFace(int face);

	// 获取悬浮梯的矩形，检测主角是否在上面
	Rect getLadderRect();

	Size getLadderSize();

	LadderState getLadderState();

	// 检测状态
	virtual void checkLadderState();

protected:
	Sprite *ladderBody;
	Size bodySize;
	LadderType ladderType;  //悬浮梯类型（左右或上下）
	LadderState ladderState;  // 悬浮梯状态

	float moveOffset;
	float ccMoveOffset;

	float jumpOffset;
	float ccJumpOffset;

	Point startPos;  // 初始位置

	// 第一次移动的方向，有0和1选项
	// 在LR中0代表左，1代表右
	// 在UD中0代表上，1代表下
	int startFace;

};

// 左右移动的梯子
class SL_LeftRight : public SuspensionLadder
{
public:
	SL_LeftRight(float dis);
	~SL_LeftRight();

	void ladderUpdateH();

	void launchLadder();

	void setLRdis(float dis);

	void update(float dt);

private:
	float leftSide;    // 左边界
	float rightSide;   // 右边界
	float lrDis;       // 左右距离
};

class SL_UpDown : public SuspensionLadder
{
public:
	SL_UpDown(float dis);
	~SL_UpDown();

	void ladderUpdateV();

	void launchLadder();

	void setUDdis(float dis);

	void update(float dt);

private:
	float upSide;      // 上边界
	float downSide;    // 下边界
	float udDis;       // 上下距离
};
#endif
