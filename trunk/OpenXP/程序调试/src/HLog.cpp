#include "StdAfx.h"
#include "../include/HLog.h"
#include <shlwapi.h>
#pragma warning(disable:4996)

class CLog
{
public:
	CLog(void);
	void WriteLog(const char* fmt, ...); //������֣������͸�printfһ��
	~CLog(void);

private:
	enum {BUFSIZE = 3000}; //����������
	char m_tBuf[BUFSIZE];
	CRITICAL_SECTION m_crit; //����һ���ٽ���

protected:
	void JudgeDirectory();
	CString GetCurFilePath();
};

CLog::CLog(void) //���캯����������־�ļ���Ĭ��·��
{
	::InitializeCriticalSection(&m_crit); //��ʼ���ٽ���
}

CLog::~CLog(void)
{
	::DeleteCriticalSection(&m_crit); //�ͷ����ٽ���
}

void CLog::JudgeDirectory()
{
	//�õ���ǰ·����
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
	//�õ���ǰ·����
	TCHAR path[MAX_PATH];
	::GetModuleFileName(NULL,path,MAX_PATH);
	CString p(path);
	CString subp;
	int nPos = p.ReverseFind('\\');
	p = p.Left(nPos+1);
	p += "\\log\\";

	CTime ct ; //���뵱ǰʱ��
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
	/*-----------------------�����ٽ���(д�ļ�)------------------------------------*/ 
	::EnterCriticalSection(&m_crit); 
	try 
	{
		va_list argptr; //�����ַ����ĸ�ʽ
		va_start(argptr, fmt);
		_vsnprintf(m_tBuf, BUFSIZE, fmt, argptr);
		va_end(argptr);
	}
	catch (...)
	{
		m_tBuf[0] = 0;
	}
	JudgeDirectory();
	FILE *fp = fopen(GetCurFilePath(), "a"); //����ӵķ�ʽ������ļ�
	if (fp)
	{
		CTime ct ; //���뵱ǰʱ��
		ct = CTime::GetCurrentTime();
		fprintf(fp,"%s  -- ",ct.Format("%H:%M:%S"));
		fprintf(fp, "%s\n", m_tBuf); 
		fclose(fp); 
	} 
	::LeaveCriticalSection(&m_crit); 
	/*-------------------�˳��ٽ���----------------------------------------*/ 
}

CLog g_Log;
bool HWriteLogA(const char* fmt, ...)
{
	char chBuf[3000];
	if (!AfxIsValidString(fmt, -1)) return false;
	try
	{
		va_list argptr; //�����ַ����ĸ�ʽ
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