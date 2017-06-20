skeletonNode = new CCSkeletonAnimation("spineboy.json", "spineboy.atlas");
skeletonNode->setAnimation("walk", true);

CCSize windowSize = CCDirector::sharedDirector()->getWinSize();
skeletonNode->setPosition(ccp(windowSize.width / 2, 20));
addChild(skeletonNode);
skeletonNode->release();

if (skeletonNode->states[0]->loop) {
    if (skeletonNode->states[0]->time > 2) skeletonNode->setAnimation("jump", false);
} else {
    if (skeletonNode->states[0]->time > 1) skeletonNode->setAnimation("walk", true);
}
