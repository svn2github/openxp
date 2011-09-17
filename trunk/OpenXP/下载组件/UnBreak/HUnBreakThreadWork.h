#ifndef __THREADWORK__H__
#define __THREADWORK__H__
#pragma once
#include "HUnBreakThreadPool.h"
#include <Urlmon.h>
#include <Wininet.h>
#pragma comment(lib,"Urlmon.lib")
#pragma comment(lib,"Wininet.lib")
#pragma warning(disable:4996)

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

#ifdef _CONSOLE
#else
	#ifndef WM_UNBREAKNPTIFYPROCESS
	#define WM_UNBREAKNPTIFYPROCESS  WM_USER+1999
	#endif
#endif

typedef struct tagUnBreakMisson
{
public:
	tagUnBreakMisson()
	{
		nMissonID = 0;
		memset(chURL,0,sizeof(chURL));
		memset(chFileName,0,sizeof(chFileName));
#ifdef _CONSOLE
#else
		pMsgWnd = NULL;
#endif
	}
public:
	long nMissonID; 
	char chURL[MAX_PATH];
	char chFileName[MAX_PATH];
#ifdef _CONSOLE
#else
	CWnd *pMsgWnd;  
#endif
}UnBreakMisson,*pUnBreakMisson;

//实现IBindStatusCallback接口，以便处理进度条
class CBindCBHttpCallback : public IBindStatusCallback
{
public:
	CBindCBHttpCallback()
	{
#ifdef _CONSOLE
#else
		m_pMsgWnd=NULL;
#endif
	}

	~CBindCBHttpCallback(){} 

	STDMETHOD(OnStartBinding)(DWORD dwReserved,IBinding __RPC_FAR *pib)
	{ return E_NOTIMPL; }

	STDMETHOD(GetPriority)(LONG __RPC_FAR *pnPriority)
	{ return E_NOTIMPL; }

	STDMETHOD(OnLowResource)(DWORD reserved)
	{ return E_NOTIMPL; }

	//描述任务进度
	STDMETHOD(OnProgress)(ULONG ulProgress,ULONG ulProgressMax,ULONG ulStatusCode,LPCWSTR wszStatusText)
	{ 
#ifdef _CONSOLE
		float f = (float)(100.0*ulProgress/ulProgressMax);
		printf("process:%.2f%%\n",f>=0.0f?f:0.0f);
#else
		ASSERT(m_pMsgWnd!=NULL);
		m_pMsgWnd->PostMessage(WM_UNBREAKNPTIFYPROCESS,(WPARAM)ulProgressMax,(LPARAM)ulProgress);
#endif
		return S_OK; 
	}

	STDMETHOD(OnStopBinding)(HRESULT hresult,LPCWSTR szError)
	{ return E_NOTIMPL; }

	STDMETHOD(GetBindInfo)(DWORD __RPC_FAR *grfBINDF,BINDINFO __RPC_FAR *pbindinfo)
	{ return E_NOTIMPL; }

	STDMETHOD(OnDataAvailable)(DWORD grfBSCF,DWORD dwSize,FORMATETC __RPC_FAR *pformatetc,STGMEDIUM __RPC_FAR *pstgmed)
	{ return E_NOTIMPL; }

	STDMETHOD(OnObjectAvailable)(REFIID riid,IUnknown __RPC_FAR *punk)
	{ return E_NOTIMPL; }

	STDMETHOD_(ULONG,AddRef)()
	{ return 0; }

	STDMETHOD_(ULONG,Release)()
	{ return 0; }

	STDMETHOD(QueryInterface)(REFIID riid,void __RPC_FAR *__RPC_FAR *ppvObject)
	{ return E_NOTIMPL; }

#ifdef _CONSOLE
#else
public:
	CWnd   *m_pMsgWnd;
#endif
};

//工作线程
class HUnBreakThreadWork : public IUnBreakThreadWork
{
public:
	virtual void DoWork(void *p)//自定义的虚函数
	{
		tagUnBreakMisson *pMi = (tagUnBreakMisson*)p;
		DeleteUrlCacheEntry(pMi->chURL);//先清理缓存地址 
		CBindCBHttpCallback  callBackProcess;
#ifdef _CONSOLE
#else
		callBackProcess.m_pMsgWnd = pMi->pMsgWnd;
#endif
		HRESULT hrRet = URLDownloadToFile(0,pMi->chURL,pMi->chFileName,0,&callBackProcess);
		if (hrRet == S_OK)
		{
#ifdef _CONSOLE
			char chBuf[260];
			sprintf(chBuf,"misson %d finish\n",pMi->nMissonID);
			printf(chBuf);
#else
			callBackProcess.m_pMsgWnd->PostMessage(WM_UNBREAKNPTIFYPROCESS,(WPARAM)pMi->nMissonID,(LPARAM)10000); 
#endif			
		}
		else
		{
#ifdef _CONSOLE
			char chBuf[260];
			sprintf(chBuf,"misson %d finish\n",pMi->nMissonID);
			printf(chBuf);
#else
			callBackProcess.m_pMsgWnd->PostMessage(WM_UNBREAKNPTIFYPROCESS,(WPARAM)pMi->nMissonID,(LPARAM)-10000); 
#endif	
		}
	}
};

#endif