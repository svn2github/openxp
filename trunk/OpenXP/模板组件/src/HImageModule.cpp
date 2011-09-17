#include "stdafx.h"
#include "../include/HImageModule.h"


//����png��Alphaͨ��
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
	//�����봫��DC���ݵ���ʱDC
	hmemDC = ::CreateCompatibleDC(NULL);
	HBITMAP hOldBmp = (HBITMAP)::SelectObject(hmemDC,hbmp);

	//�����ܵĴ������򣬳�ʼregionΪ0
	HRGN hrgn;
	hrgn = ::CreateRectRgn(0,0,0,0);

	for(int y = 0;y < nHeight; y++)
	{
		HRGN rgnTemp; //������ʱregion
		int iX = 0;
		do
		{
			//����͸��ɫ�ҵ���һ����͸��ɫ�ĵ�.
			while(iX < nWidth  && ::GetPixel(hmemDC,iX, y) == TransColor)
				iX++;

			//��ס�����ʼ��
			int iLeftX = iX;

			//Ѱ���¸�͸��ɫ�ĵ�
			while(iX < nWidth  && ::GetPixel(hmemDC,iX, y) != TransColor)
				++iX;

			//����һ������������ص���Ϊ1���ص���ʱ��region��
			rgnTemp = ::CreateRectRgn(iLeftX, y, iX, y+1);

			//�ϲ�����"region".
			CombineRgn( hrgn,hrgn,rgnTemp, RGN_OR);

			//ɾ����ʱ"region",�����´δ���ʱ�ͳ���
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
	//��������
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

	//�滭�߿�
	pDC->SetTextColor(crFrame);
	for (int i = 0; i < SafeCountArray(nXExcursion); i++)
	{
		pDC->TextOut(nXPos+nXExcursion[i],nYPos+nYExcursion[i],pszString,nStringLength);
	}

	//�滭����
	pDC->SetTextColor(crText);
	pDC->TextOut(nXPos,nYPos,pszString,nStringLength);
	pDC->SelectObject(oldfont);

	return;
}