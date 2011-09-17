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
		CString strProto,		//类型
		CString strSource,		//源IP
		CString strSourPort,	//源端口
		CString strDest,		//目的IP	
		CString strDestPort,	//目的端口
		CString strSize,		//数据大小
		CString strData			//数据内容
		);
protected:
	DWORD				m_dwIPSource;			// IP in net format
	DWORD				m_dwIPHostSource;		// same IP in host format
	DWORD				m_dwIPHost;
	DWORD				m_dwIPCheckedHost;		
	SOCKET				m_socSniffer;			//嗅探SOCKET
	DWORD				m_dwThreadID;			//线程ID
	BOOL				m_bMultiHome;			//是否多IP
	CDWordArray			m_IPArr;
	ULONG				m_lSnifferSize;

	CWebSnifferDlg      *m_pDlg;

protected:
	friend UINT threadFunc (LPVOID p) ;
};

#endif