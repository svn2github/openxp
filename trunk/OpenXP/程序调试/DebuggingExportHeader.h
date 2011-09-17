#ifndef __DEBUGGINGEXPOERTHEADER__H__
#define __DEBUGGINGEXPOERTHEADER__H__
#pragma once

//////////////////////////////////////////////////////////////////////////
//�����궨��

//��������
#ifdef  DEBUGGING_CONTROL_CLASS
	#define DEBUGGING_CONTROL_API _declspec(dllexport)
#else
	#define DEBUGGING_CONTROL_API _declspec(dllimport)
#endif

//ģ�鶨��
#ifdef _DEBUG
	#define DEBUGGING_CONTROL_DLL_NAME	TEXT("Debugging_d.dll")			//��� DLL ����
#else
	#define DEBUGGING_CONTROL_DLL_NAME	TEXT("Debugging_r.dll")			//��� DLL ����
#endif
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//����ͷ�ļ�
#define DEBUGGING_EXPORT_HEADER

#ifdef DEBUGGING_EXPORT_HEADER
	#include "include/HMemoryCheck.h"
	#include "include/HDOSWindow.h"
	#include "include/HLog.h"
	#include "include/HTimeTrace.h"
#endif
//////////////////////////////////////////////////////////////////////////

#endif