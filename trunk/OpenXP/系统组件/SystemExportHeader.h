#ifndef __SYSTEMEXPOERTHEADER__H__
#define __SYSTEMEXPOERTHEADER__H__
#pragma once

//导出定义
#ifdef HSYSTEM_CLASS
	#define HSYSTEM_API __declspec(dllexport)
#else
	#define HSYSTEM_API __declspec(dllimport)
#endif

//模块定义
#ifdef _DEBUG
#define HSYSTEM_DLL_NAME	TEXT("SystemModule_d.dll")			
#else
#define HSYSTEM_DLL_NAME	TEXT("SystemModule_r.dll")			
#endif

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//导出头文件
#define SYSTEM_EXPORT_HEADER
#ifdef SYSTEM_EXPORT_HEADER
	#include "include/HAPIHook.h"
	#include "include/HSystemInfo.h"
	#include "include/HNetCardInfo.h"
	#include "include/HFileVersion.h"
#endif
//////////////////////////////////////////////////////////////////////////

#endif