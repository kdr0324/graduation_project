#ifndef _LABELING_H_
#define _LABELING_H_

#include "../Common/Global.h"
#include "../Common/ImageManagement.h"
#include "../Common/Point.h"

typedef struct tagObject {
	int areaSize;		// ��ü�� ���� ũ��
	POINT2D left;
	POINT2D right;
	POINT2D bottom;
	POINT2D top;
	POINT2D center;
	int width;			// ������ �ʺ�
	int height;			// ������ ����
	double histo[360];
	int lost_object=1;	
} OBJECT;

int CallLabel(IMG_INFO *src, OBJECT *object, const int sizeFilter, const int MAX_LABEL);

#endif // _LABELING_H_
