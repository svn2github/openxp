#ifndef __DATABASEEXPOERTHEADER__H__
#define __DATABASEEXPOERTHEADER__H__
#pragma once

//////////////////////////////////////////////////////////////////////////
//�����궨��

//��������
#ifdef  DATABASE_CONTROL_CLASS
	#define DATABASE_CONTROL_API _declspec(dllexport)
#else
	#define DATABASE_CONTROL_API _declspec(dllimport)
#endif

//ģ�鶨��
#ifdef _DEBUG
	#define DATABASE_CONTROL_DLL_NAME	TEXT("DataBaseModule_d.dll")			//��� DLL ����
#else
	#define DATABASE_CONTROL_DLL_NAME	TEXT("DataBaseModule_r.dll")			//��� DLL ����
#endif
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//����ͷ�ļ�
#define DATABASE_EXPORT_HEADER
#ifdef DATABASE_EXPORT_HEADER
	#include "include/HAdoConnect.h"
#endif
//////////////////////////////////////////////////////////////////////////

#endif