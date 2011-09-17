#ifndef __HSNIFFERMGR__H__
#define __HSNIFFERMGR__H__
#pragma once

class CWebSnifferDlg;

class HSnifferMgr
{
public:
	HSnifferMgr();
	void SetRev(CWebSnifferDlg *p){m_pDlg=p;}
	~HSnifferMgr();
	bool StartSniffer();
	bool StopSniffer();
	ULONG GetMgrSize() const;
protected:
	void TraceData(
		CString strProto,		//����
		CString strSource,		//ԴIP
		CString strSourPort,	//Դ�˿�
		CString strDest,		//Ŀ��IP	
		CString strDestPort,	//Ŀ�Ķ˿�
		CString strSize,		//���ݴ�С
		CString strData			//��������
		);
protected:
	DWORD				m_dwIPSource;			// IP in net format
	DWORD				m_dwIPHostSource;		// same IP in host format
	DWORD				m_dwIPHost;
	DWORD				m_dwIPCheckedHost;		
	SOCKET				m_socSniffer;			//��̽SOCKET
	DWORD				m_dwThreadID;			//�߳�ID
	BOOL				m_bMultiHome;			//�Ƿ��IP
	CDWordArray			m_IPArr;
	ULONG				m_lSnifferSize;

	CWebSnifferDlg      *m_pDlg;

protected:
	friend UINT threadFunc (LPVOID p) ;
};

#endif