/********************************************************************
created:	2009/08/13
created:	08:13:2009   15:37
filename: 	HLog.h
author:		XinPing Huang
email:		77967292@qq.com
Qualifier:  ������
*********************************************************************/
#ifndef __LOG__H__
#define __LOG__H__
#pragma once

//��������
extern "C" bool DEBUGGING_CONTROL_API HWriteLogA(const char* fmt, ...);
#ifdef _DEBUG
	#define HWriteLog HWriteLogA
#else
	#define HWriteLog
#endif

#endif