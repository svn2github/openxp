#ifndef __HAUTHCODE_H__
#define __HAUTHCODE_H__
#pragma once

//��֤�����
class TEMPLATE_CONTROL_API HAuthCode : public CWnd
{
public:
	HAuthCode();
	~HAuthCode(){};

public:
	CString	GetNum();//��ȡ���
	bool InvalidateNum();//��������
	void SetValidTime(DWORD dwTime=60000);//��Чʱ��

	//��Ϣӳ��
protected:
	afx_msg void OnDestroy();
	afx_msg int	OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()

private:
	int				m_nRandomNum;				//�������
	int				m_nLineHeight;				//�����߸�
	DWORD			m_dwLastTime;				//����ʱ��
	DWORD			m_dwValidTime;				//��Чʱ��
	CString			m_strResult;				//�����
	CPoint			m_ptNumPoint[6];			//��������
};

#endif	