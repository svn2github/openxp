#include "stdafx.h"
#include "DlgAlterPW.h"

IMPLEMENT_DYNAMIC(CDlgAlterPW, CDialog)

CDlgAlterPW::CDlgAlterPW(CWnd* pParent) : CDialog(CDlgAlterPW::IDD, pParent)
{
}

CDlgAlterPW::~CDlgAlterPW()
{
}

void CDlgAlterPW::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CDlgAlterPW, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgAlterPW::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgAlterPW::OnBnClickedCancel)
END_MESSAGE_MAP()

BOOL CDlgAlterPW::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_edOldPW.CreateSkinEdit(90,30,100,this,1879);
	m_edNewPW.CreateSkinEdit(90,63,100,this,1880);
	m_edNewPWSure.CreateSkinEdit(90,96,100,this,1881);
	return TRUE; 
}

void CDlgAlterPW::OnBnClickedOk()
{
	CString strOldPW,strNewPW,strNewSure;
	m_edOldPW.GetWindowText(strOldPW);
	m_edNewPW.GetWindowText(strNewPW);
	m_edNewPWSure.GetWindowText(strNewSure);

	TCHAR szMd5PW[260] = {0};
	HMD5Encrypt::EncryptData(strOldPW,szMd5PW);
	CString strMd5OldPW;
	strMd5OldPW.Format("%s",szMd5PW);

	CFile file;
	file.Open(GetExeModulePath(AfxGetInstanceHandle())+"\\bak\\only.lj",CFile::modeRead);
	TCHAR szOnlyPW[260] = {0};
	file.Read(&szOnlyPW,sizeof(szOnlyPW));
	file.Close();
	CString strLogonPW;
	strLogonPW.Format("%s",szOnlyPW);

	if (strMd5OldPW == strLogonPW)
	{
		TCHAR szMd5NewPW[260] = {0},szMd5SurePW[260] = {0};
		//新密文
		HMD5Encrypt::EncryptData(strNewPW,szMd5NewPW);
		CString strMd5NewPW;
		strMd5NewPW.Format("%s",szMd5NewPW);
		//确认密文
		HMD5Encrypt::EncryptData(strNewSure,szMd5SurePW);
		CString strMd5SurePW;
		strMd5SurePW.Format("%s",szMd5SurePW);

		if (strNewPW != strNewSure)
		{
			AfxMessageBox("新密文与确认密文不一致！");
			return;
		}
		else
		{
			CFile file;
			file.Open(GetExeModulePath(AfxGetInstanceHandle())+"\\bak\\only.lj",CFile::modeCreate|CFile::modeWrite);
			file.Write(szMd5NewPW,sizeof(szMd5NewPW));
			file.Close();
		}
	}
	OnOK();
}

void CDlgAlterPW::OnBnClickedCancel()
{
	OnCancel();
}
