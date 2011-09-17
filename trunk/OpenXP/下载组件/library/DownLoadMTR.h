#ifndef __DOWNLOADMTR__H__
#define __DOWNLOADMTR__H__            
#pragma once                      // MTR - MultiThread Resume : 多线程断点续传
#include "DownloadFtp.h"
#include "DownloadHttp.h"
#include <Afxmt.h>

#define NOTE_DownloadMTR  "FTP、HTTP 多线程断点续传下载文件"

#define DEFAULT_THREAD_COUNT		1			// 默认下载一个文件所用的线程数
#define MAX_DOWNLOAD_THREAD_COUNT	16			// 下载一个文件最大使用的线程数

// 单个对象的下载信息
typedef struct tagDownLoadCellInfo
{
	int nWillDownloadStartPos;			// 要下载文件的开始位置
	int nWillDownloadSize;				// 本次需要下载的大小，-1表示一直下载到文件尾
	DWORD nDownloadedSize;				// 该线程已下载的大小
}DownLoadCellInfo;

// 下载信息
typedef struct tagBaseDownInfo
{
	DWORD dwThreadCount;				// 该文件由多少个线程在下载
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
	BOOL StartDownload();  // 模拟断点续传
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
	int m_nTotalDownloadedSize_ThisTimes;		// 表示这次启动下载任务以来总共下载的字节数
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
	int m_nThreadCount;							// 线程数
	DownLoadPublic *m_pDownloadPub_Info;			// 取站点信息对象
	DownLoadPublic	*m_pDownloadPub_MTR;			// 多线程下载对象
	BaseDownInfo	m_BaseDownInfo;				// 下载基本信息，线程数等
	DownLoadCellInfo *m_pDownloadCellInfo;	// 各个下载对象的参数
	// 模块结束事件
	HANDLE m_hEvtEndModule;
};

#endif 