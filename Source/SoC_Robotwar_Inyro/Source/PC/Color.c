#include "stdafx.h"

#include "color.h"

// LSB부터 차례대로 각 비트가 1이면
// BGR2GRAY, GRAY2BGR, BGR2BGR565, BGR5652BGR, BGR2HSI, HSI2BGR, BGR2YCbCr, YCbCr2BGR, BGR2YUV844, YUV8442BGR의
// Look Up Table이 할당 되었음을 할려준다.
short mallocValue = 0;

UCHAR *lookUpBGR2GRAY = NULL;
UCHAR *lookUpGRAY2BGR = NULL;

short *lookUpBGR2BGR565 = NULL;
UCHAR *lookUpBGR5652BGR = NULL;

short *lookUpBGR2HSI = NULL;
UCHAR *lookUpHSI2BGR = NULL;

UCHAR *lookUpBGR2YCbCr = NULL;
UCHAR *lookUpYCbCr2BGR = NULL;

short *lookUpBGR2YUV844 = NULL;
UCHAR *lookUpYUV8442BGR = NULL;

static int MallocLookUpTableBGR2GRAY(void)
{
	int red, green, blue;
	int index = 0;
	lookUpBGR2GRAY = (UCHAR *)malloc(sizeof(UCHAR) * 256 * 256 * 256);

	if(lookUpBGR2GRAY == NULL) {
		return 0;
	} else {
		mallocValue |= 0x0001;

		// BGR2GRAY 계산
		for(red = 0; red < 256; red++) {
			for(green = 0; green < 256; green++) {
				for(blue = 0; blue < 256; blue++) {
					lookUpBGR2GRAY[index++] = (UCHAR)((red + green + blue) / 3.0);
				}
			}
		}

		return 1;
	}
}

static int MallocLookUpTableGRAY2BGR(void)
{
	int gray;
	int index = 0;
	const int R = 2, G = 1, B = 0;
	lookUpGRAY2BGR = (UCHAR *)malloc(sizeof(UCHAR) * 256 * 3);

	if(lookUpGRAY2BGR == NULL) {
		return 0;
	} else {
		mallocValue |= 0x0002;

		// GRAY2BGR 계산
		for(gray = 0; gray < 256; gray++) {
			lookUpGRAY2BGR[index + R] = lookUpGRAY2BGR[index + G] = lookUpGRAY2BGR[index + B] = gray;
			index += 3;
		}

		return 1;
	}
}

static int mallocLookUpTableBGR2BGR565(void)
{
	UCHAR red, green, blue;
	int index = 0;
	short rgb565;
	lookUpBGR2BGR565 = (short *)malloc(sizeof(short) * 256 * 256 * 256);

	if(lookUpBGR2BGR565 == NULL) {
		return 0;
	} else {
		mallocValue |= 0x0004;

		// BGR2BGR565 계산
		for(red = 0; red < 256; red++) {
			rgb565 = (red >> 3) << 11;
			for(green = 0; green < 256; green++) {
				rgb565 &= 0xF800;
				rgb565 |= (green >> 3) << 5;
				for(blue = 0; blue < 256; blue++) {
					rgb565 &= 0xFFE0;
					rgb565 |= (blue >> 3);
					lookUpBGR2BGR565[index++] = rgb565;
				}
			}
		}

		return 1;
	}
}

static int mallocLookUpTableBGR5652BGR(void)
{
	short rgb565;
	int index;
	const int R = 2, G = 1, B = 0;
	lookUpBGR5652BGR = (UCHAR *)malloc(sizeof(UCHAR) * 65536 * 3);

	if(lookUpBGR5652BGR == NULL) {
		return 0;
	} else {
		mallocValue |= 0x0008;

		// BGR5652BGR 계산
		for(rgb565 = 0; rgb565 < 65536; rgb565++) {
			lookUpBGR5652BGR[index + R] = ((0xF800 & rgb565) >> 0x0B) << 3;
			lookUpBGR5652BGR[index + G] = ((0x07E0 & rgb565) >> 0x05) << 2;
			lookUpBGR5652BGR[index + B] = ((0x0005 & rgb565) >> 0x00) << 3;
			index += 3;
		}

		return 1;
	}
}

