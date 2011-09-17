#include "stdafx.h"
#include "../include/HEdit.h"


IMPLEMENT_DYNAMIC(HEdit, CEdit)

HEdit::HEdit()
{
	m_Font.CreateFont(14,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,ANSI_CHARSET,3,2,1,2,_T("����"));
	m_bForcus = false;
	m_colorFocus = RGB(0,0,0);
	m_colorKillFocus = RGB(124,124,124);
	m_colorBk = RGB(255,255,255);
	m_colorBord = RGB(160,213,238);
}

HEdit::~HEdit()
{
	if (((HFONT)m_Font.m_hObject) != NULL)
	{
		m_Font.DeleteObject();
	}
}


BEGIN_MESSAGE_MAP(HEdit, CEdit)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()



// HEdit ��Ϣ�������

BOOL HEdit::CreateSkinEdit( int nX,int nY,int nCX,CWnd *parent,UINT id )
{	
	if (Create(WS_VISIBLE|WS_CHILD|WS_BORDER,CRect(nX,nY,nX+nCX,nY+17),parent,id))
	{
		return TRUE;
	}
	return FALSE;
}

HBRUSH HEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
	pDC->SetBkMode(OPAQUE);
	pDC->SelectObject(&m_Font);
	HBRUSH hbr;
 	if (IsWindowEnabled())
 	{
 		if (m_bForcus)
 		{
 			hbr = CreateSolidBrush(m_colorBk);
 			pDC->SetTextColor(m_colorFocus);
 			pDC->SetBkColor(m_colorBk);
 			return hbr;
 		}
 		else 
 		{
 			hbr = CreateSolidBrush(m_colorBk);
 			pDC->SetTextColor(m_colorKillFocus);
 			pDC->SetBkColor(m_colorBk);
 			return hbr;
 		}
 	}
	return NULL;
}

void HEdit::OnSetFocus(CWnd* pOldWnd)
{
	m_bForcus=true;
	Invalidate();
	CEdit::OnSetFocus(pOldWnd);
}

void HEdit::OnKillFocus(CWnd* pNewWnd)
{
	m_bForcus=false;
	Invalidate();
	CEdit::OnKillFocus(pNewWnd);
}

