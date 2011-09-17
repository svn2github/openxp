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

#define MAX_HTML_BUFFER  1024
enum enHTML
{
	HTML_COLOR_NORMAL=0,	//正常信息
	HTML_COLOR_ERROR,		//错误信息
	HTML_COLOR_WARNING,		//警告信息
};
static TCHAR g_szHTMLSavePath[MAX_HTML_BUFFER] = {0};

static void ClearLog()
{
	if (PathFileExists("trace.log"))
		DeleteFile("trace.log");
	if (PathFileExists("trace.htm"))
		DeleteFile("trace.htm");
}

static void HTrace(LPCTSTR lpsz,...)
{
#ifdef _DEBUG
	time_t curtime = time(0); 
	tm tim = *localtime(&curtime); 
	int day,mon,year,hour,min,sec; 
	sec		=	tim.tm_sec;
	min		=	tim.tm_min;
	hour	=	tim.tm_hour;
	day		=	tim.tm_mday; 
	mon		=	tim.tm_mon+1; 
	year	=	tim.tm_year+1900; 

#define BUFFER_MAX_LOG 2048
	va_list arglist;
	TCHAR szTrace[BUFFER_MAX_LOG] = {0};
	size_t nTimeLength = 0;

	sprintf(szTrace,"[%.4d-%.2d-%.2d %.2d:%.2d:%.2d]：",year,mon,day,hour,min,sec);
	nTimeLength = strlen(szTrace);

	va_start(arglist, lpsz);
	_vsnprintf(szTrace+nTimeLength,BUFFER_MAX_LOG-nTimeLength-1,lpsz,arglist);

#ifdef _CONSOLE
	printf(szTrace);
	printf("\n")
#else
	TRACE(szTrace);
	TRACE("\n");
#endif

	ofstream outFile("trace.log",ios::app);
	outFile << szTrace;
	outFile << "\r\n";
	outFile.flush(); 
	outFile.close();
#endif
}

static void HTraceHtml(enHTML enType,LPCTSTR lpsz,...)
{
//#ifdef _DEBUG
	time_t curtime = time(0); 
	tm tim = *localtime(&curtime); 
	int day,mon,year,hour,min,sec; 
	sec		=	tim.tm_sec;
	min		=	tim.tm_min;
	hour	=	tim.tm_hour;
	day		=	tim.tm_mday; 
	mon		=	tim.tm_mon+1; 
	year	=	tim.tm_year+1900; 

#define BUFFER_MAX_LOG 2048
	va_list arglist;
	TCHAR szTrace[BUFFER_MAX_LOG] = {0};
	TCHAR szFileName[BUFFER_MAX_LOG] = {0};
	size_t nTimeLength = 0;

	sprintf(szTrace,"[%.4d-%.2d-%.2d %.2d:%.2d:%.2d]：",year,mon,day,hour,min,sec);
	sprintf(szFileName,".\\%.4d-%.2d-%.2d",year,mon,day);
	strcat(szFileName,".htm");

	switch(enType)
	{
	case HTML_COLOR_NORMAL:
		{
			strcat(szTrace,"<font color=#000000>");
			break;
		}
	case HTML_COLOR_ERROR:
		{
			strcat(szTrace,"<font color=#ff0000>");
			break;
		}
	case HTML_COLOR_WARNING:
		{
			strcat(szTrace,"<font color=#0000ff>");
			break;
		}
	default:
		break;
	}
	nTimeLength = strlen(szTrace);

	va_start(arglist, lpsz);
	_vsnprintf(szTrace+nTimeLength,BUFFER_MAX_LOG-nTimeLength-1,lpsz,arglist);

	strcat(szTrace,"</font><br>");

#ifdef _CONSOLE
	printf(szTrace);
	printf("\n")
#else
	TRACE(szTrace);
	TRACE("\n");
#endif

	ofstream outFile(szFileName,ios::app);
	outFile << szTrace;
	outFile.flush(); 
	outFile.close();
//#endif
}

#ifdef _DEBUG
	#define HTRACE HTrace
	#define HTRACEHTML HTraceHtml
#else
	#define HTRACE
	#define HTRACEHTML
#endif

#endif