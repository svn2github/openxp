#ifndef __IMAGEMODULE__H__
#define __IMAGEMODULE__H__
#pragma once
#include <atlimage.h>

//处理有Alpha通道的图像
extern "C" void TEMPLATE_CONTROL_API DealPngAlphaChannel(CImage *pImage);

//得到位图HRGN
extern "C" HRGN TEMPLATE_CONTROL_API GetBitmapRgn(int nWidth,int nHeight,HBITMAP hbmp,COLORREF cTrans);

//绘制任意旋转角度文字
extern "C"  void TEMPLATE_CONTROL_API DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos, int aph);

#endif