#include "StdAfx.h"
#include "DownLoadPublic.h"
#include "PublicFunc.h"
#include <Shlwapi.h>
#pragma comment(lib,"shlwapi.lib")

// �������ݱ������ʱ�����С
#define TEMP_SAVE_BUFFER_SIZE		(10*NET_BUFFER_SIZE)
// �����ص����ݴﵽ�������ʱ��ű��浽�ļ���
#define WRITE_TEMP_SAVE_MIN_BYTES	(TEMP_SAVE_BUFFER_SIZE - 2*NET_BUFFER_SIZE)
void DownloadNotify ( int nIndex, UINT nNotityType, LPVOID lpNotifyData, LPVOID pDownloadMTR );
// ���Զ��ٴ�
int g_nRetryTimes = 100;

void SetRetryTimes ( int nRetryTimes )
{
	g_nRetryTimes = nRetryTimes;
}

DownLoadPublic::DownLoadPublic(void)
{
	m_hThread					=		NULL;
	m_TimeLastModified			=		-1;
	m_strDownloadUrl			=		_T("");
	m_strSaveFileName			=		_T("");

	m_funcSaveDownloadInfo		=		NULL;
	m_wSaveDownloadInfo_Param	=		NULL;

	m_bSupportResume			=		FALSE;
	m_nFileTotalSize			=		-1;
	m_strReferer				=		_T("");
	m_strUserAgent				=		_T("HuangXinPing-HttpDown/2.0");
	m_strUsername				=		_T("");
	m_strPassword				=		_T("");
	m_strProtocolType			=		"http";
	m_strServer					=		_T("");
	m_strObject					=		_T("");
	m_strFileName				=		_T("");
	m_nPort						=		DEFAULT_HTTP_PORT ;
	m_nIndex					=		-1;
	m_strCookieFlag				=		_T("");
	m_hEvtEndModule				=		::CreateEvent ( NULL, TRUE, FALSE, NULL );
	m_pDownloadMTR				=		NULL;
	ResetVar();
}

DownLoadPublic::~DownLoadPublic(void)
{
	StopDownload();
	ClearThreadHandle();
}

void DownLoadPublic::ResetVar()
{
	ClearThreadHandle();
	m_nWillDownloadStartPos		=		0;
	m_nWillDownloadSize			=		-1;
	m_nDownloadedSize			=		0;
	m_nTempSaveBytes			=		0;
	m_bDownloadSuccess			=		FALSE;
}

void DownLoadPublic::StopDownload ()
{
	m_SocketClient.CancelBlockingCall();
	if (HANDLE_IS_VALID(m_hEvtEndModule))
	{
		::SetEvent(m_hEvtEndModule);
		WaitForThreadEnd(m_hThread,30*1000);
		CLOSE_HANDLE(m_hEvtEndModule);
		m_hEvtEndModule = NULL;
		ClearThreadHandle();
	}
}

//
// ������֤��Ϣ
//
void DownLoadPublic::SetAuthorization ( LPCTSTR lpszUsername, LPCTSTR lpszPassword )
{
	if( lpszUsername != NULL )
	{
		m_strUsername	= GET_SAFE_STRING(lpszUsername);
		m_strPassword	= GET_SAFE_STRING(lpszPassword);
	}
	else
	{
		m_strUsername	= _T("");
		m_strPassword	= _T("");
	}
}

// ����Referer
void DownLoadPublic::SetReferer(LPCTSTR lpszReferer)
{
	if(lpszReferer != NULL)
		m_strReferer = lpszReferer;
	else
		m_strReferer = _T("");
}

// ����UserAgent
void DownLoadPublic::SetUserAgent(LPCTSTR lpszUserAgent)
{
	m_strUserAgent = lpszUserAgent;	
	if(m_strUserAgent.IsEmpty())
		m_strUserAgent = _T("HuangXinPing-HttpDown/2.0");
}

