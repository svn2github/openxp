#include "StdAfx.h"
#include "HSocketClient.h"
#include "PublicFunc.h"
#pragma comment(lib, "wSock32.lib")

#define		_afxSockThreadState			AfxGetModuleThreadState()
#define		_AFX_SOCK_THREAD_STATE		AFX_MODULE_THREAD_STATE
#define		WM_SOCKET_NOTIFY			0x0373   
#define		WM_SOCKET_DEAD				0x0374 

HSocketClient::HSocketClient() : m_hEvtEndModule(NULL),m_bConnected(FALSE),m_nIndex(-1)
{
}

HSocketClient::~HSocketClient()
{
}

//
#if 0
BEGIN_MESSAGE_MAP(HSocketClient, CSocket)
	//{{AFX_MSG_MAP(HSocketClient)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

CString HSocketClient::GetDigitStrAtHead ( LPCTSTR lpszStr )
{
	if (!lpszStr) return "";
	CString csStr = lpszStr;
	csStr.TrimLeft();
	csStr.TrimRight();
	CString csDigitStr;
	for (int i = 0; isdigit((int)csStr[i]); i++)
	{
		csDigitStr += csStr[i];
	}
	return csDigitStr;
}

//
// return : ------------------------------------------
//		> 0		-	回应代码
//		= 0		-	未读到数据
//		= -1	-	网络出错
//s
int HSocketClient::GetResponse ( CString *pstrResponseStr, BOOL bBlock )
{
	if (pstrResponseStr) *pstrResponseStr = "";
	ASSERT (m_hSocket != INVALID_SOCKET);
	CString strOneLine = GetOneLine(m_strResponseHistoryString);
	m_strResponseHistoryString.ReleaseBuffer();
	if (strOneLine.IsEmpty())
	{
		char szRecvBuf[NET_BUFFER_SIZE] = {0};
		int nRet = Receive(szRecvBuf,sizeof(szRecvBuf),bBlock);
		if (nRet <= 0)
		{
			if (nRet < 0)
				HWriteLog("(%d) Receive response data failed",m_nIndex);
			return nRet;
		}
		m_strResponseHistoryString += szRecvBuf;
		strOneLine = GetOneLine (m_strResponseHistoryString);
		m_strResponseHistoryString.ReleaseBuffer();
		if (strOneLine.IsEmpty()) return -1;
	}

	if (pstrResponseStr) 
		*pstrResponseStr = strOneLine;
	CString strRetCode = GetDigitStrAtHead(strOneLine);

	HWriteLog("(%d) FTP server response: %s",m_nIndex,strOneLine);
	return atoi(strRetCode);
}

BOOL HSocketClient::GetResponse ( int nVerifyCode, CString *pstrResponseStr )
{
	CString strResponseStr;
	int nResponseCode = GetResponse ( &strResponseStr );
	if (pstrResponseStr) *pstrResponseStr = strResponseStr;
	if (nResponseCode != nVerifyCode)
	{
		HWriteLog("Receive error response code : %s",strResponseStr);
		return FALSE;
	}
	return TRUE;
}

BOOL HSocketClient::SendString(LPCTSTR lpszData, ...)
{
	// 格式化
	char szDataBuf[NET_BUFFER_SIZE] = {0};
	va_list  va;
	va_start(va,lpszData);
	_vsnprintf(szDataBuf,sizeof(szDataBuf)-1,(const char*)lpszData,va);
	va_end(va);

	HWriteLog("(%d) Send string : \r\n------------------------>>>\r\n%s------------------------>>>\r\n",
		m_nIndex, szDataBuf);
	return Send(szDataBuf, strlen(szDataBuf));
}

