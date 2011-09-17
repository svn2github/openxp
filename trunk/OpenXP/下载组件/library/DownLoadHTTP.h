#ifndef __DOWNLOADHTTP__H__
#define __DOWNLOADHTTP__H__
#pragma once
#include "DownLoadPublic.h"

class DownLoadHTTP : public DownLoadPublic
{
public:
	DownLoadHTTP();
	virtual ~DownLoadHTTP();

private:
	BOOL DownloadOnec();
	BOOL RequestHttpData(BOOL bGet, char *szTailData=NULL, int *pnTailSize=NULL);
	CTime ConvertHttpTimeString(CString strTimeGMT );
	CString FindAfterFlagString(LPCTSTR lpszFoundStr, CString strOrg);
	BOOL ParseResponseString(CString strResponseString, OUT BOOL &bRetryRequest);
	DWORD GetResponseCode(CString strLineText );
	BOOL SendRequest(LPCTSTR lpszReq, OUT CString &strResponse, char *szTailData=NULL, int *pnTailSize=NULL);
	CString GetRequestStr(BOOL bGet);

protected:
	virtual BOOL DownloadOnce();
	virtual BOOL GetRemoteSiteInfoPro();
};

#endif