//
// ���ñ���������Ϣ�ص�����
//
void DownLoadPublic::SetSaveDownloadInfoCallback(FUNC_SaveDownloadInfo funcSaveDownloadInfo, WPARAM wParam)
{
	m_funcSaveDownloadInfo = funcSaveDownloadInfo;
	m_wSaveDownloadInfo_Param = wParam;
}

//
// ����������̺߳���
//
DWORD WINAPI ThreadProcDownload(LPVOID lpParameter   // thread data
								 )
{
	DownLoadPublic *pDownloadPub = (DownLoadPublic*)lpParameter;
	ASSERT(pDownloadPub);
	return pDownloadPub->ThreadProcDownload();
}

BOOL DownLoadPublic::ThreadProcDownload()
{
	BOOL bRet = FALSE;
	for (int i = 0; i < g_nRetryTimes; i++)
	{
		if (DownloadOnce())
		{
			bRet = TRUE;
			goto finished;
		}
		SLEEP_RETURN_Down(5*1000);
	}

finished:
	TRACE("(%d) �߳̽��� %s", m_nIndex, bRet?"�ɹ�":"ʧ��");
	HWriteLog("(%d) �߳̽��� %s", m_nIndex, bRet?"�ɹ�":"ʧ��");
	return DownloadEnd(bRet);
}

BOOL DownLoadPublic::DownloadOnce()
{
	// ���ļ�
	if (!OpenFileForSave()) 
		return FALSE;

	// ���ӵ�������
	if (!m_SocketClient.IsConnected())
	{
		if (!Connect())
			return FALSE;
	}
	return TRUE;
}

//
// �����߳������ļ�
//
BOOL DownLoadPublic::Download (int nWillDownloadStartPos,		// Ҫ�����ļ��Ŀ�ʼλ��
							   int nWillDownloadSize,			// ������Ҫ���صĴ�С��-1��ʾһֱ���ص��ļ�β
							   int nDownloadedSize				// �����ص��ֽ�����ָ��ȫд���ļ��е��ֽ���
							 )
{
	if (nWillDownloadSize == 0) return TRUE;
	// �������ز���
	m_nWillDownloadStartPos	= nWillDownloadStartPos;
	SetWillDownloadSize(nWillDownloadSize);
	if (m_nFileTotalSize > 0 && GetWillDownloadSize() > m_nFileTotalSize)
	{
		SetWillDownloadSize(m_nFileTotalSize);
	}
	SetDownloadedSize(nDownloadedSize);

	// ����һ�������߳�
	DWORD dwThreadId = 0;
	m_hThread = CreateThread(NULL,0,::ThreadProcDownload, LPVOID(this), 0, &dwThreadId);
	if (!HANDLE_IS_VALID(m_hThread))
	{
		HWriteLog("(%d) Create download thread failed",m_nIndex);
		return FALSE;
	}
	return TRUE;
}

//
// ���ؽ���
//
BOOL DownLoadPublic::DownloadEnd(BOOL bRes)
{
	m_bDownloadSuccess = bRes;
	m_SocketClient.Disconnect();
	if (HANDLE_IS_VALID(m_file.m_hFile))
	{
		m_file.Close();
	}
	TRACE ("(%d) %s ����һ������\n", m_nIndex, bRes?"�ɹ�":"!!! ʧ�� !!!");
	HWriteLog("(%d) %s ����һ������\n", m_nIndex, bRes?"�ɹ�":"!!! ʧ�� !!!");
	return bRes;
}

BOOL DownLoadPublic::Connect()
{
	if (!HANDLE_IS_VALID(m_hEvtEndModule))
		return FALSE;

	if (m_strServer.IsEmpty())
	{
		HWriteLog("(%d) Please set download URL", m_nIndex);
		return FALSE;
	}
	m_SocketClient.SetEventOfEndModule ( m_hEvtEndModule );
	m_SocketClient.m_nIndex = m_nIndex;
	// ���ӵ�������
	if (!m_SocketClient.Connect(m_strServer, m_nPort))
		return FALSE;
	HWriteLog("(%d) Connect to server [%s:%d] successfully!", m_nIndex, m_strServer, m_nPort);
	return TRUE;
}

