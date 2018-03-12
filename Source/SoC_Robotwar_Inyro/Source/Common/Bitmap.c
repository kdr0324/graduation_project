#include "stdafx.h"

#include "bitmap.h"

void SetFile8bits(BITMAPFILEHEADER *bmHf, const int width, const int height)
{
	int rwsize = AMAZON_WIDTHBYTES(width * 8);
	bmHf->bfType = 0x4D42;
	bmHf->bfSize = 54 + sizeof(RGBQUAD) * 256 + sizeof(U8) * rwsize * height;
	bmHf->bfReserved1 = 0;
	bmHf->bfReserved2 = 0;
	bmHf->bfOffBits = 54 + sizeof(RGBQUAD) * 256;
}

void SetFile24bits(BITMAPFILEHEADER *bmHf, const int width, const int height)
{
	int rwsize = AMAZON_WIDTHBYTES(width * 24);
	bmHf->bfType = 0x4D42;
	bmHf->bfSize = 54 + sizeof(U8) * rwsize * height;
	bmHf->bfReserved1 = 0;
	bmHf->bfReserved2 = 0;
	bmHf->bfOffBits = 54;
}

void SetBmInfo8bits(BITMAPINFOHEADER *bmHi, const int width, const int height)
{
	int rwsize = AMAZON_WIDTHBYTES(width * 8);
	bmHi->biSize = sizeof(BITMAPINFOHEADER);
	bmHi->biWidth = width;
	bmHi->biHeight = height;
	bmHi->biSizeImage = rwsize * height;
	bmHi->biBitCount = 8;
	bmHi->biPlanes = 1;
	bmHi->biClrUsed = 256;
	bmHi->biCompression = 0;
	bmHi->biXPelsPerMeter = 0;
	bmHi->biYPelsPerMeter = 0;
	bmHi->biClrImportant = 0;
}

void SetBmInfo24bits(BITMAPINFOHEADER *bmHi, const int width, const int height)
{
	int rwsize = AMAZON_WIDTHBYTES(width * 24);
	bmHi->biSize = sizeof(BITMAPINFOHEADER);
	bmHi->biWidth = width;
	bmHi->biHeight = height;
	bmHi->biSizeImage = rwsize * height;
	bmHi->biBitCount = 24;
	bmHi->biPlanes = 1;
	bmHi->biClrUsed = 0;
	bmHi->biCompression = 0;
	bmHi->biXPelsPerMeter = 0;
	bmHi->biYPelsPerMeter = 0;
	bmHi->biClrImportant = 0;
}

