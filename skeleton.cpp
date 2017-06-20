skeletonNode = new CCSkeletonAnimation("spineboy.json", "spineboy.atlas");
skeletonNode->setAnimation("walk", true);

CCSize windowSize = CCDirector::sharedDirector()->getWinSize();
skeletonNode->setPosition(ccp(windowSize.width / 2, 20));
addChild(skeletonNode);
skeletonNode->release();