BOOL DownLoadPublic::SetDownloadUrl(LPCTSTR lpszDownloadUrl)
{
	if ( !lpszDownloadUrl ) return FALSE;
	m_strDownloadUrl = lpszDownloadUrl;
	// ����Ҫ���ص�URL�Ƿ�Ϊ��
	m_strDownloadUrl.TrimLeft();
	m_strDownloadUrl.TrimRight();
	if(m_strDownloadUrl.IsEmpty())
		return FALSE;
	// ����Ҫ���ص�URL�Ƿ���Ч
	if (!ParseTrueURL(m_strDownloadUrl, m_strServer, m_strObject, m_nPort, m_strProtocolType))
	{
		TRACE(_T("Failed to parse the URL: %s\n"), m_strDownloadUrl);
		HWriteLog(_T("Failed to parse the URL: %s\n"), m_strDownloadUrl);
		return FALSE;
	}
	return TRUE;
}

//
// �ӷ������������ݲ����浽�ļ���
//
BOOL DownLoadPublic::RecvDataAndSaveToFile(HSocketClient &SocketClient,char *pTailData/*=NULL*/, int nTailSize/*=0*/)
{
	int nDownloadedSize = GetDownloadedSize();
	if (pTailData && nTailSize > 0)
	{
		nDownloadedSize = SaveDataToFile(pTailData, nTailSize);
		if (nDownloadedSize < 0)
		{
			return FALSE;
		}
	}

	char szRecvBuf[NET_BUFFER_SIZE] = {0}, *szTempSaveBuf = new char[TEMP_SAVE_BUFFER_SIZE];
	if (!szTempSaveBuf)
	{
		HWriteLog("(%d) Allocate memory failed", m_nIndex);
		return FALSE;
	}
	m_nTempSaveBytes = 0;

	while (TRUE)
	{
		BOOL bDownloadFinished = FALSE;
		int nReadSize = 0;
		int nTempSaveBytes = GetTempSaveBytes ();
		int nRecvTotalBytes = nDownloadedSize+nTempSaveBytes;	// �������ļ��е��ֽ���������ʱ�����е��ֽ��������ܹ����յ��ֽ���
		int nWillDownloadSize = GetWillDownloadSize ();
		if(
			// ���ֽ����жϣ����������Ѿ�����ˡ�
			(nWillDownloadSize > 0 && nRecvTotalBytes >= nWillDownloadSize)
			||
			// ģ������¼����źţ�Ҫ�����ˡ�
			(::WaitForSingleObject(m_hEvtEndModule, 1) == WAIT_OBJECT_0)
			)
		{
			bDownloadFinished = TRUE;
		}
		else
		{
			int nRecvBytesThisTimes = sizeof(szRecvBuf);
			if (nWillDownloadSize > 0)
				nRecvBytesThisTimes = nWillDownloadSize - nRecvTotalBytes;
			ASSERT (nRecvBytesThisTimes >= 0);
			nRecvBytesThisTimes = MIN (nRecvBytesThisTimes, sizeof(szRecvBuf));
			nReadSize = SocketClient.Receive(szRecvBuf, nRecvBytesThisTimes);
			// �����������ˣ�������Ϊ�����Ѿ����
			if (nReadSize <= 0)
			{
				if (nWillDownloadSize <= 0)
				{
					bDownloadFinished = TRUE;
				}
			}
			else
			{
				//				TRACE ( "����.%d, �յ� %d �ֽڣ��ҵ������� %d (0x%08x)�ֽ�\n",
				//					m_nIndex, nReadSize, nWillDownloadSize , nWillDownloadSize );	//w
			}
		}

		// �Ƚ����ݱ��浽��ʱ������
		if (nReadSize > 0)
		{
			nReadSize = MIN(nReadSize, TEMP_SAVE_BUFFER_SIZE-nTempSaveBytes);
			memcpy (szTempSaveBuf+nTempSaveBytes, szRecvBuf, nReadSize);
			nTempSaveBytes += nReadSize;
			ASSERT (nTempSaveBytes < TEMP_SAVE_BUFFER_SIZE);
		}
		// ����������ɻ����յ������ݳ���һ������ʱ�ű��浽�ļ���
		if (bDownloadFinished || nTempSaveBytes >= WRITE_TEMP_SAVE_MIN_BYTES)
		{
			// �����ļ�ʧ�ܣ�����ҲӦ����ֹ
			nDownloadedSize = SaveDataToFile(szTempSaveBuf, nTempSaveBytes);
			if (nDownloadedSize < 0)
			{
				break;
			}
			nTempSaveBytes = 0;
		}
		SetTempSaveBytes(nTempSaveBytes);

		if (bDownloadFinished)
		{
			ASSERT((GetWillDownloadSize () > 0 && nDownloadedSize >= GetWillDownloadSize ()) || (nReadSize <= 0 && GetWillDownloadSize () <= 0) ||
				(::WaitForSingleObject ( m_hEvtEndModule, 1 ) == WAIT_OBJECT_0));
			break;
		}
	}

	if (szTempSaveBuf) delete[] szTempSaveBuf;
	szTempSaveBuf = NULL;

	BOOL bRes = FALSE;
	int nWillDownloadSize = GetWillDownloadSize ();
	if (nWillDownloadSize != -1)
	{
		if (nDownloadedSize >= nWillDownloadSize)
		{
			bRes = TRUE;
		}
	}
	else if (nDownloadedSize > 0)
	{
		bRes = TRUE;
	}
	return bRes;
}

