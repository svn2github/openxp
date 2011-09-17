#include "Stdafx.h"
#include "../include/HAuthCode.h"
#include "math.h"

//消息映射
BEGIN_MESSAGE_MAP(HAuthCode, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

//构造函数
HAuthCode::HAuthCode()
{
	m_dwValidTime		= 60000;
	m_nRandomNum		= 0;
	m_nLineHeight		= 0;
	m_dwLastTime		= 0;
	m_strResult			= TEXT("");
}

//有效时间
void HAuthCode::SetValidTime(DWORD dwTime)
{
	m_dwValidTime	= dwTime;
}

//生成验证码
bool HAuthCode::InvalidateNum()
{
	//生成数字
	srand((unsigned int)time(NULL)+rand()%10000);
	m_nRandomNum = (rand()*27)%2000000+rand()%2000000+rand()%2000000+rand()%2000000+rand()%2000000+rand()%2000000+
		rand()%2000000+rand()%2000000+rand()%2000000+rand()%2000000;

	//稍加处理
	if ( rand()%2 ) m_nRandomNum+=454321;
	if ( m_nRandomNum < 0 ) m_nRandomNum = 429845;

	//判断长度
	m_strResult.Format(TEXT("%d"), m_nRandomNum);
	if ( m_strResult.GetLength() > 6 ) m_strResult = m_strResult.Right(6);

	//生成配置
	CRect rect;
	GetClientRect(&rect);
	for ( int i=0;i<6;i++ )
	{
		m_ptNumPoint[i].SetPoint(i*16, rand()%(rect.Height()-20)-rand()%8);
	}
	m_nLineHeight = rand()%(rect.Height()-10)+rand()%10;

	//刷新界面
	Invalidate();

	//记录时间
	m_dwLastTime = GetTickCount();

	return true;
}

//获取结果
CString HAuthCode::GetNum()
{
	CString strBuffer = TEXT("");
	if ( GetTickCount() - m_dwLastTime > m_dwValidTime )
	{
		//超时失效
		strBuffer = TEXT("");
		InvalidateNum();
	}
	else
	{
		//此处可将其加密，再返回给调用的类
		//TCHAR szResult[33]={0};
		//CMD5Encrypt::EncryptData(m_strResult.GetBuffer(),szResult);
		//strBuffer = szResult;
		 strBuffer = m_strResult.GetBuffer();
	}

	return strBuffer;
}

//点击生成
void HAuthCode::OnLButtonDown(UINT nFlags, CPoint point)
{
	//重新生成
	InvalidateNum();
}

//创建接口
int HAuthCode::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if ( __super::OnCreate(lpCreateStruct) == -1 ) return -1;

	//自动生成
	InvalidateNum();

	return 1;
}

//绘制接口
void HAuthCode::OnPaint()
{
	CPaintDC dc(this);

	//设置DC
	CRect rect;
	GetClientRect(&rect);
	HMemDC mdc(&dc, rect);
	mdc.FillRect(&rect, &CBrush(RGB(255,255,255)));
	mdc.SetBkMode(TRANSPARENT);

	//绘制边框
	mdc.DrawFocusRect(&rect);

	//绘制干扰背景
	CFont DrawFont;
	DrawFont.CreateFont(-5,0,0,0,400,0,0,0,134,3,2,255,2,TEXT("宋体"));
	CFont* pOldFont = mdc.SelectObject(&DrawFont);
	for ( int i=0;i<200;i++ )
	{
		mdc.TextOut(rand()%(rect.Width()), rand()%(rect.Height()), TEXT("*"));
	}

	//输出验证码
	CFont DrawFont2;
	DrawFont2.CreateFont(-28,16,0,0,900,0,0,0,134,3,2,255,2,TEXT("宋体"));
	mdc.SelectObject(&DrawFont2);
	for(int i=0;i<m_strResult.GetLength();i++)
	{
		CString strBuf = m_strResult.Left(i+1);
		strBuf = strBuf.Right(1);
		mdc.TextOut(m_ptNumPoint[i].x, m_ptNumPoint[i].y, strBuf);
	}
	mdc.SelectObject(pOldFont);

	//绘制干扰线
	mdc.MoveTo(0, m_nLineHeight);
	mdc.LineTo(rect.Width(), m_nLineHeight);
	mdc.MoveTo(0, m_nLineHeight+10);
	mdc.LineTo(rect.Width(), m_nLineHeight+10);
}

//销毁程序
void HAuthCode::OnDestroy()
{
	CWnd::OnDestroy();
}