BOOL HSocketClient::Send(char *pData, int nSize)
{
	ASSERT(m_hEvtEndModule && m_hEvtEndModule != INVALID_HANDLE_VALUE);
	ASSERT(m_hSocket != INVALID_SOCKET);
	if (!pData || nSize < 1) return TRUE;

	int nRemainBytes = nSize;
	int nSentTotalBytes = 0;
	int nSendFailedCount = 0;
	while (nRemainBytes > 0)
	{
		int nSentBytes = CSocket::Send(pData+nSentTotalBytes,nRemainBytes);
		if (nSentBytes < 0)
		{
			nSendFailedCount++;
			if (nSendFailedCount > 10)
			{
				HWriteLog("(%d) Send net data block failed", m_nIndex);
				m_bConnected = FALSE;
				return FALSE;
			}
			else
			{
				SLEEP_RETURN(100);
			}
		}
		else
		{
			nRemainBytes -= nSentBytes;
			nSentTotalBytes += nSentBytes;
			nSendFailedCount = 0;
		}
	}
	return TRUE;
}

void HSocketClient::SetEventOfEndModule(HANDLE hEvtEndModule)
{
	m_hEvtEndModule = hEvtEndModule;
	ASSERT(m_hEvtEndModule && m_hEvtEndModule != INVALID_HANDLE_VALUE);
}

// 从类似 "(192,168,0,2,4,31)" 字符串中得到IP地址和端口号
//
BOOL HSocketClient::GetIPAndPortByPasvString(LPCTSTR lpszPasvString, OUT CString &strIP, OUT USHORT &nPort)
{
	if (!lpszPasvString ) return FALSE;
	const char *p = strchr (lpszPasvString,'(');
	if (!p) return FALSE;
	CString strPasvStr = p+1, strTemp;
	int nPosStart = 0, nPosEnd = 0;
	int nMultiple = 0, nMantissa = 0;
	for (int i = 0; ; i++)
	{
		nPosEnd = strPasvStr.Find (",",nPosStart);
		if (nPosEnd < 0)
		{
			if (i == 5)
			{
				nPosEnd = strPasvStr.Find(")", nPosStart);
				strTemp = strPasvStr.Mid(nPosStart, nPosEnd-nPosStart);
				nMantissa = atoi(strTemp);
				break;
			}
			else 
				return FALSE;
		}
		strTemp = strPasvStr.Mid(nPosStart, nPosEnd-nPosStart);
		strTemp.TrimLeft(); strTemp.TrimRight();
		if (i < 4)
		{
			if (!strIP.IsEmpty ()) strIP += ".";
			strIP += strTemp;
		}
		else if (i == 4)
		{
			nMultiple = atoi (strTemp);
		}
		else 
			return FALSE;
		nPosStart = nPosEnd + 1;
	}
	nPort = nMultiple*256 + nMantissa;
	return TRUE;
}

//
// return : -----------------------------------------------------------
//		>= 0	-	收到的字节数
//		-1		-	失败
//
int HSocketClient::Receive(char *pBuf, int nSize, BOOL bBlock)
{
	if (!pBuf || nSize < 0) return -1;
	int nReadSize = 0;
	if (bBlock)
	{
		nReadSize = CSocket::Receive(pBuf, nSize);
		if (nReadSize <= 0) 
			nReadSize = -1;
	}
	else
	{
		nReadSize = CAsyncSocket::Receive(pBuf, nSize);
		if (nReadSize < 0)
		{
			if (WSAEWOULDBLOCK  == GetLastError ())
				nReadSize = 0;
			else
				nReadSize = -1;
		}
	}
	if (nReadSize == -1)
	{
		m_bConnected = FALSE;
	}
	return nReadSize;
}

void HSocketClient::Disconnect()
{
	CancelBlockingCall();
	Close();
	m_bConnected = FALSE;
}

