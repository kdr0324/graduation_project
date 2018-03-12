#ifndef _LABELING_H_
#define _LABELING_H_

#include "../Amazon/Config.h"
#include "../Common/Point.h"
#include "../Common/ImageManagement.h"

#define MAX_LABEL_COUNT 256

typedef struct tagObject {
	int areaSize;		// 물체의 영역 크기
	POINT2D left;
	POINT2D right;
	POINT2D bottom;
	POINT2D top;
	POINT2D center;
	int width;			// 영역의 너비
	int height;			// 영역의 높이
} OBJECT;

int CallLabel(IMG_INFO *src, IMG_INFO *dest, const U16 FORWARD, OBJECT *object, const int sizeFilter, const int MAX_LABEL);
int CallLabel2(IMG_INFO *src, IMG_INFO *dest, const U16 FORWARD, OBJECT *object, const int sizeFilter, const int MAX_LABEL);


#endif // _LABELING_H_
