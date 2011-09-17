#ifndef __DEBUGGINGEXPOERTHEADER__H__
#define __DEBUGGINGEXPOERTHEADER__H__
#pragma once

//////////////////////////////////////////////////////////////////////////
//公共宏定义

//导出定义
#ifdef  DEBUGGING_CONTROL_CLASS
	#define DEBUGGING_CONTROL_API _declspec(dllexport)
#else
	#define DEBUGGING_CONTROL_API _declspec(dllimport)
#endif

//模块定义
#ifdef _DEBUG
	#define DEBUGGING_CONTROL_DLL_NAME	TEXT("Debugging_d.dll")			//组件 DLL 名字
#else
	#define DEBUGGING_CONTROL_DLL_NAME	TEXT("Debugging_r.dll")			//组件 DLL 名字
#endif
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//导出头文件
#define DEBUGGING_EXPORT_HEADER

#ifdef DEBUGGING_EXPORT_HEADER
	#include "include/HMemoryCheck.h"
	#include "include/HDOSWindow.h"
	#include "include/HLog.h"
	#include "include/HTimeTrace.h"
#endif
//////////////////////////////////////////////////////////////////////////

#endif