#include "StdAfx.h"
#include "../include/HIni.h"

class CHIni
{
public:
	CHIni();
	CHIni(LPCTSTR lpszFilePath);
	~CHIni(void);
	inline void SetOperateFile(LPCTSTR lpszFilePath){m_lpszFile=lpszFilePath;}
	void WriteDataToIni(LPCTSTR lpszModule,LPCTSTR lpszHead,LPCTSTR lpszValue);
	CString GetString(LPCTSTR lpszModule,LPCTSTR lpszHead);
	int GetInt(LPCTSTR lpszModule,LPCTSTR lpszHead);
	float GetFloat(LPCTSTR lpszModule,LPCTSTR lpszHead);
	inline bool GetIsPathValid(){return m_lpszFile==NULL?false:true;} 
private:
	LPCTSTR m_lpszFile;
	CString m_strString;
	int m_nInt;
	float m_fFloat;
};

CHIni::CHIni()
{
}

CHIni::CHIni( LPCTSTR strFilePath )
{
	m_lpszFile = strFilePath;
}

CHIni::~CHIni(void)
{
}

CString CHIni::GetString( LPCTSTR strModule,LPCTSTR strHead )
{
	::GetPrivateProfileString(strModule,strHead,_T("不存在关键字"),m_strString.GetBuffer(MAX_PATH),MAX_PATH,m_lpszFile); 
	return m_strString;
}

int CHIni::GetInt( LPCTSTR strModule,LPCTSTR strHead )
{
	m_nInt = ::GetPrivateProfileInt(strModule,strHead,32767,m_lpszFile); 
	return m_nInt;
}

float CHIni::GetFloat( LPCTSTR strModule,LPCTSTR strHead )
{
	::GetPrivateProfileString(strModule,strHead,_T("不存在关键字"),m_strString.GetBuffer(MAX_PATH),MAX_PATH,m_lpszFile); 
	if ( m_strString == _T("不存在关键字") )
		return 32767.0f;
	m_fFloat = (float)atof((LPSTR)(LPCTSTR)m_strString);
	return m_fFloat;
}

void CHIni::WriteDataToIni( LPCTSTR strModule,LPCTSTR strHead,LPCTSTR strValue )
{
	::WritePrivateProfileString(strModule,strHead,strValue,m_lpszFile); 
}


//
CHIni g_Ini;
void HSetOperateFileA( LPCTSTR lpszFilePath )
{
	if (lpszFilePath == NULL)
	{
		AfxMessageBox(_T("Ini文件路径不对..."));
		return;
	}
	g_Ini.SetOperateFile(lpszFilePath);
}

void HWriteDataToIniA( LPCTSTR lpszModule,LPCTSTR lpszHead,LPCTSTR lpszValue )
{
	if (!g_Ini.GetIsPathValid())
	{
		AfxMessageBox(_T("Ini文件路径未指定..."));
		return;
	}
	g_Ini.WriteDataToIni(lpszModule,lpszHead,lpszValue);
}

LPCTSTR HGetStringA( LPCTSTR lpszModule,LPCTSTR lpszHead )
{
	if (!g_Ini.GetIsPathValid())
	{
		AfxMessageBox(_T("Ini文件路径未指定..."));
		return NULL;
	}
	return g_Ini.GetString(lpszModule,lpszHead);
}

int HGetIntA( LPCTSTR lpszModule,LPCTSTR lpszHead )
{
	if (!g_Ini.GetIsPathValid())
	{
		AfxMessageBox(_T("Ini文件路径未指定..."));
		return 0;
	}
	return g_Ini.GetInt(lpszModule,lpszHead);
}

float HGetFloatA( LPCTSTR lpszModule,LPCTSTR lpszHead )
{
	if (!g_Ini.GetIsPathValid())
	{
		AfxMessageBox(_T("Ini文件路径未指定..."));
		return 0.0f;
	}
	return g_Ini.GetFloat(lpszModule,lpszHead);
}	