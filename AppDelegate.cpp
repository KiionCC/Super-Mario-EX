#include "SimpleAudioEngine.h"

#include "AppDelegate.h"
#include "AnimationManager.h"
#include "Scene_Game.h"
#include "Data_Global.h"
#include "Scene_Menu.h"

USING_NS_CC;
using namespace CocosDenshion;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
	SimpleAudioEngine::end();
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("SuperMario");
        director->setOpenGLView(glview);
		glview->setDesignResolutionSize(480, 320, kResolutionShowAll);
    }

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

	AnimationManager::getInstance()->initAnimationMap();

    // create a scene. it's an autorelease object
    auto scene = Scene::create();
	scene->addChild(MenuLayer::create());
    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
