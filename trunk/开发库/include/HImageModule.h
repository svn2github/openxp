#ifndef __IMAGEMODULE__H__
#define __IMAGEMODULE__H__
#pragma once
#include <atlimage.h>

//������Alphaͨ����ͼ��
extern "C" void TEMPLATE_CONTROL_API DealPngAlphaChannel(CImage *pImage);

//�õ�λͼHRGN
extern "C" HRGN TEMPLATE_CONTROL_API GetBitmapRgn(int nWidth,int nHeight,HBITMAP hbmp,COLORREF cTrans);

//����������ת�Ƕ�����
extern "C"  void TEMPLATE_CONTROL_API DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos, int aph);

#endif