static int MallocLookUpTableBGR2HSI(void)
{
	int red, green, blue;
	int index = 0;
	const int H = 2, S = 1, I = 0;
	lookUpBGR2HSI = (short *)malloc(sizeof(short) * 256 * 256 * 256 * 3);

	if(lookUpBGR2HSI == NULL) {
		return 0;
	} else {
		mallocValue |= 0x0010;

		// BGR2HSI 계산
		for(red = 0; red < 256; red++) {
			for(green = 0; green < 256; green++) {
				for(blue = 0; blue < 256; blue++) {
					double minc = MIN(red, MIN(green, blue));

					lookUpBGR2HSI[index + I] = (short)((red + green + blue) / 3.0);

					if(red == green && green == blue) {
						lookUpBGR2HSI[index + H] = (short)0;
						lookUpBGR2HSI[index + S] = (short)0;
					} else {
						float angle;
						lookUpBGR2HSI[index + S] = (short)((1.0 - 3.0 / (red + green + blue) * minc) * 255);
						angle = acosf(0.5f * ((red - green) + (red - blue)) / sqrtf((float)((red - green) * (red - green) + (red - blue) * (green - blue))));
						lookUpBGR2HSI[index + H] = (short)(angle * 57.29577951f);
						if(blue > green) {
							lookUpBGR2HSI[index + H] = 360 - lookUpBGR2HSI[index + H];
						}
					}

					index += 3;
				}
			}
		}

		return 1;
	}
}

static int mallocLookUpTableHSI2BGR(void)
{

}

static int MallocLookUpTableBGR2YCbCr(void)
{
	int red, green, blue;
	int index = 0;
	const int Y = 2, Cb = 1, Cr = 0;
	lookUpBGR2YCbCr = (UCHAR *)malloc(sizeof(UCHAR) * 256 * 256 * 256 * 3);

	if(lookUpBGR2YCbCr == NULL) {
		return 0;
	} else {
		mallocValue |= 0x0040;

		// BGR2YCbCr 계산
		for(red = 0; red < 256; red++) {
			for(green = 0; green < 256; green++) {
				for(blue = 0; blue < 256; blue++) {					
					lookUpBGR2YCbCr[index + Y] = (UCHAR)(16 + 1.0 / 256.0 * (65.738 * red + 129.057 * green + 25.064 * blue));
					lookUpBGR2YCbCr[index + Cb] = (UCHAR)(128 + 1.0 / 256.0 * (-37.945 * red - 74.494 * green + 112.439 * blue));
					lookUpBGR2YCbCr[index + Cr] = (UCHAR)(128 + 1.0 / 256.0 * (112.439 * red - 94.154 * green - 18.285 * blue));

					index += 3;
				}
			}
		}

		return 1;
	}
}

static int MallocLookUpTableYCbCr2BGR(void)
{
	int y, cb, cr;
	int index = 0;
	const int R = 2, G = 1, B = 0;
	lookUpYCbCr2BGR = (UCHAR *)malloc(sizeof(UCHAR) * 256 * 256 * 256 * 3);

	if(lookUpYCbCr2BGR == NULL) {
		return 0;
	} else {
		mallocValue |= 0x0080;

		// YCbCr2BGR 계산
		for(y = 0; y < 256; y++) {
			for(cb = 0; cb < 256; cb++) {
				for(cr = 0; cr < 256; cr++) {
					//lookUpYCbCr2BGR[index + R] = CLIP((UCHAR)(1.0 / 256.0 * (298.082 * y + 408.583 * cr) - 222.921));
					//lookUpYCbCr2BGR[index + G] = CLIP((UCHAR)(1.0 / 256.0 * (298.082 * y - 100.291 * cb) + 135.576));
					//lookUpYCbCr2BGR[index + B] = CLIP((UCHAR)(1.0 / 256.0 * (298.082 * y + 516.412 * cb) - 276.836));
					lookUpYCbCr2BGR[index + R] = (UCHAR)(CLIP(1.0 * y + 1.402 * (cr - 128)));
					lookUpYCbCr2BGR[index + G] = (UCHAR)(CLIP(1.0 * y - 0.34414 * (cb - 128) - 0.71414 * (cr - 128)));
					lookUpYCbCr2BGR[index + B] = (UCHAR)(CLIP(1.0 * y + 1.772 * (cb - 128)));

					index += 3;
				}
			}
		}

		return 1;
	}
}

