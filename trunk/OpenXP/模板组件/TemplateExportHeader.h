#ifndef __TEMPLATEEXPOERTHEADER__H__
#define __TEMPLATEEXPOERTHEADER__H__
#pragma once

//////////////////////////////////////////////////////////////////////////
//�����궨��

//��������
#ifdef  TEMPLATE_CONTROL_CLASS
	#define TEMPLATE_CONTROL_API _declspec(dllexport)
#else
	#define TEMPLATE_CONTROL_API _declspec(dllimport)
#endif

//ģ�鶨��
#ifdef _DEBUG
	#define TEMPLATE_CONTROL_DLL_NAME	TEXT("TemplateModule_d.dll")			//��� DLL ����
#else
	#define TEMPLATE_CONTROL_DLL_NAME	TEXT("TemplateModule_r.dll")			//��� DLL ����
#endif
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//����ͷ�ļ�
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