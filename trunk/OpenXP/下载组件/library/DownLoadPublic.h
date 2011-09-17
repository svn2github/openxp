#ifndef __DOWNLOADPUBLIC__H__
#define __DOWNLOADPUBLIC__H__
#pragma once
#include <Afxmt.h>
#include <afxtempl.h>
#include "HSocketClient.h"

//缺省的重试次数
const UINT DEFAULT_RETRY_MAX	= 10; 

#define SLEEP_RETURN_Down(x)\
{\
	if ( ::WaitForSingleObject ( m_hEvtEndModule, x ) == WAIT_OBJECT_0 )\
	return DownloadEnd(FALSE);\
}

enum
{
	NOTIFY_TYPE_GOT_REMOTE_FILENAME,				// 取得远程站点文件名, 当被下载的文件被重定向时才发送该通知，lpNotifyData 为 LPCTSTR 类型的文件名字符串指针
	NOTIFY_TYPE_GOT_REMOTE_FILESIZE,				// 取得远程站点文件大小，lpNotifyData 为 int 类型的文件大小
	NOTIFY_TYPE_START_DOWNLOAD,						// 开始下载，lpNotifyData 无用
	NOTIFY_TYPE_END_DOWNLOAD,						// 结束下载，lpNotifyData 为 ENUM_DOWNLOAD_RESULT 类型的下载结果
	NOTIFY_TYPE_WILL_DOWNLOAD_SIZE,					// 本次需要下载的大小，lpNotifyData 为 int 类型的需要下载的大小
	NOTIFY_TYPE_THREAD_DOWNLOADED_SIZE,				// 某线程已下载的数据大小，lpNotifyData 为 int 类型的已下载的大小
};

typedef struct tagDownloadNotifyPara
{
	int nIndex;
	UINT nNotityType;
	LPVOID lpNotifyData;
	LPVOID pDownloadMTR;
} DownloadNotifyPara;
typedef CArray<DownloadNotifyPara,DownloadNotifyPara&> g_ArrayDownloadNotifyPara;

// 消息通知回调函数
typedef void (*FUNC_DownloadNotify) ( DownloadNotifyPara *pDownloadNotifyPara, WPARAM wParam );
typedef void (*FUNC_SaveDownloadInfo) ( int nIndex, int nDownloadedSize, int nSimpleSaveSize, WPARAM wParam );

// 缺省端口号
#define DEFAULT_HTTP_PORT			80
#define DEFAULT_HTTPS_PORT			443
#define DEFAULT_FTP_PORT			21
#define DEFAULT_SOCKS_PORT			6815

typedef enum _DownloadResult
{
	ENUM_DOWNLOAD_RESULT_SUCCESS,
	ENUM_DOWNLOAD_RESULT_FAILED,
	ENUM_DOWNLOAD_RESULT_CANCEL,
} ENUM_DOWNLOAD_RESULT;


