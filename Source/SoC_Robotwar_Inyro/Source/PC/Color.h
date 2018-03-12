/*
	�Ϲ������� ����ϴ� ����ü��� BGR�̴�.
	BGR��? 3-channel color ���󿡼�
	B�� '+0'�� Index��
	G�� '+1'�� Index��
	B�� '+2'�� Index�� ������.

	HSI(or HSV)
	I�� ��(��)�� '+0+�� Index��
	S��	ä��(��)�� '+1'�� Index��
	H�� ����(��)�� '+2'�� Index�� ������.

	YUV -> �ַ� Color Tv ���ſ� ���� ���
	V�� ����(��)�� '+0'�� Index��
	U�� ����(��)�� '+1'�� Index��
	Y�� ��(��)�� '+2'�� Index�� ������.

	BGR565�� MSB�� RED, LSB�� BLUE
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

	// �̹��� ������ �ٲپ� �ִ� �Լ�
	int MallocColorLookUpTable(void);
	int ReleaseColorLookUpTable(void);
	IMG_INFO *ConvertColorFormat(IMG_INFO *src, const int MODE);
	IMG_INFO *ExtractChannel(IMG_INFO *src, const int channel);

#ifdef __cplusplus
}
#endif

#endif // _COLOR_H_