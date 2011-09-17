#ifndef __SOUNDEXPOERTHEADER__H__
#define __SOUNDEXPOERTHEADER__H__
#pragma once

//导出定义
#ifdef HSOUND_CLASS
#define HSOUND_API __declspec(dllexport)
#else
#define HSOUND_API __declspec(dllimport)
#endif

//模块定义
#ifdef _DEBUG
#define HSOUND_DLL_NAME	TEXT("SoundModule_d.dll")			
#else
#define HSOUND_DLL_NAME	TEXT("SoundModule_r.dll")			
#endif

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//导出头文件
#define SOUND_EXPORT_HEADER
#ifdef SOUND_EXPORT_HEADER
	#include "include/HAudioPlay.h"
#endif
//////////////////////////////////////////////////////////////////////////

#endif