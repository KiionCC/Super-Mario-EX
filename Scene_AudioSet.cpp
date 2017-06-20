#include <string>

#include "SimpleAudioEngine.h"
#include"Date_Global.h"
#include "Scene_AudioSet.h"
#include "Scene_Menu.h"
using namespace std;


AudioSet::AudioSet()
{

}

AudioSet::~AudioSet()
{

}

bool AudioSet::init()
{
	Sprite *pBg = Sprite::create("bg_Audio.png");
	Point origin = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();

	pBg->setPosition(ccp(origin.x + visibleSize.width / 2 + 3, origin.y + visibleSize.height / 2));
	this->addChild(pBg, 0);


	MenuItemImage *pBack = MenuItemImage::create("backA.png", "backB.png",
		this, menu_selector(AudioSet::menuBackMainMenu));
	pBack->setPosition(ccp(origin.x + 20, origin.y + visibleSize.height - 20));
	Menu *pMenu = Menu::create(pBack, NULL);
	pMenu->setAnchorPoint(Vec2(0, 0));
	pMenu->setPosition(Vec2(0, 0));
	this->addChild(pMenu, 2);

	pMusicOn = SpriteFrame::create("music_on.png", CCRectMake(0, 0, 70, 63));
	pMusicOn->retain();
	pMusicOff = SpriteFrame::create("music_off.png", CCRectMake(0, 0, 70, 63));
	pMusicOff->retain();
	pEffectOn = SpriteFrame::create("sound_effect_on.png", CCRectMake(0, 0, 70, 63));
	pEffectOn->retain();
	pEffectOff = SpriteFrame::create("sound_effect_off.png", CCRectMake(0, 0, 70, 63));
	pEffectOff->retain();

	auto MusicOn = Sprite::createWithSpriteFrame(pMusicOn);
	auto MusicOff = Sprite::createWithSpriteFrame(pMusicOff);
	auto MusicOnMenuItem = MenuItemSprite::create(MusicOn, NULL);
	auto MusicOffMenuItem = MenuItemSprite::create(MusicOff, NULL);
	auto MusicToggleMenuItem = MenuItemToggle::createWithCallback(CC_CALLBACK_1(AudioSet::menuMusicToggleCallback, this),
		MusicOffMenuItem,MusicOnMenuItem,  NULL);
	MusicToggleMenuItem->setPosition(ccp(origin.x + visibleSize.width / 2 - 80, origin.y + visibleSize.height / 2 + 40));

	auto EffectOn = Sprite::createWithSpriteFrame(pEffectOn);
	auto EffectOff = Sprite::createWithSpriteFrame(pEffectOff);
	auto EffectOnMenuItem = MenuItemSprite::create(EffectOn, NULL);
	auto EffectOffMenuItem = MenuItemSprite::create(EffectOff, NULL);
	auto EffectToggleMenuItem = MenuItemToggle::createWithCallback(CC_CALLBACK_1(AudioSet::menuEffectToggleCallback, this),
		 EffectOffMenuItem,EffectOnMenuItem, NULL);
	EffectToggleMenuItem->setPosition(ccp(origin.x + visibleSize.width / 2 + 80, origin.y + visibleSize.height / 2 + 40));

	Menu* mn = Menu::create(MusicToggleMenuItem, EffectToggleMenuItem,NULL);
	mn->setPosition(Vec2::ZERO);
	this->addChild(mn);

	UserDefault *defaults = UserDefault::getInstance();
	if (defaults->getBoolForKey(MUSIC_KEY)) 
		MusicToggleMenuItem->setSelectedIndex(0);
	else 
		MusicToggleMenuItem->setSelectedIndex(1);

	if (defaults->getBoolForKey(EFFECT_KEY)) 
		EffectToggleMenuItem->setSelectedIndex(0);
	else 
		EffectToggleMenuItem->setSelectedIndex(1);
	return true;
}

void AudioSet::menuBackMainMenu(Ref *pSender)
{
	Scene *pScene = Scene::create();
	pScene->addChild(MenuLayer::create());
	Director::getInstance()->replaceScene(pScene);
}

void AudioSet::menuEffectToggleCallback(Ref *pSender)
{
	auto soundToggleMenuItem = (MenuItemToggle*)pSender;
	UserDefault *defaults = UserDefault::getInstance();
	if (defaults->getBoolForKey(EFFECT_KEY)) 
		defaults->setBoolForKey(EFFECT_KEY, false);
	else 
		defaults->setBoolForKey(EFFECT_KEY, true);
}
void AudioSet::menuMusicToggleCallback(Ref *pSender)
{
	
	UserDefault *defaults = UserDefault::getInstance();
	
	if (defaults->getBoolForKey(MUSIC_KEY)) {
		defaults->setBoolForKey(MUSIC_KEY, false);
		CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	}
	else {
		defaults->setBoolForKey(MUSIC_KEY, true);
		
	}
}
void AudioSet::onEnter()
{
	Layer::onEnter();
}

void AudioSet::onExit()
{
	Layer::onExit();
}
