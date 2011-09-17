#include "StdAfx.h"
#include "../include/HGlobalFunc.h"


DWORD TEMPLATE_CONTROL_API FtoDw( float f )
{
	return (*(DWORD*)&f);
}

float TEMPLATE_CONTROL_API GetRandomFloat( float lowBound, float highBound )
{
	if( lowBound >= highBound )
		return lowBound;
	float f = (rand() % 10000) * 0.0001f; 
	return (f * (highBound - lowBound)) + lowBound; 
}

// 以2为底的数字n的log值.
int	TEMPLATE_CONTROL_API Log2( int n )
{
	for( int i = 1 ; i < 64 ; i++ )
	{
		n = n >> 1;
		if( n == 1 ) return i;
	}
	return 1;
}

// 求得2^n值
int	TEMPLATE_CONTROL_API Pow2( int n )
{
	int val = 1;
	val = val << n;
	return val;
}

// 检查pt是否包含在 rc内.(名为PtInRect()的 API函数不能进行负数(-)值处理.)
BOOL TEMPLATE_CONTROL_API IsInRect( RECT* rc, POINT pt )
{
	if((rc->left <= pt.x) && (pt.x <= rc->right) &&
		(rc->bottom <= pt.y) && (pt.y <= rc->top))
		return TRUE;
	return FALSE;
}

bool TEMPLATE_CONTROL_API IsMachineBigEndian()
{
	unsigned short test = 0x1122;
	unsigned char  *cp = reinterpret_cast<unsigned char*>(&test);
	return (*cp == 0x11);//true  大端  false 小端
}

BOOL TEMPLATE_CONTROL_API IsChineseChar( const char ch )
{
	if(ch & 0x08)
	{
		// ch 为汉字或全角字符
		return TRUE;
	}
	return FALSE;
}

CString TEMPLATE_CONTROL_API GetExeModulePath( HINSTANCE hInstance )
{
	TCHAR szFilePath[MAX_PATH + 1]; 
	AfxGetInstanceHandle();
	GetModuleFileName(hInstance, szFilePath, MAX_PATH); 
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;//删除文件名，只获得路径
	CString str_url = szFilePath;
	return str_url;
}

void TEMPLATE_CONTROL_API SetWndAcrossDesk( HWND hWnd )
{
	UINT intExTemp = GetWindowLong(hWnd,GWL_EXSTYLE);   
	UINT oldGWLEx = SetWindowLong(hWnd,GWL_EXSTYLE,intExTemp|WS_EX_TRANSPARENT|WS_EX_LAYERED);   
	SetLayeredWindowAttributes(hWnd,0,255,LWA_ALPHA);   
}

CRect TEMPLATE_CONTROL_API GetDeskWndRect()
{
	int nX = GetSystemMetrics(SM_CXSCREEN);
	int nY = GetSystemMetrics(SM_CYSCREEN);
	CRect rectDeskWnd;
	rectDeskWnd.SetRect(0,0,nX,nY);
	return rectDeskWnd;
}

CRect TEMPLATE_CONTROL_API GetTrayWndRect()
{
	CRect rectShellTray;
	HWND hWnd = ::FindWindow("Shell_TrayWnd",NULL);   
	::GetWindowRect(hWnd,rectShellTray);
	return rectShellTray;
}

void TEMPLATE_CONTROL_API LoadFontFromFile( LPCSTR lpFilePath )
{
	int nRet = AddFontResource(lpFilePath);
	if (0 == nRet) return;
	::SendMessage(HWND_BROADCAST,WM_FONTCHANGE,0,0);
}

void TEMPLATE_CONTROL_API RemoveLoadFont( LPCSTR lpFilePath )
{
	RemoveFontResource(lpFilePath);
	::SendMessage(HWND_BROADCAST,WM_FONTCHANGE,0,0);
}

BOOL TEMPLATE_CONTROL_API IsSupportMMX()
{
	int nMMX = 0;
	__asm
	{
		mov eax,1				;
		cpuid					;
		test edx,0x00800000		;
		jz __NotSupport__		;
		mov nMMX,1				;
__NotSupport__:
	}
	return nMMX==1?TRUE:FALSE;
}

BOOL TEMPLATE_CONTROL_API IsSupportSSE()
{
	int nSSE = 0;
	__asm
	{
		mov eax,1				;
		cpuid					;
		shr edx,0x1A			;
		jz __NotSupport__		;
		mov nSSE,1				;
__NotSupport__:
	}
	return nSSE==1?TRUE:FALSE;
}

BOOL TEMPLATE_CONTROL_API SetDragShow( BOOL bShow )
{
	BOOL bCurShow;
	SystemParametersInfo(SPI_GETDRAGFULLWINDOWS,NULL,&bCurShow,0);   
	if (bShow)
	{
		if (!bCurShow) 
			return SystemParametersInfo(SPI_SETDRAGFULLWINDOWS,TRUE,NULL,0);   
	}
	else
	{
		if (bCurShow) 
			return SystemParametersInfo(SPI_SETDRAGFULLWINDOWS,FALSE,NULL,0);   
	}
	return TRUE;
}

BOOL TEMPLATE_CONTROL_API SetSmallShow( BOOL bShow )
{
	tagANIMATIONINFO animationInfo;
	::memset(&animationInfo,0,sizeof(tagANIMATIONINFO));
	animationInfo.cbSize = sizeof(tagANIMATIONINFO);
	SystemParametersInfo(SPI_GETANIMATION,sizeof(tagANIMATIONINFO),&animationInfo,0);   

	if (bShow)
	{
		if (!animationInfo.iMinAnimate)
		{
			animationInfo.iMinAnimate = 1;
			return SystemParametersInfo(SPI_SETANIMATION,sizeof(tagANIMATIONINFO),&animationInfo,0);  
		}
	}
	else
	{
		if (animationInfo.iMinAnimate)
		{
			animationInfo.iMinAnimate = 0;
			return SystemParametersInfo(SPI_SETANIMATION,sizeof(tagANIMATIONINFO),&animationInfo,0);  
		}
	}
	return TRUE;
}