static int mallocLookUpTableBGR2YUV844(void)
{

}

static int mallocLookUpTableYUV8442BGR(void)
{

}

static IMG_INFO *convertBGR2GRAY(IMG_INFO *src)
{
	int i, jBuf, jSrc;
	IMG_INFO *buf = NULL;
	UCHAR *pSrc, *pBuf;
	const int R = 2, G = 1, B = 0;

	if((mallocValue & 0x0001) != 0x0001) {
		if(MallocLookUpTableBGR2GRAY() == 0) {
			return NULL;
		}
	}

	if(src->m_nChannels != 3 || src->m_nDepth != D_U8) {
		return NULL;
	}

	buf = CreateImageInfo(src->m_nWidth, src->m_nHeight, 1, D_U8);
	if(buf == NULL) {
		return NULL;
	}

	for(i = 0; i < src->m_nHeight; i++) {
		pSrc = &src->m_pImgU8[i * src->m_nWStep];
		pBuf = &buf->m_pImgU8[i * buf->m_nWStep];
		for(jBuf = 0, jSrc = 0; jBuf < buf->m_nWidth; jBuf++, jSrc += 3) {
			pBuf[jBuf] = lookUpBGR2GRAY[(pSrc[jSrc + R] << 16) + (pSrc[jSrc + G] << 8) + pSrc[jSrc + B]];
		}
	}

	return buf;
}

static IMG_INFO *convertGRAY2BGR(IMG_INFO *src)
{
	int i, jBuf, jSrc;
	IMG_INFO *buf = NULL;
	UCHAR *pSrc, *pBuf;
	const int R = 2, G = 1, B = 0;

	if((mallocValue & 0x0002) != 0x0002) {
		if(MallocLookUpTableGRAY2BGR() == 0) {
			return NULL;
		}
	}

	if(src->m_nChannels != 1 || src->m_nDepth != D_U8) {
		return NULL;
	}

	buf = CreateImageInfo(src->m_nWidth, src->m_nHeight, 3, D_U8);
	if(buf == NULL) {
		return NULL;
	}

	for(i = 0; i < src->m_nHeight; i++) {
		pSrc = &src->m_pImgU8[i * src->m_nWStep];
		pBuf = &buf->m_pImgU8[i * buf->m_nWStep];
		for(jBuf = 0, jSrc = 0; jBuf < buf->m_nWidth; jSrc++, jBuf++) {
			pBuf[jBuf * 3 + R] = pSrc[jSrc];
			pBuf[jBuf * 3 + G] = pSrc[jSrc];
			pBuf[jBuf * 3 + B] = pSrc[jSrc];
		}
	}

	return buf;
}

static IMG_INFO *convertBGR2HSI(IMG_INFO *src)
{
	int i, jBuf, jSrc;
	IMG_INFO *buf = NULL;
	UCHAR *pSrc;
	USHORT *pBuf;
	const int R = 2, G = 1, B = 0;
	const int H = 2, S = 1, I = 0;
	int index;

	if((mallocValue & 0x0010) != 0x0010) {
		if(MallocLookUpTableBGR2HSI() == 0) {
			return NULL;
		}
	}

	if(src->m_nChannels != 3 || src->m_nDepth != D_U8) {
		return NULL;
	}

	buf = CreateImageInfo(src->m_nWidth, src->m_nHeight, 3, D_U16);
	if(buf == NULL) {
		return NULL;
	}

	for(i = 0; i < src->m_nHeight; i++) {
		pSrc = &src->m_pImgU8[i * src->m_nWStep];
		pBuf = &buf->m_pImgU16[i * buf->m_nWStep];
		for(jBuf = 0, jSrc = 0; jBuf < buf->m_nWidth; jSrc += 3, jBuf++) {
			index = ((pSrc[jSrc + R] << 16) + (pSrc[jSrc + G] << 8) + (pSrc[jSrc + B])) * 3;

			pBuf[jBuf * 3 + H] = lookUpBGR2HSI[index + H];
			pBuf[jBuf * 3 + S] = lookUpBGR2HSI[index + S];
			pBuf[jBuf * 3 + I] = lookUpBGR2HSI[index + I];
		}
	}

	return buf;
}

