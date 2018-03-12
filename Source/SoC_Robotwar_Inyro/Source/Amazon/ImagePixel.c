#include "stdafx.h"

#include "imagePixel.h"

// 원하는 색이면 FORWARD 아니면 BLACK
int binary_HSI(HSIVALUE *src, IMG_INFO *dest, const U16 FORWARD)
{
	int i;

	int hMax = src->h.max, hMin = src->h.min;
	int sMax = src->s.max, sMin = src->s.min;
	int iMax = src->i.max, iMin = src->i.min;

	U16 lookUpH[180], lookUpS[256], lookUpI[256];

	U8 *pH = src->hImg->m_pImgU8;
	U8 *pS = src->sImg->m_pImgU8;
	U8 *pI = src->iImg->m_pImgU8;
	U16 *pDest = dest->m_pImgU16;

	int imageSize = src->hImg->m_nImageSize;
	int sWidth = src->hImg->m_nWidth;
	int sHeight = src->hImg->m_nHeight;

	SetImageInfo(dest, sWidth, sHeight, 1, D_U16);

	if(hMax > hMin) {
		for(i = 179; i > hMax; --i) {
			lookUpH[i] = BLACK_16;
		}
		for(i = hMax; i > hMin; --i) {
			lookUpH[i] = FORWARD;
		}
		for(i = hMin; i >= 0; --i) {
			lookUpH[i] = BLACK_16;
		}
	} else {
		for(i = 179; i > hMin; --i) {
			lookUpH[i] = FORWARD;
		}
		for(i = hMin; i > hMax; --i) {
			lookUpH[i] = BLACK_16;
		}
		for(i = hMax; i >= 0; --i) {
			lookUpH[i] = FORWARD;
		}
	}

	for(i = 255; i > sMax; --i) {
		lookUpS[i] = BLACK_16;
	}
	for(i = sMax; i > sMin; --i) {
		lookUpS[i] = FORWARD;
	}
	for(i = sMin; i >= 0; --i) {
		lookUpS[i] = BLACK_16;
	}

	for(i = 255; i > iMax; --i) {
		lookUpI[i] = BLACK_16;
	}
	for(i = iMax; i > iMin; --i) {
		lookUpI[i] = FORWARD;
	}
	for(i = iMin; i >= 0; --i) {
		lookUpI[i] = BLACK_16;
	}

	for(i = imageSize - 1; i >= 0; --i) {
		pDest[i] = lookUpH[pH[i]] & lookUpS[pS[i]] & lookUpI[pI[i]];
	}

	return 0;
}

int binary_HS(HSVALUE *src, IMG_INFO *dest, const U16 FORWARD)
{
	int i;

	int hMax = src->h.max, hMin = src->h.min;
	int sMax = src->s.max, sMin = src->s.min;

	U16 lookUpH[180], lookUpS[256];

	U8 *pH = src->hImg->m_pImgU8;
	U8 *pS = src->sImg->m_pImgU8;
	U16 *pDest = dest->m_pImgU16;

	int imageSize = src->hImg->m_nImageSize;
	int sWidth = src->hImg->m_nWidth;
	int sHeight = src->hImg->m_nHeight;

	SetImageInfo(dest, sWidth, sHeight, 1, D_U16);

	if(hMax > hMin) {
		for(i = 179; i > hMax; --i) {
			lookUpH[i] = BLACK_16;
		}
		for(i = hMax; i > hMin; --i) {
			lookUpH[i] = FORWARD;
		}
		for(i = hMin; i >= 0; --i) {
			lookUpH[i] = BLACK_16;
		}
	} else {
		for(i = 179; i > hMin; --i) {
			lookUpH[i] = FORWARD;
		}
		for(i = hMin; i > hMax; --i) {
			lookUpH[i] = BLACK_16;
		}
		for(i = hMax; i >= 0; --i) {
			lookUpH[i] = FORWARD;
		}
	}

	for(i = 255; i > sMax; --i) {
		lookUpS[i] = BLACK_16;
	}
	for(i = sMax; i > sMin; --i) {
		lookUpS[i] = FORWARD;
	}
	for(i = sMin; i >= 0; --i) {
		lookUpS[i] = BLACK_16;
	}

	for(i = imageSize - 1; i >= 0; --i) {
		pDest[i] = lookUpH[pH[i]] & lookUpS[pS[i]];
	}

	return 0;
}

