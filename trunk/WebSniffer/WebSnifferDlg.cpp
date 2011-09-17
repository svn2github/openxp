// WebSnifferDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WebSniffer.h"
#include "WebSnifferDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWebSnifferDlg �Ի���




CWebSnifferDlg::CWebSnifferDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWebSnifferDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWebSnifferDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_INFO, m_ctrList);
}

BEGIN_MESSAGE_MAP(CWebSnifferDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CWebSnifferDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCE_SNIFFER, &CWebSnifferDlg::OnBnClickedSniffer)
END_MESSAGE_MAP()


// CWebSnifferDlg ��Ϣ�������

BOOL CWebSnifferDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	

	m_SniffMgr.SetRev(this);

	m_ctrList.InsertColumn(0,"����",LVCFMT_LEFT,525);
	m_ctrList.InsertColumn(0,"��С",LVCFMT_LEFT,80);
	m_ctrList.InsertColumn(0,"�˿�",LVCFMT_LEFT,40);
	m_ctrList.InsertColumn(0,"Ŀ�ĵ�ַ",LVCFMT_LEFT,100);
	m_ctrList.InsertColumn(0,"�˿�",LVCFMT_LEFT,40);
	m_ctrList.InsertColumn(0,"Դ��ַ",LVCFMT_LEFT,100);
	m_ctrList.InsertColumn(0,"Э��",LVCFMT_LEFT,50);

	::SendMessage(m_ctrList.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
		LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);


	GetDlgItem(IDOK)->EnableWindow(TRUE);
	GetDlgItem(IDCANCE_SNIFFER)->EnableWindow(FALSE);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CWebSnifferDlg::OnPaint()
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
HCURSOR CWebSnifferDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CWebSnifferDlg::OnBnClickedOk()
{
	GetDlgItem(IDCANCE_SNIFFER)->EnableWindow(TRUE);
	GetDlgItem(IDOK)->EnableWindow(FALSE);

	m_SniffMgr.StartSniffer();
}

void CWebSnifferDlg::OnBnClickedSniffer()
{
	m_SniffMgr.StopSniffer();

	m_ctrList.DeleteAllItems();

	GetDlgItem(IDCANCE_SNIFFER)->EnableWindow(FALSE);
	GetDlgItem(IDOK)->EnableWindow(TRUE);
}

void CWebSnifferDlg::AddData(CString s0,CString s1, CString s2, CString s3, CString s4, CString s5, CString s6)
{
	int index;

	index = m_ctrList.InsertItem(0,s0);
	m_ctrList.SetItem(index,1,LVIF_TEXT,s1, 0, 0, 0,0);
	m_ctrList.SetItem(index,2,LVIF_TEXT,s2, 0, 0, 0,0);
	m_ctrList.SetItem(index,3,LVIF_TEXT,s3, 0, 0, 0,0);
	m_ctrList.SetItem(index,4,LVIF_TEXT,s4, 0, 0, 0,0);
	m_ctrList.SetItem(index,5,LVIF_TEXT,s5, 0, 0, 0,0);
	m_ctrList.SetItem(index,6,LVIF_TEXT,s6, 0, 0, 0,0);

	SetDlgItemInt(IDC_STATIC_SIZE,m_SniffMgr.GetMgrSize());
}