BOOL HEdit::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void HEdit::OnPaint()
{
	CPaintDC dc(this);
	CRect rect;
	GetClientRect(&rect);
	dc.FillSolidRect(rect,m_colorBk);
	CPen pen;
	if (m_bForcus)
	{
		pen.CreatePen(PS_SOLID,1,m_colorBord);
	}
	else
	{
		pen.CreatePen(PS_SOLID,1,RGB(207,216,222));
	}
	dc.SelectObject(&pen);
	dc.Rectangle(rect);
	CString strText;
	int len = LineLength(LineIndex(0));
	if (len <= 0)
		return;
	GetLine(0, strText.GetBuffer(len), len);
	strText.ReleaseBuffer(len);
	dc.SelectObject(&m_Font);
	dc.SetBkMode(TRANSPARENT);
	if (!m_bForcus)
		dc.SetTextColor(m_colorKillFocus);
	else 
		dc.SetTextColor(m_colorFocus);
	if (!IsWindowEnabled())
		dc.SetTextColor(RGB(125,125,125));
	rect.left+=2;
	rect.bottom-=1;
	rect.right-=1;
	dc.DrawText(strText,rect,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
const int nMargin = 5;
#define SHIFTED 0x8000
HEditWnd::HEditWnd()
{
	m_ptCursor.y = m_ptTxtBegin.y = 0;
	m_ptTxtBegin.x = m_ptCursor.x = nMargin;
	m_strEdit = "";
	m_strSelect = "";
	m_nCharPos = 0;
	m_nShowChar = 0;
	m_nMaxShowChars = 0;
	m_nSelectBegin = 0;	
	m_nSelectEnd = 0;
	m_bLBtDown = false;
	m_ptFirst.x = m_ptFirst.y = 0;//m_ptSecond.x = m_ptSecond.y = 0;
	::memset(&m_tm, 0, sizeof(TEXTMETRIC));
	m_TxtFont.CreateFont(15, 0, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, OUT_TT_PRECIS, 
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS,
		"����");
	m_nShiftBegin = -1;
}

HEditWnd::~HEditWnd()
{
	if (m_TxtFont.GetSafeHandle())
		m_TxtFont.DeleteObject();
	if (m_BmpBK.GetSafeHandle())
		m_BmpBK.DeleteObject();
}

IMPLEMENT_DYNCREATE(HEditWnd, CWnd)

BEGIN_MESSAGE_MAP(HEditWnd, CWnd)
	//{{AFX_MSG_MAP(HEditWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_CHAR()
	ON_WM_GETDLGCODE()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// HEditWnd message handlers

void HEditWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{	
	CWnd::OnLButtonDown(nFlags, point);
	SetFocus();
	SetCapture();
	m_nCharPos = CharFromPos(point.x);			// �ӵ�ǰ����λ�û�ö�Ӧ�ַ������
	if ( m_nCharPos > m_strEdit.GetLength() )	// ���� m_nCharPos ��ȷ����ֵ���ᳬ���ַ����ĳ���
		m_nCharPos = m_strEdit.GetLength();

	m_ptCursor.x = PosFromChar(m_nCharPos);		// ���ù���λ��
	::SetCaretPos(m_ptCursor.x, m_ptCursor.y);

	m_bLBtDown = true;							// �����Ҽ����±�־λ

	m_ptFirst = point;
	m_strSelect = "";
	m_nSelectBegin = m_nSelectEnd = 0;
	m_nShiftBegin = m_nCharPos;//-1;
	Invalidate(FALSE);
}

void HEditWnd::OnPaint() 
{
	CPaintDC dc(this);
	CPen PenLine,*pOldPen;
	HBRUSH OldBrush;
	CRgn DrawRgn;
	CDC BKdc;
	CBitmap BmpMem, *pOldBmp;

	// ����������������ı���
	BKdc.CreateCompatibleDC(&dc);
	BmpMem.CreateCompatibleBitmap(&dc, m_rcWnd.Width(), m_rcWnd.Height());
	pOldBmp = (CBitmap *)BKdc.SelectObject(&BmpMem);
	BKdc.BitBlt(0, 0, m_rcWnd.Width(), m_rcWnd.Height(), &dc, 0, 0, SRCCOPY);

	// ���û��Ƶ�����
	PenLine.CreatePen(PS_SOLID, 1, RGB(135,155,200));
	pOldPen = (CPen *)BKdc.SelectObject(&PenLine);
	OldBrush = (HBRUSH)::SelectObject(BKdc.GetSafeHdc(),GetStockObject(NULL_BRUSH)); 
	BKdc.BeginPath();
	BKdc.RoundRect(m_rcWnd, CPoint(5, 5));
	BKdc.EndPath();
	DrawRgn.CreateFromPath(&BKdc);
	BKdc.SelectClipRgn(&DrawRgn, RGN_COPY);

	if ( m_BmpBK.GetSafeHandle() )		// �ж��Ƿ������˱���ͼƬ
	{	// ���Ʊ���
		CDC MemDC;
		MemDC.CreateCompatibleDC(&dc);
		CBitmap *pOldBmp = (CBitmap *)MemDC.SelectObject(&m_BmpBK);
		BKdc.BitBlt(m_rcWnd.left, m_rcWnd.top, m_rcWnd.Width(), m_rcWnd.Height(), &MemDC, 0, 0, SRCCOPY);
		MemDC.SelectObject(pOldBmp);
		MemDC.DeleteDC();
	}
	else	// ����ȱʡ����
		MakeBKGround(&BKdc, RGB(148, 182, 225), RGB(46, 74, 137), m_rcWnd);

	BKdc.SelectClipRgn(NULL, RGN_COPY);		// �ָ�DC�ֳ�
	DrawRgn.DeleteObject();					// ɾ������

	BKdc.RoundRect(m_rcWnd, CPoint(5, 5));	// ���ƴ���ı߿�
	BKdc.SelectObject(OldBrush);

	BKdc.SelectObject(pOldPen);				// �ָ�DC�ֳ�
	PenLine.DeleteObject();

	CFont *pOldFont;
	pOldFont = (CFont *)BKdc.SelectObject(&m_TxtFont);
	int nOldMode = BKdc.SetBkMode(TRANSPARENT);
	COLORREF clrOld = BKdc.SetTextColor(RGB(54,73,165));

	if ( m_strEdit.GetLength()>0 )		// ���ƴ����е��ı���Ϣ
	{
		CString strTemp;
		strTemp = m_strEdit.Right(m_strEdit.GetLength() - m_nShowChar);
		BKdc.TextOut(m_ptTxtBegin.x+2,m_ptTxtBegin.y+2,strTemp);
		BKdc.SetTextColor(RGB(42,56,127));
		BKdc.TextOut(m_ptTxtBegin.x+1,m_ptTxtBegin.y+1,strTemp);
		BKdc.SetTextColor(RGB(253,255,247));
		BKdc.TextOut(m_ptTxtBegin.x,m_ptTxtBegin.y,strTemp);
	}

	if ( m_strSelect.GetLength()>0 )	// ���Ʊ�ѡ���ַ���
	{
		BLENDFUNCTION bf;
		int nBeginPosX, nEndPosX;

		bf.BlendOp = AC_SRC_OVER;
		bf.BlendFlags = 0;
		bf.AlphaFormat = 0;
		bf.SourceConstantAlpha=  100;

		if ( m_nSelectBegin < m_nShowChar )
			nBeginPosX = PosFromChar(m_nShowChar);
		else
			nBeginPosX = PosFromChar(m_nSelectBegin);
		nEndPosX = PosFromChar(m_nSelectEnd);
		CDC MemDC;
		CBitmap BmpMask, *pOldBmp;
		MemDC.CreateCompatibleDC(&dc);
		BmpMask.CreateCompatibleBitmap(&dc, nEndPosX-nBeginPosX, m_tm.tmHeight);
		pOldBmp = (CBitmap *)MemDC.SelectObject(&BmpMask);
		MemDC.FillSolidRect(0,0,nEndPosX-nBeginPosX, m_tm.tmHeight,RGB(0,0,0));
		AlphaBlend(BKdc.GetSafeHdc(), nBeginPosX, m_ptTxtBegin.y, nEndPosX-nBeginPosX, m_tm.tmHeight, 
			MemDC.GetSafeHdc(), 0, 0, nEndPosX-nBeginPosX, m_tm.tmHeight,bf);
		MemDC.SelectObject(pOldBmp);
		MemDC.DeleteDC();
		BmpMask.DeleteObject();
	}

	dc.BitBlt(0, 0,  m_rcWnd.Width(), m_rcWnd.Height(), &BKdc, 0, 0, SRCCOPY);
	BKdc.SetTextColor(clrOld);
	BKdc.SetBkMode(nOldMode);
	BKdc.SelectObject(pOldFont);
	BKdc.SelectObject(pOldBmp);
	BmpMem.DeleteObject();
	BKdc.DeleteDC();
}

BOOL HEditWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style |= WS_TABSTOP;			// ���øô����ܹ���ù��

	return CWnd::PreCreateWindow(cs);
}

void HEditWnd::OnSetFocus(CWnd* pOldWnd) 
{
	// �����ý����Ժ����ù���λ��
	CWnd::OnSetFocus(pOldWnd);
	::CreateCaret(m_hWnd,(HBITMAP) NULL, 0, m_tm.tmHeight);
	::SetCaretPos(m_ptCursor.x, m_ptCursor.y); 
	::ShowCaret(m_hWnd);

}

void HEditWnd::OnKillFocus(CWnd* pNewWnd) 
{
	CWnd::OnKillFocus(pNewWnd);		// ����ʧȥ�����ʱ�����ٹ��
	::HideCaret(m_hWnd); 
	::DestroyCaret();

}
void HEditWnd::GetString(CString &strWnd)
{
	strWnd = m_strEdit;
}

void HEditWnd::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{	
	static bool bSpecial = false;
	::HideCaret(m_hWnd);
	switch(nChar) {
	case 8:			// ��Ӧ�û�ɾ������
		{
			if ( m_strSelect.GetLength()>0 )
			{
				m_strEdit.Delete(m_nSelectBegin, m_nSelectEnd - m_nSelectBegin);
				m_strSelect = "";
				m_nCharPos = m_nSelectBegin;
				m_ptCursor.x = PosFromChar(m_nCharPos);
				m_nSelectBegin = m_nSelectEnd = 0;
				m_nShiftBegin = -1;
				Invalidate(FALSE);
			}
			else if ( m_nCharPos>0 && m_strEdit.GetLength()>0 )
			{
				const char *cText = (const char *)m_strEdit;
				if ( (unsigned char)cText[m_nCharPos-1]>127 )
				{
					m_strEdit.Delete(m_nCharPos-2,2);
					m_nCharPos -= 2;
				}
				else
				{
					m_strEdit.Delete(m_nCharPos-1);
					m_nCharPos--;
				}
				if ( m_nShowChar>0 && (unsigned char)cText[m_nShowChar-1]>127 )
					m_nShowChar -= 2;
				else
					m_nShowChar--;

				if ( m_nShowChar<0 )
					m_nShowChar = 0;
				m_ptCursor.x = PosFromChar(m_nCharPos);
				Invalidate(FALSE);
			}
		}
		break;
	default:
		{
			if ( nChar<32 )
			{					// �˴�����Ctrl+C��Ctrl+V��Ctrl+X����
				switch(nChar) {
	case 3:
		{
			CopyString();
		}
		break;
	case 22:
		{
			PasteString();
		}
		break;
	case 24:
		{
			CutString();
		}
				}
			}
			else	// �˴�����һ�������ַ��Ĵ���
			{
				if ( m_strSelect.GetLength()>0 )
				{	// �����ǰ�б�ѡ�е��ַ������Ƚ���ɾ������
					m_strEdit.Delete(m_nSelectBegin, m_nSelectEnd - m_nSelectBegin);
					m_strSelect = "";
					m_nCharPos = m_nSelectBegin;
				}
				m_nSelectBegin = m_nSelectEnd = 0;
				m_nShiftBegin = -1;
				m_strEdit.Insert(m_nCharPos,nChar);
				m_nCharPos++;
				if ( nChar>127 )
				{
					if ( bSpecial )
					{
						while ( m_nCharPos - m_nShowChar>m_nMaxShowChars )
							m_nShowChar++;
						m_ptCursor.x = PosFromChar(m_nCharPos);
						Invalidate(FALSE);
						bSpecial = false;
					}
					else
					{
						bSpecial = true;
					}
				}
				else
				{
					const char	*cText = (const char *)m_strEdit;
					cText += m_nShowChar;
					while ( m_nCharPos - m_nShowChar>m_nMaxShowChars )
					{
						if ( ((unsigned char)*cText)>127 && 
							((unsigned char)*(cText+1))>127 )
						{
							m_nShowChar += 2;
							cText += 2;
						}
						else
						{
							m_nShowChar++;
							cText++;
						}
					}
					m_ptCursor.x = PosFromChar(m_nCharPos);
					Invalidate(FALSE);
				}
			}
		}
		break;
	}
	::ShowCaret(m_hWnd);
	::SetCaretPos(m_ptCursor.x, m_ptCursor.y); 
	CWnd::OnChar(nChar, nRepCnt, nFlags);
}

BOOL HEditWnd::PreTranslateMessage(MSG* pMsg) 
{	
	switch(pMsg->message)
	{	// ������̲���
	case WM_KEYDOWN:
		{
			switch(pMsg->wParam)
			{
			case VK_UP:
			case VK_LEFT:
				{
					int nVirtKey = GetKeyState(VK_SHIFT);
					if ( nVirtKey&SHIFTED )
					{
						if ( m_nShiftBegin<0 )
						{
							if ( m_strSelect.GetLength()>0 )
							{
								if ( m_nCharPos<m_nSelectEnd )
									m_nShiftBegin = m_nSelectEnd;
								else
									m_nShiftBegin = m_nSelectBegin;
							}
							else
							{
								m_nShiftBegin = m_nCharPos;
							}
						}
					}
					else
					{
						m_nShiftBegin = -1;
						m_strSelect = "";
					}
					::HideCaret(m_hWnd);
					const char *cText = (const char *)m_strEdit;
					if( m_nCharPos>0 )
					{
						if ( (unsigned char)cText[m_nCharPos-1]>127 )
						{
							m_nCharPos -= 2;
						}
						else
							m_nCharPos--;
					}
					while ( m_nCharPos - m_nShowChar<0 ) 
					{
						if ( (unsigned char)cText[m_nShowChar-1]>127 )
							m_nShowChar -= 2;
						else
							m_nShowChar--;
					}
					if ( nVirtKey&SHIFTED )
					{
						if ( m_nShiftBegin<m_nCharPos )
						{
							m_nSelectBegin = m_nShiftBegin;
							m_nSelectEnd = m_nCharPos;
						}
						else
						{
							m_nSelectBegin = m_nCharPos;
							m_nSelectEnd = m_nShiftBegin;
						}
						m_strSelect = m_strEdit.Mid(m_nSelectBegin, m_nSelectEnd-m_nSelectBegin);
					}
					m_ptCursor.x = PosFromChar(m_nCharPos);
					Invalidate(FALSE);
					::SetCaretPos(m_ptCursor.x, m_ptCursor.y);
					::ShowCaret(m_hWnd);
					return TRUE;
				}
			case VK_DOWN:
			case VK_RIGHT:
				{
					int nVirtKey = GetKeyState(VK_SHIFT);
					if ( nVirtKey&SHIFTED )
					{
						if ( m_nShiftBegin<0 )
						{
							if ( m_strSelect.GetLength()>0 )
							{
								if ( m_nCharPos<m_nSelectEnd )
									m_nShiftBegin = m_nSelectEnd;
								else
									m_nShiftBegin = m_nSelectBegin;
							}
							else
							{
								m_nShiftBegin = m_nCharPos;
							}
						}
					}
					else
					{
						m_nShiftBegin = -1;
						m_strSelect = "";
					}
					::HideCaret(m_hWnd);
					const char *cText = (const char *)m_strEdit;
					if( m_nCharPos<m_strEdit.GetLength() )
					{
						if ( (unsigned char)cText[m_nCharPos]>127 )
						{
							m_nCharPos += 2;
						}
						else
							m_nCharPos++;
					}				
					cText += m_nShowChar;
					while ( m_nCharPos - m_nShowChar>m_nMaxShowChars )
					{
						if ( ((unsigned char)*cText)>127 && ((unsigned char)*(cText+1))>127 )
						{
							m_nShowChar += 2;
							cText += 2;
						}
						else
						{
							m_nShowChar++;
							cText++;
						}
					}
					if ( nVirtKey&SHIFTED )
					{
						if ( m_nShiftBegin<m_nCharPos )
						{
							m_nSelectBegin = m_nShiftBegin;
							m_nSelectEnd = m_nCharPos;
						}
						else
						{
							m_nSelectBegin = m_nCharPos;
							m_nSelectEnd = m_nShiftBegin;
						}
						m_strSelect = m_strEdit.Mid(m_nSelectBegin, m_nSelectEnd-m_nSelectBegin);
					}
					m_ptCursor.x = PosFromChar(m_nCharPos);
					Invalidate(FALSE);
					::ShowCaret(m_hWnd);
					::SetCaretPos(m_ptCursor.x, m_ptCursor.y);
					return TRUE;
				}
			case VK_HOME:
				{
					int nVirtKey = GetKeyState(VK_SHIFT);
					if ( nVirtKey&SHIFTED )
					{
						if ( m_nShiftBegin<0 )
						{
							m_nSelectEnd = m_nCharPos;
							m_nShiftBegin = m_nCharPos;
						}
						else
							m_nSelectEnd = m_nShiftBegin;
						m_nSelectBegin = 0;
						m_strSelect = m_strEdit.Mid(m_nSelectBegin, m_nSelectEnd-m_nSelectBegin);
					}
					else
					{
						m_nShiftBegin = -1;
						m_strSelect = "";
						m_nSelectBegin = m_nSelectEnd = 0;
					}
					::HideCaret(m_hWnd);
					m_nCharPos = 0;
					m_nShowChar = 0;
					m_ptCursor.x = PosFromChar(m_nCharPos);
					Invalidate(FALSE);
					::ShowCaret(m_hWnd);
					::SetCaretPos(m_ptCursor.x, m_ptCursor.y);
					return TRUE;
				}
			case VK_END:
				{
					int nVirtKey = GetKeyState(VK_SHIFT);
					if ( nVirtKey&SHIFTED )
					{
						if ( m_nShiftBegin<0 )
						{
							m_nSelectBegin = m_nCharPos;
							m_nShiftBegin = m_nCharPos;
						}
						else
							m_nSelectBegin = m_nShiftBegin;
						m_nSelectEnd = m_strEdit.GetLength();
						m_strSelect = m_strEdit.Mid(m_nSelectBegin, m_nSelectEnd-m_nSelectBegin);
					}
					else
					{
						m_nShiftBegin = -1;
						m_strSelect = "";
						m_nSelectBegin = m_nSelectEnd = 0;
					}
					::HideCaret(m_hWnd);
					m_nCharPos = m_strEdit.GetLength();
					//////////////////////////////////////////////////////////////
					const char *cText = (const char *)m_strEdit;
					int nIndex = m_nCharPos-1;
					int nShowChars = 0;
					while( nShowChars<m_nMaxShowChars )
					{
						if ( (unsigned char)cText[nIndex]>127 )
						{
							nIndex -= 2;
							nShowChars += 2;
						}
						else
						{
							nIndex -= 1;
							nShowChars += 1;
						}
					}
					//////////////////////////////////////////////////////////////
					m_nShowChar = m_nCharPos - nShowChars;//m_nMaxShowChars;
					if ( m_nShowChar<0 )
						m_nShowChar = 0;
					m_ptCursor.x = PosFromChar(m_nCharPos);
					Invalidate(FALSE);
					::ShowCaret(m_hWnd);
					::SetCaretPos(m_ptCursor.x, m_ptCursor.y);
					return TRUE;
				}
			case VK_DELETE:
				{
					if ( m_strEdit.GetLength()>0 && m_nCharPos<m_strEdit.GetLength() )
					{
						const char *cText = (const char *)m_strEdit;
						if ( (unsigned char)cText[m_nCharPos]>127 )
						{
							m_strEdit.Delete(m_nCharPos,2);
						}
						else
							m_strEdit.Delete(m_nCharPos);
						Invalidate(FALSE);
					}
					break;
				}
			}
		}
		break;
	}
	return CWnd::PreTranslateMessage(pMsg);
}

UINT HEditWnd::OnGetDlgCode() 
{	
	return (CWnd::OnGetDlgCode() | DLGC_WANTCHARS );	// �˴�ʹ��CWnd�ܹ����OnChar()�¼�
}

// ���������ĺ���
void HEditWnd::MakeBKGround(CDC *pDC, COLORREF clrBegin, COLORREF clrEnd, CRect rect)
{
	CDC MemDC;
	int x1=0,y1=0;
	int x2=0,y2=0;
	int r1=GetRValue(clrBegin),g1=GetGValue(clrBegin),b1=GetBValue(clrBegin);
	int r2=GetRValue(clrEnd),g2=GetGValue(clrEnd),b2=GetBValue(clrEnd);
	MemDC.CreateCompatibleDC(pDC);

	if ( m_BmpBK.GetSafeHandle() )
		m_BmpBK.DeleteObject();

	m_BmpBK.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	CBitmap *pOldBmp = MemDC.SelectObject(&m_BmpBK);
	while( x1<rect.Width() && y1<rect.Height() )
	{
		if( y1<rect.Height()-1 )
			y1++;
		else
			x1++;

		if( x2<rect.Width()-1 )
			x2++;
		else
			y2++;

		int r,g,b;
		int i = x1+y1;
		r = r1 + (i * (r2-r1) / (rect.Width()+rect.Height()));
		g = g1 + (i * (g2-g1) / (rect.Width()+rect.Height()));
		b = b1 + (i * (b2-b1) / (rect.Width()+rect.Height()));

		CPen p(PS_SOLID,1,RGB(r,g,b));
		CPen *oldpen = MemDC.SelectObject(&p); 

		MemDC.MoveTo(x1,y1);
		MemDC.LineTo(x2,y2);

		MemDC.SelectObject(oldpen);
		p.DeleteObject();
	}
	pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &MemDC, 0, 0, SRCCOPY);
	MemDC.SelectObject(pOldBmp);
	MemDC.DeleteDC();
}


int HEditWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if ( CWnd::OnCreate(lpCreateStruct)==-1 )
		return -1;

	GetClientRect(m_rcWnd);
	CClientDC dc(this);
	CFont *pOldFont = dc.SelectObject(&m_TxtFont);
	dc.GetTextMetrics(&m_tm);
	m_ptTxtBegin.y = m_ptCursor.y = (m_rcWnd.Height() - m_tm.tmHeight) / 2;
	dc.SelectObject(pOldFont);
	m_nMaxShowChars = (m_rcWnd.Width() - nMargin) / m_tm.tmAveCharWidth;

	return 0;
}

void HEditWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	GetClientRect(m_rcWnd);	
	CClientDC dc(this);
	CFont *pOldFont = dc.SelectObject(&m_TxtFont);
	dc.GetTextMetrics(&m_tm);
	m_ptTxtBegin.y = m_ptCursor.y = (m_rcWnd.Height() - m_tm.tmHeight) / 2;
	dc.SelectObject(pOldFont);
	m_nMaxShowChars = (m_rcWnd.Width() - nMargin) / m_tm.tmAveCharWidth;
	if ( m_BmpBK.GetSafeHandle() )
		m_BmpBK.DeleteObject();
}

void HEditWnd::OnContextMenu(CWnd* pWnd, CPoint point) 
{	
}