int binary_HI(HIVALUE *src, IMG_INFO *dest, const U16 FORWARD)
{
	int i;

	int hMax = src->h.max, hMin = src->h.min;
	int iMax = src->i.max, iMin = src->i.min;

	U16 lookUpH[180], lookUpI[256];

	U8 *pH = src->hImg->m_pImgU8;
	U8 *pI = src->iImg->m_pImgU8;
	U16 *pDest = dest->m_pImgU16;

	int imageSize = src->hImg->m_nImageSize;
	int sWidth = src->hImg->m_nWidth;
	int sHeight = src->hImg->m_nHeight;

	SetImageInfo(dest, sWidth, sHeight, 1, D_U16);

	if(hMax > hMin) {
		for(i = 179; i > hMax; --i) {
			lookUpH[i] = BLACK_16;
		}
		for(i = hMax; i > hMin; --i) {
			lookUpH[i] = FORWARD;
		}
		for(i = hMin; i >= 0; --i) {
			lookUpH[i] = BLACK_16;
		}
	} else {
		for(i = 179; i > hMin; --i) {
			lookUpH[i] = FORWARD;
		}
		for(i = hMin; i > hMax; --i) {
			lookUpH[i] = BLACK_16;
		}
		for(i = hMax; i >= 0; --i) {
			lookUpH[i] = FORWARD;
		}
	}

	for(i = 255; i > iMax; --i) {
		lookUpI[i] = BLACK_16;
	}
	for(i = iMax; i > iMin; --i) {
		lookUpI[i] = FORWARD;
	}
	for(i = iMin; i >= 0; --i) {
		lookUpI[i] = BLACK_16;
	}


	for(i = imageSize - 1; i >= 0; --i) {
		pDest[i] = lookUpH[pH[i]] & lookUpI[pI[i]];
	}

	return 0;
}

int binary_SI(SIVALUE *src, IMG_INFO *dest, const U16 FORWARD)
{
	int i;

	int sMax = src->s.max, sMin = src->s.min;
	int iMax = src->i.max, iMin = src->i.min;

	U16 lookUpS[256], lookUpI[256];

	U8 *pS = src->sImg->m_pImgU8;
	U8 *pI = src->iImg->m_pImgU8;
	U16 *pDest = dest->m_pImgU16;

	int imageSize = src->sImg->m_nImageSize;
	int sWidth = src->sImg->m_nWidth;
	int sHeight = src->sImg->m_nHeight;

	SetImageInfo(dest, sWidth, sHeight, 1, D_U16);

	for(i = 255; i > sMax; --i) {
		lookUpS[i] = BLACK_16;
	}
	for(i = sMax; i > sMin; --i) {
		lookUpS[i] = FORWARD;
	}
	for(i = sMin; i >= 0; --i) {
		lookUpS[i] = BLACK_16;
	}

	for(i = 255; i > iMax; --i) {
		lookUpI[i] = BLACK_16;
	}
	for(i = iMax; i > iMin; --i) {
		lookUpI[i] = FORWARD;
	}
	for(i = iMin; i >= 0; --i) {
		lookUpI[i] = BLACK_16;
	}


	for(i = imageSize - 1; i >= 0; --i) {
		pDest[i] = lookUpS[pS[i]] & lookUpI[pI[i]];
	}

	return 0;
}

int binary_H(HVALUE *src, IMG_INFO *dest, const U16 FORWARD)
{
	int i;

	int hMax = src->h.max, hMin = src->h.min;

	U16 lookUpH[180];

	U8 *pH = src->hImg->m_pImgU8;
	U16 *pDest = dest->m_pImgU16;

	int imageSize = src->hImg->m_nImageSize;
	int sWidth = src->hImg->m_nWidth;
	int sHeight = src->hImg->m_nHeight;

	SetImageInfo(dest, sWidth, sHeight, 1, D_U16);

	if(hMax > hMin) {
		for(i = 179; i > hMax; --i) {
			lookUpH[i] = BLACK_16;
		}
		for(i = hMax; i > hMin; --i) {
			lookUpH[i] = FORWARD;
		}
		for(i = hMin; i >= 0; --i) {
			lookUpH[i] = BLACK_16;
		}
	} else {
		for(i = 179; i > hMin; --i) {
			lookUpH[i] = FORWARD;
		}
		for(i = hMin; i > hMax; --i) {
			lookUpH[i] = BLACK_16;
		}
		for(i = hMax; i >= 0; --i) {
			lookUpH[i] = FORWARD;
		}
	}

	for(i = imageSize - 1; i >= 0; --i) {
		pDest[i] = lookUpH[pH[i]];
	}

	return 0;
}

