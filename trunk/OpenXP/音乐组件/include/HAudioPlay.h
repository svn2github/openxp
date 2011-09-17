#ifndef __HAUDIOPLAY__H__
#define __HAUDIOPLAY__H__
#pragma once
#ifndef HSOUND_CLASS
	#include "IrrKlang/irrKlang.h"
#endif
using namespace irrklang;

class HSOUND_API HAudioPlay
{
public:
	HAudioPlay();
	~HAudioPlay();
	void Play2D(LPCSTR szFilleName,bool bLoop=true);
	void Play2D(void *pMemory,unsigned int nSize,bool bLoop=true);
	void Play3D(LPCSTR szFilleName,POINT ptPlay,POINT ptListenerPos,bool bLoop=true);
	void SetListenerPos(POINT ptLP);
	void Stop();
	void SetSoundVolume(float fVolume);
	float GetSoundVolume(){return m_fVolume;}
protected:
	ISoundEngine	*m_pSoundEngine;
	float			m_fVolume;
	POINT			m_ptPlay;
	POINT			m_ptListenerPos;
};

#endif