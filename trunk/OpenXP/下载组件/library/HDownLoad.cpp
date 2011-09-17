#include "StdAfx.h"
#include "../include/HDownLoad.h"
#include "DownLoadControl.h"

DownLoadControl *g_pDownloadControl = NULL;

extern "C" int HDOWNLOAD_API HDownloaderInit( HWND hWndOwn )
{
	if(g_pDownloadControl)
		return -1;
	g_pDownloadControl = new DownLoadControl();
	HWriteLog(NOTE_DownloadMTR);
	return 1;
}

extern "C" void HDOWNLOAD_API HDownloaderEnd()
{
	if(g_pDownloadControl)
	{
		delete g_pDownloadControl;
		g_pDownloadControl = NULL;
	}
}

extern "C" int HDOWNLOAD_API HDownloaderGetTaskNum()
{
	if(NULL == g_pDownloadControl)
		return -1;
	return g_pDownloadControl->GetTaskNum();
}

extern "C" int HDOWNLOAD_API HDownloaderAddTask( char *pURL, char *pSavePath, char *pSaveFileName, int nThreadNum )
{
	if( NULL == g_pDownloadControl )
		return -1;
	return g_pDownloadControl->AddTask(pURL,pSavePath,pSaveFileName,nThreadNum);
}

extern "C" int HDOWNLOAD_API HDownloaderSelTask( int nIndex )
{
	if( NULL == g_pDownloadControl )
		return -1;
	return g_pDownloadControl->SelTask(nIndex);
}

extern "C" int HDOWNLOAD_API HDownloaderRemoveTask()
{
	if( NULL == g_pDownloadControl )
		return -1;
	return g_pDownloadControl->RemoveTask();
}

extern "C" int HDOWNLOAD_API HDownloaderQueryState( int nIndex,int &nFileSize,int &nDownloadedSize,int &nDownloadedSize_ThisTimes,int &nSpendTime )
{
	if( NULL == g_pDownloadControl )
		return -1;
	return g_pDownloadControl->QueryState(nIndex,nFileSize,nDownloadedSize,nDownloadedSize_ThisTimes,nSpendTime );
}