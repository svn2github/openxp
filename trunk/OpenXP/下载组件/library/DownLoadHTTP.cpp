#include "StdAfx.h"
#include "DownLoadHTTP.h"
#include "PublicFunc.h"

void DownloadNotify(int nIndex, UINT nNotityType, LPVOID lpNotifyData, LPVOID pDownloadMTR);

DownLoadHTTP::DownLoadHTTP(void)
{
}

DownLoadHTTP::~DownLoadHTTP(void)
{
}

BOOL DownLoadHTTP::DownloadOnce()
{
	// ����Ҫ������
	int nWillDownloadSize = GetWillDownloadSize();				// ����Ӧ�����ص��ֽ���
	int nDownloadedSize = GetDownloadedSize();				// �������ֽ���
	if (nWillDownloadSize > 0 && nDownloadedSize >= nWillDownloadSize)
		return DownloadEnd(TRUE);

	if (!DownLoadPublic::DownloadOnce())
		return DownloadEnd(FALSE);

	char szTailData[NET_BUFFER_SIZE] = {0};
	int nTailSize = sizeof(szTailData);
	if (!RequestHttpData(TRUE, szTailData,&nTailSize))
		return DownloadEnd(FALSE);
	// ��HTTP�������ж�ȡ���ݣ������浽�ļ���
	return DownloadEnd(RecvDataAndSaveToFile(m_SocketClient,szTailData,nTailSize));

}

BOOL DownLoadHTTP::RequestHttpData(BOOL bGet, char *szTailData/*=NULL*/, int *pnTailSize/*=NULL*/ )
{
	int nTailSizeTemp = 0;
	BOOL bRetryRequest = TRUE;
	while (bRetryRequest)
	{
		CString strReq = GetRequestStr(bGet);
		CString strResponse;
		nTailSizeTemp = pnTailSize?(*pnTailSize):0;
		if (!SendRequest(strReq, strResponse, szTailData, &nTailSizeTemp))
			return FALSE;

		CString strReferer_Old = m_strReferer;
		CString strDownloadUrl_Old = m_strDownloadUrl;
		CString strServer_Old = m_strServer;
		CString strObject_Old = m_strObject;
		USHORT nPort_Old = m_nPort;
		CString strProtocolType_Old = m_strProtocolType;
		if (!ParseResponseString(strResponse, bRetryRequest))
		{
			if (!m_strCookieFlag.IsEmpty())
			{
				m_strCookieFlag.Empty();
				return FALSE;
			}
			m_strReferer = strReferer_Old;
			m_strDownloadUrl = strDownloadUrl_Old;
			m_strServer = strServer_Old;
			m_strObject = strObject_Old;
			m_nPort = nPort_Old;
			m_strProtocolType = strProtocolType_Old;
			m_strCookieFlag = "Flag=UUIISPoweredByUUSoft";
			bRetryRequest = TRUE;
		}
	}
	if (pnTailSize)
		*pnTailSize = nTailSizeTemp;

	return TRUE;
}

//
// ��ȡԶ��վ����Ϣ���磺�Ƿ�֧�ֶϵ�������Ҫ���ص��ļ���С�ʹ���ʱ���
//
BOOL DownLoadHTTP::GetRemoteSiteInfoPro()
{
	BOOL bRet = FALSE;
	if (!DownLoadPublic::GetRemoteSiteInfoPro())
		goto finished;

	if (!RequestHttpData(TRUE))
		goto finished;

	bRet = TRUE;

finished:
	return bRet;
}

