#include "stdafx.h"
#include "HScrollTip.h"

#define IDM_SCORLL 1121
static int g_nPaintX=0;

IMPLEMENT_DYNAMIC(HScrollTip, CWnd)

HScrollTip::HScrollTip()
{
	m_strScroll = "�¹�ϴȥ��ֻ�����յ����գ����µ�ȴ������Ĵ��죡";
	m_fontText.CreateFont(12,8,0,0,FW_NORMAL,FALSE,FALSE,FALSE,ANSI_CHARSET,3,2,1,2,_T("����"));
}

HScrollTip::~HScrollTip()
{
}


BEGIN_MESSAGE_MAP(HScrollTip, CWnd)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

//��ʱ��
void HScrollTip::OnTimer(UINT_PTR nIDEvent)
{
	g_nPaintX--;
	if (g_nPaintX < -1*(m_strScroll.GetLength()*8))
	{
		CRect rcClient;
		GetClientRect(&rcClient);
		g_nPaintX = rcClient.Width();
	}
	Invalidate();
	CWnd::OnTimer(nIDEvent);
}

//����
int HScrollTip::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	CString m_strEXE = GetExeModulePath(AfxGetInstanceHandle());
	TCHAR szBackImage[260] = {0};
	::GetPrivateProfileString("default","tip",_T("�����ڹؼ���"),szBackImage,sizeof(szBackImage),m_strEXE+"config.ini"); 
	m_strScroll.Format("%s",szBackImage);
	if (m_strScroll == "�����ڹؼ���")
		m_strScroll = "�¹�ϴȥ��ֻ�����յ����գ����µ�ȴ������Ĵ��죡";
	SetTimer(IDM_SCORLL,50,NULL);
	return 0;
}

//����
void HScrollTip::OnPaint()
{
	CPaintDC dc(this);
	CRect rcClient;
	GetClientRect(&rcClient);
	HMemDC memDC(&dc,rcClient);
	memDC->SetTextColor(RGB(128,128,0));
	CFont *pOldFont = memDC->SelectObject(&m_fontText);
	memDC->DrawText(m_strScroll,CRect(g_nPaintX,0,rcClient.Width(),rcClient.Height()),DT_LEFT);
	memDC->SelectObject(pOldFont);
}

//��������
BOOL HScrollTip::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}
