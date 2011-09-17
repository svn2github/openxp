#ifndef __SYSTEMEXPOERTHEADER__H__
#define __SYSTEMEXPOERTHEADER__H__
#pragma once

//��������
#ifdef HSYSTEM_CLASS
	#define HSYSTEM_API __declspec(dllexport)
#else
	#define HSYSTEM_API __declspec(dllimport)
#endif

//ģ�鶨��
#ifdef _DEBUG
#define HSYSTEM_DLL_NAME	TEXT("SystemModule_d.dll")			
#else
#define HSYSTEM_DLL_NAME	TEXT("SystemModule_r.dll")			
#endif

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//����ͷ�ļ�
#define SYSTEM_EXPORT_HEADER
#ifdef SYSTEM_EXPORT_HEADER
	#include "include/HAPIHook.h"
	#include "include/HSystemInfo.h"
	#include "include/HNetCardInfo.h"
	#include "include/HFileVersion.h"
#endif
//////////////////////////////////////////////////////////////////////////

#endif