void HEditWnd::CopyString()
{
	if ( m_strSelect.GetLength()<=0 )
		return;
	if ( OpenClipboard() )
	{
		EmptyClipboard();
		HGLOBAL hClipboardData;
		hClipboardData = GlobalAlloc(GMEM_DDESHARE, 
			m_strSelect.GetLength()+1);
		char * pchData;
		pchData = (char*)GlobalLock(hClipboardData);
		strcpy(pchData, LPCSTR(m_strSelect));
		GlobalUnlock(hClipboardData);
		SetClipboardData(CF_TEXT,hClipboardData);
		CloseClipboard();
	}
}

void HEditWnd::PasteString()
{
	if ( OpenClipboard() ) 
	{
		if ( ::IsClipboardFormatAvailable(CF_TEXT) || 
			::IsClipboardFormatAvailable(CF_OEMTEXT) )
		{
			::HideCaret(m_hWnd);
			HANDLE hClipboardData = GetClipboardData(CF_TEXT);
			char *pchData = (char*)GlobalLock(hClipboardData);
			if ( m_strSelect.GetLength()>0 )
			{
				m_strEdit.Delete(m_nSelectBegin, m_strSelect.GetLength());
				m_nCharPos = m_nSelectBegin;
				m_strSelect = "";
			}
			m_strSelect.Format("%s", pchData);
			GlobalUnlock(hClipboardData);
			m_strEdit.Insert(m_nCharPos,(LPCTSTR)m_strSelect);
			m_nCharPos += m_strSelect.GetLength();
			const char	*cText = (const char *)m_strEdit;
			cText += m_nShowChar;
			while ( m_nCharPos - m_nShowChar>m_nMaxShowChars )
			{
				if ( ((unsigned char)*cText)>127 && 
					((unsigned char)*(cText+1)) > 127 )
				{
					m_nShowChar += 2;
					cText += 2;
				}
				else
				{
					m_nShowChar++;
					cText++;
				}
			}
			m_ptCursor.x = PosFromChar(m_nCharPos);
			Invalidate(FALSE);
			::ShowCaret(m_hWnd);
			::SetCaretPos(m_ptCursor.x, m_ptCursor.y); 
			m_strSelect = "";
		}
		CloseClipboard();
	}
}

