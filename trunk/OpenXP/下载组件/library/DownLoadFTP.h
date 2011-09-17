#ifndef __DOWNLOADFTP__H__
#define __DOWNLOADFTP__H__
#pragma once
#include "DownLoadPublic.h"

class DownLoadFTP : public DownLoadPublic
{
public:
	DownLoadFTP();
	virtual ~DownLoadFTP();
	BOOL Connect ();

private:
	void GetFileTimeInfo(CString strFileTime1, CString strFileTime2, CString strFileTime3);
	BOOL CreateFTPDataConnect(HSocketClient &SocketClient);
	void ParseFileInfoStr(CString &strFileInfoStr);

protected:
	virtual BOOL GetRemoteSiteInfoPro();
	virtual BOOL DownloadOnce();
};

#endif