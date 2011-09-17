// XLSReaderDlg.h : 头文件
//

#pragma once


// CXLSReaderDlg 对话框
class CXLSReaderDlg : public CDialog
{
// 构造
public:
	CXLSReaderDlg(CWnd* pParent = NULL);	// 标准构造函数

	_RecordsetPtr pRecordset;

	void ReaderCell();

// 对话框数据
	enum { IDD = IDD_XLSREADER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	int m_nCount;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
