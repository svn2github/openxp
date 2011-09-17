#include "StdAfx.h"
#include "DownLoadFTP.h"
#include "PublicFunc.h"

void DownloadNotify(int nIndex,UINT nNotityType,LPVOID lpNotifyData,LPVOID pDownloadMTR);

DownLoadFTP::DownLoadFTP(void)
{
}

DownLoadFTP::~DownLoadFTP(void)
{
}

//
// ����һ�� FTP ������ͨ������
//
BOOL DownLoadFTP::CreateFTPDataConnect(HSocketClient &SocketClient)
{
	ASSERT(SocketClient.m_hSocket == INVALID_SOCKET);
	// �豻��ģʽ
	if (!m_SocketClient.SendString("PASV\r\n"))  return FALSE;
	CString strResponseStr;
	if (!m_SocketClient.GetResponse (227,&strResponseStr))
		return FALSE;
	CString strPasvIP;
	USHORT uPasvPort = 0;
	if (!m_SocketClient.GetIPAndPortByPasvString (strResponseStr,strPasvIP, uPasvPort))
		return FALSE;

	// ��������ͨ������
	if (!SocketClient.Connect(strPasvIP,uPasvPort))
		return FALSE;

	return TRUE;
}

//
// ������Ҫ���صķ�����
//
BOOL DownLoadFTP::Connect ()
{
	if ( !DownLoadPublic::Connect () )
		return FALSE;

	SLEEP_RETURN_Down(0);
	int nResponseCode = m_SocketClient.GetResponse();
	if (nResponseCode != 220)
	{
		HWriteLog("(%d) Connect to [%s:%u] failed", m_nIndex, m_strServer, m_nPort);
	}

	// ��¼
	SLEEP_RETURN_Down ( 0 );
	if (m_strUsername.IsEmpty())
		m_strUsername = "anonymous";
	if (!m_SocketClient.SendString("USER %s\r\n", m_strUsername)) return FALSE;
	nResponseCode = m_SocketClient.GetResponse();

	// ��Ҫ����
	SLEEP_RETURN_Down(0);
	if (nResponseCode == 331)
	{
		if (!m_SocketClient.SendString("PASS %s\r\n", m_strPassword)) return FALSE;
		nResponseCode = m_SocketClient.GetResponse();
	}

	// ��¼ʧ��
	if (nResponseCode != 230)
	{
		HWriteLog("(%d) Login failed", m_nIndex);
		return FALSE;
	}
	HWriteLog("(%d) Welcome message��--------------------------\r\n%s\r\n", m_nIndex,
		m_SocketClient.GetResponseHistoryString());
	m_SocketClient.ClearResponseHistoryString();

	return TRUE;
}

BOOL DownLoadFTP::DownloadOnce()
{
	// ����Ҫ������
	int nWillDownloadSize = GetWillDownloadSize();				// ����Ӧ�����ص��ֽ���
	int nDownloadedSize = GetDownloadedSize();				// �������ֽ���
	if (nWillDownloadSize > 0 && nDownloadedSize >= nWillDownloadSize)
		return DownloadEnd(TRUE);

	if ( !DownLoadPublic::DownloadOnce())
		return DownloadEnd(FALSE);

	// ���ý�����������Ϊ ������ģʽ
	if (!m_SocketClient.SendString("TYPE I\r\n"))
		return DownloadEnd(FALSE);
	if (!m_SocketClient.GetResponse(200))
		return DownloadEnd(FALSE);
	SLEEP_RETURN_Down(0);

	// ��������ͨ������
	HSocketClient SocketClient;
	SocketClient.SetEventOfEndModule(m_hEvtEndModule);
	SocketClient.m_nIndex = m_nIndex;
	if (!CreateFTPDataConnect (SocketClient))
		return DownloadEnd(FALSE);
	SLEEP_RETURN_Down(0);

	// �趨�����ļ�����ʼλ��
	int nWillDownloadStartPos = GetWillDownloadStartPos();	// ��ʼλ��
	if (!m_SocketClient.SendString("REST %d\r\n", nWillDownloadStartPos+nDownloadedSize))
		return DownloadEnd(FALSE);
	if (!m_SocketClient.GetResponse (350))
		return DownloadEnd(FALSE);

	// �ύ�����ļ�������
	if (!m_SocketClient.SendString ("RETR %s\r\n", m_strObject))
		return DownloadEnd(FALSE);
	if (!m_SocketClient.GetResponse(150))
		return DownloadEnd(FALSE);
	SLEEP_RETURN_Down(0);

	// ������ͨ����ȡ���ݣ������浽�ļ���
	BOOL bRet = RecvDataAndSaveToFile(SocketClient);
	SocketClient.Disconnect();
	return DownloadEnd (bRet);
}

