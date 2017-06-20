#include "Resource_Item.h"
#include "Scene_Game.h"

//********************* Item **********************//
Item::Item(ItemType _itemtype)
{
	itemType = _itemtype;
}

Item::~Item()
{
	this->unscheduleAllSelectors();
}

bool Item::itemInit()
{
	switch (itemType)
	{
	case eBlinkCoin:
		itemBody = Sprite::create("coinani.png", CCRectMake(0, 0, 8, 14));
		break;
	default:
		break;
	}
	itemBody->setAnchorPoint(ccp(0, 0));
	this->addChild(itemBody);
	itemBody->retain();
	this->setContentSize(CCSizeMake(8, 14));
	this->setAnchorPoint(ccp(0.5, 0));
	return true;
}

Item* Item::create(ItemType _itemtype)
{
	Item *pItem = new Item(_itemtype);
	if (pItem && pItem->itemInit())
	{
		pItem->autorelease();
		return pItem;
	}
	CC_SAFE_DELETE(pItem);
	return NULL;
}


//********************* SuspensionLadder **********************//
SuspensionLadder::SuspensionLadder():
ladderBody(NULL)
{
	ladderState = eLadderState_nonactive;

	moveOffset = 0.0f;
	ccMoveOffset = 0.0f;
	jumpOffset = 0.0f;
	ccJumpOffset = 0.0f;

	startPos = Vec2(0, 0);
}

SuspensionLadder::~SuspensionLadder()
{

}

void SuspensionLadder::ladderUpdateH()
{

}

void SuspensionLadder::ladderUpdateV()
{

}

float SuspensionLadder::getMoveOffset()
{
	return moveOffset;
}

float SuspensionLadder::getJumpOffset()
{
	return jumpOffset;
}

void SuspensionLadder::setStartPos(Point pos)
{
	startPos = pos;
}

Point SuspensionLadder::getStartPos()
{
	return startPos;
}

void SuspensionLadder::setStartFace(int face)
{
	startFace = face;
}

Rect SuspensionLadder::getLadderRect()
{
	Point pos = this->getPosition();
	return CCRectMake(pos.x - bodySize.width/2, pos.y,
		bodySize.width, bodySize.height);
}

Size SuspensionLadder::getLadderSize()
{
	return bodySize;
}

LadderState SuspensionLadder::getLadderState()
{
	return ladderState;
}

void SuspensionLadder::checkLadderState()
{
	Size winSize = Director::getInstance()->getWinSize();
	float tempMaxH = GameLayer::getMapMaxH();
	Point pos = this->getPosition();

	if ( (pos.x + bodySize.width/2 - tempMaxH >= 0) &&
		(pos.x - bodySize.width/2 - tempMaxH) <= winSize.width )
	{
		ladderState = eLadderState_active;
	}
	else
	{
		if (pos.x + bodySize.width/2 - tempMaxH < 0)
		{
			ladderState = eLadderState_over;
			ladderBody->stopAllActions();
			this->stopAllActions();
			this->unscheduleUpdate();
			this->setVisible(false);
		}
		else
		{
			ladderState = eLadderState_nonactive;
		}
	}
}




//********************* SL_LeftRight **********************//
SL_LeftRight::SL_LeftRight(float dis)
{
	ladderType = eSL_LeftRight;
	ladderBody = Sprite::create("ladder.png");
	bodySize = CCSizeMake(48.0f, 8.0f);
	ladderBody->setAnchorPoint(ccp(0, 0));
	this->setContentSize(bodySize);
	this->addChild(ladderBody);
	this->setAnchorPoint(ccp(0.5f, 0.0f));

	ccMoveOffset = 0.6f;
	leftSide = 0.0f;
	rightSide = 0.0f;
	lrDis = dis;
}

SL_LeftRight::~SL_LeftRight()
{
	this->unscheduleAllSelectors();
}

void SL_LeftRight::ladderUpdateH()
{
	Point pos = this->getPosition();
	if (pos.x <= leftSide || pos.x >= rightSide)
	{
		moveOffset *= -1;
	}
}


void SL_LeftRight::launchLadder()
{
	leftSide = startPos.x - lrDis;
	rightSide = startPos.x + lrDis;
	switch (startFace)
	{
	case 0:
		moveOffset = -ccMoveOffset;
		break;
	case 1:
		moveOffset = ccMoveOffset;
		break;
	default:
		break;
	}

	this->scheduleUpdate();
	
}

void SL_LeftRight::update(float dt)
{
	this->checkLadderState();

	if (ladderState == eLadderState_active)
	{
		Point currentPos = this->getPosition();
		currentPos.x += moveOffset;
		this->setPosition(currentPos);

		this->ladderUpdateH();
	}	
}

void SL_LeftRight::setLRdis(float dis)
{
	lrDis = dis;
}


//********************* SL_UpDown **********************//
SL_UpDown::SL_UpDown(float dis)
{
	ladderType = eSL_UpDown;
	ladderBody = Sprite::create("ladder.png");
	bodySize = CCSizeMake(48.0f, 8.0f);
	ladderBody->setAnchorPoint(ccp(0, 0));
	this->setContentSize(bodySize);
	this->addChild(ladderBody);
	this->setAnchorPoint(ccp(0.5f, 0.0f));

	ccJumpOffset = 0.5f;

	upSide = 0.0f;
	downSide = 0.0f;
	udDis = dis;
}

SL_UpDown::~SL_UpDown()
{
	this->unscheduleAllSelectors();
}

void SL_UpDown::ladderUpdateV()
{
	Point pos = this->getPosition();
	if (pos.y <= downSide || pos.y >= upSide)
	{
		jumpOffset *= -1;
	}
}

void SL_UpDown::launchLadder()
{
	upSide = startPos.y + udDis;
	downSide = startPos.y - udDis;
	switch (startFace)
	{
	case 0:
		jumpOffset = ccJumpOffset;
		break;
	case 1:
		jumpOffset = -ccJumpOffset;
		break;
	default:
		break;
	}

	this->scheduleUpdate();
}

void SL_UpDown::update(float dt)
{
	this->checkLadderState();

	if (ladderState == eLadderState_active)
	{
		Point currentPos = this->getPosition();
		currentPos.y += jumpOffset;
		this->setPosition(currentPos);

		this->ladderUpdateV();
	}

}

void SL_UpDown::setUDdis(float dis)
{
	udDis = dis;
}





