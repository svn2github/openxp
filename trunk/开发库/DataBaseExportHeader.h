#ifndef __DATABASEEXPOERTHEADER__H__
#define __DATABASEEXPOERTHEADER__H__
#pragma once

//////////////////////////////////////////////////////////////////////////
//公共宏定义

//导出定义
#ifdef  DATABASE_CONTROL_CLASS
	#define DATABASE_CONTROL_API _declspec(dllexport)
#else
	#define DATABASE_CONTROL_API _declspec(dllimport)
#endif

//模块定义
#ifdef _DEBUG
	#define DATABASE_CONTROL_DLL_NAME	TEXT("DataBaseModule_d.dll")			//组件 DLL 名字
#else
	#define DATABASE_CONTROL_DLL_NAME	TEXT("DataBaseModule_r.dll")			//组件 DLL 名字
#endif
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//导出头文件
#define DATABASE_EXPORT_HEADER
#ifdef DATABASE_EXPORT_HEADER
	#include "include/HAdoConnect.h"
#endif
//////////////////////////////////////////////////////////////////////////

#endif