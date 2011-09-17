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
	HKEY hkRoot,hSubKey; //����ע�����ؼ��ּ��ӹؼ��� 
	char ValueName[256]; 
	unsigned char DataValue[256]; 
	unsigned long cbValueName=256; 
	unsigned long cbDataValue=256; 
	char ShellChar[256]; //���������� 
	DWORD dwType; 

	//��ע�����ؼ��� 
	if(RegOpenKey(HKEY_CLASSES_ROOT,NULL,&hkRoot)==ERROR_SUCCESS) 
	{ 
		//���ӹؼ��� 
		if(RegOpenKeyEx(hkRoot, 
			"htmlfile\\shell\\open\\command", 
			0, 
			KEY_ALL_ACCESS, 
			&hSubKey)==ERROR_SUCCESS) 
		{ 
			//��ȡע�����ȡĬ���������������  
			RegEnumValue(hSubKey,  
				0, 
				ValueName, 
				&cbValueName, 
				NULL, 
				&dwType, 
				DataValue, 
				&cbDataValue); 
			// ���ò�������ҳ��ַ����ֵ 
			strcpy(ShellChar,(char *)DataValue); 
			strcat(ShellChar,sURL); 
			// ��������� 
			WinExec(ShellChar,SW_SHOW); 

		} 
		else 
		{
			//�ر�ע��� 
			RegCloseKey(hSubKey); 
			RegCloseKey(hkRoot); 
		}
	}
}

BOOL DialogOpenURL(LPWSTR url)
{
	//װ�ض�̬��
	HINSTANCE hinstMSHTML = LoadLibrary("MSHTML.DLL");
	//�˵�ַ���ƿ�ֱ����html�ļ�������
	LPWSTR lpUrl=url; 
	if(hinstMSHTML)//װ�ض�̬���ӿ�ɹ�
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
			//����ShowHTMLDialog������ʾURL�ϵ�HTML�ļ�
			pfnShowHTMLDialog(NULL, moniker, NULL, NULL, NULL);

			if(moniker != NULL)
				moniker->Release();

			//��ʾ�ɹ�������TRUE
			return TRUE;

		}
		else //GetProcessAddressʧ��
			return FALSE;

		FreeLibrary(hinstMSHTML);
	}
	else //װ�ض�̬���ӿ�ʧ��
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