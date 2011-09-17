/********************************************************************
created:	2009/11/18
created:	14:06:2009   14:06
filename: 	GlobalFunc.h
author:		XinPing Huang
email:		77967292@qq.com
Qualifier:  全局的常用函数、宏
*********************************************************************/
#ifndef _GLOBALFUNC_H_
#define _GLOBALFUNC_H_
#pragma once

//禁止函数进行内联（inline）
//#pragma auto_inline
#pragma message("-----------------------------------------------")
#pragma message("---------全局常用函数、宏，编译开始！------------")
#pragma message("-----------------------------------------------")

// 用STRICT方式编译所有文件  <让编译器进行严格类型检查，比如表示对Windows应用程序中使用的句柄进行严格的类型检查>
#ifndef STRICT
	#define STRICT
#endif

//计算数组维数
#define SafeCountArray(Array) (sizeof(Array)/sizeof(Array[0]))
//计算字符长度
#define SafeCountString(String) ((UINT)((lstrlen(String)+1)*sizeof(TCHAR)))
//安全删除指针
#define  SafeDelete(pData)	{ try { delete pData; } catch (...) { } pData=NULL; } 
//安全删除指针数组
#define  SafeDeleteArray(pData)	{ try { delete [] pData; } catch (...) { } pData=NULL; } 
//安全new对象
#define SafeNew(pData) { if(pData) { delete pData;} new pData; }
//安全释放对象
#define SafeRelease(p)		{ if(p) { (p)->Release(); (p) = NULL; } }
//安全抛出异常
#define SafeThrowString(what) {\
	char ch[260];       \
	sprintf(ch,"异常：%s\n文件：%s\n函数：%s\n行号：%d",what,__FILE__,__FUNCSIG__,__LINE__);\
	throw(ch);\
}
//安全释放GDI对象
#define SafeReleaseGDI(gdiResource) if(gdiResource.GetSafeHandle()){gdiResource.DeleteObject();}

//宏的连接方式
//#define Safe_OutPutMsg(x) Safe##x              
//#define Safe_OutPutChar(x) #@x
//#define Safe_OutPutString(x) #x

//检测exe文件路径
CString TEMPLATE_CONTROL_API GetExeModulePath(HINSTANCE hInstance);

//将窗口钉入桌面
void TEMPLATE_CONTROL_API SetWndAcrossDesk(HWND hWnd);

//获取桌面属性
CRect TEMPLATE_CONTROL_API GetDeskWndRect();

//获取任务栏属性
CRect TEMPLATE_CONTROL_API GetTrayWndRect();

//测试机器是否是大端
bool TEMPLATE_CONTROL_API IsMachineBigEndian(); 

//判断字符串中的字符是汉字(或全角字符)
BOOL TEMPLATE_CONTROL_API IsChineseChar(const char ch);

//从文件载入字体
void TEMPLATE_CONTROL_API LoadFontFromFile(LPCSTR lpFilePath);

//移除载入的字体
void TEMPLATE_CONTROL_API RemoveLoadFont(LPCSTR lpFilePath);

//float转换到DWORD
DWORD TEMPLATE_CONTROL_API FtoDw(float f);

//得到随机的数字
float TEMPLATE_CONTROL_API GetRandomFloat(float lowBound, float highBound);

// 以2为底的数字n的log值.
int TEMPLATE_CONTROL_API Log2( int n );

// 求得2^n值
int TEMPLATE_CONTROL_API Pow2( int n );

// 检查pt是否包含在 rc内.(名为PtInRect()的 API函数不能进行负数(-)值处理.)
BOOL TEMPLATE_CONTROL_API IsInRect( RECT* rc, POINT pt );

//检查机器是否支持MMX
BOOL TEMPLATE_CONTROL_API IsSupportMMX();

//检测机器是否支持SSE
BOOL TEMPLATE_CONTROL_API IsSupportSSE();

//设置窗体拖拉时显示效果
BOOL TEMPLATE_CONTROL_API SetDragShow(BOOL bShow=TRUE);

//设置窗体最小化时显示动画效果
BOOL TEMPLATE_CONTROL_API SetSmallShow(BOOL bShow=TRUE);

#endif