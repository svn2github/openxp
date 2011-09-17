#include "StdAfx.h"
#include "../include/HAPIHook.h"

HHOOK g_hKBHook = NULL;
HHOOK g_hMSHook = NULL;
HAPIHook *g_pHook = NULL;

//键盘钩子处理函数。
extern "C" LRESULT WINAPI KeyboardProc(int nCode,WPARAM wParam,LPARAM lParam)
{
	if(((DWORD)lParam&0x40000000) && (HC_ACTION==nCode))
	{
		(*g_pHook->m_pCallBack)(wParam,lParam);
	}
	return CallNextHookEx(g_hKBHook,nCode,wParam,lParam);
}

//鼠标钩子处理函数。
extern "C" LRESULT WINAPI MouseProc(int nCode,WPARAM wParam,LPARAM lParam)
{
	LPMOUSEHOOKSTRUCT pMouseHook=(MOUSEHOOKSTRUCT FAR *)lParam;
	if (nCode >= 0)
	{
		(*g_pHook->m_pCallBack)(wParam,lParam);
	}
	return CallNextHookEx(g_hKBHook,nCode,wParam,lParam);
}



HAPIHook::HAPIHook()
{
	m_pCallBack = NULL;
	g_pHook = this;
	InitializeCriticalSection(&m_cs);
}

HAPIHook::~HAPIHook()
{
	DeleteCriticalSection(&m_cs);
	if(g_hKBHook)
		StopHook();
	if(g_hMSHook)
		StopHook();
}

//WH_CALLWNDPROC		//窗口钩子，当系统向目标窗口发送消息时将触发此钩子 
//WH_CALLWNDPROCRET     //窗口钩子，当窗口处理完消息后将触发此钩子 
//WH_CBT				//当Windows激活、产生、释放（关闭）、最小化、最大化或改变窗口时都将触发此事件
//WH_DEBUG				//调试钩子 
//WH_GETMESSAGE			//当往消息队列中增加一个消息时将触发此钩子 
//WH_JOURNALPLAYBACK    //回放钩子，可以用于播放已记录的鼠标和键盘的操作 
//WH_JOURNALRECORD		//记录钩子，可以用于记录鼠标和键盘的操作，木马程序可以使用此钩子窃取受控方在屏幕中敲入的密码 
//WH_KEYBOARD			//当敲击键盘时将触发此钩子 
//WH_MOUSE				//当有鼠标操作时将触发此钩子 
//WH_MSGFILTER			//消息过滤钩子 
//WH_SHELL				//Shell钩子 
//WH_SYSMSGFILTER		//系统消息过滤钩子

HHOOK HAPIHook::StartHook(PFCALLBACK pCallBack)
{
	m_pCallBack = pCallBack;
	g_hKBHook = SetWindowsHookEx(WH_KEYBOARD,KeyboardProc,g_hInstance,0);
	g_hMSHook = SetWindowsHookEx(WH_MOUSE,MouseProc,g_hInstance,0);
	return g_hKBHook;
}

BOOL HAPIHook::StopHook()
{
	BOOL bErrorCode = FALSE;
	if (g_hMSHook)
		bErrorCode = UnhookWindowsHookEx(g_hMSHook);
	if(g_hKBHook)
		bErrorCode = UnhookWindowsHookEx(g_hKBHook);
	return bErrorCode;
}

#define MakePtr(cast,ptr,addValue)(cast)((DWORD)(ptr) + (DWORD)(addValue))
void* HAPIHook::InterceptDllMember(HMODULE hModule,char *szDllName,char *szFunctionName,DWORD pNewFunction)
{
	EnterCriticalSection(&m_cs);
	PIMAGE_DOS_HEADER pDosHeader;
	PIMAGE_NT_HEADERS pNTHeader;
	PIMAGE_IMPORT_DESCRIPTOR pImportDesc;
	PIMAGE_THUNK_DATA pThunk;
	DWORD dwOldProtect;
	DWORD dwOldProtect2;
	void *pOldFunction;

	if(!(pOldFunction = GetProcAddress(GetModuleHandle(szDllName),szFunctionName)))
		return 0;

	pDosHeader = (PIMAGE_DOS_HEADER)hModule;
	if(pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
		return(NULL);

	pNTHeader = MakePtr(PIMAGE_NT_HEADERS,pDosHeader,pDosHeader->e_lfanew);
	if(pNTHeader->Signature != IMAGE_NT_SIGNATURE
		|| ( pImportDesc = MakePtr(PIMAGE_IMPORT_DESCRIPTOR,pDosHeader,pNTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress)) == (PIMAGE_IMPORT_DESCRIPTOR )pNTHeader)
		return(NULL);

	while(pImportDesc->Name)
	{
		char *szModuleName = MakePtr(char *,pDosHeader,pImportDesc->Name);
		if(!stricmp(szModuleName,szDllName))
			break;
		pImportDesc++;
	}
	if(pImportDesc->Name == NULL)
		return(NULL);

	pThunk = MakePtr(PIMAGE_THUNK_DATA,pDosHeader,pImportDesc->FirstThunk);
	while(pThunk->u1.Function)
	{
		if(pThunk->u1.Function == (DWORD)pOldFunction)
		{
			VirtualProtect((void*)&pThunk->u1.Function,sizeof(DWORD),PAGE_EXECUTE_READWRITE,&dwOldProtect);
			pThunk->u1.Function = pNewFunction;
			VirtualProtect((void*)&pThunk->u1.Function,sizeof(DWORD),dwOldProtect,&dwOldProtect2 );
			return(pOldFunction);
		}
		pThunk++;
	}
	LeaveCriticalSection(&m_cs);
	return(NULL);
}