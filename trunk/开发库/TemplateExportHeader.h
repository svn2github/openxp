#ifndef __TEMPLATEEXPOERTHEADER__H__
#define __TEMPLATEEXPOERTHEADER__H__
#pragma once

//////////////////////////////////////////////////////////////////////////
//公共宏定义

//导出定义
#ifdef  TEMPLATE_CONTROL_CLASS
	#define TEMPLATE_CONTROL_API _declspec(dllexport)
#else
	#define TEMPLATE_CONTROL_API _declspec(dllimport)
#endif

//模块定义
#ifdef _DEBUG
	#define TEMPLATE_CONTROL_DLL_NAME	TEXT("TemplateModule_d.dll")			//组件 DLL 名字
#else
	#define TEMPLATE_CONTROL_DLL_NAME	TEXT("TemplateModule_r.dll")			//组件 DLL 名字
#endif
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//导出头文件
#define TEMPLATE_EXPORT_HEADER

#ifdef TEMPLATE_EXPORT_HEADER
	#include "include/HImageModule.h"
	#include "include/HMemoryDC.h"
	#include "include/HEncrypt.h"
	#include "include/HGlobalFunc.h"
	#include "include/HKeyPixelFrame.h"
	#include "include/HAnimationGif.h"
	#include "include/HHtmlCtrl.h"
	#include "include/HPngPixelFrame.h"
	#include "include/HGetRunTime.h"
	#include "include/HAuthCode.h"
	#include "include/HFlashPlay.h"
#endif
//////////////////////////////////////////////////////////////////////////

#endif