#include "stdafx.h"

#include "ImageConvert.h"

#include "../Amazon/Config.h"

#include "Memory.h"

#ifdef _WIN32
#define MAKE_RGB565(r,g,b) \
	(U16)((((r)& 0xf8) << 8) | (((g)& 0xfc) << 3) | (((b)& 0xf8) >> 3))
#else
#include "amazon2_sdk.h"
#endif

int RGB565toRGB888(IMG_INFO *rgb565, IMG_INFO *rgb888)
{
  int i, j;
  int nImageSize = rgb565->m_nImageSize;
  U16 *pRGB565;
  U8 *pRGB888;
  U16 lookUpValue;

  int sWidth = rgb565->m_nWidth;
  int sHeight = rgb565->m_nHeight;  
  int sWStep = rgb565->m_nWStep;
  int dWStep;
  
  SetImageInfo(rgb888, sWidth, sHeight, 3, D_U8);
  dWStep = rgb888->m_nWStep;
  

  for (i = 0; i < sHeight; ++i) {
	  pRGB565 = &rgb565->m_pImgU16[i * sWStep];
	  pRGB888 = &rgb888->m_pImgU8[i * dWStep];
	  for (j = 0; j < sWidth; j++) {
		  lookUpValue = pRGB565[j];
		  pRGB888[j * 3 + 2] = r_lookup[lookUpValue];
		  pRGB888[j * 3 + 1] = g_lookup[lookUpValue];
		  pRGB888[j * 3 + 0] = b_lookup[lookUpValue];
	  }
  }

  return 0;
}

int RGB888toRGB565(IMG_INFO *rgb888, IMG_INFO *rgb565)
{
	int i;
	int nImageSize;
	U16 *pRGB565;
	U8 *pRGB888;

	int sWidth = rgb888->m_nWidth;
	int sHeight = rgb888->m_nHeight;

	SetImageInfo(rgb565, sWidth, sHeight, 1, D_U16);
	nImageSize = rgb565->m_nImageSize;
	pRGB565 = rgb565->m_pImgU16;
	pRGB888 = rgb888->m_pImgU8;

	for (i = nImageSize - 1; i >= 0; --i) {
		pRGB565[i] = ((pRGB888[i * 3 + 2] >> 3) << 11) + ((pRGB888[i * 3 + 1] >> 2) << 5) + (pRGB888[i * 3 + 0] >> 3);	
	}

	return 0;
}

// RGB565를 HSI로 변환
int RGB565toHSI(IMG_INFO *rgb565, IMG_INFO *hImg, IMG_INFO *sImg, IMG_INFO *iImg)
{
	int i;
	int nImageSize = rgb565->m_nImageSize;
	U16 *pRGB565;
	U8 *pHimg, *pSimg, *pIimg;
	U16 lookUpValue;

	int sWidth = rgb565->m_nWidth;
	int sHeight = rgb565->m_nHeight;

	SetImageInfo(hImg, sWidth, sHeight, 1, D_U8);
	SetImageInfo(sImg, sWidth, sHeight, 1, D_U8);
	SetImageInfo(iImg, sWidth, sHeight, 1, D_U8);

	pRGB565 = rgb565->m_pImgU16;
	pHimg = hImg->m_pImgU8;
	pSimg = sImg->m_pImgU8;
	pIimg = iImg->m_pImgU8;

	for(i = nImageSize - 1; i >= 0; --i) {
		lookUpValue = pRGB565[i];

		pHimg[i] = h_lookup[lookUpValue];
		pSimg[i] = s_lookup[lookUpValue];
		pIimg[i] = i_lookup[lookUpValue];
	}

	return 0;
}

int RGB565toHS(IMG_INFO *rgb565, IMG_INFO *hImg, IMG_INFO *sImg)
{
	int i;
	int nImageSize = rgb565->m_nImageSize;
	U16 *pRGB565;
	U8 *pHimg, *pSimg;
	U16 lookUpValue;

	int sWidth = rgb565->m_nWidth;
	int sHeight = rgb565->m_nHeight;

	SetImageInfo(hImg, sWidth, sHeight, 1, D_U8);
	SetImageInfo(sImg, sWidth, sHeight, 1, D_U8);

	pRGB565 = rgb565->m_pImgU16;
	pHimg = hImg->m_pImgU8;
	pSimg = sImg->m_pImgU8;

	for(i = nImageSize - 1; i >= 0; --i) {
		lookUpValue = pRGB565[i];

		pHimg[i] = h_lookup[lookUpValue];
		pSimg[i] = s_lookup[lookUpValue];
	}

	return 0;
}

int RGB565toHI(IMG_INFO *rgb565, IMG_INFO *hImg, IMG_INFO *iImg)
{
	int i;
	int nImageSize = rgb565->m_nImageSize;
	U16 *pRGB565;
	U8 *pHimg, *pIimg;
	U16 lookUpValue;

	int sWidth = rgb565->m_nWidth;
	int sHeight = rgb565->m_nHeight;

	SetImageInfo(hImg, sWidth, sHeight, 1, D_U8);
	SetImageInfo(iImg, sWidth, sHeight, 1, D_U8);

	pRGB565 = rgb565->m_pImgU16;
	pHimg = hImg->m_pImgU8;
	pIimg = iImg->m_pImgU8;

	for(i = nImageSize - 1; i >= 0; --i) {
		lookUpValue = pRGB565[i];

		pHimg[i] = h_lookup[lookUpValue];
		pIimg[i] = i_lookup[lookUpValue];
	}

	return 0;
}

