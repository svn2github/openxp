#ifndef __SOUNDEXPOERTHEADER__H__
#define __SOUNDEXPOERTHEADER__H__
#pragma once

//��������
#ifdef HSOUND_CLASS
#define HSOUND_API __declspec(dllexport)
#else
#define HSOUND_API __declspec(dllimport)
#endif

//ģ�鶨��
#ifdef _DEBUG
#define HSOUND_DLL_NAME	TEXT("SoundModule_d.dll")			
#else
#define HSOUND_DLL_NAME	TEXT("SoundModule_r.dll")			
#endif

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//����ͷ�ļ�
#define SOUND_EXPORT_HEADER
#ifdef SOUND_EXPORT_HEADER
	#include "include/HAudioPlay.h"
#endif
//////////////////////////////////////////////////////////////////////////

#endif