CString DownLoadHTTP::GetRequestStr(BOOL bGet)
{
	CString strVerb;
	if(bGet)
		strVerb = _T("GET ");
	else
		strVerb = _T("HEAD ");

	CString strReq, strAuth, strRange;
	strReq  = strVerb  + m_strObject + " HTTP/1.1\r\n";

	if (!m_strUsername.IsEmpty ())
	{
		strAuth = _T("");
		Base64Encode(m_strUsername + ":" + m_strPassword, strAuth);
		strReq += "Authorization: Basic " + strAuth + "\r\n";
	}

	CString strPort;
	if (m_nPort != DEFAULT_HTTP_PORT)
		strPort.Format (":%d", m_nPort);
	strReq += "Host: " + m_strServer + strPort + "\r\n";
	strReq += "Accept: */*\r\n";
	strReq += "Pragma: no-cache\r\n"; 
	strReq += "Cache-Control: no-cache\r\n";
	strReq += "User-Agent: "+m_strUserAgent+"\r\n";
	if(m_strReferer.IsEmpty())
	{
		m_strReferer = GetRefererFromURL ();
	}
	strReq += "Referer: "+m_strReferer+"\r\n";
	strReq += "Connection: close\r\n";
	if (!m_strCookieFlag.IsEmpty())
	{
		strReq += "Cookie: " + m_strCookieFlag + "\r\n";
	}

	// ָ��Ҫ���ص��ļ���Χ
	CString strEndPos;
	int nWillDownloadStartPos = GetWillDownloadStartPos();	// ��ʼλ��
	int nWillDownloadSize = GetWillDownloadSize();				// ����Ӧ�����ص��ֽ���
	int nDownloadedSize = GetDownloadedSize();				// �������ֽ���
	if (nWillDownloadSize > 0)
		strEndPos.Format("%d", nWillDownloadStartPos+nWillDownloadSize-1);
	ASSERT (nWillDownloadSize < 0 || nDownloadedSize < nWillDownloadSize);
	strRange.Format(_T("Range: bytes=%d-%s\r\n"), nWillDownloadStartPos+nDownloadedSize, strEndPos);

	strReq += strRange;
	strReq += "\r\n";

	return strReq;
}

//
// ��������ύ���󣬲��õ������ַ���
//
BOOL DownLoadHTTP::SendRequest(LPCTSTR lpszReq, CString &strResponse, char *szTailData/*=NULL*/, int *pnTailSize/*=NULL*/ )
{
	m_SocketClient.Disconnect();
	if (!Connect ()) 
		return FALSE;
	if (!m_SocketClient.SendString (lpszReq))
	{
		return FALSE;
	}

	for (int i = 0; ; i++)
	{
		char szRecvBuf[NET_BUFFER_SIZE] = {0};
		int nReadSize = m_SocketClient.Receive(szRecvBuf, sizeof(szRecvBuf));
		if (nReadSize <= 0)
		{
			HWriteLog("(%d) Receive response data failed", m_nIndex);
			return FALSE;
		}
		strResponse += szRecvBuf;
		char *p = strstr(szRecvBuf, "\r\n\r\n");
		if (p)
		{
			if (szTailData && pnTailSize && *pnTailSize > 0)
			{
				p += 4;
				int nOtioseSize = nReadSize - int( p - szRecvBuf );
				*pnTailSize = MIN ( nOtioseSize, *pnTailSize );
				memcpy(szTailData, p, *pnTailSize);
			}
#ifdef _DEBUG
			int nPos = strResponse.Find ("\r\n\r\n", 0);
			CString strDump;
			if (nPos >= 0)
				strDump = strResponse.Left(nPos);
			else 
				strDump = strResponse;
			HWriteLog("(%d) HTTP server response : \r\n<<<++++++++++++++++++++++++\r\n%s\r\n<<<++++++++++++++++++++++++",
				m_nIndex, strDump);
#endif
			break;
		}
	}

	return TRUE;
}

DWORD DownLoadHTTP::GetResponseCode(CString strLineText)
{
	strLineText.MakeLower ();
	ASSERT(strLineText.Find ("http/", 0) >= 0);
	int nPos = strLineText.Find (" ", 0);
	if (nPos < 0) return 0;
	CString strCode = strLineText.Mid(nPos + 1);
	strCode.TrimLeft(); 
	strCode.TrimRight();
	nPos = strCode.Find (" ", 0);
	if (nPos < 0) 
		nPos = strCode.GetLength() - 1;
	strCode = strCode.Left (nPos);

	return (DWORD)atoi(strCode);
}

