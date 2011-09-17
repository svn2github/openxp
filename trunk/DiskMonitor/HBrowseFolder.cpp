#include "StdAfx.h"
#include "HBrowseFolder.h"

int CALLBACK HBrowseFolder::BrowseCallbackProc(HWND hWnd,UINT uMsg,LPARAM lParam,LPARAM lpData)
{
	HBrowseFolder *pBrowseFolder=(HBrowseFolder *)lpData;
	if(uMsg == BFFM_INITIALIZED)
		SendMessage(hWnd,BFFM_SETSELECTION,TRUE,(LPARAM) pBrowseFolder->m_pDirPath);
	return 0;
}

HBrowseFolder::HBrowseFolder()
{
	m_pDirPath = new TCHAR[MAX_PATH];
}

HBrowseFolder::~HBrowseFolder()
{
	delete m_pDirPath;
}

int HBrowseFolder::DoModal(CWnd *pParentWnd,const TCHAR *pStartPath)
{
	LPMALLOC pMalloc;
	if(SHGetMalloc(&pMalloc)!=NOERROR)
		return -1;
	if(pStartPath && IsValuePath(pStartPath))
	{
		TCHAR *pStr = m_pDirPath;
		while(*pStr++=*pStartPath++);
	}
	BROWSEINFO bInfo={pParentWnd->m_hWnd,NULL,0,_T("ÇëÑ¡ÔñÎÄ¼þ¼Ð:"),0,BrowseCallbackProc,(LPARAM) this,0};
	ITEMIDLIST *pItemList=SHBrowseForFolder(&bInfo);
	if(pItemList)
	{
		SHGetPathFromIDList(pItemList,m_pDirPath);
		pMalloc->Free(pItemList);
		pMalloc->Release();
		return TRUE;
	}
	pMalloc->Free(pItemList);
	pMalloc->Release();
	return FALSE;
}

TCHAR *HBrowseFolder::GetDirPath()
{
	return m_pDirPath;
}

BOOL HBrowseFolder::IsValuePath(const TCHAR *pDirPath)
{
	DWORD dFileAttrib=GetFileAttributes(pDirPath);
	return (dFileAttrib!=0xffffffff && dFileAttrib & FILE_ATTRIBUTE_DIRECTORY)?TRUE:FALSE;
}