BOOL HSocketClient::Connect(LPCTSTR lpszHost, USHORT nPort)
{
	if (!lpszHost || strlen(lpszHost) <= 0 || nPort < 1)
		return FALSE;

	HWriteLog("(%d) Connecting to [%s:%d] ...", m_nIndex, lpszHost, nPort);
	Close ();
	if (!AfxSocketInit())
	{
		AfxMessageBox("AfxSocketInit() failed!");
		return FALSE;
	}
	if (!Create())
		return FALSE;
	m_bConnected = CSocket::Connect(lpszHost, nPort);
	if (!m_bConnected)
		HWriteLog("(%d) Connect to [%s:%d] failed", m_nIndex, lpszHost, nPort);
	return m_bConnected;
}

//
// 改写这个虚函数，可以在调用 CancelBlockingCall() 函数时立即终止连接
//
BOOL HSocketClient::ConnectHelper(const SOCKADDR *lpSockAddr, int nSockAddrLen)
{
	if (!CAsyncSocket::ConnectHelper(lpSockAddr, nSockAddrLen))
	{
		if (GetLastError() == WSAEWOULDBLOCK)
		{
			while (PumpMessagesMy(FD_CONNECT))
			{
				if (m_nConnectError != -1)
				{
					WSASetLastError(m_nConnectError);
					return (m_nConnectError == 0);
				}
			}
		}
		return FALSE;
	}
	return TRUE;
}

BOOL HSocketClient::PumpMessagesMy(UINT uStopFlag)
{
	// The same socket better not be blocking in more than one place.
	ASSERT(m_pbBlocking == NULL);

	_AFX_SOCK_THREAD_STATE* pState = _afxSockThreadState;

	ASSERT(pState->m_hSocketWindow != NULL);

	BOOL bBlocking = TRUE;
	m_pbBlocking = &bBlocking;
	CWinThread* pThread = AfxGetThread();

	// This is not a timeout in the WinSock sense, but more
	// like a WM_KICKIDLE to keep message pumping alive
	UINT nTimerID = ::SetTimer(pState->m_hSocketWindow, 1, m_nTimeOut, NULL);

	if (nTimerID == 0)
		AfxThrowResourceException();

	BOOL bPeek = TRUE;

	while (bBlocking)
	{
		TRY
		{
			MSG msg;
			if ( ::PeekMessage(&msg, pState->m_hSocketWindow,
				WM_SOCKET_NOTIFY, WM_SOCKET_DEAD, PM_REMOVE) )
			{
				if (msg.message == WM_SOCKET_NOTIFY && (SOCKET)msg.wParam == m_hSocket)
				{
					if (WSAGETSELECTEVENT(msg.lParam) == FD_CLOSE)
					{
						break;
					}
					if (WSAGETSELECTEVENT(msg.lParam) == uStopFlag)
					{
						if (uStopFlag == FD_CONNECT)
							m_nConnectError = WSAGETSELECTERROR(msg.lParam);
						break;
					}
				}
				if (msg.wParam != 0 || msg.lParam != 0)
					CSocket::AuxQueueAdd(msg.message, msg.wParam, msg.lParam);

				bPeek = TRUE;
			}
			else if ( ::PeekMessage(&msg, pState->m_hSocketWindow,
				WM_TIMER, WM_TIMER, PM_REMOVE) )
			{
				break;
			}

			if (bPeek && ::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
			{
				if (OnMessagePending())
				{
					// allow user-interface updates
					pThread->OnIdle(-1);
				}
				else
				{
					bPeek = FALSE;
				}
			}
			else
			{
				// no work to do -- allow CPU to sleep
				SLEEP_RETURN ( 100 );
				bPeek = TRUE;
			}
		}
		CATCH_ALL(e)
		{
			TRACE0("Error: caught exception in PumpMessage - continuing.\n");
			e->Delete ();
			bPeek = TRUE;
		}
		END_CATCH_ALL
	}

	::KillTimer(pState->m_hSocketWindow, nTimerID);

	if (!bBlocking)
	{
		WSASetLastError(WSAEINTR);
		return FALSE;
	}
	m_pbBlocking = NULL;

	::PostMessage(pState->m_hSocketWindow, WM_SOCKET_NOTIFY, 0, 0);

	return TRUE;

}