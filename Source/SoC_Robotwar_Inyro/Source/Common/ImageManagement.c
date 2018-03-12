#include "stdafx.h"

#include "../Common/ImageManagement.h"

static int uchar2uchar(U8 *dest, U8 *src, int nImageSize);
static int uchar2short(U16 *dest, U8 *src, const int nImageSize);

static int short2uchar(U8 *dest, U16 *src, const int nImageSize);
static int short2short(U16 *dest, U16 *src, int nImageSize);

// 이미지를 저장할 구조체를 'ALLOC'하는 함수 
IMG_INFO *CreateImageInfo(const int nWidth, const int nHeight, const short nChannels, const short nDepth)
{
	IMG_INFO *buf = NULL;
	int size, tmp;

	// buf 메모리 할당
	buf = (IMG_INFO *)malloc(sizeof(IMG_INFO));
	if(buf == NULL) {									// 유효성 검사
		return NULL;
	}

	buf->m_nWidth = nWidth;								// 너비 저장
	buf->m_nHeight = nHeight;							// 높이 저장
	buf->m_nChannels = nChannels;						// 채널 저장
	buf->m_nDepth = nDepth;								// 깊이 타입 저장
	buf->m_pImg = NULL;

	// 이미지를 저장할 배열 가로의 사이즈를 4의 배수로 맞춰주는 과정
	buf->m_nWStep = nWidth * nChannels;
	tmp = buf->m_nWStep & 0x00000003;
	buf->m_nWStep += tmp == 0 ? 0 : 4 - tmp;

	buf->m_nImageSize = size = buf->m_nWStep * buf->m_nHeight;	// 이미지 배열의 갯수
	size *= nDepth;

	buf->m_pImg = (void *)malloc(size);
	if(buf->m_pImg == NULL) {
		free(buf);
		buf = NULL;
		return NULL;
	}

	return buf;
}

// 이미지를 저장하는 구조체를 'FREE'하는 함수
void ReleaseImageInfo(IMG_INFO **buf)
{
	if(*buf != NULL) {
		if((*buf)->m_pImg != NULL) {
			free((*buf)->m_pImg);
			(*buf)->m_pImg = NULL;
		}
		free(*buf);
		*buf = NULL;
	}

	return;
}

void SetImageInfo(IMG_INFO *buf, const int nWidth, const int nHeight, const short nChannels, const short nDepth)
{
	int tmp;

	buf->m_nWidth = nWidth;								// 너비 저장
	buf->m_nHeight = nHeight;							// 높이 저장
	buf->m_nChannels = nChannels;						// 채널 저장
	buf->m_nDepth = nDepth;								// 깊이 타입 저장

	// 이미지를 저장할 배열 가로의 사이즈를 4의 배수로 맞춰주는 과정
	buf->m_nWStep = nWidth * nChannels;
	tmp = buf->m_nWStep & 0x00000003;
	buf->m_nWStep += tmp == 0 ? 0 : 4 - tmp;

	buf->m_nImageSize = buf->m_nWStep * buf->m_nHeight;	// 이미지 배열의 갯수
}

// 이미지를 저장하는 구조체의 '대표타입을 유지 or 변환하면서 COPY' 함수
/*
IMG_INFO *src -> 원본 이미지를 저장하는 구조체
short nDepth -> COPY하는 버퍼의 대표 타입
*/
IMG_INFO *CloneImageInfo(const IMG_INFO *src, const short nDepth)
{
	IMG_INFO *buf = NULL;
	int call = src->m_nDepth * 16 + nDepth;
	int size;

	// buf 메모리 할당
	buf = CreateImageInfo(src->m_nWidth, src->m_nHeight, src->m_nChannels, nDepth);
	if(buf == NULL) {									// 유효성 검사
		return NULL;
	}

	size = src->m_nImageSize;
	switch(call) {

	case 0x00:
		uchar2uchar(buf->m_pImgU8, src->m_pImgU8, src->m_nImageSize);
		break;
	case 0x01:
		uchar2short(buf->m_pImgU16, src->m_pImgU8, src->m_nImageSize);
		break;

	case 0x10:
		short2uchar(buf->m_pImgU8, src->m_pImgU16, src->m_nImageSize);
		break;
	case 0x11:
		short2short(buf->m_pImgU16, src->m_pImgU16, src->m_nImageSize);
		break;
	}

	return buf;
}

/* UCHAR -> 변환 */
static int uchar2uchar(U8 *dest, U8 *src, int nImageSize)
{
	nImageSize *= sizeof(U8);

	memcpy(dest, src, nImageSize);

	return 1;
}

static int uchar2short(U16 *dest, U8 *src, const int nImageSize)
{
	int i;

	for(i = nImageSize - 1; i >= 0; --i)
		dest[i] = (short)src[i];

	return 1;
}

/* short -> 변환 */
static int short2uchar(U8 *dest, U16 *src, const int nImageSize)
{
	int i;

	for(i = nImageSize - 1; i >= 0; --i)
		dest[i] = (U8)CLIP(src[i]);

	return 1;
}

static int short2short(U16 *dest, U16 *src, int nImageSize)
{
	nImageSize *= sizeof(short);

	memcpy(dest, src, nImageSize);

	return 1;
}
