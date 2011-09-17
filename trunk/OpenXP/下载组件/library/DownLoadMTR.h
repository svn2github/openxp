#ifndef __DOWNLOADMTR__H__
#define __DOWNLOADMTR__H__            
#pragma once                      // MTR - MultiThread Resume : ���̶߳ϵ�����
#include "DownloadFtp.h"
#include "DownloadHttp.h"
#include <Afxmt.h>

#define NOTE_DownloadMTR  "FTP��HTTP ���̶߳ϵ����������ļ�"

#define DEFAULT_THREAD_COUNT		1			// Ĭ������һ���ļ����õ��߳���
#define MAX_DOWNLOAD_THREAD_COUNT	16			// ����һ���ļ����ʹ�õ��߳���

// ���������������Ϣ
typedef struct tagDownLoadCellInfo
{
	int nWillDownloadStartPos;			// Ҫ�����ļ��Ŀ�ʼλ��
	int nWillDownloadSize;				// ������Ҫ���صĴ�С��-1��ʾһֱ���ص��ļ�β
	DWORD nDownloadedSize;				// ���߳������صĴ�С
}DownLoadCellInfo;

// ������Ϣ
typedef struct tagBaseDownInfo
{
	DWORD dwThreadCount;				// ���ļ��ɶ��ٸ��߳�������
}BaseDownInfo;

class DownLoadMTR
{
public:
	int GetTotalDownloadedSize_ThisTimes();
	int GetTotalDownloadedSize();
	int GetFileTotaleSize();
	static CString GetLocalFileNameByURL(LPCTSTR lpszDownloadURL);
	void StopDownload();
	DWORD GetDownloadElapsedTime();
	BOOL StartDownload();  // ģ��ϵ�����
	BOOL Download (
		LPCTSTR lpszDownloadURL,
		LPCTSTR lpszSavePath,
		LPCTSTR lpszSaveOnlyFileName,
		LPCTSTR lpszUsername=NULL,
		LPCTSTR lpszPassword=NULL,
		BOOL bForceDownload=FALSE
		);
	BOOL SetThreadCount (int nThreadCount);
	DownLoadMTR();
	virtual ~DownLoadMTR();
	void CallbackSaveDownloadInfo(int nIndex, int nDownloadedSize, int nSimpleSaveSize);
	BOOL ThreadProcDownloadMTR();

private:
	int m_nTotalDownloadedSize_ThisTimes;		// ��ʾ��������������������ܹ����ص��ֽ���
	CCriticalSection m_csDownloadedData;
	void StandardSaveFileName();
	BOOL m_bForceDownload;
	HANDLE m_hThread;
	DWORD m_dwDownloadStartTime;
	int GetDownloadInfoWholeSize();
	int FindIndexByThreadHandle(HANDLE hThread);
	ENUM_DOWNLOAD_RESULT WaitForDownloadFinished();
	BOOL GetDownloadResult();
	BOOL AttemperDownloadTask(int nIndex);
	int GetUndownloadMaxBytes(int &nUndownloadBytes);
	BOOL HandleDownloadFinished (ENUM_DOWNLOAD_RESULT eDownloadResult);
	BOOL SaveDownloadInfo();
	BOOL AssignDownloadTask();
	BOOL DownloadInfoIsValid();
	BOOL ReadDownloadInfo();
	CString GetTempFilePath();
	int StartMTRDownload();
	void DeleteDownloadObjectAndDataMTR();
	void DeleteDownloadObjectInfo();
	BOOL CreateDownloadObjectAndDataMTR();
	DownLoadPublic* CreateDownloadObject(int nCount=1);
	void DeleteDownloadObject(DownLoadPublic *pDownloadPub);
	CString m_strSavePath, m_strSaveOnlyFileName, m_strSavePathFileName, m_strDownloadURL;
	CString m_strProtocolType;
	int m_nThreadCount;							// �߳���
	DownLoadPublic *m_pDownloadPub_Info;			// ȡվ����Ϣ����
	DownLoadPublic	*m_pDownloadPub_MTR;			// ���߳����ض���
	BaseDownInfo	m_BaseDownInfo;				// ���ػ�����Ϣ���߳�����
	DownLoadCellInfo *m_pDownloadCellInfo;	// �������ض���Ĳ���
	// ģ������¼�
	HANDLE m_hEvtEndModule;
};

#endif 