#include "Stdafx.h"
#include "../include/HAuthCode.h"
#include "math.h"

//��Ϣӳ��
BEGIN_MESSAGE_MAP(HAuthCode, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

//���캯��
HAuthCode::HAuthCode()
{
	m_dwValidTime		= 60000;
	m_nRandomNum		= 0;
	m_nLineHeight		= 0;
	m_dwLastTime		= 0;
	m_strResult			= TEXT("");
}

//��Чʱ��
void HAuthCode::SetValidTime(DWORD dwTime)
{
	m_dwValidTime	= dwTime;
}

//������֤��
bool HAuthCode::InvalidateNum()
{
	//��������
	srand((unsigned int)time(NULL)+rand()%10000);
	m_nRandomNum = (rand()*27)%2000000+rand()%2000000+rand()%2000000+rand()%2000000+rand()%2000000+rand()%2000000+
		rand()%2000000+rand()%2000000+rand()%2000000+rand()%2000000;

	//�ԼӴ���
	if ( rand()%2 ) m_nRandomNum+=454321;
	if ( m_nRandomNum < 0 ) m_nRandomNum = 429845;

	//�жϳ���
	m_strResult.Format(TEXT("%d"), m_nRandomNum);
	if ( m_strResult.GetLength() > 6 ) m_strResult = m_strResult.Right(6);

	//��������
	CRect rect;
	GetClientRect(&rect);
	for ( int i=0;i<6;i++ )
	{
		m_ptNumPoint[i].SetPoint(i*16, rand()%(rect.Height()-20)-rand()%8);
	}
	m_nLineHeight = rand()%(rect.Height()-10)+rand()%10;

	//ˢ�½���
	Invalidate();

	//��¼ʱ��
	m_dwLastTime = GetTickCount();

	return true;
}

//��ȡ���
CString HAuthCode::GetNum()
{
	CString strBuffer = TEXT("");
	if ( GetTickCount() - m_dwLastTime > m_dwValidTime )
	{
		//��ʱʧЧ
		strBuffer = TEXT("");
		InvalidateNum();
	}
	else
	{
		//�˴��ɽ�����ܣ��ٷ��ظ����õ���
		//TCHAR szResult[33]={0};
		//CMD5Encrypt::EncryptData(m_strResult.GetBuffer(),szResult);
		//strBuffer = szResult;
		 strBuffer = m_strResult.GetBuffer();
	}

	return strBuffer;
}

//�������
void HAuthCode::OnLButtonDown(UINT nFlags, CPoint point)
{
	//��������
	InvalidateNum();
}

//�����ӿ�
int HAuthCode::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if ( __super::OnCreate(lpCreateStruct) == -1 ) return -1;

	//�Զ�����
	InvalidateNum();

	return 1;
}

//���ƽӿ�
void HAuthCode::OnPaint()
{
	CPaintDC dc(this);

	//����DC
	CRect rect;
	GetClientRect(&rect);
	HMemDC mdc(&dc, rect);
	mdc.FillRect(&rect, &CBrush(RGB(255,255,255)));
	mdc.SetBkMode(TRANSPARENT);

	//���Ʊ߿�
	mdc.DrawFocusRect(&rect);

	//���Ƹ��ű���
	CFont DrawFont;
	DrawFont.CreateFont(-5,0,0,0,400,0,0,0,134,3,2,255,2,TEXT("����"));
	CFont* pOldFont = mdc.SelectObject(&DrawFont);
	for ( int i=0;i<200;i++ )
	{
		mdc.TextOut(rand()%(rect.Width()), rand()%(rect.Height()), TEXT("*"));
	}

	//�����֤��
	CFont DrawFont2;
	DrawFont2.CreateFont(-28,16,0,0,900,0,0,0,134,3,2,255,2,TEXT("����"));
	mdc.SelectObject(&DrawFont2);
	for(int i=0;i<m_strResult.GetLength();i++)
	{
		CString strBuf = m_strResult.Left(i+1);
		strBuf = strBuf.Right(1);
		mdc.TextOut(m_ptNumPoint[i].x, m_ptNumPoint[i].y, strBuf);
	}
	mdc.SelectObject(pOldFont);

	//���Ƹ�����
	mdc.MoveTo(0, m_nLineHeight);
	mdc.LineTo(rect.Width(), m_nLineHeight);
	mdc.MoveTo(0, m_nLineHeight+10);
	mdc.LineTo(rect.Width(), m_nLineHeight+10);
}

//���ٳ���
void HAuthCode::OnDestroy()
{
	CWnd::OnDestroy();
}
