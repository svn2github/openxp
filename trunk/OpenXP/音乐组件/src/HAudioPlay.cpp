#include "StdAfx.h"
#include "../include/HAudioPlay.h"

HAudioPlay::HAudioPlay()
:m_fVolume(1.0f)
{
	memset(&m_ptListenerPos,0,sizeof(m_ptListenerPos));
	memset(&m_ptPlay,0,sizeof(m_ptPlay));
	m_pSoundEngine = createIrrKlangDevice();
}

HAudioPlay::~HAudioPlay()
{
	if (m_pSoundEngine)
		m_pSoundEngine->drop();
}

void HAudioPlay::Play2D(LPCSTR szFilleName,bool bLoop)
{
	if (m_pSoundEngine)
	{
		m_pSoundEngine->play2D(szFilleName,bLoop);
		m_pSoundEngine->setSoundVolume(m_fVolume);
	}
}

void HAudioPlay::Play2D( void *pMemory,unsigned int nSize,bool bLoop )
{
	if (m_pSoundEngine)
	{
		ISoundSource *iss = NULL;
		iss = m_pSoundEngine->addSoundSourceFromMemory(pMemory,nSize,"m_s");
		if (iss != NULL)
		{
			m_pSoundEngine->play2D(iss,bLoop);
			m_pSoundEngine->setSoundVolume(m_fVolume);
		}
	}
}

void HAudioPlay::Play3D( LPCSTR szFilleName,POINT ptPlay,POINT ptListenerPos,bool bLoop )
{
	if (m_pSoundEngine)
	{
		m_pSoundEngine->play3D(szFilleName,vec3df((float)ptPlay.x,0,(float)ptPlay.y),bLoop);
		m_pSoundEngine->setListenerPosition(vec3df((float)ptListenerPos.x,0,(float)ptListenerPos.y),vec3df((float)ptListenerPos.x-1,0,(float)ptListenerPos.y));
		m_pSoundEngine->setSoundVolume(m_fVolume);
	}
}

void HAudioPlay::Stop()
{	
	if (m_pSoundEngine)
		m_pSoundEngine->stopAllSounds();
}

void HAudioPlay::SetSoundVolume( float fVolume )
{
	if (m_pSoundEngine)
		m_pSoundEngine->setSoundVolume(fVolume);
	m_fVolume = fVolume;
}

void HAudioPlay::SetListenerPos( POINT ptLP )
{
	m_ptListenerPos.x = ptLP.x;
	m_ptListenerPos.y = ptLP.y;
}