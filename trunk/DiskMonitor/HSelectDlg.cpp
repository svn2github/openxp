#include "StdAfx.h"
#include "HSelectDlg.h"

HSelectDlg::HSelectDlg(CWnd* pParent) : CDialog(HSelectDlg::IDD, pParent)
{
}

HSelectDlg::~HSelectDlg()
{
}

void HSelectDlg::DoDataExchange( CDataExchange* pDX )
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(HSelectDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_YESALL, &HSelectDlg::OnBnClickedButtonYesall)
	ON_BN_CLICKED(IDC_BUTTON_NOALL, &HSelectDlg::OnBnClickedButtonNoall)
	ON_BN_CLICKED(IDOK, &HSelectDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL HSelectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_bAddNew = (::GetPrivateProfileInt("Selections","Add",0,".\\Settings.ini")==0 ?false:true);
	m_bDel = (::GetPrivateProfileInt("Selections","Delete",0,".\\Settings.ini")==0 ?false:true);
	m_bModify = (::GetPrivateProfileInt("Selections","Modify",0,".\\Settings.ini")==0 ?false:true);
	m_bOther = (::GetPrivateProfileInt("Selections","Rename",0,".\\Settings.ini")==0 ?false:true);
	m_bRename=(::GetPrivateProfileInt("Selections","Rename",0,".\\Settings.ini")==0 ?false:true);

	CButton *p = (CButton*)GetDlgItem(IDC_CHECK_ADD);
	p->SetCheck(m_bAddNew);

	p = (CButton*)GetDlgItem(IDC_CHECK_DELETE);
	p->SetCheck(m_bDel);

	p = (CButton*)GetDlgItem(IDC_CHECK_RENAME);
	p->SetCheck(m_bRename);

	p = (CButton*)GetDlgItem(IDC_CHECK_MODIFY);
	p->SetCheck(m_bModify);

	p = (CButton*)GetDlgItem(IDC_CHECK_OTHER);
	p->SetCheck(m_bOther);
	return TRUE;  
}

void HSelectDlg::OnBnClickedButtonYesall()
{
	CButton *p = (CButton*)GetDlgItem(IDC_CHECK_ADD);
	p->SetCheck(BST_CHECKED);

	p = (CButton*)GetDlgItem(IDC_CHECK_DELETE);
	p->SetCheck(BST_CHECKED);

	p = (CButton*)GetDlgItem(IDC_CHECK_RENAME);
	p->SetCheck(BST_CHECKED);

	p = (CButton*)GetDlgItem(IDC_CHECK_MODIFY);
	p->SetCheck(BST_CHECKED);

	p = (CButton*)GetDlgItem(IDC_CHECK_OTHER);
	p->SetCheck(BST_CHECKED);

	m_bAddNew = m_bDel = m_bModify = m_bOther = m_bRename = TRUE;
}

void HSelectDlg::OnBnClickedButtonNoall()
{
	CButton *p = (CButton*)GetDlgItem(IDC_CHECK_ADD);
	p->SetCheck(BST_UNCHECKED);

	p = (CButton*)GetDlgItem(IDC_CHECK_DELETE);
	p->SetCheck(BST_UNCHECKED);

	p = (CButton*)GetDlgItem(IDC_CHECK_RENAME);
	p->SetCheck(BST_UNCHECKED);

	p = (CButton*)GetDlgItem(IDC_CHECK_MODIFY);
	p->SetCheck(BST_UNCHECKED);

	p = (CButton*)GetDlgItem(IDC_CHECK_OTHER);
	p->SetCheck(BST_UNCHECKED);

	m_bAddNew = m_bDel = m_bModify = m_bOther = m_bRename = FALSE;
}

void HSelectDlg::OnBnClickedOk()
{
	CButton *p = (CButton*)GetDlgItem(IDC_CHECK_ADD);
	m_bAddNew = p->GetCheck();

	p = (CButton*)GetDlgItem(IDC_CHECK_DELETE);
	m_bDel = p->GetCheck();

	p = (CButton*)GetDlgItem(IDC_CHECK_RENAME);
	m_bRename = p->GetCheck();

	p = (CButton*)GetDlgItem(IDC_CHECK_MODIFY);
	m_bModify = p->GetCheck();

	p = (CButton*)GetDlgItem(IDC_CHECK_OTHER);
	m_bOther = p->GetCheck();

	OnOK();
}