/* Using PRImage */
IMG_INFO *OpenBitmap(const char *filename)
{
	BITMAPSTRUCT bm;
	FILE *fp = NULL;
	IMG_INFO *image = NULL;

	fp = fopen(filename, "rb");
	if(fp == NULL) {
		printf("파일을 여는데 실패하였습니다.\n");
		return NULL;
	}

	fread(&(bm.bmHf.bfType), sizeof(bm.bmHf.bfType), 1, fp);
	fread(&(bm.bmHf.bfSize), sizeof(bm.bmHf.bfSize), 1, fp);
	fread(&(bm.bmHf.bfReserved1), sizeof(bm.bmHf.bfReserved1), 1, fp);
	fread(&(bm.bmHf.bfReserved2), sizeof(bm.bmHf.bfReserved2), 1, fp);
	fread(&(bm.bmHf.bfOffBits), sizeof(bm.bmHf.bfOffBits), 1, fp);
	fread(&(bm.bmHi.biSize), sizeof(bm.bmHi.biSize), 1, fp);
	fread(&(bm.bmHi.biWidth), sizeof(bm.bmHi.biWidth), 1, fp);
	fread(&(bm.bmHi.biHeight), sizeof(bm.bmHi.biHeight), 1, fp);
	fread(&(bm.bmHi.biPlanes), sizeof(bm.bmHi.biPlanes), 1, fp);
	fread(&(bm.bmHi.biBitCount), sizeof(bm.bmHi.biBitCount), 1, fp);
	fread(&(bm.bmHi.biCompression), sizeof(bm.bmHi.biCompression), 1, fp);
	fread(&(bm.bmHi.biSizeImage), sizeof(bm.bmHi.biSizeImage), 1, fp);
	fread(&(bm.bmHi.biXPelsPerMeter), sizeof(bm.bmHi.biXPelsPerMeter), 1, fp);
	fread(&(bm.bmHi.biYPelsPerMeter), sizeof(bm.bmHi.biYPelsPerMeter), 1, fp);
	fread(&(bm.bmHi.biClrUsed), sizeof(bm.bmHi.biClrUsed), 1, fp);
	fread(&(bm.bmHi.biClrImportant), sizeof(bm.bmHi.biClrImportant), 1, fp);

	if(bm.bmHf.bfType != 0x4D42) {
		printf("BITMAP 형식이 아닙니다.\n");
		fclose(fp);
		return NULL;
	}

	if(bm.bmHi.biBitCount != 8 && bm.bmHi.biBitCount != 24) {
		printf("8비트 또는 24비트가 아닙니다.\n");
		fclose(fp);
		return NULL;
	} else if(bm.bmHi.biCompression) {
		printf("영상이 압축되어 있습니다.\n");
		fclose(fp);
		return NULL;
	}

	if(bm.bmHi.biBitCount == 8) {
		bm.palRGB = (RGBQUAD *)malloc(sizeof(RGBQUAD) * 256);
		if(bm.palRGB == NULL) {
			printf("팔레트 메모리 할당을 실패하였습니다.\n");
			fclose(fp);
			return NULL;
		}
		fread(bm.palRGB, sizeof(RGBQUAD), 256, fp);
		image = CreateImageInfo(bm.bmHi.biWidth, bm.bmHi.biHeight, 1, D_U8);
		free(bm.palRGB);
	} else {
		image = CreateImageInfo(bm.bmHi.biWidth, bm.bmHi.biHeight, 3, D_U8);
	}

	if(image == NULL) {
		printf("이미지 버퍼 할당을 실패하였습니다.\n");
		fclose(fp);
		return NULL;
	}

	fread(image->m_pImgU8, sizeof(U8), image->m_nImageSize, fp);

	fclose(fp);

	return image;
}

