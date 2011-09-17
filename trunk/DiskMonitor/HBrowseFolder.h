#ifndef __HBROWSEFOLDER__H__
#define __HBROWSEFOLDER__H__
#pragma once

class HBrowseFolder
{
public:
	HBrowseFolder();
	virtual ~HBrowseFolder();
	TCHAR *GetDirPath();
	int DoModal(CWnd *pParentWnd,const TCHAR *pStartPath=NULL);

protected:
	static int CALLBACK BrowseCallbackProc(HWND hWnd,UINT uMsg,LPARAM lParam,LPARAM lpData);
	BOOL IsValuePath(const TCHAR *pDirPath);
	TCHAR *m_pDirPath;
};

#endif