int binary_S(SVALUE *src, IMG_INFO *dest, const U16 FORWARD)
{
	int i;

	int sMax = src->s.max, sMin = src->s.min;

	U16 lookUpS[256];

	U8 *pS = src->sImg->m_pImgU8;
	U16 *pDest = dest->m_pImgU16;

	int imageSize = src->sImg->m_nImageSize;
	int sWidth = src->sImg->m_nWidth;
	int sHeight = src->sImg->m_nHeight;

	SetImageInfo(dest, sWidth, sHeight, 1, D_U16);

	for(i = 255; i > sMax; --i) {
		lookUpS[i] = BLACK_16;
	}
	for(i = sMax; i > sMin; --i) {
		lookUpS[i] = FORWARD;
	}
	for(i = sMin; i >= 0; --i) {
		lookUpS[i] = BLACK_16;
	}

	for(i = imageSize - 1; i >= 0; --i) {
		pDest[i] = lookUpS[pS[i]];
	}

	return 0;
}

int binary_I(IVALUE *src, IMG_INFO *dest, const U16 FORWARD)
{
	int i;

	int iMax = src->i.max, iMin = src->i.min;

	U16 lookUpI[256];

	U8 *pI = src->iImg->m_pImgU8;
	U16 *pDest = dest->m_pImgU16;

	int imageSize = src->iImg->m_nImageSize;
	int sWidth = src->iImg->m_nWidth;
	int sHeight = src->iImg->m_nHeight;

	SetImageInfo(dest, sWidth, sHeight, 1, D_U16);

	for(i = 255; i > iMax; --i) {
		lookUpI[i] = BLACK_16;
	}
	for(i = iMax; i > iMin; --i) {
		lookUpI[i] = FORWARD;
	}
	for(i = iMin; i >= 0; --i) {
		lookUpI[i] = BLACK_16;
	}

	for(i = imageSize - 1; i >= 0; --i) {
		pDest[i] = lookUpI[pI[i]];
	}

	return 0;
}

// 카메라가 꼬져서 침식은 쓸일 없음
int dilationBinary(IMG_INFO *src, IMG_INFO *dest, const U16 FORWARD)
{
	int i, j;	
	int nWidth = src->m_nWidth;
	int nWidth_1 = nWidth - 1;
	int nHeight = src->m_nHeight;
	int nHeight_1 = nHeight - 1;
	int nWStep = src->m_nWStep;
	int index;
	int imageSize = src->m_nImageSize;

	U16 *pSrc, *pDest;

	SetImageInfo(dest, nWidth, nHeight, 1, D_U16);

	pSrc = src->m_pImgU16;
	pDest = dest->m_pImgU16;

	memset(pDest, BLACK_16, sizeof(U16)* imageSize);

	for(i = 1; i < nHeight_1; ++i) {
		index = i * nWStep;
		pSrc = &src->m_pImgU16[index];
		pDest = &dest->m_pImgU16[index];
		for(j = 1; j < nWidth_1; ++j) {
			if(pSrc[j] == FORWARD) {
				pDest[j] = FORWARD;
				continue;
			}

			// 왼쪽
			if(pSrc[j - 1] == FORWARD) {
				pDest[j] = FORWARD;
				continue;
			}

			// 오른쪽
			if(pSrc[j + 1] == FORWARD) {
				pDest[j] = FORWARD;
				continue;
			}

			// 위쪽
			if(pSrc[j - nWStep] == FORWARD) {
				pDest[j] = FORWARD;
				continue;
			}

			// 아래쪽
			if(pSrc[j + nWStep] == FORWARD) {
				pDest[j] = FORWARD;
				continue;
			}

			// 왼쪽 위
			if(pSrc[j - nWStep - 1] == FORWARD) {
				pDest[j] = FORWARD;
				continue;
			}

			// 오른쪽 위
			if(pSrc[j - nWStep + 1] == FORWARD) {
				pDest[j] = FORWARD;
				continue;
			}

			// 왼쪽 아래
			if(pSrc[j + nWStep - 1] == FORWARD) {
				pDest[j] = FORWARD;
				continue;
			}

			// 오른쪽 아래
			if(pSrc[j + nWStep + 1] == FORWARD) {
				pDest[j] = FORWARD;
				continue;
			}

			pDest[j] = BLACK_16;
		}
	}

	return 1;
}


