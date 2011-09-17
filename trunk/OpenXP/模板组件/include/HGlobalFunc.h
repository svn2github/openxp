/********************************************************************
created:	2009/11/18
created:	14:06:2009   14:06
filename: 	GlobalFunc.h
author:		XinPing Huang
email:		77967292@qq.com
Qualifier:  ȫ�ֵĳ��ú�������
*********************************************************************/
#ifndef _GLOBALFUNC_H_
#define _GLOBALFUNC_H_
#pragma once

//��ֹ��������������inline��
//#pragma auto_inline
#pragma message("-----------------------------------------------")
#pragma message("---------ȫ�ֳ��ú������꣬���뿪ʼ��------------")
#pragma message("-----------------------------------------------")

// ��STRICT��ʽ���������ļ�  <�ñ����������ϸ����ͼ�飬�����ʾ��WindowsӦ�ó�����ʹ�õľ�������ϸ�����ͼ��>
#ifndef STRICT
	#define STRICT
#endif

//��������ά��
#define SafeCountArray(Array) (sizeof(Array)/sizeof(Array[0]))
//�����ַ�����
#define SafeCountString(String) ((UINT)((lstrlen(String)+1)*sizeof(TCHAR)))
//��ȫɾ��ָ��
#define  SafeDelete(pData)	{ try { delete pData; } catch (...) { } pData=NULL; } 
//��ȫɾ��ָ������
#define  SafeDeleteArray(pData)	{ try { delete [] pData; } catch (...) { } pData=NULL; } 
//��ȫnew����
#define SafeNew(pData) { if(pData) { delete pData;} new pData; }
//��ȫ�ͷŶ���
#define SafeRelease(p)		{ if(p) { (p)->Release(); (p) = NULL; } }
//��ȫ�׳��쳣
#define SafeThrowString(what) {\
	char ch[260];       \
	sprintf(ch,"�쳣��%s\n�ļ���%s\n������%s\n�кţ�%d",what,__FILE__,__FUNCSIG__,__LINE__);\
	throw(ch);\
}
//��ȫ�ͷ�GDI����
#define SafeReleaseGDI(gdiResource) if(gdiResource.GetSafeHandle()){gdiResource.DeleteObject();}

//������ӷ�ʽ
//#define Safe_OutPutMsg(x) Safe##x              
//#define Safe_OutPutChar(x) #@x
//#define Safe_OutPutString(x) #x

//���exe�ļ�·��
CString TEMPLATE_CONTROL_API GetExeModulePath(HINSTANCE hInstance);

//�����ڶ�������
void TEMPLATE_CONTROL_API SetWndAcrossDesk(HWND hWnd);

//��ȡ��������
CRect TEMPLATE_CONTROL_API GetDeskWndRect();

//��ȡ����������
CRect TEMPLATE_CONTROL_API GetTrayWndRect();

//���Ի����Ƿ��Ǵ��
bool TEMPLATE_CONTROL_API IsMachineBigEndian(); 

//�ж��ַ����е��ַ��Ǻ���(��ȫ���ַ�)
BOOL TEMPLATE_CONTROL_API IsChineseChar(const char ch);

//���ļ���������
void TEMPLATE_CONTROL_API LoadFontFromFile(LPCSTR lpFilePath);

//�Ƴ����������
void TEMPLATE_CONTROL_API RemoveLoadFont(LPCSTR lpFilePath);

//floatת����DWORD
DWORD TEMPLATE_CONTROL_API FtoDw(float f);

//�õ����������
float TEMPLATE_CONTROL_API GetRandomFloat(float lowBound, float highBound);

// ��2Ϊ�׵�����n��logֵ.
int TEMPLATE_CONTROL_API Log2( int n );

// ���2^nֵ
int TEMPLATE_CONTROL_API Pow2( int n );

// ���pt�Ƿ������ rc��.(��ΪPtInRect()�� API�������ܽ��и���(-)ֵ����.)
BOOL TEMPLATE_CONTROL_API IsInRect( RECT* rc, POINT pt );

//�������Ƿ�֧��MMX
BOOL TEMPLATE_CONTROL_API IsSupportMMX();

//�������Ƿ�֧��SSE
BOOL TEMPLATE_CONTROL_API IsSupportSSE();

//���ô�������ʱ��ʾЧ��
BOOL TEMPLATE_CONTROL_API SetDragShow(BOOL bShow=TRUE);

//���ô�����С��ʱ��ʾ����Ч��
BOOL TEMPLATE_CONTROL_API SetSmallShow(BOOL bShow=TRUE);

#endif