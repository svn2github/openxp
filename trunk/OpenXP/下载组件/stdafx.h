#pragma once

#ifndef VC_EXTRALEAN
	#define VC_EXTRALEAN    
#endif

#include "targetver.h"
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      

#include <afxwin.h>							// MFC ��������ͱ�׼���
#include <afxext.h>							// MFC ��չ

#ifndef _AFX_NO_OLE_SUPPORT
	#include <afxole.h>						// MFC OLE ��
	#include <afxodlgs.h>					// MFC OLE �Ի�����
	#include <afxdisp.h>					// MFC �Զ�����
#endif 

#ifndef _AFX_NO_DB_SUPPORT
	#include <afxdb.h>                      // MFC ODBC ���ݿ���
#endif 

#ifndef _AFX_NO_DAO_SUPPORT
	#include <afxdao.h>                     // MFC DAO ���ݿ���
#endif 

#ifndef _AFX_NO_OLE_SUPPORT
	#include <afxdtctl.h>					// MFC �� Internet Explorer 4 �����ؼ���֧��
#endif

#ifndef _AFX_NO_AFXCMN_SUPPORT
	#include <afxcmn.h>                     // MFC �� Windows �����ؼ���֧��
#endif

#include "../�������/DebuggingExportHeader.h"
#ifdef _DEBUG
	#pragma comment(lib,"../���ӿ�/Debugging_d.lib")
#else
	#pragma comment(lib,"../���ӿ�/Debugging_r.lib")
#endif

#pragma warning(disable:4996)

#include "DownLoadExportHeader.h"