int erosionBinary(IMG_INFO *src, IMG_INFO *dest, const U16 FORWARD)
{
	int i, j;
	int nWidth = src->m_nWidth;
	int nWidth_1 = nWidth - 1;
	int nHeight = src->m_nHeight;
	int nHeight_1 = nHeight - 1;
	int nWStep = src->m_nWStep;
	int index;

	int imageSize = src->m_nImageSize;

	U16 *pSrc, *pDest;

	SetImageInfo(dest, nWidth, nHeight, 1, D_U16);

	pSrc = src->m_pImgU16;
	pDest = dest->m_pImgU16;

	memset(pDest, BLACK_16, sizeof(U16)* imageSize);

	for (i = 1; i < nHeight_1; ++i) {
		index = i * nWStep;
		pSrc = &src->m_pImgU16[index];
		pDest = &dest->m_pImgU16[index];
		for (j = 1; j < nWidth_1; ++j) {
			if (pSrc[j] == BLACK_16) {
				pDest[j] = BLACK_16;
				continue;
			}

			// 왼쪽
			if (pSrc[j - 1] == BLACK_16) {
				pDest[j] = BLACK_16;
				continue;
			}

			// 오른쪽
			if (pSrc[j + 1] == BLACK_16) {
				pDest[j] = BLACK_16;
				continue;
			}

			// 위쪽
			if (pSrc[j - nWStep] == BLACK_16) {
				pDest[j] = BLACK_16;
				continue;
			}

			// 아래쪽
			if (pSrc[j + nWStep] == BLACK_16) {
				pDest[j] = BLACK_16;
				continue;
			}

			// 왼쪽 위
			if (pSrc[j - nWStep - 1] == BLACK_16) {
				pDest[j] = BLACK_16;
				continue;
			}

			// 오른쪽 위
			if (pSrc[j - nWStep + 1] == BLACK_16) {
				pDest[j] = BLACK_16;
				continue;
			}

			// 왼쪽 아래
			if (pSrc[j + nWStep - 1] == BLACK_16) {
				pDest[j] = BLACK_16;
				continue;
			}

			// 오른쪽 아래
			if (pSrc[j + nWStep + 1] == BLACK_16) {
				pDest[j] = BLACK_16;
				continue;
			}

			pDest[j] = FORWARD;
		}
	}

	return 1;
}

int medianBinary(IMG_INFO *src, IMG_INFO *dest, const U16 FORWARD)
{
	int i, j;
	int nWidth = src->m_nWidth;
	int nWidth_1 = nWidth - 1;
	int nHeight = src->m_nHeight;
	int nHeight_1 = nHeight - 1;
	int nWStep = src->m_nWStep;
	int flag;
	int index;
	const int value = FORWARD * 5;

	int imageSize = src->m_nImageSize;

	U16 *pSrc, *pDest;

	SetImageInfo(dest, nWidth, nHeight, 1, D_U16);

	pSrc = src->m_pImgU16;
	pDest = dest->m_pImgU16;

	memset(pDest, BLACK_16, sizeof(U16)* imageSize);

	for(i = 1; i < nHeight_1; ++i) {
		index = i * nWStep;
		pSrc = &src->m_pImgU16[index];
		pDest = &dest->m_pImgU16[index];
		for(j = 1; j < nWidth_1; ++j) {
			flag = 0;
			flag = pSrc[j - nWStep - 1] + pSrc[j - nWStep] + pSrc[j - nWStep + 1]
				+ pSrc[j - 1] + pSrc[j] + pSrc[j + 1]
				+ pSrc[j + nWStep - 1] + pSrc[j + nWStep] + pSrc[j + nWStep + 1];

			if(flag >= value) {
				pDest[j] = FORWARD;
			} else {
				pDest[j] = BLACK_16;
			}
		}
	}

	return 1;
}


