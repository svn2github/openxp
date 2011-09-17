// XLSReaderDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "XLSReader.h"
#include "XLSReaderDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CXLSReaderDlg �Ի���




CXLSReaderDlg::CXLSReaderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CXLSReaderDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CXLSReaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CXLSReaderDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CXLSReaderDlg ��Ϣ�������






//��ȡ�ַ�������������������

void CXLSReaderDlg::ReaderCell()
{
	CFile file;
	file.Open("c:\\ret.xml",CFile::modeCreate|CFile::modeWrite);

	pRecordset.CreateInstance(__uuidof(Recordset));
	pRecordset->Open("select * from [Sheet1z$]",
		theApp.pCon_ex.GetInterfacePtr(),
		adOpenDynamic,
		adLockOptimistic,
		adCmdText);

	if(pRecordset->adoEOF) return;
	
	pRecordset->MoveFirst();
	while(!pRecordset->adoEOF)
	{
		{//��ȡ����
			CString result;
			for (long j = 1; j < pRecordset->Fields->Count; j++)
			{
				_variant_t t = _variant_t(long(j));
				_variant_t tRet = pRecordset->GetCollect(t);
				if (tRet.vt == VT_NULL) continue;
				result = (LPCSTR)_bstr_t(tRet);
				result+="\t\t\t";
				file.Write(result,result.GetLength());
			}
		}
		file.Write("\r\n",strlen("\r\n"));
		pRecordset->MoveNext();
	}
	pRecordset->Close();
	if(pRecordset) pRecordset.Release();

	file.Close();
}





BOOL CXLSReaderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	m_nCount = 0;
	
 	ReaderCell();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CXLSReaderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CXLSReaderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

