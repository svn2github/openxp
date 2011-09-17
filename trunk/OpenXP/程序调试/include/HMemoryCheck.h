/********************************************************************
created:	2009/11/13
created:	13:11:2009   15:37
filename: 	HMemoryCheck.h
author:		XinPing Huang
email:		77967292@qq.com
Qualifier:  ÄÚ´æ¼ì²â
*********************************************************************/
#ifndef _MEMORYCHECK_H_
#define _MEMORYCHECK_H_
#pragma once

#ifdef _DEBUG
	extern "C" void DEBUGGING_CONTROL_API HCreateCheckPointA();
	extern "C" void DEBUGGING_CONTROL_API HShowCheckPointA(BOOL bShowAll=TRUE);
//////////////////////////////////////////////////////////////////////////
	#define HCreateCheckPoint HCreateCheckPointA
	#define HOutPutMemoryInfo HShowCheckPointA
#endif

#endif