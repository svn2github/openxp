#ifndef __HAUTHCODE_H__
#define __HAUTHCODE_H__
#pragma once

//验证码程序
class TEMPLATE_CONTROL_API HAuthCode : public CWnd
{
public:
	HAuthCode();
	~HAuthCode(){};

public:
	CString	GetNum();//获取结果
	bool InvalidateNum();//重新生成
	void SetValidTime(DWORD dwTime=60000);//有效时间

	//消息映射
protected:
	afx_msg void OnDestroy();
	afx_msg int	OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()

private:
	int				m_nRandomNum;				//随机数字
	int				m_nLineHeight;				//干扰线高
	DWORD			m_dwLastTime;				//生成时间
	DWORD			m_dwValidTime;				//有效时间
	CString			m_strResult;				//最后结果
	CPoint			m_ptNumPoint[6];			//保存配置
};

#endif	