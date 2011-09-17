#include "StdAfx.h"
#include "HSnifferMgr.h"
#include "WebSnifferDlg.h"

UINT threadFunc ( LPVOID p )
{
	HSnifferMgr *pSnifferMgr = static_cast<HSnifferMgr*>(p) ;
	char  buf[1000],*bufwork;
	MSG   msg;
	int   nRet;
	DWORD dwError;
	char  *pSource,*pDest;
	IPHEADER *pIpHeader;
	in_addr ina;
	char   szSource[16],szDest[16],szErr[50];
	char *pLastBuf = NULL;

	int		HdrLen, totallen;
	WORD	sourport, destport;

	struct TCPPacketHead	*pTCPHead;
	struct ICMPPacketHead	*pICMPHead;
	struct UDPPacketHead	*pUDPHead;
	BYTE					*pdata = NULL;

	/*---------------------------------------------------------------------*/

	PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE); // Force to make the queue
	pSnifferMgr->m_dwThreadID = GetCurrentThreadId();

	while(TRUE)
	{
		if(PeekMessage(&msg,0,WM_CLOSE,WM_CLOSE,PM_NOREMOVE))
		{
			closesocket(pSnifferMgr->m_socSniffer);
			pSnifferMgr->m_dwThreadID = 0;
			break;
		}
		memset(buf,0,sizeof(buf));
		nRet = 	recv(pSnifferMgr->m_socSniffer,buf,sizeof(buf),0);
		if(nRet == SOCKET_ERROR)
		{
			dwError = WSAGetLastError() ;
			sprintf(szErr,"Error recv() = %ld ",dwError) ;
			continue;
		}
		else
		{
			if(*buf)
			{	
				bufwork   = buf ;
				pIpHeader = (IPHEADER *)bufwork ;
				WORD wLen = ntohs(pIpHeader->total_len) ;
				while(TRUE)
				{
					if(wLen <= nRet)
					{
						ina.S_un.S_addr = pIpHeader->sourceIP;
						pSource = inet_ntoa(ina);
						strcpy(szSource,pSource);
						ina.S_un.S_addr = pIpHeader->destIP ;
						pDest = inet_ntoa(ina);
						strcpy(szDest,pDest);
						CString str, strProto, strSourPort, strDestPort, strData, strSize;


						strProto = GetProtoName(pIpHeader->proto);

						/*-------------------zhuwei add(2002.11.9)-----------------------------*/
						HdrLen = pIpHeader->header_len&0xf;
						HdrLen *= 4;
						totallen = ntohs(pIpHeader->total_len);
						totallen -= HdrLen;
						switch(pIpHeader->proto)
						{
						case IPPROTO_ICMP:
							{
								pICMPHead=(struct ICMPPacketHead *)(buf+HdrLen);
								//strL4.Format(" type:%d code:%d\n",pICMPHead->Type,pICMPHead->Code);
								strSourPort = "-";
								strDestPort = "-";
								pdata=((BYTE *)pICMPHead)+ICMP_HEAD_LEN;
								totallen -= ICMP_HEAD_LEN;
								break;
							}
						case IPPROTO_TCP:
							{
								pTCPHead=(struct TCPPacketHead*)(buf+HdrLen);
								sourport = ntohs(pTCPHead->SourPort);
								destport = ntohs(pTCPHead->DestPort);
								//strL4.Format(" sour port:%d,dest port:%d",sourport,destport);
								strSourPort.Format("%d",sourport);
								strDestPort.Format("%d",destport);
								HdrLen = (pTCPHead->HLen)>>4;  //in fact only 4 bits
								HdrLen *= 4;
								pdata=((BYTE *)pTCPHead)+HdrLen;
								totallen -= HdrLen;
								break;
							}
						case IPPROTO_UDP:
							{
								pUDPHead=(struct UDPPacketHead *)(buf+HdrLen);
								sourport = ntohs(pUDPHead->SourPort);
								destport = ntohs(pUDPHead->DestPort);
								//strL4.Format(" sour port:%d,dest port:%d",sourport,destport);
								strSourPort.Format("%d",sourport);
								strDestPort.Format("%d",destport);
								pdata=((BYTE *)pUDPHead)+UDP_HEAD_LEN;
								totallen -= UDP_HEAD_LEN;
								break;
							}
						}

						if(pIpHeader->proto == IPPROTO_ICMP)
							strData.Format("type:%d code:%d data:%s",pICMPHead->Type,pICMPHead->Code,pdata);
						else strData.Format("  %s",pdata);

						strSize.Format("%d",totallen);
						pSnifferMgr->TraceData(strProto,szSource,strSourPort,szDest,strDestPort,strSize,strData);

						if(wLen < nRet)
						{
							nRet -= wLen ;
							bufwork  += wLen ;
							pIpHeader = (IPHEADER *)bufwork ;
						}
						else
							break ; // pIpHeader->total_len == nRet and go out
					}
					else
					{ // read last part of buf. I wrote it , but always recv() read exactly 
						// the lenght of the packet
						int iLast = wLen - nRet;
						pLastBuf = new char [wLen];
						int iReaden = nRet ;
						memcpy(pLastBuf,bufwork,iReaden);
						nRet = 	recv(pSnifferMgr->m_socSniffer,pLastBuf+iReaden,iLast,0);
						if(nRet == SOCKET_ERROR)
						{
							dwError = WSAGetLastError() ;
							sprintf(szErr,"Error recv() = %ld ",dwError) ;
							break;
						}
						else
						{
							bufwork = pLastBuf ;	
							pIpHeader = (IPHEADER *)bufwork ;
							if(nRet == iLast)
								nRet = wLen;
							else
							{ // read all last data
								iReaden += nRet;
								iLast -= nRet;
								while(TRUE)
								{
									nRet = recv(pSnifferMgr->m_socSniffer,pLastBuf+iReaden,iLast,0) ;
									if(nRet == SOCKET_ERROR)
									{
										dwError = WSAGetLastError() ;
										sprintf(szErr,"Error recv() = %ld ",dwError);
										break;
									}
									else
									{
										iReaden += nRet ;
										iLast -= nRet ;
										if(iLast <= 0) 
											break ;
									}	
								} // while
							}
						}
					}	
				}   // while
				if(pLastBuf)
					delete [] pLastBuf ;
			}
			else
			{
				AfxMessageBox( "No data on network" ) ;
				continue ;
			}
		}
	}

	return TRUE ;
}

