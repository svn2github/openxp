#include "stdafx.h"
#include "../include/HPngPixelFrame.h"

#define  IDS_ERRORLOADING  _T("载入图像失败...(支持格式:PSP、PNG、PCX、TGA、TIF)")


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

//得到alpha通道
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

//alpha窗口
void HPngPixelFrame::DoUpdateDummyDialog(CWnd &DestWnd, CBitmap &bmp, BYTE SourceConstantAlpha/*= 255*/)
{
	//确保窗口有WS_EX_LAYERED风格
	DestWnd.ModifyStyleEx(0, WS_EX_LAYERED);

	//创建内存DC及屏幕DC
	CDC dcScreen;
	CDC dcMemory;
	dcScreen.Attach(::GetDC(NULL));
	dcMemory.CreateCompatibleDC(&dcScreen);
	CBitmap *pOldBitmap= dcMemory.SelectObject(&bmp);

	//得到图像的信息
	BITMAP bmpInfo;
	bmp.GetBitmap(&bmpInfo);

	//得到旧窗口的大小(我们仅仅需要窗口的起始点)
	CRect rectDlg;
	DestWnd.GetWindowRect(rectDlg);

	//计算新窗口的大小
	CPoint ptWindowScreenPosition(rectDlg.TopLeft());
	CSize szWindow(bmpInfo.bmWidth, bmpInfo.bmHeight);

	OnRenderFace(&dcMemory);

	//组织数据
	BLENDFUNCTION blendPixelFunction= { AC_SRC_OVER, 0, SourceConstantAlpha, AC_SRC_ALPHA };
	CPoint ptSrc(0,0);//从缓冲内存DC拷贝到屏幕的起始点
	//更新混合窗口
	BOOL bRet= ::UpdateLayeredWindow(DestWnd, dcScreen, &ptWindowScreenPosition, &szWindow, dcMemory,
		&ptSrc, 0, &blendPixelFunction, ULW_ALPHA);
	ASSERT(bRet);

	//清理资源
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
		//得到bitmap结构信息
		BITMAP bmpInfo;
		::GetObject(hBitmap, sizeof(BITMAP), &bmpInfo);
		if (bmpInfo.bmBitsPixel != 32)//如果图像不是32位的
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
	//设置CBitmap
	m_bmpPaint.DeleteObject();	
	m_bmpPaint.Attach(hBitmap);
	//先得到alpha通道
	PreMultiplyRGBChannels(m_bmpPaint, pBitmapBits);
	//更新对话框
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
	//修改风格
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
	font.CreateFont(56,12,0,0,FW_BOLD,0,0,0,ANSI_CHARSET,0,0,0,0,_T("宋体"));
	CFont *pOldFont = pDC->SelectObject(&font);
	pDC->SetTextColor(RGB(255,0,0));
	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(_T("搜索歌词文件出错!"),rectDlg,DT_CENTER);
	pDC->SelectObject(pOldFont);
	font.DeleteObject();
}