BOOL DownLoadHTTP::ParseResponseString ( CString strResponseString, OUT BOOL &bRetryRequest )
{
	bRetryRequest = FALSE;
	// ��ȡ���ش���
	CString strOneLine = GetOneLine (strResponseString);
	strResponseString.ReleaseBuffer();
	DWORD dwResponseCode = GetResponseCode (strOneLine);
	if (dwResponseCode < 1)
	{
		HWriteLog("(%d) Received error response code : %s", m_nIndex, strOneLine);
		return FALSE;
	}

	int nPos = 0;
	// �����ļ����ض���
	if(dwResponseCode >= 300 && dwResponseCode < 400)
	{
		bRetryRequest = TRUE;
		// �õ������ļ��µ�URL
		CString strRedirectFileName = FindAfterFlagString("location:", strResponseString);

		// ���� Referer
		m_strReferer = GetRefererFromURL();

		// �ض��������ķ�����
		nPos = strRedirectFileName.Find("://");
		if (nPos >= 0)
		{
			m_strDownloadUrl = strRedirectFileName;
			// ����Ҫ���ص�URL�Ƿ���Ч
			if (!ParseTrueURL(m_strDownloadUrl, m_strServer, m_strObject, m_nPort, m_strProtocolType))
			{
				HWriteLog("(%d) Redirect media path [%s] invalid", m_nIndex, m_strDownloadUrl);
				return FALSE;
			}
			return TRUE;
		}

		// �ض��򵽱��������������ط�
		strRedirectFileName.Replace("\\", "/");
		// �ض����ڸ�Ŀ¼
		if(strRedirectFileName[0] == '/')
		{
			m_strObject = strRedirectFileName;
			CString str = GetDownloadObjectFileName();
			DownloadNotify(-1, NOTIFY_TYPE_GOT_REMOTE_FILENAME, (LPVOID)(str.GetBuffer(str.GetLength())), m_pDownloadMTR);
			str.ReleaseBuffer();
			return TRUE;
		}

		// ��������Ե�ǰĿ¼
		int nParentDirCount = 0;
		nPos = strRedirectFileName.Find ("../");
		while (nPos >= 0)
		{
			strRedirectFileName = strRedirectFileName.Mid(nPos+3);
			nParentDirCount++;
			nPos = strRedirectFileName.Find("../");
		}
		for (int i = 0; i <= nParentDirCount; i++)
		{
			nPos = m_strDownloadUrl.ReverseFind('/');
			if (nPos != -1)
				m_strDownloadUrl = m_strDownloadUrl.Left(nPos);
		}
		if (strRedirectFileName.Find ( "./", 0 ) == 0)
			strRedirectFileName.Delete(0, 2);
		m_strDownloadUrl = m_strDownloadUrl+"/"+strRedirectFileName;

		return ParseTrueURL(m_strDownloadUrl, m_strServer, m_strObject, m_nPort, m_strProtocolType);
	}
	// ���󱻳ɹ����ա����ͽ���
	else if(dwResponseCode >= 200 && dwResponseCode < 300)
	{
		if (m_nIndex == -1)	// ���̲߳���Ҫ��ȡ�ļ���С����Ϣ
		{
			// ��ȡ Content-Length
			CString strDownFileLen = FindAfterFlagString("content-length:", strResponseString);
			m_nFileTotalSize = (int)_ttoi(strDownFileLen.GetBuffer(strDownFileLen.GetLength()));
			strDownFileLen.ReleaseBuffer();
			DownloadNotify (-1, NOTIFY_TYPE_GOT_REMOTE_FILESIZE, (LPVOID)m_nFileTotalSize, m_pDownloadMTR);
			int nWillDownloadStartPos = GetWillDownloadStartPos();	// ��ʼλ��
			int nWillDownloadSize = GetWillDownloadSize();				// ����Ӧ�����ص��ֽ���
			int nDownloadedSize = GetDownloadedSize();				// �������ֽ���
			if (m_nFileTotalSize > 0 && nWillDownloadSize-nDownloadedSize > m_nFileTotalSize)
				SetWillDownloadSize(m_nFileTotalSize-nDownloadedSize);
		}

		// ��ȡ�������ļ�������޸�ʱ��
		CString strModifiedTime = FindAfterFlagString("last-modified:", strResponseString);
		if (!strModifiedTime.IsEmpty())
		{
			m_TimeLastModified = ConvertHttpTimeString(strModifiedTime);
		}

		if (dwResponseCode == 206)	// ֧�ֶϵ�����
		{
			m_bSupportResume = TRUE;
		}
		else							// ��֧�ֶϵ�����
		{
			m_bSupportResume = FALSE;
		}
		return TRUE;
	}

	HWriteLog("(%d) Receive invalid code : %d", m_nIndex, dwResponseCode );
	return FALSE;
}

