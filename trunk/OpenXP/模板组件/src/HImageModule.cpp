#include "stdafx.h"
#include "../include/HImageModule.h"


//运算png的Alpha通道
void DealPngAlphaChannel( CImage *pImage )
{
	ASSERT(pImage!=NULL);
	ASSERT(!pImage->IsNull());
	for(int i = 0; i < pImage->GetWidth(); ++i)
	{
		for(int j = 0; j < pImage->GetHeight(); ++j)
		{
			unsigned char* pucColor = (unsigned char*)pImage->GetPixelAddress(i,j);
			pucColor[0] = pucColor[0]*pucColor[3]/255;
			pucColor[1] = pucColor[1]*pucColor[3]/255;
			pucColor[2] = pucColor[2]*pucColor[3]/255;
		}
	}
}

HRGN GetBitmapRgn(int nWidth,int nHeight,HBITMAP hbmp, COLORREF TransColor)
{
	HDC  hmemDC;
	//创建与传入DC兼容的临时DC
	hmemDC = ::CreateCompatibleDC(NULL);
	HBITMAP hOldBmp = (HBITMAP)::SelectObject(hmemDC,hbmp);

	//创建总的窗体区域，初始region为0
	HRGN hrgn;
	hrgn = ::CreateRectRgn(0,0,0,0);

	for(int y = 0;y < nHeight; y++)
	{
		HRGN rgnTemp; //保存临时region
		int iX = 0;
		do
		{
			//跳过透明色找到下一个非透明色的点.
			while(iX < nWidth  && ::GetPixel(hmemDC,iX, y) == TransColor)
				iX++;

			//记住这个起始点
			int iLeftX = iX;

			//寻找下个透明色的点
			while(iX < nWidth  && ::GetPixel(hmemDC,iX, y) != TransColor)
				++iX;

			//创建一个包含起点与重点间高为1像素的临时“region”
			rgnTemp = ::CreateRectRgn(iLeftX, y, iX, y+1);

			//合并到主"region".
			CombineRgn( hrgn,hrgn,rgnTemp, RGN_OR);

			//删除临时"region",否则下次创建时和出错
			::DeleteObject(rgnTemp);
		}while(iX < nWidth);
		iX = 0;
	}
	::SelectObject(hmemDC,hOldBmp);
	::DeleteDC(hmemDC);
	return hrgn;
}

void DrawTextString( CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos, int aph )
{
	//变量定义
	int nStringLength = lstrlen(pszString);
	int nXExcursion[8] = {1,1,1,0,-1,-1,-1,0};
	int nYExcursion[8] = {-1,0,1,1,1,0,-1,-1};
	LOGFONT		logfont;
	CFont		*oldfont, nowfont;

	oldfont = pDC->GetCurrentFont();

	oldfont->GetLogFont(&logfont);
	logfont.lfEscapement = 10*aph;
	nowfont.CreateFontIndirect(&logfont);
	oldfont = pDC->SelectObject(&nowfont);

	//绘画边框
	pDC->SetTextColor(crFrame);
	for (int i = 0; i < SafeCountArray(nXExcursion); i++)
	{
		pDC->TextOut(nXPos+nXExcursion[i],nYPos+nYExcursion[i],pszString,nStringLength);
	}

	//绘画字体
	pDC->SetTextColor(crText);
	pDC->TextOut(nXPos,nYPos,pszString,nStringLength);
	pDC->SelectObject(oldfont);

	return;
}