int DownLoadPublic::SaveDataToFile(char *pData, int nSize)
{
	ASSERT ( HANDLE_IS_VALID ( m_file.m_hFile ) );
	if ( !pData || nSize < 0 ) return -1;

	int nDownloadedSize = -1;
	// �������ص�����
	ASSERT (HANDLE_IS_VALID(m_file.m_hFile));
	BOOL bRet = TRUE;
	TRY
	{
		m_file.Write (pData, nSize);
	}
	CATCH(CFileException, e)
	{
		e->Delete ();
		bRet = FALSE;
	}
	END_CATCH

	if (!bRet)
	{
		HWriteLog("(%d) Write data to local file [%s] failed", m_nIndex, m_file.GetFileName());
	}
	else
	{
		nDownloadedSize = GetDownloadedSize();
		//		TRACE ( "����.%d, ���� %d(0x%08x) �ֽڣ�ƫ�� %d(0x%08x) �ֽڣ� �� %d(0x%08x) �ֽ�\n",
		//			m_nIndex, size, size, m_nWillDownloadStartPos+nDownloadedSize, m_nWillDownloadStartPos+nDownloadedSize,
		//			nDownloadedSize + size, nDownloadedSize + size );	//w
		nDownloadedSize += nSize;
		SetDownloadedSize(nDownloadedSize);
		if (m_funcSaveDownloadInfo)
			m_funcSaveDownloadInfo(m_nIndex, nDownloadedSize, nSize, m_wSaveDownloadInfo_Param);
	}
	if (!bRet) return -1;
	return nDownloadedSize;
}

BOOL DownLoadPublic::GetRemoteSiteInfoPro()
{
	return TRUE;
}

BOOL DownLoadPublic::GetRemoteSiteInfo ()
{
	for (int i = 0; i < g_nRetryTimes; i++)
	{
		if (GetRemoteSiteInfoPro())
		{
			m_SocketClient.Disconnect();
			return TRUE;
		}
		SLEEP_RETURN_Down (5*1000);
	}
	m_SocketClient.Disconnect();
	return FALSE;
}

