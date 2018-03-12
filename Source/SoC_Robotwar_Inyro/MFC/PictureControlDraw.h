#pragma once

#include <afxwin.h>
#include "../Source/Common/ImageManagement.h"

struct strStaticFrame {
	CDC*				pStaticFrameDC;		// 영상이 출력될 static Frame의 DC
	CRect				FrameRect;			// static Frame의 사이즈
};

// CPictureControlDraw view

class CPictureControlDraw
{
public:
	CPictureControlDraw();
	virtual ~CPictureControlDraw();

protected:
	// etc
	strStaticFrame		m_pStaticFrame;
	

public:
	IMG_INFO *m_image;

	void				setStaticFrame(CDC* pDC, CRect rect) { m_pStaticFrame.pStaticFrameDC = pDC;		m_pStaticFrame.FrameRect = rect; }
	HDC					getStaticFrameDC() { return m_pStaticFrame.pStaticFrameDC->m_hDC; }
	CRect				getStaticFrameRect() { return m_pStaticFrame.FrameRect; }

	void        Init_Image(int nWidth, int nHeight, int nChannels);
	void				DrawToHDC();
};