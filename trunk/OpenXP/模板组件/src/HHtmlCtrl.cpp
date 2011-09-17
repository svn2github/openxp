#include "stdAfx.h"
#include "../include/HHtmlCtrl.h"
#include <urlmon.h>

WCHAR* ToWChar(char *str)   
{   
	static WCHAR buffer[1024];             
	wcsset(buffer,0);             
	MultiByteToWideChar(CP_ACP,0,str,(int)strlen(str),buffer,1024);             
	return buffer;     
}

void DefaultOpenURL(CString sURL)
{
	HKEY hkRoot,hSubKey; //定义注册表根关键字及子关键字 
	char ValueName[256]; 
	unsigned char DataValue[256]; 
	unsigned long cbValueName=256; 
	unsigned long cbDataValue=256; 
	char ShellChar[256]; //定义命令行 
	DWORD dwType; 

	//打开注册表根关键字 
	if(RegOpenKey(HKEY_CLASSES_ROOT,NULL,&hkRoot)==ERROR_SUCCESS) 
	{ 
		//打开子关键字 
		if(RegOpenKeyEx(hkRoot, 
			"htmlfile\\shell\\open\\command", 
			0, 
			KEY_ALL_ACCESS, 
			&hSubKey)==ERROR_SUCCESS) 
		{ 
			//读取注册表，获取默认浏览器的命令行  
			RegEnumValue(hSubKey,  
				0, 
				ValueName, 
				&cbValueName, 
				NULL, 
				&dwType, 
				DataValue, 
				&cbDataValue); 
			// 调用参数（主页地址）赋值 
			strcpy(ShellChar,(char *)DataValue); 
			strcat(ShellChar,sURL); 
			// 启动浏览器 
			WinExec(ShellChar,SW_SHOW); 

		} 
		else 
		{
			//关闭注册表 
			RegCloseKey(hSubKey); 
			RegCloseKey(hkRoot); 
		}
	}
}

BOOL DialogOpenURL(LPWSTR url)
{
	//装载动态连
	HINSTANCE hinstMSHTML = LoadLibrary("MSHTML.DLL");
	//此地址名称可直接用html文件名代替
	LPWSTR lpUrl=url; 
	if(hinstMSHTML)//装载动态连接库成功
	{
		SHOWHTMLDIALOGFN *pfnShowHTMLDialog;
		pfnShowHTMLDialog = (SHOWHTMLDIALOGFN*) GetProcAddress(hinstMSHTML, "ShowHTMLDialog");
		if(pfnShowHTMLDialog)
		{
			IMoniker *moniker=NULL;
			//
			if( FAILED(CreateURLMoniker( NULL, (LPWSTR)lpUrl, &moniker ) ))
		 {
			 FreeLibrary(hinstMSHTML);
			 return FALSE;
		 }
			//调用ShowHTMLDialog函数显示URL上的HTML文件
			pfnShowHTMLDialog(NULL, moniker, NULL, NULL, NULL);

			if(moniker != NULL)
				moniker->Release();

			//显示成功，返回TRUE
			return TRUE;

		}
		else //GetProcessAddress失败
			return FALSE;

		FreeLibrary(hinstMSHTML);
	}
	else //装载动态连接库失败
		return FALSE;
}

IMPLEMENT_DYNAMIC(HHtmlCtrl, CHtmlView)
BEGIN_MESSAGE_MAP(HHtmlCtrl, CHtmlView)
	ON_WM_MOUSEACTIVATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


BOOL HHtmlCtrl::CreateFromStatic( UINT nID, CWnd* pParent )
{
	CStatic wndStatic;
	if (!wndStatic.SubclassDlgItem(nID, pParent))
		return FALSE;

	CRect rc;
	wndStatic.GetWindowRect(&rc);
	pParent->ScreenToClient(&rc);
	wndStatic.DestroyWindow();

	return Create(NULL,NULL,(WS_CHILD|WS_VISIBLE),rc,pParent,nID,NULL);	                                    
}

void HHtmlCtrl::OpenURL( LPCTSTR lpszURL,UINT nOpenType )
{
	if (nOpenType == (UINT)HTML_OPEN_VIEW)
	{
		Navigate(lpszURL);
	}
	else if (nOpenType == (UINT)HTML_OPEN_DEFAULT)
	{
		DefaultOpenURL(lpszURL);
	}
	else if (nOpenType == (UINT)HTML_OPEN_DIALOG)
	{
		char *pchTemp = const_cast<char*>(lpszURL);
		DialogOpenURL(ToWChar(pchTemp));
	}
}

void HHtmlCtrl::OnDestroy()
{
	CHtmlView::OnDestroy();
}

int HHtmlCtrl::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT msg)
{
	return CHtmlView::OnMouseActivate(pDesktopWnd, nHitTest, msg);
}

HHtmlCtrl::HHtmlCtrl()
{

}

HHtmlCtrl::~HHtmlCtrl()
{

}

void HHtmlCtrl::PostNcDestroy()
{

}