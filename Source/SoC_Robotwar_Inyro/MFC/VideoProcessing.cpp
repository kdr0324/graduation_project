#include "stdafx.h"

#include "VideoProcessing.h"

void CaptureCAM_VFW::init()
{
	memset(&caps, 0, sizeof(caps));
	capWnd = 0;
	hdr = 0;
	fourcc = 0;
	hic = 0;
	frame = NULL;
	width = height = -1;
	widthSet = heightSet = 0;
}

void CaptureCAM_VFW::closeHIC()
{
	if(hic) {
		ICDecompressEnd(hic);
		ICClose(hic);
		hic = 0;
	}
}


LRESULT PASCAL CaptureCAM_VFW::frameCallback(HWND hWnd, VIDEOHDR* hdr)
{
	CaptureCAM_VFW* capture = 0;

	if(!hWnd) return FALSE;

	capture = (CaptureCAM_VFW*)capGetUserData(hWnd);
	capture->hdr = hdr;

	return (LRESULT)TRUE;
}


// Initialize camera input
bool CaptureCAM_VFW::open(int wIndex)
{
	char szDeviceName[80];
	char szDeviceVersion[80];
	HWND hWndC = 0;

	close();

	if((unsigned)wIndex >= 10)
		wIndex = 0;
	
	for(; wIndex < 10; wIndex++) {
		if(capGetDriverDescription(wIndex, szDeviceName,
															 sizeof(szDeviceName), szDeviceVersion,
															 sizeof(szDeviceVersion))) {
			hWndC = capCreateCaptureWindow("My Own Capture Window",
																		 WS_POPUP | WS_CHILD, 0, 0, 320, 240, 0, 0);
			if(capDriverConnect(hWndC, wIndex))
				break;
			DestroyWindow(hWndC);
			hWndC = 0;
		}
	}

	if(hWndC) {
		capWnd = hWndC;
		hdr = 0;
		hic = 0;
		fourcc = (DWORD)-1;

		memset(&caps, 0, sizeof(caps));
		capDriverGetCaps(hWndC, &caps, sizeof(caps));
		CAPSTATUS status = {};
		capGetStatus(hWndC, &status, sizeof(status));
		::SetWindowPos(hWndC, NULL, 0, 0, status.uiImageWidth, status.uiImageHeight, SWP_NOZORDER | SWP_NOMOVE);
		capSetUserData(hWndC, (size_t)this);
		capSetCallbackOnFrame(hWndC, frameCallback);
		CAPTUREPARMS p;
		capCaptureGetSetup(hWndC, &p, sizeof(CAPTUREPARMS));
		p.dwRequestMicroSecPerFrame = 1000000 / 30; // 30 FPS
		capCaptureSetSetup(hWndC, &p, sizeof(CAPTUREPARMS));
		//capPreview( hWndC, 1 );
		capPreviewScale(hWndC, FALSE);
		capPreviewRate(hWndC, 1);

		// Get frame initial parameters.

		BITMAPINFO bmpInfo;

		capGetVideoFormat(capWnd, &bmpInfo, sizeof(BITMAPINFO));
		bmpInfo.bmiHeader.biBitCount = 24;
		bmpInfo.bmiHeader.biCompression = BI_RGB;
		if(!capSetVideoFormat(capWnd, &bmpInfo, sizeof(BITMAPINFO))) {
			// 설정에 실패하면 원래 값으로 되돌림
			capGetVideoFormat(capWnd, &bmpInfo, sizeof(BITMAPINFO));
		}

		BITMAPINFOHEADER& vfmt = bmpInfo.bmiHeader;
		widthSet = vfmt.biWidth;
		heightSet = vfmt.biHeight;
		fourcc = vfmt.biCompression;
		
		// And alternative way in case of failure.
		if(widthSet == 0 || heightSet == 0) {
			widthSet = status.uiImageWidth;
			heightSet = status.uiImageHeight;
		}

	}
	return capWnd != 0;
}


void CaptureCAM_VFW::close()
{
	if(capWnd) {
		capSetCallbackOnFrame(capWnd, NULL);
		capDriverDisconnect(capWnd);
		DestroyWindow(capWnd);
		closeHIC();
	}

	ReleaseImageInfo(&frame);

	init();
}