static IMG_INFO *convertBGR2YCbCr(IMG_INFO *src)
{
	int i, jBuf, jSrc;
	IMG_INFO *buf = NULL;
	UCHAR *pSrc;
	UCHAR *pBuf;
	const int R = 2, G = 1, B = 0;
	const int Y = 2, Cb = 1, Cr = 0;
	int index;

	if((mallocValue & 0x0040) != 0x0040) {
		if(MallocLookUpTableBGR2YCbCr() == 0) {
			return NULL;
		}
	}

	if(src->m_nChannels != 3 || src->m_nDepth != D_U8) {
		return NULL;
	}

	buf = CreateImageInfo(src->m_nWidth, src->m_nHeight, 3, D_U8);
	if(buf == NULL) {
		return NULL;
	}

	for(i = 0; i < src->m_nHeight; i++) {
		pSrc = &src->m_pImgU8[i * src->m_nWStep];
		pBuf = &buf->m_pImgU8[i * buf->m_nWStep];
		for(jBuf = 0, jSrc = 0; jBuf < buf->m_nWidth; jSrc += 3, jBuf++) {
			index = ((pSrc[jSrc + R] << 16) + (pSrc[jSrc + G] << 8) + (pSrc[jSrc + B])) * 3;

			pBuf[jBuf * 3 + Y] = lookUpBGR2YCbCr[index + Y];
			pBuf[jBuf * 3 + Cb] = lookUpBGR2YCbCr[index + Cb];
			pBuf[jBuf * 3 + Cr] = lookUpBGR2YCbCr[index + Cr];
		}
	}

	return buf;
}

static IMG_INFO *convertYCbCr2BGR(IMG_INFO *src)
{
	int i, jBuf, jSrc;
	IMG_INFO *buf = NULL;
	UCHAR *pSrc;
	UCHAR *pBuf;
	const int R = 2, G = 1, B = 0;
	const int Y = 2, Cb = 1, Cr = 0;
	int index;

	if((mallocValue & 0x0080) != 0x0080) {
		if(MallocLookUpTableYCbCr2BGR() == 0) {
			return NULL;
		}
	}

	if(src->m_nChannels != 3 || src->m_nDepth != D_U8) {
		return NULL;
	}

	buf = CreateImageInfo(src->m_nWidth, src->m_nHeight, 3, D_U8);
	if(buf == NULL) {
		return NULL;
	}

	for(i = 0; i < src->m_nHeight; i++) {
		pSrc = &src->m_pImgU8[i * src->m_nWStep];
		pBuf = &buf->m_pImgU8[i * buf->m_nWStep];
		for(jBuf = 0, jSrc = 0; jBuf < buf->m_nWStep; jSrc += 3, jBuf += 3) {
			index = ((pSrc[jSrc + Y] << 16) + (pSrc[jSrc + Cb] << 8) + (pSrc[jSrc + Cr])) * 3;

			pBuf[jBuf + R] = lookUpYCbCr2BGR[index + R];
			pBuf[jBuf + G] = lookUpYCbCr2BGR[index + G];
			pBuf[jBuf + B] = lookUpYCbCr2BGR[index + B];
		}
	}

	return buf;
}

int MallocColorLookUpTable(void)
{
	MallocLookUpTableBGR2GRAY();
	MallocLookUpTableGRAY2BGR();
	MallocLookUpTableBGR2HSI();
	MallocLookUpTableBGR2YCbCr();
	MallocLookUpTableYCbCr2BGR();

	return 0;
}