//
// ��ȡԶ��վ����Ϣ���磺�Ƿ�֧�ֶϵ�������Ҫ���ص��ļ���С�ʹ���ʱ���
//
BOOL DownLoadFTP::GetRemoteSiteInfoPro()
{
	BOOL bRet = FALSE;
	HSocketClient SocketClient;
	CString strReadData;
	char szRecvBuf[NET_BUFFER_SIZE] = {0};
	int nReadSize = 0;
	CString strOneLine;
	int i;

	SocketClient.m_nIndex = m_nIndex;
	if (!HANDLE_IS_VALID(m_hEvtEndModule))
		goto Finished;
	SocketClient.SetEventOfEndModule(m_hEvtEndModule);

	if ( !DownLoadPublic::GetRemoteSiteInfoPro())
		goto Finished;

	if (!m_SocketClient.IsConnected () && !Connect())
		goto Finished;
	SLEEP_RETURN(0);

	// ���ý�����������Ϊ ASCII
	if (!m_SocketClient.SendString("TYPE A\r\n")) goto Finished;
	if (!m_SocketClient.GetResponse(200))
		goto Finished;
	SLEEP_RETURN(0);

	// �ж��Ƿ�֧�ֶϵ�����
	if (!m_SocketClient.SendString ("REST 1\r\n")) goto Finished;
	m_bSupportResume = (m_SocketClient.GetResponse() == 350);
	TRACE ("�Ƿ�֧�ֶϵ�������%s\n", m_bSupportResume ? "֧��":"��֧��");
	if (m_bSupportResume)
	{
		if (!m_SocketClient.SendString("REST 0\r\n")) goto Finished;
		VERIFY (m_SocketClient.GetResponse(350));
	}
	SLEEP_RETURN(0);

	// ��������ͨ������
	if (!CreateFTPDataConnect(SocketClient))
		goto Finished;
	// �����о��ļ�����
	if (!m_SocketClient.SendString("LIST %s\r\n",m_strObject))
		goto Finished;
	if (!m_SocketClient.GetResponse(150))
		goto Finished;
	SLEEP_RETURN(0);

	// ������ͨ����ȡ�ļ��б���Ϣ��ֱ������ͨ������ "226" ��Ӧ�룬ע�⣺������������÷������͵ġ�
	for (i = 0; ; i++)
	{
		memset(szRecvBuf, 0, sizeof(szRecvBuf));
		nReadSize = SocketClient.Receive(szRecvBuf, sizeof(szRecvBuf), FALSE);
		if (nReadSize < 0)
		{
			HWriteLog("(%d) Receive file list info failed", m_nIndex);
			break;
		}
		strReadData += szRecvBuf;
		int nResponseCode = m_SocketClient.GetResponse((CString*)NULL,FALSE);
		if (nResponseCode == -1) goto Finished;
		else if (nResponseCode == 0)
		{
			SLEEP_RETURN(100);
		}
		else if (nResponseCode == 226)
		{
			break;
		}
		else
			goto Finished;
		SLEEP_RETURN(0);
	}

	for (i = 0; !strReadData.IsEmpty(); i++)
	{
		strOneLine = GetOneLine(strReadData);
		strReadData.ReleaseBuffer();
		if (!strOneLine.IsEmpty())
		{
			ParseFileInfoStr(strOneLine);
		}
	}
	bRet = TRUE;

Finished:
	return bRet;
}

