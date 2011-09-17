#include "stdafx.h"
#include "../include/HTimeTrace.h"

void ClearLog()
{
	if (PathFileExists("trace.log"))
		DeleteFile("trace.log");
	if (PathFileExists("trace.htm"))
		DeleteFile("trace.htm");
}

void HTrace( LPCTSTR lpsz,... )
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

	sprintf(szTrace,"[%.4d-%.2d-%.2d %.2d:%.2d:%.2d]£º",year,mon,day,hour,min,sec);
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

void HTraceHtml( enHTML enType,LPCTSTR lpsz,... )
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

	sprintf(szTrace,"[%.4d-%.2d-%.2d %.2d:%.2d:%.2d]£º",year,mon,day,hour,min,sec);

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
			strcat(szTrace,"<font color=#ffdd00>");
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

	ofstream outFile("trace.htm",ios::app);
	outFile << szTrace;
	outFile.flush(); 
	outFile.close();
#endif
}