// GetWeatherDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GetWeather.h"
#include "GetWeatherDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGetWeatherDlg �Ի���



#include <afxinet.h>
bool getgooglewhearth(CString strAreaNum)
{
	CInternetSession   *pInetSession=NULL;  
	CHttpConnection   *pHttpConnection=NULL;  
	CHttpFile	*pHttpFile = NULL;
	CString baseUrl;
	//baseUrl.Format("http://www.google.co.uk/ig/api?weather=%s",strAreaNum);   //Ӣ��
	baseUrl.Format("http://www.google.com.hk/ig/api?weather=%s",strAreaNum);    //����
	try
	{
		pInetSession=new CInternetSession(AfxGetAppName(),1,PRE_CONFIG_INTERNET_ACCESS);  
		pHttpFile = (CHttpFile*)pInetSession->OpenURL(baseUrl);
		int uCount = 0;
		char buf[2048]={0};	
		uCount = pHttpFile->Read(buf,sizeof(buf));
		OutputDebugString(buf);
	}
	catch(CInternetException   *pEx)  
	{  
		pEx->Delete();  
		delete pInetSession;  
		return false;
	} 
	delete pInetSession;  
	return true;
}

CGetWeatherDlg::CGetWeatherDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGetWeatherDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGetWeatherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGetWeatherDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CGetWeatherDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CGetWeatherDlg ��Ϣ�������

BOOL CGetWeatherDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_htsWord.SetPart("");
	SetDlgItemText(IDC_EDIT2,"");
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CGetWeatherDlg::OnPaint()
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
HCURSOR CGetWeatherDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CGetWeatherDlg::OnBnClickedButton1()
{
	CString strPart;
	GetDlgItemText(IDC_EDIT2,strPart);
	m_htsWord.SetPart(strPart);

	CString strIn,strResult;
	GetDlgItemText(IDC_EDIT1,strIn);
	strResult = m_htsWord.ParseWords(strIn);
	SetDlgItemText(IDC_STATIC_RES,strResult.MakeLower());

	getgooglewhearth(strResult.MakeLower());
}
