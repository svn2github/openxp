// XLSReaderDlg.h : ͷ�ļ�
//

#pragma once


// CXLSReaderDlg �Ի���
class CXLSReaderDlg : public CDialog
{
// ����
public:
	CXLSReaderDlg(CWnd* pParent = NULL);	// ��׼���캯��

	_RecordsetPtr pRecordset;

	void ReaderCell();

// �Ի�������
	enum { IDD = IDD_XLSREADER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	int m_nCount;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
