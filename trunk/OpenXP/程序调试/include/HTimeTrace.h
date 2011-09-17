#ifndef __TIMETRACE__H__
#define __TIMETRACE__H__
#pragma once
#include <time.h>
#include <fstream>
#ifdef _WINDOWS          //窗口项目
	#include <Shlwapi.h>
#else					 //控制台项目
	#include <atlpath.h>
#endif
using namespace std;

enum enHTML
{
	HTML_COLOR_NORMAL=0,	//正常信息
	HTML_COLOR_ERROR,		//错误信息
	HTML_COLOR_WARNING,		//警告信息
};

void DEBUGGING_CONTROL_API ClearLog();
void DEBUGGING_CONTROL_API HTrace(LPCTSTR lpsz,...);
void DEBUGGING_CONTROL_API HTraceHtml(enHTML enType,LPCTSTR lpsz,...);


#ifdef _DEBUG
	#define HTRACE HTrace
	#define HTRACEHTML HTraceHtml
#else
	#define HTRACE
	#define HTRACEHTML
#endif

#endif