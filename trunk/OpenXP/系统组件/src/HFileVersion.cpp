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

//ȡ�����е��ļ��汾��Ϣ������Ϣ����\n�ָ� 
CString HFileVersion::GetFullVersion() 
{ 
	CString strRet; 
	/* 
	szRet = "�ļ��汾: " + GetFileVersion() + "\n"; 
	szRet += "˵��: " + GetFileDescription() +"\n"; 
	szRet += "��Ȩ: " + GetLegalCopyright() +"\n"; 
	szRet += "��ע: " + GetComments() + "\n"; 
	szRet += "��Ʒ�汾: " + GetProductVersion() +"\n"; 
	szRet += "��Ʒ����: " + GetProductName() +"\n"; 
	szRet += "�������ڲ��汾˵��: " + GetPrivateBuild() +"\n"; 
	szRet += "��˾����: " + GetCompanyName() +"\n"; 
	szRet += "�Ϸ��̱�: " + GetLegalTrademarks() +"\n"; 
	szRet += "�ڲ�����: " + GetInternalName() +"\n"; 
	szRet += "�����ڲ��汾˵��: " + GetSpecialBuild() +"\n"; 
	szRet += "Դ�ļ���: " + GetOriginalFilename() +"\n"; 
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

//ȡ�ñ�ע 
CString HFileVersion::GetComments() 
{ 
	return GetVer("Comments"); 
} 

//ȡ���ڲ����� 
CString HFileVersion::GetInternalName() 
{ 
	return GetVer("InternalName"); 
} 

//ȡ�ò�Ʒ���� 
CString HFileVersion::GetProductName() 
{ 
	return GetVer("ProductName"); 
} 

//ȡ�ù�˾���� 
CString HFileVersion::GetCompanyName() 
{ 
	return GetVer("CompanyName"); 
} 

//ȡ�ð�Ȩ 
CString HFileVersion::GetLegalCopyright() 
{ 
	return GetVer("LegalCopyright"); 
} 

//ȡ�ò�Ʒ�汾 
CString HFileVersion::GetProductVersion() 
{ 
	return GetVer("ProductVersion"); 
} 

//ȡ���ļ����� 
CString HFileVersion::GetFileDescription() 
{ 
	return GetVer("FileDescription"); 
} 

//ȡ�úϷ��̱� 
CString HFileVersion::GetLegalTrademarks() 
{ 
	return GetVer("LegalTrademarks"); 
} 

//ȡ�ø����ڲ��汾˵�� 
CString HFileVersion::GetPrivateBuild() 
{ 
	return GetVer("PrivateBuild"); 
} 

//ȡ���ļ��汾 
CString HFileVersion::GetFileVersion() 
{ 
	return GetVer("FileVersion"); 
} 

//ȡ��Դ�ļ��� 
CString HFileVersion::GetOriginalFilename() 
{ 
	return GetVer("OriginalFilename"); 
} 

//ȡ�������ڲ��汾˵�� 
CString HFileVersion::GetSpecialBuild() 
{ 
	return GetVer("SpecialBuild"); 
} 

CString HFileVersion::GetVer(CString strSubBlock) 
{ 
	CString strRet; 

	struct LANGANDCODEPAGE 
	{ 
		WORD wLanguage; //���Դ���,����������2052 
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
