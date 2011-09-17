#include "stdafx.h"
#include "../include/HPngPixelFrame.h"

#define  IDS_ERRORLOADING  _T("����ͼ��ʧ��...(֧�ָ�ʽ:PSP��PNG��PCX��TGA��TIF)")


IMPLEMENT_DYNCREATE(HPngPixelFrame, CFrameWnd)

HPngPixelFrame::HPngPixelFrame()
{
	m_pImg = NULL;
	m_pImgDecoder = NULL;
}

HPngPixelFrame::~HPngPixelFrame()
{
	::ImgDeleteDIBSection(m_pImg);
	::ImgDeleteDecoder(m_pImgDecoder);
}


BEGIN_MESSAGE_MAP(HPngPixelFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_NCHITTEST()
	ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()

//�õ�alphaͨ��
void HPngPixelFrame::PreMultiplyRGBChannels(CBitmap &bmp, LPBYTE pBitmapBits)
{
	BITMAP bmpInfo;
	bmp.GetBitmap(&bmpInfo);
	for (int i = 0; i < bmpInfo.bmHeight; ++i)
	{
		BYTE *pbyPixel = pBitmapBits+bmpInfo.bmWidth*4*i;
		for (int j = 0; j < bmpInfo.bmWidth; ++j)
		{
			pbyPixel[0]= pbyPixel[0]*pbyPixel[3]/255;
			pbyPixel[1]= pbyPixel[1]*pbyPixel[3]/255;
			pbyPixel[2]= pbyPixel[2]*pbyPixel[3]/255;
			pbyPixel+= 4;
		}
	}
}

//alpha����
void HPngPixelFrame::DoUpdateDummyDialog(CWnd &DestWnd, CBitmap &bmp, BYTE SourceConstantAlpha/*= 255*/)
{
	//ȷ��������WS_EX_LAYERED���
	DestWnd.ModifyStyleEx(0, WS_EX_LAYERED);

	//�����ڴ�DC����ĻDC
	CDC dcScreen;
	CDC dcMemory;
	dcScreen.Attach(::GetDC(NULL));
	dcMemory.CreateCompatibleDC(&dcScreen);
	CBitmap *pOldBitmap= dcMemory.SelectObject(&bmp);

	//�õ�ͼ�����Ϣ
	BITMAP bmpInfo;
	bmp.GetBitmap(&bmpInfo);

	//�õ��ɴ��ڵĴ�С(���ǽ�����Ҫ���ڵ���ʼ��)
	CRect rectDlg;
	DestWnd.GetWindowRect(rectDlg);

	//�����´��ڵĴ�С
	CPoint ptWindowScreenPosition(rectDlg.TopLeft());
	CSize szWindow(bmpInfo.bmWidth, bmpInfo.bmHeight);

	OnRenderFace(&dcMemory);

	//��֯����
	BLENDFUNCTION blendPixelFunction= { AC_SRC_OVER, 0, SourceConstantAlpha, AC_SRC_ALPHA };
	CPoint ptSrc(0,0);//�ӻ����ڴ�DC��������Ļ����ʼ��
	//���»�ϴ���
	BOOL bRet= ::UpdateLayeredWindow(DestWnd, dcScreen, &ptWindowScreenPosition, &szWindow, dcMemory,
		&ptSrc, 0, &blendPixelFunction, ULW_ALPHA);
	ASSERT(bRet);

	//������Դ
	dcMemory.SelectObject(pOldBitmap);
}

void HPngPixelFrame::DoLoadFile( LPCTSTR pFilePath )
{
	ASSERT(this->GetSafeHwnd()!=NULL);
	HBITMAP  hBitmap;
	LPBYTE   pBitmapBits;
	LPVOID   pImg = NULL;
	if (!::ImgNewDIBFromFile(m_pImgDecoder, pFilePath, &pImg))
	{
		::ImgGetHandle(pImg, &hBitmap, (LPVOID *)&pBitmapBits);
		//�õ�bitmap�ṹ��Ϣ
		BITMAP bmpInfo;
		::GetObject(hBitmap, sizeof(BITMAP), &bmpInfo);
		if (bmpInfo.bmBitsPixel != 32)//���ͼ����32λ��
		{
			::ImgDeleteDIBSection(pImg);
			AfxMessageBox(IDS_ERRORLOADING);
			return;
		}
	}
	else
	{
		if (ERR_NOERROR != ::LoadPSPFileToDIB(pFilePath, hBitmap, (LPVOID *)&pBitmapBits))
		{
			AfxMessageBox(IDS_ERRORLOADING);
			return;
		}
	}

	::ImgDeleteDIBSection(m_pImg);
	m_pImg = pImg;
	//����CBitmap
	m_bmpPaint.DeleteObject();	
	m_bmpPaint.Attach(hBitmap);
	//�ȵõ�alphaͨ��
	PreMultiplyRGBChannels(m_bmpPaint, pBitmapBits);
	//���¶Ի���
	DoUpdateDummyDialog(*this, m_bmpPaint, 255);
}

void HPngPixelFrame::DoAlphaFrame( BYTE byAlpha/*=255*/ )
{
	if (m_bmpPaint.m_hObject)
		DoUpdateDummyDialog(*this, m_bmpPaint, byAlpha);
}

int HPngPixelFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	::ImgNewDecoder(&m_pImgDecoder);
	//�޸ķ��
	LONG Style= GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE); 
	Style = Style | WS_EX_APPWINDOW; 
	SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE, Style ); 
	ModifyStyle(0, WS_SYSMENU | WS_MINIMIZEBOX);
	return 0;
}

LRESULT HPngPixelFrame::OnNcHitTest(CPoint point)
{
	return HTCAPTION;
}

void HPngPixelFrame::OnRenderFace( CDC *pDC )
{
	CRect rectDlg;
	this->GetClientRect(rectDlg);
	CFont font;
	font.CreateFont(56,12,0,0,FW_BOLD,0,0,0,ANSI_CHARSET,0,0,0,0,_T("����"));
	CFont *pOldFont = pDC->SelectObject(&font);
	pDC->SetTextColor(RGB(255,0,0));
	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(_T("��������ļ�����!"),rectDlg,DT_CENTER);
	pDC->SelectObject(pOldFont);
	font.DeleteObject();
}