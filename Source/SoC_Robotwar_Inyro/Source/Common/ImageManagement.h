#ifndef _IMAGE_MANAGEMENT_H_
#define _IMAGE_MANAGEMENT_H_

#include "Global.h"

enum DEPTH_TABLE {
	D_U8 = 1,			// BYTE
	D_U16,			// SHORT
	D_S32,
	D_U32			// INT
};

typedef struct tagSoCImageinfo {
	int m_nWidth;			// �ʺ�
	int m_nHeight;			// ����
	short m_nChannels;		// ä��(1, 3)
	short m_nDepth;			// ���� Ÿ��
	int m_nWStep;			// �̹��� �迭�� ���� ����	
	int m_nImageSize;		// �̹��� �迭�� ����

	// ������ �����̹Ƿ� ��� 4bytes�� �ο� ����
	union {
		void *m_pImg;
		U8 *m_pImgU8;
		U16 *m_pImgU16;
		U32 *m_pImgU32;
	};
} IMG_INFO;

#ifdef __cplusplus
extern "C" {
#endif

	IMG_INFO *CreateImageInfo(const int nWidth, const int nHeight, const short nChannels, const short nDepth);
	void ReleaseImageInfo(IMG_INFO **buf);
	void SetImageInfo(IMG_INFO *buf, const int nWidth, const int nHeight, const short nChannels, const short nDepth);
	IMG_INFO *CloneImageInfo(const IMG_INFO *src, const short nDepth);

#ifdef __cplusplus
}
#endif

#endif // _IMAGE_MANAGEMENT_H_