CString DownLoadPublic::GetRemoteFileName()
{
	int nPos = m_strObject.ReverseFind ('/');
	if ( nPos <= 0) return m_strObject;
	return m_strObject.Mid(nPos+1);
}

const char* g_szBase64TAB = _T("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");
// BASE64����
int Base64Encode(LPCTSTR lpszEncoding, CString &strEncoded)
{
	UINT m_nBase64Mask[]= { 0, 1, 3, 7, 15, 31, 63, 127, 255 };
	int nDigit;
	int nNumBits = 6;
	int nIndex = 0;
	int nInputSize;

	strEncoded = _T( "" );
	if(lpszEncoding == NULL)
		return 0;

	if((nInputSize = lstrlen(lpszEncoding)) == 0)
		return 0;

	int nBitsRemaining = 0;
	long lBitsStorage	=0;
	long lScratch		=0;
	int nBits;
	UCHAR c;

	while(nNumBits > 0)
	{
		while((nBitsRemaining < nNumBits) && (nIndex < nInputSize)) 
		{
			c = lpszEncoding[nIndex++];
			lBitsStorage <<= 8;
			lBitsStorage |= (c&0xff);
			nBitsRemaining += 8;
		}
		if(nBitsRemaining < nNumBits) 
		{
			lScratch = lBitsStorage << (nNumBits - nBitsRemaining);
			nBits    = nBitsRemaining;
			nBitsRemaining = 0;
		}	 
		else 
		{
			lScratch = lBitsStorage >> (nBitsRemaining - nNumBits);
			nBits	 = nNumBits;
			nBitsRemaining -= nNumBits;
		}
		nDigit = (int)(lScratch & m_nBase64Mask[nNumBits]);
		nNumBits = nBits;
		if( nNumBits <=0 )
			break;

		strEncoded += g_szBase64TAB[nDigit];
	}
	// Pad with '=' as per RFC 1521
	while(strEncoded.GetLength() % 4 != 0)
		strEncoded += '=';

	return strEncoded.GetLength();

}

// STATIC BASE64����
int Base64Decode(LPCTSTR lpszDecoding, CString &strDecoded)
{
	int nIndex =0;
	int nDigit;
	int nDecode[ 256 ];
	int nSize;
	int nNumBits = 6;
	int i;

	if(lpszDecoding == NULL)
		return 0;

	if((nSize = lstrlen(lpszDecoding)) == 0)
		return 0;

	// Build Decode Table
	for(i = 0; i < 256; i++) 
		nDecode[i] = -2; // Illegal digit
	for(i=0; i < 64; i++)
	{
		nDecode[g_szBase64TAB[i]] = i;
		nDecode['='] = -1; 
	}

	// Clear the output buffer
	strDecoded = _T("");
	long lBitsStorage  =0;
	int nBitsRemaining = 0;
	int nScratch = 0;
	UCHAR c;

	// Decode the Input
	for(nIndex = 0, i = 0; nIndex < nSize; nIndex++)
	{
		c = lpszDecoding[nIndex];

		// �������в��Ϸ����ַ�
		if( c> 0x7F)
			continue;

		nDigit = nDecode[c];
		if(nDigit >= 0) 
		{
			lBitsStorage = (lBitsStorage << nNumBits) | (nDigit & 0x3F);
			nBitsRemaining += nNumBits;
			while(nBitsRemaining > 7) 
			{
				nScratch = lBitsStorage >> (nBitsRemaining - 8);
				strDecoded += (wchar_t)(nScratch & 0xFF);
				i++;
				nBitsRemaining -= 8;
			}
		}
	}	

	return strDecoded.GetLength();
}

