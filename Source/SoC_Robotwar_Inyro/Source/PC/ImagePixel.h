/*
	FORWARD를 값의 기준으로 픽셀단위로 처리되는 기본적인 영상처리 함수
	FORWARD는 0x0000(BLACK)이 될 수 없다.
*/

#ifndef _IMAGE_PIXEL_H_
#define _IMAGE_PIXEL_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../Common/ImageManagement.h"
#include "../Common/Global.h"

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
IMG_INFO *Binary_HSI(HSIVALUE *src);
IMG_INFO *Binary_HS(HSVALUE *src);
IMG_INFO *Binary_HI(HIVALUE *src);
IMG_INFO *Binary_SI(SIVALUE *src);
IMG_INFO *Binary_H(HVALUE *src);
IMG_INFO *Binary_S(SVALUE *src);
IMG_INFO *Binary_I(IVALUE *src);

IMG_INFO *DilationBinary(IMG_INFO *src);
IMG_INFO *ErosionBinary(IMG_INFO *src);
IMG_INFO *MedianBinary(IMG_INFO *src);

#ifdef __cplusplus
}
#endif

#endif // _IMAGE_PIXEL_H_
