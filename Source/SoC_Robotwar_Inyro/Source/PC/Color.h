/*
	일반적으로 사용하는 색상체계는 BGR이다.
	BGR란? 3-channel color 영상에서
	B는 '+0'의 Index를
	G는 '+1'의 Index를
	B는 '+2'의 Index를 가진다.

	HSI(or HSV)
	I는 명도(으)로 '+0+의 Index를
	S는	채도(으)로 '+1'의 Index를
	H는 색상(으)로 '+2'의 Index를 가진다.

	YUV -> 주로 Color Tv 수신에 많이 사용
	V는 색상(으)로 '+0'의 Index를
	U는 색상(으)로 '+1'의 Index를
	Y는 명도(으)로 '+2'의 Index를 가진다.

	BGR565는 MSB가 RED, LSB가 BLUE
*/

#ifndef _COLOR_H_
#define _COLOR_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../Common/Global.h"
#include "../Common/ImageManagement.h"

enum {
	// 3-channel Byte to 1-channel Byte
	BGR2GRAY = 0,

	// 1-channel Byte to 3-channel Byte
	GRAY2BGR = 1,

	// 3-channel Byte to 1-channel Short
	BGR2BGR565 = 2,

	// 1-channel Short to 3-channel Byte
	BGR5652BGR = 3,

	// 3-channel Byte to 3-channel Short
	BGR2HSI = 4,
	BGR2HSV = BGR2HSI,

	// 3-channel Short to 3-channel Byte
	HSI2BGR = 5,
	HSV2BGR = HSI2BGR,

	// 3-channel Byte to 3-channel Byte
	BGR2YCbCr = 6,

	// 3-channel Byte to 3-channel Byte
	YCbCr2BGR = 7,

	// 3-channel Byte to 1-channel Short
	BGR2YUV844 = 8,

	// 1-channel Short to 3-channel Byte
	YUV8442BGR = 9
};

#ifdef __cplusplus
extern "C" {
#endif

	// 이미지 형식을 바꾸어 주는 함수
	int MallocColorLookUpTable(void);
	int ReleaseColorLookUpTable(void);
	IMG_INFO *ConvertColorFormat(IMG_INFO *src, const int MODE);
	IMG_INFO *ExtractChannel(IMG_INFO *src, const int channel);

#ifdef __cplusplus
}
#endif

#endif // _COLOR_H_