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
	int m_nWidth;			// 너비
	int m_nHeight;			// 높이
	short m_nChannels;		// 채널(1, 3)
	short m_nDepth;			// 깊이 타입
	int m_nWStep;			// 이미지 배열의 가로 길이	
	int m_nImageSize;		// 이미지 배열의 갯수

	// 포인터 변수이므로 모두 4bytes를 부여 받음
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