CString DownLoadHTTP::FindAfterFlagString(LPCTSTR lpszFoundStr, CString strOrg)
{
	ASSERT (lpszFoundStr && strlen(lpszFoundStr) > 0);
	CString strReturing, strFoundStr = GET_SAFE_STRING(lpszFoundStr);
	strFoundStr.MakeLower();

	char *pTmp = new char[strOrg.GetLength()+1];
	strcpy(pTmp,strOrg.GetBuffer(strOrg.GetLength()));
	strOrg.ReleaseBuffer();
	MytoLower(pTmp);
	CString strOrgLower(pTmp);
	int nPos = strOrgLower.Find(strFoundStr );
	if (nPos < 0) return "";
	strReturing = strOrg.Mid(nPos + strFoundStr.GetLength());
	nPos = strReturing.Find("\r\n");
	if (nPos < 0) return "";
	strReturing = strReturing.Left(nPos);
	strReturing.TrimLeft();
	strReturing.TrimRight();

	delete pTmp;

	return strReturing;
}

//
// �� HTTP ��������ʾ��ʱ��ת��Ϊ CTime ��ʽ���磺Wed, 16 May 2007 14:29:53 GMT
//
CTime DownLoadHTTP::ConvertHttpTimeString(CString strTimeGMT)
{
	CString strYear, strMonth, strDay, strTime;
	CTime tReturning = -1;
	int nPos = strTimeGMT.Find(",",0);
	if (nPos < 0 || nPos >= strTimeGMT.GetLength()-1)
		return tReturning;
	strTimeGMT = strTimeGMT.Mid(nPos+1);
	strTimeGMT.TrimLeft();
	strTimeGMT.TrimRight();

	// ��
	nPos = strTimeGMT.Find (" ", 0);
	if (nPos < 0 || nPos >= strTimeGMT.GetLength()-1)
		return tReturning;
	strDay = strTimeGMT.Left(nPos);
	strTimeGMT = strTimeGMT.Mid(nPos + 1);
	strTimeGMT.TrimLeft();
	strTimeGMT.TrimRight();

	// ��
	nPos = strTimeGMT.Find (" ", 0);
	if (nPos < 0 || nPos >= strTimeGMT.GetLength()-1)
		return tReturning;
	strMonth = strTimeGMT.Left(nPos);
	int nMonth = GetMouthByShortStr(strMonth);
	ASSERT(nMonth >= 1 && nMonth <= 12);
	strMonth.Format ("%02d", nMonth);
	strTimeGMT = strTimeGMT.Mid(nPos + 1);
	strTimeGMT.TrimLeft(); 
	strTimeGMT.TrimRight();

	// ��
	nPos = strTimeGMT.Find (" ", 0);
	if (nPos < 0 || nPos >= strTimeGMT.GetLength()-1)
		return tReturning;
	strYear = strTimeGMT.Left(nPos);
	strTimeGMT = strTimeGMT.Mid(nPos + 1);
	strTimeGMT.TrimLeft(); 
	strTimeGMT.TrimRight();

	// ʱ��
	nPos = strTimeGMT.Find (" ", 0);
	if (nPos < 0 || nPos >= strTimeGMT.GetLength()-1)
		return tReturning;
	strTime = strTimeGMT.Left(nPos);
	strTimeGMT = strTimeGMT.Mid (nPos + 1);

	CString strFileTimeInfo;
	strFileTimeInfo.Format("%s-%s-%s %s", strYear, strMonth, strDay, strTime);
	ConvertStrToCTime(strFileTimeInfo.GetBuffer(strFileTimeInfo.GetLength()), tReturning);
	strFileTimeInfo.ReleaseBuffer();
	return tReturning;
}
