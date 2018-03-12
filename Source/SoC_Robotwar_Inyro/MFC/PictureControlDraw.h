#pragma once

#include <afxwin.h>
#include "../Source/Common/ImageManagement.h"

struct strStaticFrame {
	CDC*				pStaticFrameDC;		// ������ ��µ� static Frame�� DC
	CRect				FrameRect;			// static Frame�� ������
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