int InverseImage(IMG_INFO * src, const int FORWARD)
{
	int i;
	int imageSize = src->m_nImageSize;	
	U16 *pSrc = src->m_pImgU16;
	
	for (i = imageSize - 1; i >= 0; --i) {
		pSrc[i] = pSrc[i] ? BLACK_16 : FORWARD;
	}

	return 0;
}

int Gaussian(IMG_INFO *src, IMG_INFO *dest)
{
	int G[9];
	int nWidth = src->m_nWidth;  //120까지만
	int nWidth_60 = nWidth -60;
	int nHeight = src->m_nHeight;
	int nHeight_1 = nHeight -1;
	int nWStep = src->m_nWStep;
	int index, i, j, y, x;
	int py, px, sum;
	int imageSize = src->m_nImageSize;
	U16 *pSrc, *pDest;

	SetImageInfo(dest, nWidth, nHeight, 1, D_U16);

	pSrc = src->m_pImgU16;
	pDest = dest->m_pImgU16;

	G[0] = 1; G[1] = 2; G[2] = 1;
	G[3] = 2; G[4] = 4; G[5] = 2;
	G[6] = 1; G[7] = 2; G[8] = 1;

	memset(pDest, BLACK_16, sizeof(U8)* imageSize);

	for (i = 1; i < nHeight_1; i++) {
		index = i * nWStep;
		pDest = &dest->m_pImgU16[index];
		for (j = 1; j < nWidth - 60; j++) {
			sum = 0;
			for (y = 0; y < 3; y++)
			{
				for (x = 0; x < 3; x++)
				{
					py = i - 1 + y;
					px = j - 1 + x;
					if (px >= 0 && px < nWidth && py >= 0 && py < nHeight)
						sum += G[y * 3 + x] * pSrc[py * nWStep + px];
				}
			}
			pDest[j] = sum / 16;
		}
	}
	return 1;
}

int SobelEdge(IMG_INFO *src, IMG_INFO *dilation, IMG_INFO *dest, int * Line)
{
	int G[9];
	int nWidth = src->m_nWidth; //120까지만
	int nWidth_60 = nWidth - 60;
	int nHeight = src->m_nHeight;
	int nHeight_1 = nHeight - 1;
	int nWStep = src->m_nWStep;
	int index, i, j, y, x;
	int py, px, sum, cnt=1, mx=0;
	int imageSize = src->m_nImageSize;
	U16 *pSrc, *pDest, *pDil;

	SetImageInfo(dest, nWidth, nHeight, 1, D_U16);

	pSrc = src->m_pImgU16;
	pDest = dest->m_pImgU16;

	G[0] = 1; G[1] = 1; G[2] = 1;
	G[3] = 1; G[4] = -8; G[5] = 1;
	G[6] = 1; G[7] = 1; G[8] = 1;

	memset(pDest, BLACK_16, sizeof(U16)* imageSize);

	for (i = 1; i < nHeight_1; i++) {
		index = i * nWStep;
		Line[i] = nWidth_60;
		pDest = &dest->m_pImgU16[index];
		pDil = &dilation->m_pImgU16[index];
		for (j = 3; j < nWidth_60; j++) {
			sum = 0;
			for (y = 0; y < 3; y++)
			{
				for (x = 0; x < 3; x++)
				{
					py = i - 1 + y;
					px = j - 1 + x;
					if (px >= 0 && px < nWidth && py >= 0 && py < nHeight)
						sum += G[y * 3 + x] * pSrc[py * nWStep + px];
				}
			}

			if (sum > 0x2FFF && !pDil[j] ) {
				pDest[j] = WHITE_16;
				Line[i] = j;
				break;
			}
		}
		if (Line[i] == nWidth_60) {
			pDest[j] = WHITE_16;
		}
	}
	Line[0] = Line[1];
	Line[nHeight_1] = Line[nHeight_1 - 1];

	return 1;
}