int SaveBitmap(const char *filename, const IMG_INFO *image)
{
	BITMAPSTRUCT bm;
	FILE *fp = NULL;
	int i;
	short rwsize;
	unsigned char tmp[10];

	if(image->m_nDepth != D_U8) {
		printf("No Type.\n");
		return 0;
	}

	if(image->m_nChannels == 1) {
		SetFile8bits(&bm.bmHf, image->m_nWidth, image->m_nHeight);
		SetBmInfo8bits(&bm.bmHi, image->m_nWidth, image->m_nHeight);
		bm.palRGB = (RGBQUAD *)malloc(sizeof(RGBQUAD) * 256);
		if(bm.palRGB == NULL) {
			printf("No Allocate Pallete.\n");
			return 0;
		}
		// 팔레트 메모리를 초기화 하는 과정
		for(i = 0; i < 256; i++) {
			bm.palRGB[i].rgbRed = bm.palRGB[i].rgbGreen = bm.palRGB[i].rgbBlue = i;
		}

	} else if(image->m_nChannels == 3) {
		SetFile24bits(&bm.bmHf, image->m_nWidth, image->m_nHeight);
		SetBmInfo24bits(&bm.bmHi, image->m_nWidth, image->m_nHeight);
	} else {
		printf("No Channel.\n");
		return 0;
	}

	fp = fopen(filename, "wb");
	if (fp == NULL) {
		printf("No Create.\n");
		return 0;
	}

	fwrite(&(bm.bmHf.bfType), sizeof(bm.bmHf.bfType), 1, fp);
	fwrite(&(bm.bmHf.bfSize), sizeof(bm.bmHf.bfSize), 1, fp);
	fwrite(&(bm.bmHf.bfReserved1), sizeof(bm.bmHf.bfReserved1), 1, fp);
	fwrite(&(bm.bmHf.bfReserved2), sizeof(bm.bmHf.bfReserved2), 1, fp);
	fwrite(&(bm.bmHf.bfOffBits), sizeof(bm.bmHf.bfOffBits), 1, fp);
	fwrite(&(bm.bmHi.biSize), sizeof(bm.bmHi.biSize), 1, fp);
	fwrite(&(bm.bmHi.biWidth), sizeof(bm.bmHi.biWidth), 1, fp);
	fwrite(&(bm.bmHi.biHeight), sizeof(bm.bmHi.biHeight), 1, fp);
	fwrite(&(bm.bmHi.biPlanes), sizeof(bm.bmHi.biPlanes), 1, fp);
	fwrite(&(bm.bmHi.biBitCount), sizeof(bm.bmHi.biBitCount), 1, fp);
	fwrite(&(bm.bmHi.biCompression), sizeof(bm.bmHi.biCompression), 1, fp);
	fwrite(&(bm.bmHi.biSizeImage), sizeof(bm.bmHi.biSizeImage), 1, fp);
	fwrite(&(bm.bmHi.biXPelsPerMeter), sizeof(bm.bmHi.biXPelsPerMeter), 1, fp);
	fwrite(&(bm.bmHi.biYPelsPerMeter), sizeof(bm.bmHi.biYPelsPerMeter), 1, fp);
	fwrite(&(bm.bmHi.biClrUsed), sizeof(bm.bmHi.biClrUsed), 1, fp);
	fwrite(&(bm.bmHi.biClrImportant), sizeof(bm.bmHi.biClrImportant), 1, fp);

	if(bm.bmHi.biBitCount == 8) {
		fwrite(bm.palRGB, sizeof(RGBQUAD), 256, fp);
		free(bm.palRGB);
	}

	fwrite(image->m_pImgU8, sizeof(U8), image->m_nImageSize, fp);

	//rwsize = (short)AMAZON_WIDTHBYTES(bm.bmHi.biWidth * 8);
	//for (i = 0; i < bm.bmHi.biHeight; i++) {
	//	fwrite(&image->m_pImgU8[i * bm.bmHi.biWidth], sizeof(U8), bm.bmHi.biWidth, fp);
	//	fwrite(tmp, sizeof(U8), rwsize - bm.bmHi.biWidth, fp);
	//}

	fclose(fp);

	return 1;
}

IMG_INFO *OpenRawRGB565(const char *filename)
{
	FILE *fp = NULL;
	IMG_INFO *image = NULL;
	int width, height;

	fp = fopen(filename, "rb");
	if (fp == NULL) {
		printf("파일을 여는데 실패하였습니다.\n");
		return NULL;
	}

	fread(&width, sizeof(width), 1, fp);
	if (width & 0x00000003) {
		printf("4의 배수가 아닙니다.\n");
		fclose(fp);
		return NULL;
	}
	fread(&height, sizeof(height), 1, fp);

	image = CreateImageInfo(width, height, 1, D_U16);

	if (image == NULL) {
		printf("이미지 버퍼 할당을 실패하였습니다.\n");
		fclose(fp);
		return NULL;
	}

	fread(image->m_pImgU16, sizeof(U16), image->m_nImageSize, fp);
	fclose(fp);

	return image;
}

int SaveRawRGB565(const char *filename, const IMG_INFO *image)
{
	FILE *fp = NULL;

	if (image->m_nDepth != D_U16) {
		printf("저장이 불가능한 타입입니다.\n");
		return 0;
	}

	if (image->m_nWidth & 0x00000003) {
		printf("저장이 불가능한 타입입니다.\n");
		return 0;
	}

	fp = fopen(filename, "wb");
	if (fp == NULL) {
		printf("파일을 여는데 실패하였습니다.\n");
		return 0;
	}

	fwrite(&(image->m_nWidth), sizeof(image->m_nWidth), 1, fp);
	fwrite(&(image->m_nHeight), sizeof(image->m_nHeight), 1, fp);
	fwrite(image->m_pImgU16, sizeof(U16), image->m_nImageSize, fp);
	fclose(fp);

	return 0;
}
