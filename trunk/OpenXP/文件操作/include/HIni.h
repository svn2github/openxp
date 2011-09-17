/********************************************************************
created:	2009/8/25
created:	08:25:2009   9:10
filename: 	HIni.h
author:		XinPing Huang
email:		77967292@qq.com
Qualifier:  ini�����ļ���ȡ
*********************************************************************/
#ifndef __HINI_H__
#define __HINI_H__
#pragma once

//���ò����ļ�·��(д�롢����)
extern "C" void FILE_CONTROL_API HSetOperateFileA(LPCTSTR lpszFilePath);
//д������
extern "C" void FILE_CONTROL_API HWriteDataToIniA(LPCTSTR lpszModule,LPCTSTR lpszHead,LPCTSTR lpszValue);
//�õ��ִ���Ϣ
extern "C" LPCTSTR FILE_CONTROL_API HGetStringA(LPCTSTR lpszModule,LPCTSTR lpszHead);
//�õ�������Ϣ
extern "C" int FILE_CONTROL_API HGetIntA(LPCTSTR lpszModule,LPCTSTR lpszHead);
//�õ���������Ϣ
extern "C" float FILE_CONTROL_API HGetFloatA(LPCTSTR lpszModule,LPCTSTR lpszHead);

#define HSetOperateFile HSetOperateFileA
#define HWriteDataToIni HWriteDataToIniA
#define HGetString HGetStringA
#define HGetInt HGetIntA
#define HGetFloat HGetFloatA

#endif