//
// �� "-rw-rw-rw-   1 user     group    37979686 Mar  9 13:39 FTP-Callgle 1.4.0_20060309.cab" �ַ����з���
// ���ļ���Ϣ
//
void DownLoadFTP::ParseFileInfoStr(CString &strFileInfoStr)
{
	strFileInfoStr.TrimLeft(); 
	strFileInfoStr.TrimRight();
	if (strFileInfoStr.IsEmpty()) return;
	BOOL bLastCharIsSpace = (strFileInfoStr[0]==' ');
	int nSpaceCount = 0;
	CString strFileTime1, strFileTime2, strFileTime3;
	CString strNodeStr;
	CString strFileName;
	for (int i = 0; i < strFileInfoStr.GetLength(); i++)
	{
		if (strFileInfoStr[i]==' ')
		{
			if (!bLastCharIsSpace)
			{
				strNodeStr = strFileInfoStr.Mid(i);
				strNodeStr.TrimLeft(); 
				strNodeStr.TrimRight();
				nSpaceCount ++;
				int nFindPos = strNodeStr.Find (" ", 0);
				if ( nFindPos < 0 ) 
					nFindPos = strNodeStr.GetLength()-1;
				strNodeStr = strNodeStr.Left(nFindPos);
				// �ļ���С
				if (nSpaceCount == 4)
				{
					if (m_nIndex == -1)	// ���̲߳���Ҫ��ȡ�ļ���С����Ϣ
					{
						m_nFileTotalSize = (int)atoi(strNodeStr);
						DownloadNotify (m_nIndex, NOTIFY_TYPE_GOT_REMOTE_FILESIZE,(LPVOID)m_nFileTotalSize, m_pDownloadMTR);
						int nWillDownloadStartPos = GetWillDownloadStartPos();	// ��ʼλ��
						int nWillDownloadSize = GetWillDownloadSize();				// ����Ӧ�����ص��ֽ���
						int nDownloadedSize = GetDownloadedSize();				// �������ֽ���
						if (m_nFileTotalSize > 0 && nWillDownloadSize-nDownloadedSize > m_nFileTotalSize)
							SetWillDownloadSize(m_nFileTotalSize-nDownloadedSize);
					}
				}
				// �ļ�ʱ���һ��
				else if (nSpaceCount == 5)
				{
					strFileTime1 = strNodeStr;
				}
				// �ļ�ʱ��ڶ���
				else if (nSpaceCount == 6)
				{
					strFileTime2 = strNodeStr;
				}
				// �ļ�ʱ�������
				else if (nSpaceCount == 7)
				{
					strFileTime3 = strNodeStr;
				}
				else if (nSpaceCount > 7)
				{
					strFileName = strFileInfoStr.Mid(i);
					strFileName.TrimLeft(); 
					strFileName.TrimRight();
					break;
				}
			}
			bLastCharIsSpace = TRUE;
		}
		else
		{
			bLastCharIsSpace = FALSE;
		}
	}
	GetFileTimeInfo(strFileTime1, strFileTime2, strFileTime3);
}

void DownLoadFTP::GetFileTimeInfo(CString strFileTime1, CString strFileTime2, CString strFileTime3)
{
	if (strFileTime3.IsEmpty()) return;
	CString strYear, strMonth, strDay, strHour, strMinute, strSecond;
	int nMonth = GetMouthByShortStr(strFileTime1);
	ASSERT(nMonth >= 1 && nMonth <= 12);
	strMonth.Format ("%02d", nMonth);

	COleDateTime tOleNow = COleDateTime::GetCurrentTime();
	strDay.Format("%02d",atoi(strFileTime2));
	strSecond = "00";
	// ��������ڣ��磺Aug 21 2006
	if (strFileTime3.Find (":", 0) < 0)
	{
		strYear = strFileTime3;
		strHour = "00";
		strMinute = "00";
	}
	// ����������ڣ��磺Feb 21 01:11
	else
	{
		strYear.Format("%04d", tOleNow.GetYear());
		int nPos = strFileTime3.Find (":", 0);
		if (nPos < 0) 
			nPos = strFileTime3.GetLength() - 1;
		strHour = strFileTime3.Left(nPos);
		strMinute = strFileTime3.Mid (nPos+1);
	}

	CString strFileTimeInfo;
	strFileTimeInfo.Format("%s-%s-%s %s:%s:%s", strYear, strMonth, strDay, strHour, strMinute, strSecond);
	ConvertStrToCTime(strFileTimeInfo.GetBuffer(strFileTimeInfo.GetLength()), m_TimeLastModified);
	strFileTimeInfo.ReleaseBuffer();
}
