#include "StdAfx.h"
#include "../include/HLog.h"
#include <shlwapi.h>
#pragma warning(disable:4996)

class CLog
{
public:
	CLog(void);
	void WriteLog(const char* fmt, ...); //输出文字，参数就跟printf一样
	~CLog(void);

private:
	enum {BUFSIZE = 3000}; //工作缓冲区
	char m_tBuf[BUFSIZE];
	CRITICAL_SECTION m_crit; //设置一个临界区

protected:
	void JudgeDirectory();
	CString GetCurFilePath();
};

CLog::CLog(void) //构造函数，设置日志文件的默认路径
{
	::InitializeCriticalSection(&m_crit); //初始化临界区
}

CLog::~CLog(void)
{
	::DeleteCriticalSection(&m_crit); //释放里临界区
}

void CLog::JudgeDirectory()
{
	//得到当前路径名
	TCHAR path[MAX_PATH];
	::GetModuleFileName(NULL,path,MAX_PATH);
	CString p(path);
	CString subp;
	int nPos = p.ReverseFind('\\');
	p = p.Left(nPos+1);
	p += "\\log";
	if(!PathIsDirectory(p)){
		CreateDirectory(p,NULL);
	}  
}

CString CLog::GetCurFilePath()
{
	//得到当前路径名
	TCHAR path[MAX_PATH];
	::GetModuleFileName(NULL,path,MAX_PATH);
	CString p(path);
	CString subp;
	int nPos = p.ReverseFind('\\');
	p = p.Left(nPos+1);
	p += "\\log\\";

	CTime ct ; //加入当前时间
	ct = CTime::GetCurrentTime();
	CString strNowTime = ct.Format("%Y-%m-%d");//20090512
	strNowTime += ".log";
	p += strNowTime;
	return p;
}

void CLog::WriteLog(const char* fmt, ...)
{
	if (!AfxIsValidString(fmt,-1)) 
		return ;
	/*-----------------------进入临界区(写文件)------------------------------------*/ 
	::EnterCriticalSection(&m_crit); 
	try 
	{
		va_list argptr; //分析字符串的格式
		va_start(argptr, fmt);
		_vsnprintf(m_tBuf, BUFSIZE, fmt, argptr);
		va_end(argptr);
	}
	catch (...)
	{
		m_tBuf[0] = 0;
	}
	JudgeDirectory();
	FILE *fp = fopen(GetCurFilePath(), "a"); //以添加的方式输出到文件
	if (fp)
	{
		CTime ct ; //加入当前时间
		ct = CTime::GetCurrentTime();
		fprintf(fp,"%s  -- ",ct.Format("%H:%M:%S"));
		fprintf(fp, "%s\n", m_tBuf); 
		fclose(fp); 
	} 
	::LeaveCriticalSection(&m_crit); 
	/*-------------------退出临界区----------------------------------------*/ 
}

CLog g_Log;
bool HWriteLogA(const char* fmt, ...)
{
	char chBuf[3000];
	if (!AfxIsValidString(fmt, -1)) return false;
	try
	{
		va_list argptr; //分析字符串的格式
		va_start(argptr, fmt);
		_vsnprintf(chBuf, 3000, fmt, argptr);
		va_end(argptr);
	}
	catch (...)
	{
		chBuf[0] = 0;
		return false;
	}
	g_Log.WriteLog(chBuf);
	return true;
}