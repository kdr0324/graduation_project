#include "stdafx.h"

#include "imagePixel.h"

// 원하는 색이면 WHITE_8 아니면 BLACK
IMG_INFO *Binary_HSI(HSIVALUE *src)
{
	int i;
	
	IMG_INFO *dest = CloneImageInfo(src->hImg, D_U8);
	int hMax = src->h.max, hMin = src->h.min;
	int sMax = src->s.max, sMin = src->s.min;
	int iMax = src->i.max, iMin = src->i.min;

	U8 lookUpH[360] ,lookUpS[256], lookUpI[256];

	int imageSize = dest->m_nImageSize;
	U16 *pH = src->hImg->m_pImgU16;
	U16 *pS = src->sImg->m_pImgU16;
	U16 *pI = src->iImg->m_pImgU16;
	U8 *pDest = dest->m_pImgU8;

	if(hMax > hMin) {
		for(i = 359; i > hMax; --i) {
			lookUpH[i] = BLACK_8;
		}
		for(i = hMax; i >= hMin; --i) {
			lookUpH[i] = WHITE_8;
		}
		for(i = hMin - 1; i >= 0; --i) {
			lookUpH[i] = BLACK_8;
		}
	} else {
		for(i = 359; i >= hMin; --i) {
			lookUpH[i] = WHITE_8;
		}
		for(i = hMin - 1; i > hMax; --i) {
			lookUpH[i] = BLACK_8;
		}
		for(i = hMax; i >= 0; --i) {
			lookUpH[i] = WHITE_8;
		}
	}

	for(i = 255; i > sMax; --i) {
		lookUpS[i] = BLACK_8;
	}
	for(i = sMax; i >= sMin; --i) {
		lookUpS[i] = WHITE_8;
	}
	for(i = sMin - 1; i >= 0; --i) {
		lookUpS[i] = BLACK_8;
	}

	for(i = 255; i > iMax; --i) {
		lookUpI[i] = BLACK_8;
	}
	for(i = iMax; i >= iMin; --i) {
		lookUpI[i] = WHITE_8;
	}
	for(i = iMin - 1; i >= 0; --i) {
		lookUpI[i] = BLACK_8;
	}

	for(i = imageSize - 1; i >= 0; --i) {
		pDest[i] = lookUpH[pH[i]] & lookUpS[pS[i]] & lookUpI[pI[i]];
	}

	return dest;
}

IMG_INFO *Binary_HS(HSVALUE *src)
{
	int i;

	IMG_INFO *dest = CloneImageInfo(src->hImg, D_U8);
	int hMax = src->h.max, hMin = src->h.min;
	int sMax = src->s.max, sMin = src->s.min;

	U8 lookUpH[360], lookUpS[256];

	int imageSize = dest->m_nImageSize;
	U16 *pH = src->hImg->m_pImgU16;
	U16 *pS = src->sImg->m_pImgU16;
	U8 *pDest = dest->m_pImgU8;

	if(hMax > hMin) {
		for(i = 359; i > hMax; --i) {
			lookUpH[i] = BLACK_8;
		}
		for(i = hMax; i >= hMin; --i) {
			lookUpH[i] = WHITE_8;
		}
		for(i = hMin - 1; i >= 0; --i) {
			lookUpH[i] = BLACK_8;
		}
	} else {
		for(i = 359; i >= hMin; --i) {
			lookUpH[i] = WHITE_8;
		}
		for(i = hMin - 1; i > hMax; --i) {
			lookUpH[i] = BLACK_8;
		}
		for(i = hMax; i >= 0; --i) {
			lookUpH[i] = WHITE_8;
		}
	}

	for(i = 255; i > sMax; --i) {
		lookUpS[i] = BLACK_8;
	}
	for(i = sMax; i >= sMin; --i) {
		lookUpS[i] = WHITE_8;
	}
	for(i = sMin - 1; i >= 0; --i) {
		lookUpS[i] = BLACK_8;
	}

	for(i = imageSize - 1; i >= 0; --i) {
		pDest[i] = lookUpH[pH[i]] & lookUpS[pS[i]];
	}

	return dest;
}

