#include "stdafx.h"
#include <afxwin.h>
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static AFX_EXTENSION_MODULE HookGameDLL = { NULL, NULL };
HINSTANCE g_hInstance = NULL; //DLLÊµÀý¾ä±ú

extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		if (!AfxInitExtensionModule(HookGameDLL, hInstance))
			return 0;
		new CDynLinkLibrary(HookGameDLL);
		if (g_hInstance == NULL)
			g_hInstance = hInstance;
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		AfxTermExtensionModule(HookGameDLL);
	}
	return 1;  
}