HSnifferMgr::HSnifferMgr()
{
	m_lSnifferSize = 0;

	TCHAR		szHostName[128]={0};
	HOSTENT		*pHost=NULL;
	TCHAR		*pszIp=NULL;
	int			nNum=0;
	if(!AfxSocketInit(NULL)) 
		return;

	if(gethostname(szHostName,128)==0)
	{
		pHost = gethostbyname(szHostName);
		if(pHost != NULL)
		{
			pszIp = inet_ntoa(*(in_addr*)pHost->h_addr_list[nNum]);
			m_dwIPSource = inet_addr(pszIp);
		}
		else
		{
			return;
		}
	}
	else
	{
		return;
	}

	//从Host读取所有IP
	DWORD dwSize = 0 ;
	GetIpAddrTable(NULL ,&dwSize,FALSE);
	PMIB_IPADDRTABLE pIpAddrTable = (PMIB_IPADDRTABLE)new BYTE[dwSize]; 
	if(pIpAddrTable)
	{
		if(GetIpAddrTable((PMIB_IPADDRTABLE)pIpAddrTable,//IP表缓冲
			&dwSize,                //缓冲大小
			FALSE                   //是否对IP排序 
			) == NO_ERROR)
		{
			if(pIpAddrTable->dwNumEntries > 2) 
			{
				m_bMultiHome = TRUE;
				TCHAR szIP[16];
				for(int i = 0 ; i < (int)pIpAddrTable->dwNumEntries; i++)
				{
					in_addr ina;
					ina.S_un.S_addr = pIpAddrTable->table[i].dwAddr;
					TCHAR *pIP = inet_ntoa( ina );
					strcpy(szIP,pIP);
					if(stricmp(szIP,"127.0.0.1"))
						m_IPArr.Add(pIpAddrTable->table[i].dwAddr);
				}
			}
		}
		delete []pIpAddrTable;
	}
}

HSnifferMgr::~HSnifferMgr()
{
}

bool HSnifferMgr::StartSniffer()
{
	TCHAR			szError[50],szHostName[MAX_PATH];
	DWORD			dwError;
	SOCKADDR_IN		sa;

	gethostname(szHostName,sizeof(szHostName)) ;
	m_dwIPHostSource = m_dwIPSource;

	m_dwIPCheckedHost = ntohl(m_dwIPHost) ;

	DWORD dwBufferLen[10];
	DWORD dwBufferInLen = 1; 
	DWORD dwBytesReturned = 0;
	m_socSniffer = socket(AF_INET,SOCK_RAW,IPPROTO_IP);
	if(INVALID_SOCKET == m_socSniffer)
	{
		dwError = WSAGetLastError();
		sprintf(szError,"Error socket() = %ld ",dwError);
		TRACE(szError);
		closesocket(m_socSniffer);
		return false;

	}
	int rcvtimeo = 5000; 
	if(setsockopt(m_socSniffer,SOL_SOCKET,SO_RCVTIMEO,(const char *)&rcvtimeo,sizeof(rcvtimeo)) == SOCKET_ERROR)
	{
		dwError = WSAGetLastError();
		sprintf(szError,"Error WSAIoctl = %ld ",dwError);
		TRACE(szError);
		closesocket(m_socSniffer);
		return false;
	}
	sa.sin_family = AF_INET;
	sa.sin_port = htons(7000);
	sa.sin_addr.s_addr = m_dwIPHostSource;
	if (bind(m_socSniffer,(PSOCKADDR)&sa, sizeof(sa)) == SOCKET_ERROR)
	{
		dwError = WSAGetLastError();
		sprintf(szError,"Error bind() = %ld ",dwError);
		TRACE(szError);
		closesocket(m_socSniffer);
		return false;
	} 
	if(SOCKET_ERROR != WSAIoctl(m_socSniffer,SIO_RCVALL,&dwBufferInLen,sizeof(dwBufferInLen),             
		&dwBufferLen,sizeof(dwBufferLen),
		&dwBytesReturned,NULL,NULL))
		AfxBeginThread(threadFunc,(LPVOID)this);
	else
	{
		dwError = WSAGetLastError();
		sprintf(szError,"Error WSAIoctl = %ld ",dwError);
		TRACE(szError);
		closesocket(m_socSniffer);
		return false;
	}
	return true;
}

bool HSnifferMgr::StopSniffer()
{
	if(NULL != m_dwThreadID)
		PostThreadMessage(m_dwThreadID,WM_CLOSE,0,0) ;
	if(m_IPArr.GetSize())
		m_IPArr.RemoveAll();
	m_lSnifferSize = 0;
	return true;
}

void HSnifferMgr::TraceData(CString strProto,CString strSource,CString strSourPort,CString strDest,CString strDestPort,CString strSize,CString strData)
{
	ULONG lDataSize = atoi(strSize);
	m_lSnifferSize += lDataSize;
	m_pDlg->AddData(strProto,strSource,strSourPort,strDest,strDestPort,strSize,strData);	
}

ULONG HSnifferMgr::GetMgrSize() const
{
	return m_lSnifferSize;
}