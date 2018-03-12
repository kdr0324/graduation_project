/*
	FORWARD를 값의 기준으로 픽셀단위로 처리되는 기본적인 영상처리 함수
	FORWARD는 0x0000(BLACK)이 될 수 없다.
*/

#ifndef _IMAGE_PIXEL_H_
#define _IMAGE_PIXEL_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Config.h"
#include "../Common/ImageManagement.h"

#define FLIP_V 0
#define FLIP_H 1
#define FLIP_V_H 2

typedef struct _tagValue {
	int min;
	int max;
}VALUE;

typedef struct _tagHSIVALUE {
	IMG_INFO *hImg;
	IMG_INFO *sImg;
	IMG_INFO *iImg;
	VALUE h;
	VALUE s;
	VALUE i;
}HSIVALUE;

typedef struct _tagHSVALUE {
	IMG_INFO *hImg;
	IMG_INFO *sImg;
	VALUE h;
	VALUE s;
}HSVALUE;

typedef struct _tagHIVALUE {
	IMG_INFO *hImg;
	IMG_INFO *iImg;
	VALUE h;
	VALUE i;
}HIVALUE;

typedef struct _tagSIVALUE {
	IMG_INFO *sImg;
	IMG_INFO *iImg;
	VALUE s;
	VALUE i;
}SIVALUE;

typedef struct _tagHVALUE {
	IMG_INFO *hImg;
	VALUE h;
}HVALUE;

typedef struct _tagSVALUE {
	IMG_INFO *sImg;
	VALUE s;
}SVALUE;

typedef struct _tagIVALUE {
	IMG_INFO *iImg;
	VALUE i;
}IVALUE;

#ifdef __cplusplus
extern "C" {
#endif

/* Translation */
int binary_HSI(HSIVALUE *src, IMG_INFO *dest, const U16 FORWARD);
int binary_HS(HSVALUE *src, IMG_INFO *dest, const U16 FORWARD);
int binary_HI(HIVALUE *src, IMG_INFO *dest, const U16 FORWARD);
int binary_SI(SIVALUE *src, IMG_INFO *dest, const U16 FORWARD);
int binary_H(HVALUE *src, IMG_INFO *dest, const U16 FORWARD);
int binary_S(SVALUE *src, IMG_INFO *dest, const U16 FORWARD);
int binary_I(IVALUE *src, IMG_INFO *dest, const U16 FORWARD);

int dilationBinary(IMG_INFO *src, IMG_INFO *dest, const U16 FORWARD);
int erosionBinary(IMG_INFO *src, IMG_INFO *dest, const U16 FORWARD);
int medianBinary(IMG_INFO *src, IMG_INFO *dest, const U16 FORWARD);


int InverseImage(IMG_INFO *src, const int FORWARD);
int Gaussian(IMG_INFO *src, IMG_INFO *dest);
int SobelEdge(IMG_INFO *src, IMG_INFO *dilation, IMG_INFO *dest, int * Line);
int Laplacian(IMG_INFO *src, IMG_INFO *dest);
int CropImage(IMG_INFO *src, IMG_INFO *dest, const short xs, const short ys, const short xe, const short ye);
int FilpImage(IMG_INFO *src, IMG_INFO *dest);
int CopyImageForView(IMG_INFO *src, IMG_INFO *dest, const short xs, const short ys);
int GetIntegralImage(IMG_INFO *src, IMG_INFO *dest);
int GetBinaryCount(IMG_INFO *src, IMG_INFO *dest);
int GetAreaSum(IMG_INFO *src, const int xs, const int ys, const int xe, const int ye);
int GetAreaSum32(IMG_INFO *src, const int xs, const int ys, const int xe, const int ye);

#ifdef __cplusplus
}
#endif

#endif // _IMAGE_PIXEL_H_
