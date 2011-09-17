#ifndef __UIEXPOERTHEADER__H__
#define __UIEXPOERTHEADER__H__
#pragma once

//////////////////////////////////////////////////////////////////////////
//公共宏定义

//导出定义
#ifdef  UI_CONTROL_CLASS
	#define UI_CONTROL_API _declspec(dllexport)
#else
	#define UI_CONTROL_API _declspec(dllimport)
#endif

//模块定义
#ifdef _DEBUG
	#define UI_CONTROL_DLL_NAME	TEXT("UI_d.dll")			//组件 DLL 名字
#else
	#define UI_CONTROL_DLL_NAME	TEXT("UI_r.dll")			//组件 DLL 名字
#endif
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//导出头文件
#define UI_EXPORT_HEADER

#ifdef UI_EXPORT_HEADER
	#include "include/HButton.h"
	#include "include/HSuperlink.h"
	#include "include/HListCtrl.h"
	#include "include/HMenu.h"
	#include "include/HScrollbar.h"
	#include "include/HComboBox.h"
	#include "include/HEdit.h"
	#include "include/HSliderCtrl.h"
	#include "include/HTabCtrl.h"
	#include "include/HTreeCtrl.h"
#endif
//////////////////////////////////////////////////////////////////////////

#endif