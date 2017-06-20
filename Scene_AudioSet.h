#ifndef _AUDIO_SET_H_
#define _AUDIO_SET_H_

#include "cocos2d.h"
using namespace cocos2d;

class AudioSet : public Layer
{
public:
	AudioSet();
	~AudioSet();

	bool init();
	
	CREATE_FUNC(AudioSet);

	void menuBackMainMenu(Ref *pSender);
void menuEffectToggleCallback(Ref *pSender);
	void menuMusicToggleCallback(Ref *pSender);

	virtual void onEnter();
	virtual void onExit();
private:
	Sprite *pMusic;
	Sprite *pSoundEffect;
	SpriteFrame *pMusicOn;
	SpriteFrame *pMusicOff;
	SpriteFrame *pEffectOn;
	SpriteFrame *pEffectOff;
};
#endif