// src: 8bits image, dest: 8bits image
int Laplacian(IMG_INFO *src, IMG_INFO *dest)
{
	int G[9];
	int nWidth = src->m_nWidth;
	int nHeight = src->m_nHeight;
	int nWidth_1 = nWidth - 1;
	int nHeight_1 = nHeight - 1;
	int nWStep = src->m_nWStep;
	int index, index2, i, j, y, x, fIndex;
	int sum;
	int imageSize = src->m_nImageSize;
	U8 *pSrc, *pDest;

	SetImageInfo(dest, nWidth, nHeight, 1, D_U8);

	G[0] = 1; G[1] = 1; G[2] = 1;
	G[3] = 1; G[4] = -8; G[5] = 1;
	G[6] = 1; G[7] = 1; G[8] = 1;

	pDest = dest->m_pImgU8;
	memset(pDest, BLACK_8, sizeof(U8)* imageSize);

	for (i = 1; i < nHeight_1; ++i) {
		index = i * nWStep;
		pDest = &dest->m_pImgU8[index];
		for (j = 1; j < nWidth_1; ++j) {
			sum = 0;
			fIndex = 0;
			pSrc = &src->m_pImgU8[index + j];
			index2 = -nWStep;
			for (y = -1; y <= 1; ++y) {
				for (x = -1; x <= 1; ++x) {
					sum += G[fIndex++] * pSrc[index2 + x];
				}
				index2 += nWStep;
			}
			pDest[j] = CLIP(sum > 0 ? sum : -sum);
		}
	}
	
	return 1;
}

int CropImage(IMG_INFO *src, IMG_INFO *dest, const short xs, const short ys, const short xe, const short ye)
{
	short i;
	int sWidth = src->m_nWidth;
	int sHeight = src->m_nHeight;
	short sChannels = src->m_nChannels;
	short sDepth = src->m_nDepth;
	int sStep = src->m_nWStep;
	int dWidth = xe - xs, dHeight = ye - ys;
	int dWStep;

	if (dWidth < 0 || dHeight < 0 ||
		xs < 0 || xs + dWidth > sWidth ||
		ys < 0 || ys + dHeight > sHeight) {
		return -1;
	}

	SetImageInfo(dest, dWidth, dHeight, sChannels, sDepth);
	dWStep = dest->m_nWStep;
	for (i = dHeight - 1; i >= 0; --i) {
		memcpy(&dest->m_pImgU8[i * dWStep * sDepth], &src->m_pImgU8[(ys + i) * sStep * sDepth + xs * sDepth * sChannels], dWidth * sDepth);
	}

	return 1;
}

int FilpImage(IMG_INFO *src, IMG_INFO *dest)
{
	short i;
	int sWidth = src->m_nWidth;
	int sHeight = src->m_nHeight;
	short sChannels = src->m_nChannels;
	short sDepth = src->m_nDepth;
	int sStep = src->m_nWStep;

	SetImageInfo(dest, sWidth, sHeight, sChannels, sDepth);
	for (i = sHeight - 1; i >= 0; --i) {
		memcpy(&dest->m_pImgU8[i * sStep * sDepth], &src->m_pImgU8[((sHeight - 1) - i) * sStep * sDepth], sStep * sDepth);
	}

	return 1;
}

// src: 16bits image, dest: 16bits image
int CopyImageForView(IMG_INFO *src, IMG_INFO *dest, const short xs, const short ys)
{
	short i;
	int sWidth = src->m_nWidth;
	int sHeight = src->m_nHeight;
	int sWStep = src->m_nWStep;
	short sChannels = src->m_nChannels;
	short sDepth = src->m_nDepth;
	int dWidth = AMAZON_WIDTH, dHeight = AMAZON_HEIGHT;
	int dWStep;

	SetImageInfo(dest, dWidth, dHeight, 1, D_U16);
	dWStep = dest->m_nWStep;
	for (i = sHeight - 1; i >= 0; --i) {
		memcpy(&dest->m_pImgU16[(ys + i) * dWStep + xs], &src->m_pImgU16[i * sWStep], sWidth * sizeof(U16));
	}

	return 1;
}