bool CaptureCAM_VFW::grabFrame()
{
	if(capWnd)
		return capGrabFrameNoStop(capWnd) == TRUE;

	return false;
}


IMG_INFO* CaptureCAM_VFW::retrieveFrame(int)
{
	BITMAPINFO vfmt;
	memset(&vfmt, 0, sizeof(vfmt));
	BITMAPINFOHEADER& vfmt0 = vfmt.bmiHeader;

	if(!capWnd)
		return 0;

	const DWORD sz = capGetVideoFormat(capWnd, &vfmt, sizeof(vfmt));
	const int prevWidth = frame ? frame->m_nWidth: 0;
	const int prevHeight = frame ? frame->m_nHeight : 0;

	if(!hdr || hdr->lpData == 0 || sz == 0)
		return 0;

	if(!frame || frame->m_nWidth != vfmt0.biWidth || frame->m_nHeight != vfmt0.biHeight) {
		ReleaseImageInfo(&frame);
		frame = CreateImageInfo(vfmt0.biWidth, vfmt0.biHeight, 3, D_U8);
	}
	
	//cvFlip(&src, frame, 0);
	memcpy(frame->m_pImgU8, hdr->lpData, sizeof(UCHAR) * frame->m_nImageSize);
	
	return frame;
}

IMG_INFO* CaptureCAM_VFW::queryFrame()
{
	if(grabFrame())
		return retrieveFrame(0);

	return NULL;
}


double CaptureCAM_VFW::getProperty(int property_id)
{
	switch(property_id) {
		case CAP_PROP_FRAME_WIDTH:
			return widthSet;
		case CAP_PROP_FRAME_HEIGHT:
			return heightSet;
		case CAP_PROP_FOURCC:
			return fourcc;
		case CAP_PROP_FPS:
		{
			CAPTUREPARMS params = {};
			if(capCaptureGetSetup(capWnd, &params, sizeof(params)))
				return 1e6 / params.dwRequestMicroSecPerFrame;
		}
		break;
		default:
			break;
	}
	return 0;
}

bool CaptureCAM_VFW::setProperty(int property_id, double value)
{
	bool handledSize = false;

	switch(property_id) {
		case CAP_PROP_FRAME_WIDTH:
			width = (int)(value + 0.5);
			handledSize = true;
			break;
		case CAP_PROP_FRAME_HEIGHT:
			height = int(value + 0.5);
			handledSize = true;
			break;
		case CAP_PROP_FOURCC:
			break;
		case CAP_PROP_FPS:
			if(value > 0) {
				CAPTUREPARMS params;
				if(capCaptureGetSetup(capWnd, &params, sizeof(params))) {
					params.dwRequestMicroSecPerFrame = int(1e6 / value + 0.5);
					return capCaptureSetSetup(capWnd, &params, sizeof(params)) == TRUE;
				}
			}
			break;
		default:
			break;
	}

	if(handledSize) {
		// If both width and height are set then change frame size.
		if(width > 0 && height > 0) {
			const DWORD size = capGetVideoFormatSize(capWnd);
			if(size == 0)
				return false;

			unsigned char *pbi = new unsigned char[size];
			if(!pbi)
				return false;

			if(capGetVideoFormat(capWnd, pbi, size) != size) {
				delete[]pbi;
				return false;
			}

			BITMAPINFOHEADER& vfmt = ((BITMAPINFO*)pbi)->bmiHeader;
			bool success = true;
			if(width != vfmt.biWidth || height != vfmt.biHeight) {
				// Change frame size.
				vfmt.biWidth = width;
				vfmt.biHeight = height;
				vfmt.biSizeImage = height * ((width * vfmt.biBitCount + 31) / 32) * 4;
				vfmt.biCompression = BI_RGB;
				success = capSetVideoFormat(capWnd, pbi, size) == TRUE;
			}
			if(success) {
				// Adjust capture window size.
				CAPSTATUS status = {};
				capGetStatus(capWnd, &status, sizeof(status));
				::SetWindowPos(capWnd, NULL, 0, 0, status.uiImageWidth, status.uiImageHeight, SWP_NOZORDER | SWP_NOMOVE);
				// Store frame size.
				widthSet = width;
				heightSet = height;
			}
			delete[]pbi;
			width = height = -1;

			return success;
		}

		return true;
	}

	return false;
}