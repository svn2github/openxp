#ifndef __TIMETRACE__H__
#define __TIMETRACE__H__
#pragma once
#include <time.h>
#include <fstream>
#ifdef _WINDOWS          //������Ŀ
	#include <Shlwapi.h>
#else					 //����̨��Ŀ
	#include <atlpath.h>
#endif
using namespace std;

enum enHTML
{
	HTML_COLOR_NORMAL=0,	//������Ϣ
	HTML_COLOR_ERROR,		//������Ϣ
	HTML_COLOR_WARNING,		//������Ϣ
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