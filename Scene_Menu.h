#ifndef _MENU_H_
#define _MENU_H_

#include "cocos2d.h"
using namespace cocos2d;

class MenuLayer : public Layer
{
public:
	MenuLayer();
	~MenuLayer();

	CCMenuItem *pStart;
	CCMenuItem *pQuit;
	CCMenuItem *pSetting;

	Menu *pMenu;

	void menuCallBackForStartGame(Ref *pSender);
	void menuQuit(Ref *pSender);
	void menuSetting(Ref *pSender);

	bool init();


	virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
	virtual void onTouchMoved(Touch *pTouch, Event *pEvent);
	virtual void onTouchEnded(Touch *pTouch, Event *pEvent);
	virtual void onTouchCancelled(Touch *pTouch, Event *pEvent);


	CREATE_FUNC(MenuLayer);

private:

};
#endif