int RGB565toSI(IMG_INFO *rgb565, IMG_INFO *sImg, IMG_INFO *iImg)
{
	int i;
	int nImageSize = rgb565->m_nImageSize;
	U16 *pRGB565;
	U8 *pSimg, *pIimg;
	U16 lookUpValue;

	int sWidth = rgb565->m_nWidth;
	int sHeight = rgb565->m_nHeight;

	SetImageInfo(sImg, sWidth, sHeight, 1, D_U8);
	SetImageInfo(iImg, sWidth, sHeight, 1, D_U8);

	pRGB565 = rgb565->m_pImgU16;
	pSimg = sImg->m_pImgU8;
	pIimg = iImg->m_pImgU8;

	for(i = nImageSize - 1; i >= 0; --i) {
		lookUpValue = pRGB565[i];

		pSimg[i] = s_lookup[lookUpValue];
		pIimg[i] = i_lookup[lookUpValue];
	}

	return 0;
}

int RGB565toH(IMG_INFO *rgb565, IMG_INFO *hImg)
{
	int i;
	int nImageSize = rgb565->m_nImageSize;
	U16 *pRGB565; 
	U8 *pHimg;

	int sWidth = rgb565->m_nWidth;
	int sHeight = rgb565->m_nHeight;

	SetImageInfo(hImg, sWidth, sHeight, 1, D_U8);	

	pRGB565 = rgb565->m_pImgU16;
	pHimg = hImg->m_pImgU8;

	for(i = nImageSize - 1; i >= 0; --i) {
		pHimg[i] = h_lookup[pRGB565[i]];
	}

	return 0;
}

int RGB565toS(IMG_INFO *rgb565, IMG_INFO *sImg)
{
	int i;
	int nImageSize = rgb565->m_nImageSize;
	U16 *pRGB565;
	U8 *pSimg;

	int sWidth = rgb565->m_nWidth;
	int sHeight = rgb565->m_nHeight;

	SetImageInfo(sImg, sWidth, sHeight, 1, D_U8);

	pRGB565 = rgb565->m_pImgU16;
	pSimg = sImg->m_pImgU8;

	for(i = nImageSize - 1; i >= 0; --i) {
		pSimg[i] = s_lookup[pRGB565[i]];
	}

	return 0;
}

int RGB565toI(IMG_INFO *rgb565, IMG_INFO *iImg)
{
	int i;
	int nImageSize = rgb565->m_nImageSize;
	U16 *pRGB565;
	U8 *pIimg;

	int sWidth = rgb565->m_nWidth;
	int sHeight = rgb565->m_nHeight;

	SetImageInfo(iImg, sWidth, sHeight, 1, D_U8);

	pRGB565 = rgb565->m_pImgU16;
	pIimg = iImg->m_pImgU8;

	for(i = nImageSize - 1; i >= 0; --i) {
		pIimg[i] = i_lookup[pRGB565[i]];
	}

	return 0;
}

int MergeOriginal(IMG_INFO *result, IMG_INFO *original)
{
	int i;
	int nImageSize = original->m_nImageSize;
	U16 *pOri = original->m_pImgU16;
	U16 *pResult = result->m_pImgU16;

	for(i = nImageSize - 1; i >= 0; --i) {
		if(pResult[i] == BLACK_16) {
			pResult[i] = pOri[i];
		}
	}

	return 0;
}

int DiffOriginal(IMG_INFO *result, IMG_INFO *original)
{
	int i;
	int nImageSize = original->m_nImageSize;
	U16 *pOri = original->m_pImgU16;
	U16 *pResult = result->m_pImgU16;

	for (i = nImageSize - 1; i >= 0; --i) {
		if (pOri[i] != BLACK_16 && pResult[i] != BLACK_16) {
			pResult[i] = BLACK_16;
		}
	}

	return 0;
}

// LCD를 위해 gray를 rgb565로 변환
int GrayToRGB565(IMG_INFO *gray, IMG_INFO *rgb565)
{
	int i;
	int nImageSize;
	unsigned short *pRGB565;
	unsigned char *pGray;
	unsigned char grayValue;

	int sWidth = gray->m_nWidth;
	int sHeight = gray->m_nHeight;

	SetImageInfo(hImg, sWidth, sHeight, 1, D_U16);

	nImageSize = rgb565->m_nImageSize;
	pRGB565 = rgb565->m_pImgU16;
	pGray = gray->m_pImgU8;

	for(i = nImageSize - 1; i >= 0; --i) {
		grayValue = pGray[i];
		pRGB565[i] = MAKE_RGB565(grayValue, grayValue, grayValue);
	}

	return 0;
}
