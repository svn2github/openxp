#ifndef __DOWNLOADEXPOERTHEADER__H__
#define __DOWNLOADEXPOERTHEADER__H__
#pragma once

//��������
#ifdef HDOWNLOAD_CLASS
	#define HDOWNLOAD_API __declspec(dllexport)
#else
	#define HDOWNLOAD_API __declspec(dllimport)
#endif

//ģ�鶨��
#ifdef _DEBUG
	#define HDOWNLOAD_DLL_NAME	TEXT("DownLoad_d.dll")			
#else
	#define HDOWNLOAD_DLL_NAME	TEXT("DownLoad_r.dll")			
#endif

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//����ͷ�ļ�
#define DOWNLOAD_EXPORT_HEADER

#ifdef DOWNLOAD_EXPORT_HEADER
	#include "include/HDownLoad.h"
	#include "include/HUnBreakDownLoad.h"
#endif
//////////////////////////////////////////////////////////////////////////

#endif