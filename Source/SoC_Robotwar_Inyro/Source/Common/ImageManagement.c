#include "stdafx.h"

#include "../Common/ImageManagement.h"

static int uchar2uchar(U8 *dest, U8 *src, int nImageSize);
static int uchar2short(U16 *dest, U8 *src, const int nImageSize);

static int short2uchar(U8 *dest, U16 *src, const int nImageSize);
static int short2short(U16 *dest, U16 *src, int nImageSize);

// �̹����� ������ ����ü�� 'ALLOC'�ϴ� �Լ� 
IMG_INFO *CreateImageInfo(const int nWidth, const int nHeight, const short nChannels, const short nDepth)
{
	IMG_INFO *buf = NULL;
	int size, tmp;

	// buf �޸� �Ҵ�
	buf = (IMG_INFO *)malloc(sizeof(IMG_INFO));
	if(buf == NULL) {									// ��ȿ�� �˻�
		return NULL;
	}

	buf->m_nWidth = nWidth;								// �ʺ� ����
	buf->m_nHeight = nHeight;							// ���� ����
	buf->m_nChannels = nChannels;						// ä�� ����
	buf->m_nDepth = nDepth;								// ���� Ÿ�� ����
	buf->m_pImg = NULL;

	// �̹����� ������ �迭 ������ ����� 4�� ����� �����ִ� ����
	buf->m_nWStep = nWidth * nChannels;
	tmp = buf->m_nWStep & 0x00000003;
	buf->m_nWStep += tmp == 0 ? 0 : 4 - tmp;

	buf->m_nImageSize = size = buf->m_nWStep * buf->m_nHeight;	// �̹��� �迭�� ����
	size *= nDepth;

	buf->m_pImg = (void *)malloc(size);
	if(buf->m_pImg == NULL) {
		free(buf);
		buf = NULL;
		return NULL;
	}

	return buf;
}

// �̹����� �����ϴ� ����ü�� 'FREE'�ϴ� �Լ�
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

	buf->m_nWidth = nWidth;								// �ʺ� ����
	buf->m_nHeight = nHeight;							// ���� ����
	buf->m_nChannels = nChannels;						// ä�� ����
	buf->m_nDepth = nDepth;								// ���� Ÿ�� ����

	// �̹����� ������ �迭 ������ ����� 4�� ����� �����ִ� ����
	buf->m_nWStep = nWidth * nChannels;
	tmp = buf->m_nWStep & 0x00000003;
	buf->m_nWStep += tmp == 0 ? 0 : 4 - tmp;

	buf->m_nImageSize = buf->m_nWStep * buf->m_nHeight;	// �̹��� �迭�� ����
}

// �̹����� �����ϴ� ����ü�� '��ǥŸ���� ���� or ��ȯ�ϸ鼭 COPY' �Լ�
/*
IMG_INFO *src -> ���� �̹����� �����ϴ� ����ü
short nDepth -> COPY�ϴ� ������ ��ǥ Ÿ��
*/
IMG_INFO *CloneImageInfo(const IMG_INFO *src, const short nDepth)
{
	IMG_INFO *buf = NULL;
	int call = src->m_nDepth * 16 + nDepth;
	int size;

	// buf �޸� �Ҵ�
	buf = CreateImageInfo(src->m_nWidth, src->m_nHeight, src->m_nChannels, nDepth);
	if(buf == NULL) {									// ��ȿ�� �˻�
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

/* UCHAR -> ��ȯ */
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

/* short -> ��ȯ */
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
