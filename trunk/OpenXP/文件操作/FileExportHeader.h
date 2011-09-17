#ifndef __FILEEXPOERTHEADER__H__
#define __FILEEXPOERTHEADER__H__
#pragma once

//////////////////////////////////////////////////////////////////////////
//公共宏定义

//导出定义
#ifdef  FILE_CONTROL_CLASS
	#define FILE_CONTROL_API _declspec(dllexport)
#else
	#define FILE_CONTROL_API _declspec(dllimport)
#endif

//模块定义
#ifdef _DEBUG
#define FILE_CONTROL_DLL_NAME	TEXT("File_d.dll")			//组件 DLL 名字
#else
#define FILE_CONTROL_DLL_NAME	TEXT("File_r.dll")			//组件 DLL 名字
#endif
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//导出头文件
#define FILE_EXPORT_HEADER

#ifdef FILE_EXPORT_HEADER
	#include "include/HIni.h"
	#include "include/HZipFile.h"
	#include "include/HMDFile.h"
	#include "include/HXMLConfig.h" 
	#include "include/HZlibComp.h"
#endif
//////////////////////////////////////////////////////////////////////////

#endif