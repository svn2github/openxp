#pragma once

#ifndef VC_EXTRALEAN
	#define VC_EXTRALEAN    
#endif

#include "targetver.h"
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      

#include <afxwin.h>							// MFC 核心组件和标准组件
#include <afxext.h>							// MFC 扩展

#ifndef _AFX_NO_OLE_SUPPORT
	#include <afxole.h>						// MFC OLE 类
	#include <afxodlgs.h>					// MFC OLE 对话框类
	#include <afxdisp.h>					// MFC 自动化类
#endif 

#ifndef _AFX_NO_DB_SUPPORT
	#include <afxdb.h>                      // MFC ODBC 数据库类
#endif 

#ifndef _AFX_NO_DAO_SUPPORT
	#include <afxdao.h>                     // MFC DAO 数据库类
#endif 

#ifndef _AFX_NO_OLE_SUPPORT
	#include <afxdtctl.h>					// MFC 对 Internet Explorer 4 公共控件的支持
#endif

#ifndef _AFX_NO_AFXCMN_SUPPORT
	#include <afxcmn.h>                     // MFC 对 Windows 公共控件的支持
#endif

#include "../程序调试/DebuggingExportHeader.h"
#ifdef _DEBUG
	#pragma comment(lib,"../链接库/Debugging_d.lib")
#else
	#pragma comment(lib,"../链接库/Debugging_r.lib")
#endif

#pragma warning(disable:4996)

#include "DownLoadExportHeader.h"