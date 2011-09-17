/********************************************************************
created:	2009/8/25
created:	08:25:2009   9:10
filename: 	HIni.h
author:		XinPing Huang
email:		77967292@qq.com
Qualifier:  ini配置文件读取
*********************************************************************/
#ifndef __HINI_H__
#define __HINI_H__
#pragma once

//设置操作文件路径(写入、读出)
extern "C" void FILE_CONTROL_API HSetOperateFileA(LPCTSTR lpszFilePath);
//写入数据
extern "C" void FILE_CONTROL_API HWriteDataToIniA(LPCTSTR lpszModule,LPCTSTR lpszHead,LPCTSTR lpszValue);
//得到字串信息
extern "C" LPCTSTR FILE_CONTROL_API HGetStringA(LPCTSTR lpszModule,LPCTSTR lpszHead);
//得到整形信息
extern "C" int FILE_CONTROL_API HGetIntA(LPCTSTR lpszModule,LPCTSTR lpszHead);
//得到浮点型信息
extern "C" float FILE_CONTROL_API HGetFloatA(LPCTSTR lpszModule,LPCTSTR lpszHead);

#define HSetOperateFile HSetOperateFileA
#define HWriteDataToIni HWriteDataToIniA
#define HGetString HGetStringA
#define HGetInt HGetIntA
#define HGetFloat HGetFloatA

#endif