// src: 8bits image, dest: 16bits image
int GetIntegralImage(IMG_INFO *src, IMG_INFO *dest)
{
	int i, j;
	int nWidth = src->m_nWidth, nHeight = src->m_nHeight;
	int nWStep = src->m_nWStep;
	U8 *pSrc = src->m_pImgU8;
	U32 *pDest = dest->m_pImgU32;

	SetImageInfo(dest, nWidth, nHeight, 1, D_U32);
	memset(pDest, 0, sizeof(U32) * src->m_nImageSize);

	pDest[0] = pSrc[0];
	for (j = 1; j < nWidth; ++j) {
		pDest[j] = pSrc[j] + pDest[j - 1];
	}

	for (i = 1; i < nHeight; ++i) {
		pDest = &dest->m_pImgU32[i * nWStep];
		pSrc = &src->m_pImgU8[i * nWStep];		
		pDest[0] = pSrc[0] + pDest[-nWStep];		
		for (j = 1; j < nWidth; ++j) {
			pDest[j] = pSrc[j] + pDest[j - 1] + pDest[j - nWStep] - pDest[j - nWStep - 1];
		}
	}

	return 1;
}

// src: 16bits image, dest: 16bits image
int GetBinaryCount(IMG_INFO *src, IMG_INFO *dest)
{
	int i, j, pt;
	int nWidth = src->m_nWidth, nHeight = src->m_nHeight;
	int nWStep = src->m_nWStep;
	U16 *pSrc = src->m_pImgU16;
	U16 *pDest = dest->m_pImgU16;

	SetImageInfo(dest, nWidth, nHeight, 1, D_U16);
	memset(pDest, 0, sizeof(U16) * src->m_nImageSize);

	pt = pSrc[0] == 0 ? 0 : 1;
	pDest[0] = pt;
	for (j = 1; j < nWidth; ++j) {
		pt = pSrc[j] == 0 ? 0 : 1;
		pDest[j] = pt + pDest[j - 1];
	}

	for (i = 1; i < nHeight; ++i) {
		pDest = &dest->m_pImgU16[i * nWStep];
		pSrc = &src->m_pImgU16[i * nWStep];
		pt = pSrc[0] == 0 ? 0 : 1;
		pDest[0] = pt + pDest[-nWStep];
		for (j = 1; j < nWidth; ++j) {
			pt = pSrc[j] == 0 ? 0 : 1;
			pDest[j] = pt + pDest[j - 1] + pDest[j - nWStep] - pDest[j - nWStep - 1];
		}
	}

	return 1;
}

int GetAreaSum(IMG_INFO *src, const int xs, const int ys, const int xe, const int ye)
{
	int nWidth = src->m_nWidth;
	int nHeight = src->m_nHeight;
	int nWStep = src->m_nWStep;
	U16 *pSrc = src->m_pImgU16;
	
	if (xs < 0 || ys < 0 || xe >= nWidth || ye >= nHeight)
		return -1;

	if (xs == 0) {
		if (ys == 0) {
			return pSrc[ye * nWStep + xe];
		} else {
			return pSrc[ye * nWStep + xe] - pSrc[(ys - 1) * nWStep + xe];
		}
	} else if (ys == 0) {
		return pSrc[ye * nWStep + xe] - pSrc[ye * nWStep + (xs - 1)];
	} else {
		return pSrc[ye * nWStep + xe] - pSrc[ye * nWStep + (xs - 1)] - pSrc[(ys - 1) * nWStep + xe] + pSrc[(ys - 1) * nWStep + (xs - 1)];
	}	
}

int GetAreaSum32(IMG_INFO *src, const int xs, const int ys, const int xe, const int ye)
{
	int nWidth = src->m_nWidth;
	int nHeight = src->m_nHeight;
	int nWStep = src->m_nWStep;
	U32 *pSrc = src->m_pImgU32;

	if (xs < 0 || ys < 0 || xe >= nWidth || ye >= nHeight)
		return -1;

	if (xs == 0) {
		if (ys == 0) {
			return pSrc[ye * nWStep + xe];
		} else {
			return pSrc[ye * nWStep + xe] - pSrc[(ys - 1) * nWStep + xe];
		}
	} else if (ys == 0) {
		return pSrc[ye * nWStep + xe] - pSrc[ye * nWStep + (xs - 1)];
	} else {
		return pSrc[ye * nWStep + xe] - pSrc[ye * nWStep + (xs - 1)] - pSrc[(ys - 1) * nWStep + xe] + pSrc[(ys - 1) * nWStep + (xs - 1)];
	}
}
