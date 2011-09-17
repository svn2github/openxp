#ifndef __DOWNLOADEXPOERTHEADER__H__
#define __DOWNLOADEXPOERTHEADER__H__
#pragma once

//导出定义
#ifdef HDOWNLOAD_CLASS
	#define HDOWNLOAD_API __declspec(dllexport)
#else
	#define HDOWNLOAD_API __declspec(dllimport)
#endif

//模块定义
#ifdef _DEBUG
	#define HDOWNLOAD_DLL_NAME	TEXT("DownLoad_d.dll")			
#else
	#define HDOWNLOAD_DLL_NAME	TEXT("DownLoad_r.dll")			
#endif

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//导出头文件
#define DOWNLOAD_EXPORT_HEADER

#ifdef DOWNLOAD_EXPORT_HEADER
	#include "include/HDownLoad.h"
	#include "include/HUnBreakDownLoad.h"
#endif
//////////////////////////////////////////////////////////////////////////

#endif