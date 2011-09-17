#ifndef __HPNGPIXELFRAME__H__
#define __HPNGPIXELFRAME__H__
#pragma once

class TEMPLATE_CONTROL_API HPngPixelFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(HPngPixelFrame)
public:
	HPngPixelFrame();          
	virtual ~HPngPixelFrame();

	//���ƺ���
public:
	void DoLoadFile(LPCTSTR pFilePath);//PSP (v6.0+, 24bit, 1 layer and uncompressed!)
									   //PNG
									   //PCX
									   //TGA
									   //TIF	
	void DoAlphaFrame(BYTE byAlpha=255); 

	virtual void OnRenderFace(CDC *pDC);

protected:
	void PreMultiplyRGBChannels(CBitmap &bmp, LPBYTE pBitmapBits);
	void DoUpdateDummyDialog(CWnd &DestWnd, CBitmap &bmp, BYTE SourceConstantAlpha=255);

	//��Ա����
protected:
	CBitmap					m_bmpPaint;//�����ͼ�ξ��
	LPVOID					m_pImg;
	LPVOID					m_pImgDecoder;

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnNcHitTest(CPoint point);
};

#endif