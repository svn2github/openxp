#ifndef __ITHREADPOOL__H__
#define __ITHREADPOOL__H__
#pragma once
#include <cassert>
#include <vector>
#include <queue>
#include <windows.h>
#pragma warning(disable: 4530)
#pragma warning(disable: 4786)
using namespace std;

class IUnBreakThreadWork //工作基类
{
public:
	virtual void DoWork(void *pPara=NULL) = 0;//供线程池调用的虚函数
};

class HUnBreakThreadPool
{
public:
	HUnBreakThreadPool(DWORD nNum=4);//nNum线程池规模
	~HUnBreakThreadPool(void);
	int AdjustSize(int nNum);//调整线程池规模
	void Call(void (*pFunc)(void *), void *pPara = NULL);//调用线程池
	void Call(IUnBreakThreadWork *p, void *pPara = NULL);//调用线程池
	bool EndAndWait(DWORD dwWaitTime = INFINITE);//结束线程池, 并同步等待
	inline void End()
	{//结束线程池
		SetEvent(m_hEventEnd);
	}
	inline DWORD GetSize() const
	{//得到线程数
		return (DWORD)m_nThreadNum;
	}
	inline DWORD GetRunningSize() const
	{//得到正在运行的线程
		return (DWORD)m_nThreadRunning;
	}
	inline bool IsRunning()
	{//是否正在运行
		return m_nThreadNum>0;
	}

protected:
	static DWORD WINAPI DefaultJobProc(LPVOID lpParameter = NULL);//工作线程
	static void CallProc(void *pPara);//调用用户对象虚函数

private:
	typedef struct tagCallProcessPara //用户对象结构
	{
	public:
		tagCallProcessPara(IUnBreakThreadWork *p, void *pPara)
		{
			pObject = p;
			pParam = pPara;
		}
	public:
		IUnBreakThreadWork *pObject;//用户对象
		void *pParam;//用户参数
	}CallProcessPara,*pCallProcessPara;

	typedef struct tagWorkItem//用户工作函数结构
	{
	public:
		tagWorkItem(void (*pFunc)(void *) = NULL, void *pPara = NULL)
		{
			pFunction = pFunc;
			pParam = pPara;
		}
	public:
		void (*pFunction)(void  *);//函数
		void *pParam; //参数
	}WorkItem,*pWorkItem;

	typedef struct tagThreadItem//线程池中的线程结构
	{
	public:
		tagThreadItem(HUnBreakThreadPool *p)
		{
			pThreadPool = p;
			hThread = NULL;
			nLastBeginTime = 0;
			nCount = 0;
			bIsRunning = false;
		} 
		~tagThreadItem()
		{
			if(hThread)
			{
				CloseHandle(hThread);
				hThread = NULL;
			}
		}
	public:
		HANDLE hThread; //线程句柄
		HUnBreakThreadPool *pThreadPool;//线程池的指针
		DWORD nLastBeginTime;//最后一次运行开始时间
		DWORD nCount; //运行次数
		bool bIsRunning;//是否正在运行
	}ThreadItem,*pThreadItem;

protected:
	volatile long m_nThreadNum;//线程数量
	volatile long m_nThreadRunning;//运行的线程数
	queue<pWorkItem> m_queWorkItem; //工作队列
	vector<pThreadItem>  m_vecThreadItem; //线程数据
	CRITICAL_SECTION m_csThreadVector;//工作队列临界
	CRITICAL_SECTION m_csWorkQueue;//线程数据临界
	HANDLE m_hEventEnd;//结束通知
	HANDLE m_hEventComplete;//完成事件
	HANDLE m_hSemaphoreCall;//工作信号
	HANDLE m_hSemaphoreDel;//删除线程信号
};

#endif