class DownLoadPublic
{
public:
	BOOL ThreadIsRunning();
	CString GetDownloadObjectFileName(CString *pstrExtensionName=NULL);
	void ClearThreadHandle();
	void ResetVar();
	int GetUndownloadBytes();
	BOOL ThreadProcDownload();
	BOOL SetSaveFileName(LPCTSTR lpszSaveFileName);
	int GetWillDownloadSize();
	void SetWillDownloadSize(int nWillDownloadSize);
	int GetDownloadedSize();
	void SetDownloadedSize(int nDownloadedSize);
	int GetTempSaveBytes();
	void SetTempSaveBytes(int nTempSaveBytes);
	CString GetRemoteFileName();
	BOOL SetDownloadUrl(LPCTSTR lpszDownloadUrl);
	virtual BOOL Connect();
	BOOL GetRemoteSiteInfo();
	DownLoadPublic();
	virtual ~DownLoadPublic();
	void SetAuthorization (LPCTSTR lpszUsername, LPCTSTR lpszPassword);
	void SetReferer(LPCTSTR lpszReferer);
	void SetUserAgent(LPCTSTR lpszUserAgent);
	void SetSaveDownloadInfoCallback(FUNC_SaveDownloadInfo Proc_SaveDownloadInfo, WPARAM wParam);
	virtual BOOL Download(
		int nWillDownloadStartPos,
		int nWillDownloadSize,
		int nDownloadedSize
		);
	BOOL IsSupportResume() { return m_bSupportResume; }
	CString GetProtocolType() { return m_strProtocolType; }
	time_t GetTimeLastModified() { return m_TimeLastModified.GetTime(); }
	int GetFileTotalSize() { return m_nFileTotalSize; }
	CString GetUserName() { return m_strUsername; }
	CString GetPassword() { return m_strPassword; }
	CString GetDownloadUrl() { return m_strDownloadUrl; }
	BOOL IsDownloadSuccess() { return m_bDownloadSuccess; }
	HANDLE GetThreadHandle() { return m_hThread; }
	int GetWillDownloadStartPos() { return m_nWillDownloadStartPos; }
	CString GetServerName() { return m_strServer; }
	void StopDownload();

public:
	LPVOID m_pDownloadMTR;
	int m_nIndex;

protected:
	virtual BOOL GetRemoteSiteInfoPro();
	virtual BOOL DownloadOnce();
	CString GetRefererFromURL();
	int SaveDataToFile(char *pData, int nSize);
	virtual BOOL RecvDataAndSaveToFile(HSocketClient &SocketClient,char *szTailData=NULL, int nTailSize=0);
	BOOL DownloadEnd(BOOL bRes);

	CFile m_file;
	// 模块结束事件
	HANDLE m_hEvtEndModule;
	// 连接服务器的 Socket
	HSocketClient m_SocketClient;
	// 待下载URL
	CString		m_strDownloadUrl;
	CString		m_strSaveFileName;

	// 保存下载信息的回调函数
	FUNC_SaveDownloadInfo m_funcSaveDownloadInfo;
	WPARAM m_wSaveDownloadInfo_Param;

	// 是否支持断点续传
	BOOL		m_bSupportResume;
	HANDLE		m_hThread;

	// 文件以及下载大小
	int					m_nFileTotalSize;				// 文件总的大小，-1表示未知文件大小
	int					m_nWillDownloadStartPos;		// 要下载文件的开始位置
	int					m_nWillDownloadSize;			// 本次需要下载的大小，-1表示不知道文件大小，所以下载到服务器关闭连接为止
	CCriticalSection	m_csWillDownloadSize;		// 访问 m_nWillDownloadSize 变量的互斥锁
	int					m_nTempSaveBytes;				// 存放在临时缓冲中的字节数
	CCriticalSection	m_csTempSaveBytes;			// 访问 m_nTempSaveBytes 变量的互斥锁
	int					m_nDownloadedSize;				// 已下载的字节数，指完全写到文件中的字节数，不包含临时缓冲里的数据
	CCriticalSection	m_csDownloadedSize;			// 访问 m_nDownloadedSize 变量的互斥锁

	// 文件日期(远程文件的信息)
	CTime		m_TimeLastModified;

	// Referer
	CString		m_strReferer;
	CString		m_strCookieFlag;

	// UserAgent
	CString		m_strUserAgent;

	// 是否进行验证 : Request-Header: Authorization
	CString		m_strUsername;
	CString		m_strPassword;

	// 下载过程中所用的变量
	CString		m_strProtocolType;	// 所使用的传输协议：http、ftp等
	CString		m_strServer;
	CString		m_strObject;
	CString		m_strFileName;
	USHORT		m_nPort;

private:
	BOOL OpenFileForSave();
	BOOL m_bDownloadSuccess;
};

int Base64Encode(LPCTSTR lpszEncoding, CString &strEncoded);
int Base64Decode(LPCTSTR lpszDecoding, CString &strDecoded);
BOOL ParseTrueURL(LPCTSTR lpszURL,CString& strServer,CString& strObject,USHORT& nPort, CString &csProtocolType);
void Set_DownloadNotify_Callback(FUNC_DownloadNotify Proc_DownloadNotify, WPARAM wParam);
void SetRetryTimes(int nRetryTimes);

#endif