//
// ��URL�����ֳ�Server��Object��Э�����͵���Ϣ������ Object ���ֵ�����ִ�Сд�ģ�������Щ��վ���ܻ����ز���
//
BOOL ParseTrueURL(LPCTSTR lpszURL, CString &strServer, CString &strObject,USHORT& nPort, CString &strProtocolType)
{
	if (!lpszURL || strlen(lpszURL) < 1) return FALSE;
	CString strURL_Lower(lpszURL);
	strURL_Lower.TrimLeft();
	strURL_Lower.TrimRight();
	strURL_Lower.Replace("\\","/");
	CString strURL = strURL_Lower;
	strURL_Lower.MakeLower();

	// �������
	strServer = _T("");
	strObject = _T("");
	nPort	  = 0;

	int nPos = strURL_Lower.Find("://");
	if(nPos == -1)
	{
		strURL_Lower.Insert(0, "http://");
		strURL.Insert(0,"http://");
		nPos = 4;
	}
	strProtocolType = strURL_Lower.Left(nPos);

	strURL_Lower = strURL_Lower.Mid(strProtocolType.GetLength()+3);
	strURL = strURL.Mid(strProtocolType.GetLength()+3);
	nPos = strURL_Lower.Find('/');
	if (nPos == -1)
		return FALSE;

	strObject = strURL.Mid(nPos);
	CString strServerAndPort = strURL_Lower.Left(nPos);

	// �����Ƿ��ж˿ںţ�վ�����������һ����Сд
	nPos = strServerAndPort.Find(":");
	if(nPos == -1)
	{
		strServer = strServerAndPort;
		nPort	  = DEFAULT_HTTP_PORT;
		if (strProtocolType == "ftp")
			nPort = DEFAULT_FTP_PORT;
	}
	else
	{
		strServer = strServerAndPort.Left(nPos);
		CString str = strServerAndPort.Mid(nPos+1);
		nPort = (USHORT)_ttoi(str.GetBuffer(str.GetLength()));
		str.ReleaseBuffer();
	}
	return TRUE;
}

void DownLoadPublic::SetDownloadedSize(int nDownloadedSize)
{
	m_csDownloadedSize.Lock();
	m_nDownloadedSize = nDownloadedSize;
	m_csDownloadedSize.Unlock();
	DownloadNotify(m_nIndex,NOTIFY_TYPE_THREAD_DOWNLOADED_SIZE,(LPVOID)nDownloadedSize, m_pDownloadMTR);
}

int DownLoadPublic::GetDownloadedSize()
{
	int nDownloadedSize = 0;
	m_csDownloadedSize.Lock ();
	nDownloadedSize = m_nDownloadedSize;
	m_csDownloadedSize.Unlock ();

	return nDownloadedSize;
}

void DownLoadPublic::SetTempSaveBytes(int nTempSaveBytes)
{
	m_csTempSaveBytes.Lock ();
	m_nTempSaveBytes = nTempSaveBytes;
	m_csTempSaveBytes.Unlock ();
}

int DownLoadPublic::GetTempSaveBytes()
{
	int nTempSaveBytes = 0;
	m_csTempSaveBytes.Lock ();
	nTempSaveBytes = m_nTempSaveBytes;
	m_csTempSaveBytes.Unlock ();

	return nTempSaveBytes;
}

void DownLoadPublic::SetWillDownloadSize(int nWillDownloadSize)
{
	m_csWillDownloadSize.Lock();
	m_nWillDownloadSize = nWillDownloadSize;
	m_csWillDownloadSize.Unlock();
	DownloadNotify(m_nIndex, NOTIFY_TYPE_WILL_DOWNLOAD_SIZE, (LPVOID)m_nWillDownloadSize, m_pDownloadMTR);
}

int DownLoadPublic::GetWillDownloadSize()
{
	int nWillDownloadSize = 0;
	m_csWillDownloadSize.Lock ();
	nWillDownloadSize = m_nWillDownloadSize;
	m_csWillDownloadSize.Unlock ();
	return nWillDownloadSize;
}

BOOL DownLoadPublic::SetSaveFileName(LPCTSTR lpszSaveFileName)
{
	if (!lpszSaveFileName) return FALSE;
	m_strSaveFileName = lpszSaveFileName;
	if (m_strSaveFileName.IsEmpty())
		return FALSE;
	return TRUE;
}

