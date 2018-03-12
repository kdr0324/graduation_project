/*
bitmap������ OPEN
bitmap������ SAVE
*/

#ifndef _BITMAP_H_
#define _BITMAP_H_

#include <stdio.h>
#include <stdlib.h>

#include "../Common/ImageManagement.h"

#ifdef AMAZON_WIDTHBYTES
#undef AMAZON_WIDTHBYTES
#endif // AMAZON_WIDTHBYTES

#define AMAZON_WIDTHBYTES(bits) (((bits) + 31) / 32 * 4) // ���� ���α��̴� 4����Ʈ�� ������� ��

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

	/* ���� MFC���� ����ϱ� ���� */
	void SetFile8bits(BITMAPFILEHEADER *bmHf, const int width, const int height);
	void SetFile24bits(BITMAPFILEHEADER *bmHf, const int width, const int height);

	void SetBmInfo8bits(BITMAPINFOHEADER *bmHi, const int width, const int height);
	void SetBmInfo24bits(BITMAPINFOHEADER *bmHi, const int width, const int height);

	// bitmap�� ������ OPEN & SAVE �ϴ� �Լ�
	IMG_INFO *OpenBitmap(const char *filename);
	int SaveBitmap(const char *filename, const IMG_INFO *image);	// ��ȯ �� 0 : ����, 1 : ����

	// RGB565�� ������ OPEN & SAVE �ϴ� �Լ�
	// �ʺ�(4bytes & 4�� ���), ����(4bytes), �̹���(�ʺ� X ����), ������ ����
	IMG_INFO *OpenRawRGB565(const char *filename);
	int SaveRawRGB565(const char *filename, const IMG_INFO *image);	// ��ȯ �� 0 : ����, 1 : ����
	

#ifdef __cplusplus
}
#endif

#endif // _BITMAP_H_
