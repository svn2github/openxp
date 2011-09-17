#ifndef __PUBLICFUNC__H__
#define __PUBLICFUNC__H__
#pragma once

#define SLEEP_RETURN(x)\
{\
	if ( ::WaitForSingleObject ( m_hEvtEndModule, x ) == WAIT_OBJECT_0 )\
	return FALSE;\
}

#define MIN(x,y) (((DWORD)(x)<(DWORD)(y))?(x):(y))
#define MAX(x,y) (((DWORD)(x)>(DWORD)(y))?(x):(y))
#define GET_SAFE_STRING(str) ((str)?(str):"")
// 有效的句柄
#define HANDLE_IS_VALID(h) ((HANDLE)(h) && ((HANDLE)(h)!=INVALID_HANDLE_VALUE))
// 关闭句柄
#define CLOSE_HANDLE(h)\
{\
	if ( HANDLE_IS_VALID(h) )\
	{\
	::CloseHandle ( h );\
	(h) = NULL;\
	}\
}
#define ASSERT_ADDRESS(p,size) ASSERT((p)!=NULL && AfxIsValidAddress((p),(size),TRUE))
#define STRLEN_SZ(sz) ((sz)?strlen((char*)(sz)):0)
#define STRNCPY_CS(sz,cs) strncpy((char*)(sz),(cs).GetBuffer((cs).GetLength()),sizeof(sz))
#define STRNCPY(sz1,sz2,size) \
{\
	strncpy(((char*)(sz1)),(sz2)?((char*)(sz2)):"",(size));\
	((char*)(sz1))[(size)-1] = '\0';\
}

char *hxpStrChr(const char *str, int c);
char *hxpStrrChr(const char *str, int c);
CString GetOneLine(CString &strIn);
CString ErrorCodeFormatMessage(DWORD dwErrorCode);
void ReplaceChineseStrToEnglish(char *szBuf, char cReplace);
CString StandardizationFileForPathName(LPCTSTR lpszFileOrPathName, BOOL bIsFileName, char cReplaceChar='_');
void StandardizationPathBuffer(char *szPath, int nSize, char cFlagChar='\\');
int GetMouthByShortStr(LPCTSTR lpszShortMonth);
int CalcCharCount(LPCTSTR lpszText, char chCalc);
int ConvertStrToCTime(char *pchTime,CTime &cTime);
BOOL CopyBuffer_Date(int *data, SYSTEMTIME &SysTime);
BOOL CopyBuffer_Time(int *pData, SYSTEMTIME &SysTime);
void MytoLower(char *pSrc); 
BOOL CreateNullFile(LPCTSTR lpszFileName, int nFileSize);
BOOL PartPathAndFileAndExtensionName (
									  IN LPCTSTR lpszFilePath,			// 全路径名(包含文件名)
									  OUT CString *pcsOnlyPath,			// 输出光路径（没有文件名）
									  OUT CString *pcsOnlyFileName,		// 输出光文件名（没有路径）
									  OUT CString *pcsExtensionName		// 输出扩展名
									  );
BOOL PartFileAndPathByFullPath (
								LPCTSTR lpszFilePath,
								char *szOnlyFileName,
								int nFileNameSize,
								char *szOnlyPath =NULL,
								int nPathSize=0
								);
BOOL PartFileAndExtensionName (
							   IN LPCTSTR lpszFileName,
							   OUT char *szFileName,
							   IN int nFileNameSize,
							   OUT char *szExtensionName=NULL,
							   IN int nExtensionNameSize=0 );
BOOL WaitForThreadEnd(HANDLE hThread, DWORD dwWaitTime=5000);


#endif