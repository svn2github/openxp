#ifndef __DOWNLOADPUBLIC__H__
#define __DOWNLOADPUBLIC__H__
#pragma once
#include <Afxmt.h>
#include <afxtempl.h>
#include "HSocketClient.h"

//ȱʡ�����Դ���
const UINT DEFAULT_RETRY_MAX	= 10; 

#define SLEEP_RETURN_Down(x)\
{\
	if ( ::WaitForSingleObject ( m_hEvtEndModule, x ) == WAIT_OBJECT_0 )\
	return DownloadEnd(FALSE);\
}

enum
{
	NOTIFY_TYPE_GOT_REMOTE_FILENAME,				// ȡ��Զ��վ���ļ���, �������ص��ļ����ض���ʱ�ŷ��͸�֪ͨ��lpNotifyData Ϊ LPCTSTR ���͵��ļ����ַ���ָ��
	NOTIFY_TYPE_GOT_REMOTE_FILESIZE,				// ȡ��Զ��վ���ļ���С��lpNotifyData Ϊ int ���͵��ļ���С
	NOTIFY_TYPE_START_DOWNLOAD,						// ��ʼ���أ�lpNotifyData ����
	NOTIFY_TYPE_END_DOWNLOAD,						// �������أ�lpNotifyData Ϊ ENUM_DOWNLOAD_RESULT ���͵����ؽ��
	NOTIFY_TYPE_WILL_DOWNLOAD_SIZE,					// ������Ҫ���صĴ�С��lpNotifyData Ϊ int ���͵���Ҫ���صĴ�С
	NOTIFY_TYPE_THREAD_DOWNLOADED_SIZE,				// ĳ�߳������ص����ݴ�С��lpNotifyData Ϊ int ���͵������صĴ�С
};

typedef struct tagDownloadNotifyPara
{
	int nIndex;
	UINT nNotityType;
	LPVOID lpNotifyData;
	LPVOID pDownloadMTR;
} DownloadNotifyPara;
typedef CArray<DownloadNotifyPara,DownloadNotifyPara&> g_ArrayDownloadNotifyPara;

// ��Ϣ֪ͨ�ص�����
typedef void (*FUNC_DownloadNotify) ( DownloadNotifyPara *pDownloadNotifyPara, WPARAM wParam );
typedef void (*FUNC_SaveDownloadInfo) ( int nIndex, int nDownloadedSize, int nSimpleSaveSize, WPARAM wParam );

// ȱʡ�˿ں�
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
	// ģ������¼�
	HANDLE m_hEvtEndModule;
	// ���ӷ������� Socket
	HSocketClient m_SocketClient;
	// ������URL
	CString		m_strDownloadUrl;
	CString		m_strSaveFileName;

	// ����������Ϣ�Ļص�����
	FUNC_SaveDownloadInfo m_funcSaveDownloadInfo;
	WPARAM m_wSaveDownloadInfo_Param;

	// �Ƿ�֧�ֶϵ�����
	BOOL		m_bSupportResume;
	HANDLE		m_hThread;

	// �ļ��Լ����ش�С
	int					m_nFileTotalSize;				// �ļ��ܵĴ�С��-1��ʾδ֪�ļ���С
	int					m_nWillDownloadStartPos;		// Ҫ�����ļ��Ŀ�ʼλ��
	int					m_nWillDownloadSize;			// ������Ҫ���صĴ�С��-1��ʾ��֪���ļ���С���������ص��������ر�����Ϊֹ
	CCriticalSection	m_csWillDownloadSize;		// ���� m_nWillDownloadSize �����Ļ�����
	int					m_nTempSaveBytes;				// �������ʱ�����е��ֽ���
	CCriticalSection	m_csTempSaveBytes;			// ���� m_nTempSaveBytes �����Ļ�����
	int					m_nDownloadedSize;				// �����ص��ֽ�����ָ��ȫд���ļ��е��ֽ�������������ʱ�����������
	CCriticalSection	m_csDownloadedSize;			// ���� m_nDownloadedSize �����Ļ�����

	// �ļ�����(Զ���ļ�����Ϣ)
	CTime		m_TimeLastModified;

	// Referer
	CString		m_strReferer;
	CString		m_strCookieFlag;

	// UserAgent
	CString		m_strUserAgent;

	// �Ƿ������֤ : Request-Header: Authorization
	CString		m_strUsername;
	CString		m_strPassword;

	// ���ع��������õı���
	CString		m_strProtocolType;	// ��ʹ�õĴ���Э�飺http��ftp��
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