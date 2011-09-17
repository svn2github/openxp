#ifndef __HSOCKETCLIENT__H__
#define __HSOCKETCLIENT__H__
#pragma once
#include <afxsock.h>

//////////////////////////////////////////////////////////////////////////
// 注意：
// 在 InitInstance() 函数中调用 AfxSocketInit();函数来初始化网络接口，在
// ExitInstance() 函数中调用 WSACleanup (); 关闭网络接口
//////////////////////////////////////////////////////////////////////////

#define NET_BUFFER_SIZE			4096		// 默认缓冲大小

class HSocketClient : public CSocket
{
public:
	BOOL IsConnected() { return m_bConnected; }

public:
	HSocketClient ();
	virtual ~HSocketClient();
	CString GetDigitStrAtHead ( LPCTSTR lpszStr );
	int GetResponse ( CString *pcsResponseStr=NULL, BOOL bBlock=TRUE );
	BOOL GetResponse ( int nVerifyCode, CString *pcsResponseStr=NULL );
	CString GetResponseHistoryString() { return m_strResponseHistoryString; }
	void ClearResponseHistoryString() { m_strResponseHistoryString.Empty(); }

public:
	int m_nIndex;
	void Disconnect();
	int Receive ( char *pBuf, int nSize, BOOL bBlock=TRUE );
	BOOL GetIPAndPortByPasvString ( LPCTSTR lpszPasvString, OUT CString &csIP, OUT USHORT &nPort );
	void SetEventOfEndModule ( HANDLE hEvtEndModule );
	BOOL Send ( char *pData, int nSize );
	BOOL SendString ( LPCTSTR lpszData, ... );
	BOOL Connect ( LPCTSTR lpszHost, USHORT nPort );

protected:
	virtual BOOL ConnectHelper(const SOCKADDR* lpSockAddr, int nSockAddrLen);

private:
	BOOL PumpMessagesMy(UINT uStopFlag);
	CString m_strResponseHistoryString;
	HANDLE m_hEvtEndModule;
	BOOL m_bConnected;
};

#endif