void HEditWnd::CutString()
{
	if ( m_strSelect.GetLength() <= 0)
		return;
	m_strEdit.Delete(m_nSelectBegin, m_strSelect.GetLength());
	CopyString();
	::HideCaret(m_hWnd);
	m_nCharPos = m_nSelectBegin;
	if ( m_nShowChar>0 )
	{
		m_nShowChar -= m_strSelect.GetLength();
		if ( m_nShowChar<0 )
			m_nShowChar = 0;
	}
	m_strSelect = "";
	m_ptCursor.x = PosFromChar(m_nCharPos);
	Invalidate(FALSE);
	::ShowCaret(m_hWnd);
	::SetCaretPos(m_ptCursor.x, m_ptCursor.y);
}

void HEditWnd::OnMouseMove(UINT nFlags, CPoint point) 
{	
	if ( m_bLBtDown )
	{
		::HideCaret(m_hWnd);
		if ( m_ptFirst.x>point.x )
		{
			m_nSelectBegin = CharFromPos(point.x);
			m_nSelectEnd = CharFromPos(m_ptFirst.x);
		}
		else
		{
			m_nSelectBegin = CharFromPos(m_ptFirst.x);
			m_nSelectEnd = CharFromPos(point.x);
		}

		if ( m_nSelectEnd>m_strEdit.GetLength() )
			m_nSelectEnd = m_strEdit.GetLength();
		else if ( m_nSelectEnd<0 )
			m_nSelectEnd = 0;

		if ( m_nSelectBegin>m_strEdit.GetLength() )
			m_nSelectBegin = m_strEdit.GetLength();
		else if ( m_nSelectBegin<0 )
			m_nSelectBegin = 0;

		if ( m_nSelectEnd-m_nSelectBegin>0 )
			m_strSelect = m_strEdit.Mid(m_nSelectBegin,m_nSelectEnd-m_nSelectBegin);

		m_nCharPos = CharFromPos(point.x);
		if ( m_nCharPos<0 )
			m_nCharPos = 0;
		else if ( m_nCharPos>m_strEdit.GetLength() )
			m_nCharPos = m_strEdit.GetLength();
		m_ptCursor.x = PosFromChar(m_nCharPos);
		Invalidate(FALSE);

		::ShowCaret(m_hWnd);
		::SetCaretPos(m_ptCursor.x, m_ptCursor.y);
	}
	CWnd::OnMouseMove(nFlags, point);
}

void HEditWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_bLBtDown = false;
	ReleaseCapture();
	CWnd::OnLButtonUp(nFlags, point);
}

int HEditWnd::PosFromChar(UINT nChar)
{
	CString strTemp = m_strEdit.Mid(m_nShowChar, nChar - m_nShowChar);
	CClientDC dc(this);
	CFont *pOldFont = dc.SelectObject(&m_TxtFont);
	CSize TestSize = dc.GetTextExtent(strTemp);
	dc.SelectObject(pOldFont);
	return (nMargin + TestSize.cx);
}

int HEditWnd::CharFromPos(int nPosX)
{
	CString strTemp = m_strEdit.Mid(m_nShowChar);
	int nCharIndex = m_nShowChar;
	int nStringLen = strTemp.GetLength();
	char	TempChar[4];
	int nBeginPos = nMargin;
	CSize TxtSize;
	const char	*cText = (const char *)strTemp;
	CClientDC dc(this);
	CFont *pOldFont = dc.SelectObject(&m_TxtFont);
	while( nStringLen>0 )
	{
		if ( ((unsigned char)*cText)>127 && 
			((unsigned char)*(cText+1))>127 )
		{
			strncpy(TempChar,cText,2);
			TempChar[2] = 0;
			cText += 2;
			nStringLen -= 2;
			nCharIndex += 2;
			TxtSize = dc.GetTextExtent(TempChar,2);
		}
		else
		{
			strncpy(TempChar,cText,1);
			TempChar[1] = 0;
			cText += 1;
			nStringLen -= 1;
			nCharIndex += 1;
			TxtSize = dc.GetTextExtent(TempChar,1);
		}
		if ( nBeginPos+TxtSize.cx>nPosX )
		{
			if ( nPosX-nBeginPos<nBeginPos+TxtSize.cx-nPosX )
				nCharIndex -= strlen(TempChar);
			break;
		}
		nBeginPos += TxtSize.cx;
	}
	dc.SelectObject(pOldFont);
	return nCharIndex;	
}