IMG_INFO *Binary_HI(HIVALUE *src)
{
	int i;

	IMG_INFO *dest = CloneImageInfo(src->hImg, D_U8);
	int hMax = src->h.max, hMin = src->h.min;
	int iMax = src->i.max, iMin = src->i.min;

	U8 lookUpH[360], lookUpI[256];

	int imageSize = dest->m_nImageSize;
	U16 *pH = src->hImg->m_pImgU16;
	U16 *pI = src->iImg->m_pImgU16;
	U8 *pDest = dest->m_pImgU8;

	if(hMax > hMin) {
		for(i = 359; i > hMax; --i) {
			lookUpH[i] = BLACK_8;
		}
		for(i = hMax; i >= hMin; --i) {
			lookUpH[i] = WHITE_8;
		}
		for(i = hMin - 1; i >= 0; --i) {
			lookUpH[i] = BLACK_8;
		}
	} else {
		for(i = 359; i >= hMin; --i) {
			lookUpH[i] = WHITE_8;
		}
		for(i = hMin - 1; i > hMax; --i) {
			lookUpH[i] = BLACK_8;
		}
		for(i = hMax; i >= 0; --i) {
			lookUpH[i] = WHITE_8;
		}
	}

	for(i = 255; i > iMax; --i) {
		lookUpI[i] = BLACK_8;
	}
	for(i = iMax; i >= iMin; --i) {
		lookUpI[i] = WHITE_8;
	}
	for(i = iMin - 1; i >= 0; --i) {
		lookUpI[i] = BLACK_8;
	}


	for(i = imageSize - 1; i >= 0; --i) {
		pDest[i] = lookUpH[pH[i]] & lookUpI[pI[i]];
	}

	return dest;
}

IMG_INFO *Binary_SI(SIVALUE *src)
{
	int i;

	IMG_INFO *dest = CloneImageInfo(src->sImg, D_U8);
	int sMax = src->s.max, sMin = src->s.min;
	int iMax = src->i.max, iMin = src->i.min;

	U8 lookUpS[256], lookUpI[256];

	int imageSize = dest->m_nImageSize;
	U16 *pS = src->sImg->m_pImgU16;
	U16 *pI = src->iImg->m_pImgU16;
	U8 *pDest = dest->m_pImgU8;

	for(i = 255; i > sMax; --i) {
		lookUpS[i] = BLACK_8;
	}
	for(i = sMax; i >= sMin; --i) {
		lookUpS[i] = WHITE_8;
	}
	for(i = sMin - 1; i >= 0; --i) {
		lookUpS[i] = BLACK_8;
	}

	for(i = 255; i > iMax; --i) {
		lookUpI[i] = BLACK_8;
	}
	for(i = iMax; i >= iMin; --i) {
		lookUpI[i] = WHITE_8;
	}
	for(i = iMin - 1; i >= 0; --i) {
		lookUpI[i] = BLACK_8;
	}


	for(i = imageSize - 1; i >= 0; --i) {
		pDest[i] = lookUpS[pS[i]] & lookUpI[pI[i]];
	}

	return dest;
}

IMG_INFO *Binary_H(HVALUE *src)
{
	int i;

	IMG_INFO *dest = CloneImageInfo(src->hImg, D_U8);
	int hMax = src->h.max, hMin = src->h.min;

	U8 lookUpH[360];

	int imageSize = dest->m_nImageSize;
	U16 *pH = src->hImg->m_pImgU16;
	U8 *pDest = dest->m_pImgU8;

	if(hMax > hMin) {
		for(i = 359; i > hMax; --i) {
			lookUpH[i] = BLACK_8;
		}
		for(i = hMax; i >= hMin; --i) {
			lookUpH[i] = WHITE_8;
		}
		for(i = hMin - 1; i >= 0; --i) {
			lookUpH[i] = BLACK_8;
		}
	} else {
		for(i = 359; i >= hMin; --i) {
			lookUpH[i] = WHITE_8;
		}
		for(i = hMin - 1; i > hMax; --i) {
			lookUpH[i] = BLACK_8;
		}
		for(i = hMax; i >= 0; --i) {
			lookUpH[i] = WHITE_8;
		}
	}

	for(i = imageSize - 1; i >= 0; --i) {
		pDest[i] = lookUpH[pH[i]];
	}

	return dest;
}

