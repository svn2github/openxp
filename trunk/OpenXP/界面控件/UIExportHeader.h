#ifndef __UIEXPOERTHEADER__H__
#define __UIEXPOERTHEADER__H__
#pragma once

//////////////////////////////////////////////////////////////////////////
//�����궨��

//��������
#ifdef  UI_CONTROL_CLASS
	#define UI_CONTROL_API _declspec(dllexport)
#else
	#define UI_CONTROL_API _declspec(dllimport)
#endif

//ģ�鶨��
#ifdef _DEBUG
	#define UI_CONTROL_DLL_NAME	TEXT("UI_d.dll")			//��� DLL ����
#else
	#define UI_CONTROL_DLL_NAME	TEXT("UI_r.dll")			//��� DLL ����
#endif
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//����ͷ�ļ�
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