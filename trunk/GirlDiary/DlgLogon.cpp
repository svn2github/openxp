#include "stdafx.h"
#include "DlgLogon.h"

IMPLEMENT_DYNAMIC(CDlgLogon, CDialog)

CDlgLogon::CDlgLogon(CWnd* pParent) : CDialog(CDlgLogon::IDD, pParent)
{
}

CDlgLogon::~CDlgLogon()
{
}

void CDlgLogon::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CDlgLogon, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgLogon::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgLogon::OnBnClickedCancel)
END_MESSAGE_MAP()
BOOL CDlgLogon::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_edPassWord.CreateSkinEdit(60,20,100,this,1879);
	CFileFind ff;
	BOOL bCode = ff.FindFile(GetExeModulePath(AfxGetInstanceHandle())+"\\bak\\only.lj");
	if (!bCode)
	{
		OnCancel();
		AfxMessageBox("�����ļ��𻵣����������������");
		return FALSE;
	}
	return TRUE;  
}

void CDlgLogon::OnBnClickedOk()
{
	CString strPW;
	m_edPassWord.GetWindowText(strPW);
	if (strPW.IsEmpty())
	{
		AfxMessageBox("���벻��Ϊ�գ�");
		return;
	}
	
	TCHAR szMd5PW[260] = {0};
	HMD5Encrypt::EncryptData(strPW,szMd5PW);
	CFile file;
	file.Open(GetExeModulePath(AfxGetInstanceHandle())+"\\bak\\only.lj",CFile::modeRead);
	TCHAR szOnlyPW[260] = {0};
	file.Read(&szOnlyPW,sizeof(szOnlyPW));
	file.Close();
	
	//�Ƚ���������
	CString strFile,strInPut;
	strFile.Format("%s",szOnlyPW);
	strInPut.Format("%s",szMd5PW);
	
	if (strFile == strInPut)
	{
		OnOK();
	}
	else
		AfxMessageBox("���벻�ԣ�");
}

void CDlgLogon::OnBnClickedCancel()
{
	OnCancel();
}
