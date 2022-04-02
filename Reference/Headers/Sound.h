#pragma once

#include "Engine_Defines.h"
#include "Base.h"
#define SOUND_MAX 1.0f
#define SOUND_MIN 0.0f
#define SOUND_DEFAULT 0.5f
#define SOUND_WEIGHT 0.1f

BEGIN(Engine)
	enum CHANNELID { BGM, 
	PLAYER00, PLAYER01, PLAYER02, PLAYER03, PLAYER04, PLAYER05, PLAYER06, PLAYER07, PLAYER08, PLAYER09, PLAYER10, PLAYER11, PLAYER12, PLAYER13,
	ENEMY00, ENEMY01, ENEMY02, ENEMY03, ENEMY04, ENEMY05, ENEMY06, ENEMY07, ENEMY08, ENEMY09, 
	ENEMY10, ENEMY11, ENEMY12, ENEMY13, ENEMY14, ENEMY15, ENEMY16, ENEMY17, ENEMY18, ENEMY19,
	ENEMY20, ENEMY21, ENEMY22, ENEMY23, ENEMY24, ENEMY25, ENEMY26, ENEMY27, ENEMY28, ENEMY29,
	EFFECT00, EFFECT01, EFFECT02, EFFECT03, EFFECT04, EFFECT05, EFFECT06, EFFECT07, EFFECT08, EFFECT09, EFFECT10, EFFECT11, EFFECT12, EFFECT013,
	UI00, UI01, UI02, UI03, UI04, UI05, UI06, UI07, UI08, UI09, UI10,
	DIALOGUE, ITEM, SLOWMO,MAPOBJ, MAXCHANNEL };

class CSound : public CBase
{
	DECLARE_SINGLETON(CSound)



private:
	CSound();
	virtual ~CSound() = default;

public:
	virtual void Free() override;
	void Initialize();

	void LoadSoundFile();
	void PlaySound(string pSoundKey, CHANNELID eID);
	void PlayBGM(string pSoundKey);
	void StopSound(CHANNELID eID);
	void StopAll();
	void SetVolume(float volume, CHANNELID eID);

private:
	// 사운드 리소스 정보를 갖는 객체 
	map<string, FMOD_SOUND*> m_mapSound;
	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL];
	FMOD_SYSTEM* m_pSystem;

	float m_volume;
	FMOD_BOOL m_bool;
};

END