IMG_INFO *Binary_S(SVALUE *src)
{
	int i;

	IMG_INFO *dest = CloneImageInfo(src->sImg, D_U8);
	int sMax = src->s.max, sMin = src->s.min;

	U8 lookUpS[256];

	int imageSize = dest->m_nImageSize;
	U16 *pS = src->sImg->m_pImgU16;
	U8 *pDest = dest->m_pImgU8;

	for(i = 255; i > sMax; --i) {
		lookUpS[i] = BLACK_8;
	}
	for(i = sMax; i >= sMin; --i) {
		lookUpS[i] = WHITE_8;
	}
	for(i = sMin - 1; i >= 0; --i) {
		lookUpS[i] = BLACK_8;
	}

	for(i = imageSize - 1; i >= 0; --i) {
		pDest[i] = lookUpS[pS[i]];
	}

	return dest;
}

IMG_INFO *Binary_I(IVALUE *src)
{
	int i;

	IMG_INFO *dest = CloneImageInfo(src->iImg, D_U8);
	int iMax = src->i.max, iMin = src->i.min;

	U8 lookUpI[256];

	int imageSize = dest->m_nImageSize;
	U16 *pI = src->iImg->m_pImgU16;
	U8 *pDest = dest->m_pImgU8;

	for(i = 255; i > iMax; --i) {
		lookUpI[i] = BLACK_8;
	}
	for(i = iMax; i >= iMin; --i) {
		lookUpI[i] = WHITE_8;
	}
	for(i = iMin - 1; i >= 0; --i) {
		lookUpI[i] = BLACK_8;
	}

	for(i = imageSize - 1; i >= 0; --i) {
		pDest[i] = lookUpI[pI[i]];
	}

	return dest;
}

// 카메라가 꼬져서 침식은 쓸일 없음
IMG_INFO *DilationBinary(IMG_INFO *src)
{
	IMG_INFO *dest = CloneImageInfo(src, D_U8);
	int i, j;
	int nWidth = src->m_nWidth;
	int nWidth_1 = nWidth - 1;
	int nHeight = src->m_nHeight;
	int nHeight_1 = nHeight - 1;
	int index;

	int imageSize = src->m_nImageSize;

	U8 *pSrc, *pDest;

	pSrc = src->m_pImgU8;
	pDest = dest->m_pImgU8;

	memset(pDest, BLACK_8, sizeof(U8)* imageSize);

	for(i = 1; i < nHeight_1; ++i) {
		index = i * nWidth;
		pSrc = &src->m_pImgU8[index];
		pDest = &dest->m_pImgU8[index];
		for(j = 1; j < nWidth_1; ++j) {
			if(pSrc[j] == WHITE_8) {
				pDest[j] = WHITE_8;
				continue;
			}

			// 왼쪽
			if(pSrc[j - 1] == WHITE_8) {
				pDest[j] = WHITE_8;
				continue;
			}

			// 오른쪽
			if(pSrc[j + 1] == WHITE_8) {
				pDest[j] = WHITE_8;
				continue;
			}

			// 위쪽
			if(pSrc[j - nWidth] == WHITE_8) {
				pDest[j] = WHITE_8;
				continue;
			}

			// 아래쪽
			if(pSrc[j + nWidth] == WHITE_8) {
				pDest[j] = WHITE_8;
				continue;
			}

			// 왼쪽 위
			if(pSrc[j - nWidth - 1] == WHITE_8) {
				pDest[j] = WHITE_8;
				continue;
			}

			// 오른쪽 위
			if(pSrc[j - nWidth_1] == WHITE_8) {
				pDest[j] = WHITE_8;
				continue;
			}

			// 왼쪽 아래
			if(pSrc[j + nWidth_1] == WHITE_8) {
				pDest[j] = WHITE_8;
				continue;
			}

			// 오른쪽 아래
			if(pSrc[j + nWidth + 1] == WHITE_8) {
				pDest[j] = WHITE_8;
				continue;
			}

			pDest[j] = BLACK_8;
		}
	}

	return dest;
}

