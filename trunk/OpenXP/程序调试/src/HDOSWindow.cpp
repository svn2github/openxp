#include "stdafx.h"
#include "../include/HDOSWindow.h"
#ifdef _DEBUG

#include <tchar.h>
#include <stdio.h>
#include <stdarg.h>
#pragma warning(disable:4996)

#define CONSOLE_TILE _T("OutPutWnd for Console")

class CHConsoleWindow 
{
public:
	CHConsoleWindow();
	virtual ~CHConsoleWindow();
	BOOL SetTile(LPCTSTR lpTile);
	BOOL WriteString(LPCTSTR lpString);
	BOOL WriteString(WORD Attrs,LPCTSTR lpString);
	void WindowSetPos(int nx,int ny);

private:
	HANDLE m_hConsole;
	BOOL   m_bCreate;
	BOOL   m_bAttrs;
	WORD   m_OldColorAttrs; 
};

CHConsoleWindow::CHConsoleWindow()
{
	m_hConsole=NULL;
	m_bCreate=FALSE;
	if(AllocConsole())
	{
		m_hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTitle(CONSOLE_TILE);
		SetConsoleMode(m_hConsole,ENABLE_PROCESSED_OUTPUT);
		m_bCreate=TRUE;
	}
	else
	{
		m_hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
		if(m_hConsole==INVALID_HANDLE_VALUE)
			m_hConsole=NULL;
	}
	if(m_hConsole)
	{
		CONSOLE_SCREEN_BUFFER_INFO csbiInfo; 
		if(GetConsoleScreenBufferInfo(m_hConsole, &csbiInfo))
		{
			m_bAttrs=TRUE;
			m_OldColorAttrs = csbiInfo.wAttributes;   
		}
		else
		{
			m_bAttrs=FALSE;
			m_OldColorAttrs = 0;
		}
	}
}

CHConsoleWindow::~CHConsoleWindow()
{
	if(m_bCreate) FreeConsole();
}

BOOL CHConsoleWindow::SetTile(LPCTSTR lpTile)
{
	return SetConsoleTitle(lpTile);
}

BOOL CHConsoleWindow::WriteString(LPCTSTR lpString)
{
	BOOL ret=FALSE;
	if(m_hConsole)
	{
		ret = WriteConsole(m_hConsole,lpString,(DWORD)_tcslen(lpString),NULL,NULL);
	}
	return ret;
}

BOOL CHConsoleWindow::WriteString(WORD Attrs,LPCTSTR lpString)
{
	BOOL ret=FALSE;
	if(m_hConsole)
	{
		if(m_bAttrs)
			SetConsoleTextAttribute(m_hConsole,Attrs);
		ret = WriteConsole(m_hConsole,lpString,(DWORD)_tcslen(lpString),NULL,NULL);
		if(m_bAttrs)
			SetConsoleTextAttribute(m_hConsole,m_OldColorAttrs);
	}
	return ret; 
}

void CHConsoleWindow::WindowSetPos( int nx,int ny )
{
	COORD setps;
	setps.X = nx;
	setps.Y = ny;
	SetConsoleCursorPosition(m_hConsole,setps);
}

CHConsoleWindow ConWindow;
#define MAX_BUF_LEN 4096

BOOL HWindowTile(LPCTSTR tile)
{
	return ConWindow.SetTile(tile);
}

BOOL HWindowWrite(LPCTSTR fmt,...)
{
	TCHAR   message[MAX_BUF_LEN];
	va_list cur_arg;
	va_start(cur_arg,fmt);
	_vsntprintf(message,MAX_BUF_LEN,fmt,cur_arg);
	va_end(cur_arg);
	return ConWindow.WriteString(message);
}

BOOL HWindowWrite(WORD Attrs,LPCTSTR fmt,...)
{
	TCHAR   message[MAX_BUF_LEN];
	va_list cur_arg;
	va_start(cur_arg,fmt);
	_vsntprintf(message,MAX_BUF_LEN,fmt,cur_arg);
	va_end(cur_arg);
	return ConWindow.WriteString(Attrs,message); 
}

BOOL HWindowDocument(const char* file,UINT nLine)
{
	TCHAR  buffer[MAX_BUF_LEN];
	wsprintf(buffer,"µ÷ÊÔ:%s   %uÐÐ\n",file,nLine);
	TCHAR   message[MAX_BUF_LEN];
	va_list cur_arg;
	va_start(cur_arg,buffer);
	_vsntprintf(message,MAX_BUF_LEN,buffer,cur_arg);
	va_end(cur_arg);
	return ConWindow.WriteString(message);
}

void HWindowSetPos(int nx,int ny)
{
	ConWindow.WindowSetPos(nx,ny);
}

#endif