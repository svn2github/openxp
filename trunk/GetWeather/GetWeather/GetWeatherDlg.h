// GetWeatherDlg.h : ͷ�ļ�
//

#pragma once
#include "HWordToSpell.h"

// CGetWeatherDlg �Ի���
class CGetWeatherDlg : public CDialog
{
// ����
public:
	CGetWeatherDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_GETWEATHER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	HWordToSpell  m_htsWord;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
