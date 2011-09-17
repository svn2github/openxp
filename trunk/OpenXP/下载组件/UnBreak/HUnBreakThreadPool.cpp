#include "StdAfx.h"
#include "HUnBreakThreadPool.h"

HUnBreakThreadPool::HUnBreakThreadPool( DWORD nNum )
	:m_nThreadNum(0),m_nThreadRunning(0)
{
	InitializeCriticalSection(&m_csThreadVector);
	InitializeCriticalSection(&m_csWorkQueue);
	m_hEventComplete = CreateEvent(0, false, false, NULL);
	m_hEventEnd = CreateEvent(0, true, false, NULL);
	m_hSemaphoreCall = CreateSemaphore(0, 0,  0x7FFFFFFF, NULL);
	m_hSemaphoreDel =  CreateSemaphore(0, 0,  0x7FFFFFFF, NULL);
	assert(m_hSemaphoreCall != INVALID_HANDLE_VALUE);
	assert(m_hEventComplete != INVALID_HANDLE_VALUE);
	assert(m_hEventEnd != INVALID_HANDLE_VALUE);
	assert(m_hSemaphoreDel != INVALID_HANDLE_VALUE);
	AdjustSize(nNum<=0?4:nNum);
}

HUnBreakThreadPool::~HUnBreakThreadPool(void)
{
	DeleteCriticalSection(&m_csWorkQueue);
	CloseHandle(m_hEventEnd);
	CloseHandle(m_hEventComplete);
	CloseHandle(m_hSemaphoreCall);
	CloseHandle(m_hSemaphoreDel);
	vector<ThreadItem*>::iterator iter;
	for(iter = m_vecThreadItem.begin(); iter != m_vecThreadItem.end(); iter++)
	{
		if(*iter)
			delete *iter;
	}
	DeleteCriticalSection(&m_csThreadVector);
}

//工作线程
DWORD WINAPI HUnBreakThreadPool::DefaultJobProc( LPVOID lpParameter )
{
	ThreadItem *pThread = static_cast<ThreadItem*>(lpParameter);
	assert(pThread);
	HUnBreakThreadPool *pThreadPoolObj = pThread->pThreadPool;
	assert(pThreadPoolObj);
	InterlockedIncrement(&pThreadPoolObj->m_nThreadNum);
	HANDLE hWaitHandle[3];
	hWaitHandle[0] = pThreadPoolObj->m_hSemaphoreCall;
	hWaitHandle[1] = pThreadPoolObj->m_hSemaphoreDel;
	hWaitHandle[2] = pThreadPoolObj->m_hEventEnd;
	WorkItem *pWork;
	bool bHasWork;
	while(true)
	{
		DWORD nWR = WaitForMultipleObjects(3, hWaitHandle, false, INFINITE);
		//响应删除线程信号
		if(nWR == WAIT_OBJECT_0 + 1)
		{ 
			break;
		}
		//从队列里取得用户作业
		EnterCriticalSection(&pThreadPoolObj->m_csWorkQueue);
		if(bHasWork = !pThreadPoolObj->m_queWorkItem.empty())
		{
			pWork = pThreadPoolObj->m_queWorkItem.front();
			pThreadPoolObj->m_queWorkItem.pop();
			assert(pWork);
		}
		LeaveCriticalSection(&pThreadPoolObj->m_csWorkQueue);
		//受到结束线程信号确定是否结束线程(结束线程信号&& 是否还有工作)
		if(nWR == WAIT_OBJECT_0 + 2 && !bHasWork)
		{ 
			break;
		}
		if(bHasWork && pWork)
		{
			InterlockedIncrement(&pThreadPoolObj->m_nThreadRunning);
			pThread->nLastBeginTime = GetTickCount();
			pThread->nCount++;
			pThread->bIsRunning = true;
			pWork->pFunction(pWork->pParam); //运行用户作业
			delete pWork;
			pThread->bIsRunning = false;
			InterlockedDecrement(&pThreadPoolObj->m_nThreadRunning);
		}
	}
	//删除自身结构
	EnterCriticalSection(&pThreadPoolObj->m_csThreadVector);
	pThreadPoolObj->m_vecThreadItem.erase(find(pThreadPoolObj->m_vecThreadItem.begin(), pThreadPoolObj->m_vecThreadItem.end(), pThread));
	LeaveCriticalSection(&pThreadPoolObj->m_csThreadVector);
	delete pThread;
	InterlockedDecrement(&pThreadPoolObj->m_nThreadNum);
	if(!pThreadPoolObj->m_nThreadNum)
	{  //所有线程结束
		SetEvent(pThreadPoolObj->m_hEventComplete);
	}
	return 0;
}

//调用用户对象虚函数
void HUnBreakThreadPool::CallProc( void *pPara )
{
	CallProcessPara *cp = static_cast<CallProcessPara *>(pPara);
	assert(cp);
	if(cp)
	{
		cp->pObject->DoWork(cp->pParam);
		delete cp;
	}
}

//调整线程池规模
int HUnBreakThreadPool::AdjustSize( int nNum )
{
	if(nNum > 0)
	{
		ThreadItem *pNew;
		EnterCriticalSection(&m_csThreadVector);
		for(int i = 0; i < nNum; i++)
		{
			m_vecThreadItem.push_back(pNew = new ThreadItem(this));
			assert(pNew);
			pNew->hThread = ::CreateThread(NULL, 0, DefaultJobProc, pNew, 0, NULL);
			assert(pNew->hThread);
		}
		LeaveCriticalSection(&m_csThreadVector);
	}
	else
	{
		nNum *= -1;
		ReleaseSemaphore(m_hSemaphoreDel,nNum>m_nThreadNum?m_nThreadNum:nNum,NULL);
	}
	return (int)m_nThreadNum;
}

//调用线程池
void HUnBreakThreadPool::Call( void (*pFunc)(void *), void *pPara )
{
	assert(pFunc);
	EnterCriticalSection(&m_csWorkQueue);
	m_queWorkItem.push(new WorkItem(pFunc, pPara));
	LeaveCriticalSection(&m_csWorkQueue);
	ReleaseSemaphore(m_hSemaphoreCall, 1, NULL);
}

//调用线程池
void HUnBreakThreadPool::Call( IUnBreakThreadWork * p, void *pPara )
{
	Call(CallProc, new CallProcessPara(p, pPara));
}

//结束线程池, 并同步等待
bool HUnBreakThreadPool::EndAndWait( DWORD dwWaitTime )
{
	SetEvent(m_hEventEnd);
	return WaitForSingleObject(m_hEventComplete, dwWaitTime) == WAIT_OBJECT_0;
}