int ReleaseColorLookUpTable(void)
{
	if(lookUpBGR2GRAY != NULL) {
		free(lookUpBGR2GRAY);
		lookUpBGR2GRAY = NULL;
	}

	if(lookUpGRAY2BGR != NULL) {
		free(lookUpGRAY2BGR);
		lookUpGRAY2BGR = NULL;
	}

	if(lookUpBGR2BGR565 != NULL) {
		free(lookUpBGR2BGR565);
		lookUpBGR2BGR565 = NULL;
	}

	if(lookUpBGR5652BGR != NULL) {
		free(lookUpBGR5652BGR);
		lookUpBGR5652BGR = NULL;
	}

	if(lookUpBGR2HSI != NULL) {
		free(lookUpBGR2HSI);
		lookUpBGR2HSI = NULL;
	}

	if(lookUpHSI2BGR != NULL) {
		free(lookUpHSI2BGR);
		lookUpHSI2BGR = NULL;
	}

	if(lookUpBGR2YCbCr != NULL) {
		free(lookUpBGR2YCbCr);
		lookUpBGR2YCbCr = NULL;
	}

	if(lookUpYCbCr2BGR != NULL) {
		free(lookUpYCbCr2BGR);
		lookUpYCbCr2BGR = NULL;
	}

	if(lookUpBGR2YUV844 != NULL) {
		free(lookUpBGR2YUV844);
		lookUpBGR2YUV844 = NULL;
	}

	if(lookUpYUV8442BGR != NULL) {
		free(lookUpYUV8442BGR);
		lookUpYUV8442BGR = NULL;
	}

	return 0;
}

IMG_INFO *ConvertColorFormat(IMG_INFO *src, const int MODE)
{
	IMG_INFO *buf = NULL;

	switch(MODE) {
		case BGR2GRAY:
			buf = convertBGR2GRAY(src);
			break;
		case GRAY2BGR:
			buf = convertGRAY2BGR(src);
			break;
		case BGR2BGR565:
			break;
		case BGR5652BGR:
			break;
		case BGR2HSI:
			buf = convertBGR2HSI(src);
			break;
		case HSI2BGR:
			break;
		case BGR2YCbCr:
			buf = convertBGR2YCbCr(src);
			break;
		case YCbCr2BGR:
			buf = convertYCbCr2BGR(src);
			break;
		case BGR2YUV844:
			break;
		case YUV8442BGR:
			break;
	}

	return buf;
}

static int ucharExtract(IMG_INFO *dest, IMG_INFO *src, const int channel)
{
	int i, jDest, jSrc;
	UCHAR *pSrc;
	UCHAR *pDest;

	for(i = 0; i < src->m_nHeight; i++) {
		pSrc = &src->m_pImgU8[i * src->m_nWStep];
		pDest = &dest->m_pImgU8[i * dest->m_nWStep];
		for(jDest = 0, jSrc = channel; jDest < dest->m_nWStep; jSrc += src->m_nChannels, jDest++) {
			pDest[jDest] = pSrc[jSrc];
		}
	}

	return 1;
}

static int ushortExtract(IMG_INFO *dest, IMG_INFO *src, const int channel)
{
	int i, jDest, jSrc;
	USHORT *pSrc;
	USHORT *pDest;

	for(i = 0; i < src->m_nHeight; i++) {
		pSrc = &src->m_pImgU16[i * src->m_nWStep];
		pDest = &dest->m_pImgU16[i * dest->m_nWStep];
		for(jDest = 0, jSrc = channel; jDest < dest->m_nWStep; jSrc += src->m_nChannels, jDest++) {
			pDest[jDest] = pSrc[jSrc];
		}
	}

	return 1;
}

IMG_INFO *ExtractChannel(IMG_INFO *src, const int channel)
{
	IMG_INFO *buf = NULL;

	if(src->m_nChannels == 1 || src->m_nChannels < channel) {
		return NULL;
	}

	buf = CreateImageInfo(src->m_nWidth, src->m_nHeight, 1, src->m_nDepth);

	if(buf == NULL) {
		return NULL;
	}

	switch(src->m_nDepth) {
		case D_U8:
			ucharExtract(buf, src, channel);
			break;
		case D_U16:
			ushortExtract(buf, src, channel);
			break;
	}

	return buf;
}
