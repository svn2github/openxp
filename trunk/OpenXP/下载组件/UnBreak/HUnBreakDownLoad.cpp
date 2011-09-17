#include "StdAfx.h"
#include "../include/HUnBreakDownLoad.h"
#include "HUnBreakThreadWork.h"
#include <vector>

class HUnBreakDownLoader
{
public:
	HUnBreakDownLoader()
	{
		g_pUnBreakThreadPool = NULL;
		g_pUnBreakThreadWork = NULL;
	}
	~HUnBreakDownLoader()
	{
		for (size_t i = 0; i < g_vecMisson.size(); ++i)
		{
			pUnBreakMisson p = NULL;
			p = g_vecMisson[i];
			if (p) delete p;
		}
		if (g_pUnBreakThreadWork)
			delete g_pUnBreakThreadWork;
		if (g_pUnBreakThreadPool)
			delete g_pUnBreakThreadPool;
	}
	int DownLoadInit()
	{
		if (g_pUnBreakThreadPool || g_pUnBreakThreadWork)
			return -1;
		g_pUnBreakThreadPool = new HUnBreakThreadPool();
		g_pUnBreakThreadWork = new HUnBreakThreadWork();
		g_vecMisson.clear();
		return 0;
	}
#ifdef _CONSOLE
	int AddTask(const char *pURL,const char *pSaveDiskPath,const char *pSaveFileName,const long lMissonID)
	{
		if (NULL == g_pUnBreakThreadPool)
			return -1;
		pUnBreakMisson  pMisson = new tagUnBreakMisson();
		sprintf(pMisson->chURL,"%s",pURL);
		sprintf(pMisson->chFileName,"%s//%s",pSaveDiskPath,pSaveFileName);
		::DeleteFile(pMisson->chFileName);
		pMisson->nMissonID = lMissonID;
		g_pUnBreakThreadPool->Call(g_pUnBreakThreadWork,pMisson);
		g_vecMisson.push_back(pMisson);
		return 0;
	}
#else
	int AddTask(const char *pURL,const char *pSaveDiskPath,const char *pSaveFileName,CWnd *pMsgWnd,const long lMissonID)
	{
		if (NULL == g_pUnBreakThreadPool)
			return -1;
		pUnBreakMisson  pMisson = new tagUnBreakMisson();
		sprintf(pMisson->chURL,"%s",pURL);
		sprintf(pMisson->chFileName,"%s//%s",pSaveDiskPath,pSaveFileName);
		::DeleteFile(pMisson->chFileName);
		pMisson->nMissonID = lMissonID;
		pMisson->pMsgWnd = pMsgWnd;
		g_pUnBreakThreadPool->Call(g_pUnBreakThreadWork,pMisson);
		g_vecMisson.push_back(pMisson);
		return 0;
	}
#endif
	int End()
	{	
		if (NULL == g_pUnBreakThreadPool || NULL == g_pUnBreakThreadWork)
			return -1;
		g_pUnBreakThreadPool->End();
		return 0;
	}
private:
	HUnBreakThreadPool *g_pUnBreakThreadPool;
	HUnBreakThreadWork *g_pUnBreakThreadWork;
	std::vector<UnBreakMisson*> g_vecMisson;
};

HUnBreakDownLoader g_UnBreakDownLoad;
int HUnBreakDownLoaderInit()
{
	return g_UnBreakDownLoad.DownLoadInit();
}

#ifdef _CONSOLE
int HUnBreakDownLoaderAddTask( const char *pURL, const char *pSaveDiskPath, const char *pSaveFileName, const long lMissonID )
{
	HUnBreakDownLoaderInit();
	return g_UnBreakDownLoad.AddTask(pURL,pSaveDiskPath,pSaveFileName,lMissonID);
}	
#else
int HUnBreakDownLoaderAddTask( const char *pURL, const char *pSaveDiskPath, const char *pSaveFileName, CWnd *pMsgWnd, const long lMissonID )
{
	HUnBreakDownLoaderInit();
	return g_UnBreakDownLoad.AddTask(pURL,pSaveDiskPath,pSaveFileName,pMsgWnd,lMissonID);
}
#endif

int HUnBreakDownLoaderEnd()
{
	return g_UnBreakDownLoad.End();
}