//
// ��ȡ��δ���ص��ֽ�����д���ļ��еĺ���ʱ������Ķ������Ѿ����ص�
//
int DownLoadPublic::GetUndownloadBytes()
{
	// �ܹ���Ҫ���ص��ֽ�����ȥ�Ѿ����ص��ֽ���
	return GetWillDownloadSize()-(GetDownloadedSize()+GetTempSaveBytes());
}

BOOL DownLoadPublic::OpenFileForSave()
{
	ASSERT (!m_strSaveFileName.IsEmpty());
	// �򿪱����ļ�
	if (HANDLE_IS_VALID(m_file.m_hFile))
	{
		m_file.Close();
	}
	BOOL bRet = FALSE;
	TRY
	{
		if (m_file.Open(m_strSaveFileName, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite|CFile::typeBinary|CFile::shareDenyNone))
		{
			int nWillDownloadStartPos = GetWillDownloadStartPos();
			nWillDownloadStartPos += GetDownloadedSize();
			if (m_file.Seek (nWillDownloadStartPos, CFile::begin) == nWillDownloadStartPos)
				bRet = TRUE;
		}
	}
	CATCH(CFileException, e)
	{
		e->Delete();
		bRet = FALSE;
	}
	END_CATCH

	if ( !bRet )
	{
		HWriteLog("(%d) Open file [%s] failed. %s", m_nIndex, m_strSaveFileName, ::ErrorCodeFormatMessage(GetLastError()));
	}
	return bRet;
}

void DownLoadPublic::ClearThreadHandle()
{
	CLOSE_HANDLE(m_hThread);
}

// ��Ϣ֪ͨ�Ļص�����
FUNC_DownloadNotify g_funcDownloadNotify = NULL;
WPARAM g_wDownloadNotify_Param = NULL;
//
// ����֪ͨ�ص�����
//
void SetDownloadNotifyCallback( FUNC_DownloadNotify Proc_DownloadNotify, WPARAM wParam )
{
	g_funcDownloadNotify = Proc_DownloadNotify;
	g_wDownloadNotify_Param = wParam;
}

void DownloadNotify( int nIndex, UINT nNotityType, LPVOID lpNotifyData, LPVOID pDownloadMTR )
{
	ASSERT (pDownloadMTR);
	DownloadNotifyPara DownloadNotifyPar = {0};
	DownloadNotifyPar.nIndex = nIndex;
	DownloadNotifyPar.nNotityType = nNotityType;
	DownloadNotifyPar.lpNotifyData = lpNotifyData;
	DownloadNotifyPar.pDownloadMTR = pDownloadMTR;

	if (g_funcDownloadNotify)
		g_funcDownloadNotify(&DownloadNotifyPar, g_wDownloadNotify_Param);
}

//
// ��ȡ���ض�����ļ���������չ���ģ�
//
CString DownLoadPublic::GetDownloadObjectFileName( CString *pstrExtensionName )
{
	ASSERT (!m_strObject.IsEmpty());
	CString strOnlyPath, strOnlyFileName, strExtensionName;
	if (!PartPathAndFileAndExtensionName(m_strObject, &strOnlyPath, &strOnlyFileName, &strExtensionName ))
		return "";
	if (pstrExtensionName) *pstrExtensionName = strExtensionName;
	if (!strExtensionName.IsEmpty())
	{
		strOnlyFileName += ".";
		strOnlyFileName += strExtensionName;
	}
	return strOnlyFileName;
}

CString DownLoadPublic::GetRefererFromURL()
{
	int nPos = m_strDownloadUrl.ReverseFind ('/');
	if (nPos < 0) return "";
	return m_strDownloadUrl.Left(nPos);
}

BOOL DownLoadPublic::ThreadIsRunning()
{
	if (!HANDLE_IS_VALID(m_hThread))
		return FALSE;
	return (WaitForSingleObject(m_hThread, 0) != WAIT_OBJECT_0);
}