IMG_INFO *ErosionBinary(IMG_INFO *src)
{
	IMG_INFO *dest = CloneImageInfo(src, D_U8);
	int i, j;
	int nWidth = src->m_nWidth;
	int nWidth_1 = nWidth - 1;
	int nHeight = src->m_nHeight;
	int nHeight_1 = nHeight - 1;
	int index;

	int imageSize = src->m_nImageSize;

	U8 *pSrc, *pDest;

	pSrc = src->m_pImgU8;
	pDest = dest->m_pImgU8;

	memset(pDest, BLACK_8, sizeof(U8)* imageSize);

	for(i = 1; i < nHeight_1; ++i) {
		index = i * nWidth;
		pSrc = &src->m_pImgU8[index];
		pDest = &dest->m_pImgU8[index];
		for(j = 1; j < nWidth_1; ++j) {
			if(pSrc[j] == BLACK_8) {
				pDest[j] = BLACK_8;
				continue;
			}

			// 왼쪽
			if(pSrc[j - 1] == BLACK_8) {
				pDest[j] = BLACK_8;
				continue;
			}

			// 오른쪽
			if(pSrc[j + 1] == BLACK_8) {
				pDest[j] = BLACK_8;
				continue;
			}

			// 위쪽
			if(pSrc[j - nWidth] == BLACK_8) {
				pDest[j] = BLACK_8;
				continue;
			}

			// 아래쪽
			if(pSrc[j + nWidth] == BLACK_8) {
				pDest[j] = BLACK_8;
				continue;
			}

			// 왼쪽 위
			if(pSrc[j - nWidth - 1] == BLACK_8) {
				pDest[j] = BLACK_8;
				continue;
			}

			// 오른쪽 위
			if(pSrc[j - nWidth_1] == BLACK_8) {
				pDest[j] = BLACK_8;
				continue;
			}

			// 왼쪽 아래
			if(pSrc[j + nWidth_1] == BLACK_8) {
				pDest[j] = BLACK_8;
				continue;
			}

			// 오른쪽 아래
			if(pSrc[j + nWidth + 1] == BLACK_8) {
				pDest[j] = BLACK_8;
				continue;
			}

			pDest[j] = WHITE_8;
		}
	}

	return dest;
}

IMG_INFO *MedianBinary(IMG_INFO *src)
{
	IMG_INFO *dest = CloneImageInfo(src, D_U8);
	int i, j;
	int nWidth = src->m_nWidth;
	int nWidth_1 = nWidth - 1;
	int nHeight = src->m_nHeight;
	int nHeight_1 = nHeight - 1;
	int flag;
	int index;
	const int value = WHITE_8 * 5;

	int imageSize = src->m_nImageSize;

	U8 *pSrc, *pDest;

	pSrc = src->m_pImgU8;
	pDest = dest->m_pImgU8;

	memset(pDest, BLACK_8, sizeof(U8)* imageSize);

	for(i = 1; i < nHeight_1; ++i) {
		index = i * nWidth;
		pSrc = &src->m_pImgU8[index];
		pDest = &dest->m_pImgU8[index];
		for(j = 1; j < nWidth_1; ++j) {
			flag = 0;
			flag = pSrc[j - nWidth - 1] + pSrc[j - nWidth] + pSrc[j - nWidth_1]
				+ pSrc[j - 1] + pSrc[j] + pSrc[j + 1]
				+ pSrc[j + nWidth_1] + pSrc[j + nWidth] + pSrc[j + nWidth + 1];

			if(flag >= value) {
				pDest[j] = WHITE_8;
			} else {
				pDest[j] = BLACK_8;
			}
		}
	}

	return dest;
}