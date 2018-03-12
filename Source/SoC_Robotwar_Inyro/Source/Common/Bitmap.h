/*
bitmap파일을 OPEN
bitmap파일을 SAVE
*/

#ifndef _BITMAP_H_
#define _BITMAP_H_

#include <stdio.h>
#include <stdlib.h>

#include "../Common/ImageManagement.h"

#ifdef AMAZON_WIDTHBYTES
#undef AMAZON_WIDTHBYTES
#endif // AMAZON_WIDTHBYTES

#define AMAZON_WIDTHBYTES(bits) (((bits) + 31) / 32 * 4) // 영상 가로길이는 4바이트의 배수여야 함

#ifndef _WINGDI_

typedef  unsigned char	BYTE;
typedef  unsigned short	WORD;
typedef  unsigned int	DWORD;
typedef  int			LONG;

typedef struct tagBITMAPFILEHEADER {
	WORD    bfType;
	DWORD   bfSize;
	WORD    bfReserved1;
	WORD    bfReserved2;
	DWORD   bfOffBits;
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
	DWORD      biSize;
	LONG       biWidth;
	LONG       biHeight;
	WORD       biPlanes;
	WORD       biBitCount;
	DWORD      biCompression;
	DWORD      biSizeImage;
	LONG       biXPelsPerMeter;
	LONG       biYPelsPerMeter;
	DWORD      biClrUsed;
	DWORD      biClrImportant;
} BITMAPINFOHEADER;

typedef struct tagRGBQUAD {
	BYTE    rgbBlue;
	BYTE    rgbGreen;
	BYTE    rgbRed;
	BYTE    rgbReserved;
} RGBQUAD;

#endif // _WINGDI_

typedef struct BITMAPSTRUCT {
	BITMAPFILEHEADER bmHf;
	BITMAPINFOHEADER bmHi;
	RGBQUAD *palRGB;
} BITMAPSTRUCT;

#ifdef __cplusplus
extern "C" {
#endif

	/* 추후 MFC에서 사용하기 위해 */
	void SetFile8bits(BITMAPFILEHEADER *bmHf, const int width, const int height);
	void SetFile24bits(BITMAPFILEHEADER *bmHf, const int width, const int height);

	void SetBmInfo8bits(BITMAPINFOHEADER *bmHi, const int width, const int height);
	void SetBmInfo24bits(BITMAPINFOHEADER *bmHi, const int width, const int height);

	// bitmap로 파일을 OPEN & SAVE 하는 함수
	IMG_INFO *OpenBitmap(const char *filename);
	int SaveBitmap(const char *filename, const IMG_INFO *image);	// 반환 값 0 : 실패, 1 : 성공

	// RGB565로 파일을 OPEN & SAVE 하는 함수
	// 너비(4bytes & 4의 배수), 높이(4bytes), 이미지(너비 X 높이), 순으로 저장
	IMG_INFO *OpenRawRGB565(const char *filename);
	int SaveRawRGB565(const char *filename, const IMG_INFO *image);	// 반환 값 0 : 실패, 1 : 성공
	

#ifdef __cplusplus
}
#endif

#endif // _BITMAP_H_
