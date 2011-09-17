#ifndef __FILEEXPOERTHEADER__H__
#define __FILEEXPOERTHEADER__H__
#pragma once

//////////////////////////////////////////////////////////////////////////
//�����궨��

//��������
#ifdef  FILE_CONTROL_CLASS
	#define FILE_CONTROL_API _declspec(dllexport)
#else
	#define FILE_CONTROL_API _declspec(dllimport)
#endif

//ģ�鶨��
#ifdef _DEBUG
#define FILE_CONTROL_DLL_NAME	TEXT("File_d.dll")			//��� DLL ����
#else
#define FILE_CONTROL_DLL_NAME	TEXT("File_r.dll")			//��� DLL ����
#endif
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//����ͷ�ļ�
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