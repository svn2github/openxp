#include "stdafx.h" 
#include "../include/HFileVersion.h" 
#pragma comment(lib, "Version.lib")

HFileVersion::HFileVersion(CString strPath) 
{ 
	m_strPath = strPath; 
} 

HFileVersion::~HFileVersion() 
{ 

} 

//取得所有的文件版本信息，各信息间以\n分隔 
CString HFileVersion::GetFullVersion() 
{ 
	CString strRet; 
	/* 
	szRet = "文件版本: " + GetFileVersion() + "\n"; 
	szRet += "说明: " + GetFileDescription() +"\n"; 
	szRet += "版权: " + GetLegalCopyright() +"\n"; 
	szRet += "备注: " + GetComments() + "\n"; 
	szRet += "产品版本: " + GetProductVersion() +"\n"; 
	szRet += "产品名称: " + GetProductName() +"\n"; 
	szRet += "个人用内部版本说明: " + GetPrivateBuild() +"\n"; 
	szRet += "公司名称: " + GetCompanyName() +"\n"; 
	szRet += "合法商标: " + GetLegalTrademarks() +"\n"; 
	szRet += "内部名称: " + GetInternalName() +"\n"; 
	szRet += "特殊内部版本说明: " + GetSpecialBuild() +"\n"; 
	szRet += "源文件名: " + GetOriginalFilename() +"\n"; 
	*/ 
	strRet = "FileVersion: " + GetFileVersion() + "\n"; 
	strRet += "FileDescription: " + GetFileDescription() +"\n"; 
	strRet += "CopyRight: " + GetLegalCopyright() +"\n"; 
	strRet += "Comments: " + GetComments() + "\n"; 
	strRet += "ProductVersion: " + GetProductVersion() +"\n"; 
	strRet += "ProductName: " + GetProductName() +"\n"; 
	strRet += "PrivateBuild: " + GetPrivateBuild() +"\n"; 
	strRet += "CompanyName: " + GetCompanyName() +"\n"; 
	strRet += "TradeMarks: " + GetLegalTrademarks() +"\n"; 
	strRet += "InternalName: " + GetInternalName() +"\n"; 
	strRet += "SpecialBuild: " + GetSpecialBuild() +"\n"; 
	strRet += "OriginalFileName: " + GetOriginalFilename() +"\n"; 
	return strRet; 
} 

//取得备注 
CString HFileVersion::GetComments() 
{ 
	return GetVer("Comments"); 
} 

//取得内部名称 
CString HFileVersion::GetInternalName() 
{ 
	return GetVer("InternalName"); 
} 

//取得产品名称 
CString HFileVersion::GetProductName() 
{ 
	return GetVer("ProductName"); 
} 

//取得公司名称 
CString HFileVersion::GetCompanyName() 
{ 
	return GetVer("CompanyName"); 
} 

//取得版权 
CString HFileVersion::GetLegalCopyright() 
{ 
	return GetVer("LegalCopyright"); 
} 

//取得产品版本 
CString HFileVersion::GetProductVersion() 
{ 
	return GetVer("ProductVersion"); 
} 

//取得文件描述 
CString HFileVersion::GetFileDescription() 
{ 
	return GetVer("FileDescription"); 
} 

//取得合法商标 
CString HFileVersion::GetLegalTrademarks() 
{ 
	return GetVer("LegalTrademarks"); 
} 

//取得个人内部版本说明 
CString HFileVersion::GetPrivateBuild() 
{ 
	return GetVer("PrivateBuild"); 
} 

//取得文件版本 
CString HFileVersion::GetFileVersion() 
{ 
	return GetVer("FileVersion"); 
} 

//取得源文件名 
CString HFileVersion::GetOriginalFilename() 
{ 
	return GetVer("OriginalFilename"); 
} 

//取得特殊内部版本说明 
CString HFileVersion::GetSpecialBuild() 
{ 
	return GetVer("SpecialBuild"); 
} 

CString HFileVersion::GetVer(CString strSubBlock) 
{ 
	CString strRet; 

	struct LANGANDCODEPAGE 
	{ 
		WORD wLanguage; //语言代码,简体中文是2052 
		WORD wCodePage; 
	} *lpTranslate; 

	DWORD dwHandle; 
	DWORD dwInfoSize; 

	dwInfoSize = GetFileVersionInfoSize((char*)(LPCTSTR)m_strPath, &dwHandle); 

	if( dwInfoSize != 0 ) 
	{ 
		char* lpData = new char[dwInfoSize]; 
		if( GetFileVersionInfo((char*)(LPCTSTR)m_strPath, 0, dwInfoSize, lpData) ) 
		{ 
			UINT cbTranslate = 0; 
			VerQueryValue(lpData, 
				TEXT("\\VarFileInfo\\Translation"), 
				(LPVOID*)&lpTranslate, 
				&cbTranslate); 

			char SubBlock[256]; 
			char* lpBuffer = 0; 
			UINT dwBytes = 0; 
			for(int i=0; i < int(cbTranslate/sizeof(struct LANGANDCODEPAGE)); i++ ) 
			{ 
				wsprintf( SubBlock, 
					TEXT("\\StringFileInfo\\%04x%04x\\%s"), 
					lpTranslate->wLanguage, 
					lpTranslate->wCodePage, 
					strSubBlock); 

				VerQueryValue(lpData, 
					SubBlock, 
					(LPVOID*)&lpBuffer, 
					&dwBytes); 

				strRet.Format("%s", lpBuffer); 

				break; //only retrieve the first language and code page 
			} 

		} 
		delete [] lpData; 
		lpData = NULL; 
	} 

	return strRet; 
} 
