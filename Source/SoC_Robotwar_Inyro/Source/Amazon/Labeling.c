#include "stdafx.h"

#include "labeling.h"
#include "../Common/Point.h"

POINT2D ptr[9] = {
	{ -1, -1 },
	{ 0, -1 },
	{ 1, -1 },
	{ -1, 0 },
	{ 1, 0 },
	{ -1, 1 },
	{ 0, 1 },
	{ 1, 1 },
	{ 0, 0 }
};

/* �󺧹�ȣ�� �ű�� �Լ� */
static void labeling(IMG_INFO *src, const int x, const int y, const U16 FORWARD,
					 int *label_num, OBJECT *object, const int sizeFilter);

static int area_sort(const void *a, const void *b)
{
	int one = ((OBJECT *)a)->areaSize;
	int two = ((OBJECT *)b)->areaSize;

	if(one < two) {
		return 1;
	} else if(one > two) {
		return -1;
	} else {
		return 0;
	}
}

/* Ư�� �󺧰��� ��ȣ�� ������ġ�� ���ϴ� �Լ� */
int CallLabel(IMG_INFO *src, IMG_INFO *dest, const U16 FORWARD, OBJECT *object, const int sizeFilter, const int MAX_LABEL)
{
	int wStep = src->m_nWStep;
	int imageSize = src->m_nImageSize;
	U16 *pSrc = src->m_pImgU16;
	int i, x, y;
	int labelNum = 1;

	// ��ü�� ���� �ʱ�ȭ
	memset(&object[1], 0, sizeof(OBJECT) * MAX_LABEL);
	for(i = 0; i < imageSize; ++i) {
		if(pSrc[i] == FORWARD) {
			memset(&object[labelNum], 0, sizeof(OBJECT) * 1);

			pSrc[i] = labelNum;
			x = i % wStep;
			y = i / wStep;

			/* ��ȣ �ű�� �Լ� ȣ�� */
			labeling(src, x, y, FORWARD, &labelNum, object, sizeFilter);
			/* �󺧹�ȣ ���� */
			labelNum++;
		}
	}

	memcpy(dest->m_pImgU16, src->m_pImgU16, sizeof(U16) * imageSize);

	for(i = imageSize - 1; i >= 0; --i) {
		if(pSrc[i] != BLACK_16) {
			pSrc[i] = FORWARD;
		}
	}

	qsort((void *)&object[1], labelNum - 1, sizeof(OBJECT), area_sort);

	return labelNum;
}

/* Ư�� �󺧰��� ��ȣ�� ������ġ�� ���ϴ� �Լ� */
int CallLabel2(IMG_INFO *src, IMG_INFO *dest, const U16 FORWARD, OBJECT *object, const int sizeFilter, const int MAX_LABEL)
{
	int wStep = src->m_nWStep;
	int imageSize = src->m_nImageSize;
	U16 *pSrc = src->m_pImgU16;
	int i, x, y;
	int labelNum = 1;

	// ��ü�� ���� �ʱ�ȭ
	memset(&object[1], 0, sizeof(OBJECT) * MAX_LABEL);
	for (i = 0; i < imageSize; ++i) {
		if (pSrc[i] == FORWARD) {
			memset(&object[labelNum], 0, sizeof(OBJECT) * 1);

			pSrc[i] = labelNum;
			x = i % wStep;
			y = i / wStep;

			/* ��ȣ �ű�� �Լ� ȣ�� */
			labeling(src, x, y, FORWARD, &labelNum, object, sizeFilter);
			/* �󺧹�ȣ ���� */
			labelNum++;
		}
	}

	memcpy(dest->m_pImgU16, src->m_pImgU16, sizeof(U16) * imageSize);

	for (y = object[1].top.y; y < object[1].bottom.y; y++) {
		pSrc = &src->m_pImgU16[y * wStep];
		for (x = object[1].left.x; x < object[1].right.x; x++) {
			if (pSrc[x] != BLACK_16) {
				pSrc[x] = FORWARD;
			}
		}
	}

	pSrc = src->m_pImgU16;
	for (i = imageSize - 1; i >= 0; --i) {
		if (pSrc[i] != FORWARD) {
			pSrc[i] = BLACK_16;
		}
	}

	qsort((void *)&object[1], labelNum - 1, sizeof(OBJECT), area_sort);

	return labelNum;
}

static void labeling(IMG_INFO *src, const int x, const int y, const U16 FORWARD, int *label_num, OBJECT *object, const int sizeFilter)
{
	int width = src->m_nWidth;
	int height = src->m_nHeight;
	int wStep = src->m_nWStep;
	int i, j, k;
	U16 *pSrc = src->m_pImgU16;
	int labelState;			// �󺧸��� �ǰ� �ִ����� ������ Ȯ��

	int areaSize = 1, leftX = x, rightX = x, bottomY = y, topY = y;
	i = y, j = x;
	for(; i < height; ++i) {		
		for(; j < width; ++j) {
			labelState = 8;
			pSrc = &src->m_pImgU16[i * wStep + j];
			if(pSrc[0] == (*label_num)) {
				for(k = 7; k >= 0; --k) {
					if((i + ptr[k].y) >= 0 && (i + ptr[k].y) < height && (j + ptr[k].x) >= 0 && (j + ptr[k].x) < width) {
						if(pSrc[ptr[k].y * wStep + ptr[k].x] == FORWARD) {
							pSrc[ptr[k].y * wStep + ptr[k].x] = (*label_num);

							/* Ư�������� ��, ��, ��, �� ���� ���� */
							if(bottomY > (i + ptr[k].y))
								bottomY = i + ptr[k].y;
							if(topY < (i + ptr[k].y))
								topY = i + ptr[k].y;
							if(leftX > (j + ptr[k].x))
								leftX = j + ptr[k].x;
							if(rightX < (j + ptr[k].x))
								rightX = j + ptr[k].x;

							/* ��ü�� ����� ���� */
							++areaSize;
							labelState = k;
						}
					}
				}
			}

			switch(labelState) {
			case 6:
			case 1:
				j = j - 1;
				break;
			case 5:
			case 3:
			case 0:
				j = j - 2;
				break;
			}
			switch(labelState) {
			case 2:
			case 1:
			case 0:
				i = i - 1 ;
				break;
			}
		}
		j = 0;
	}

	if(areaSize < sizeFilter) {
		/* �� �����̶�� ������� ������ BACKWORD�� ���� */
		for(i = bottomY; i <= topY; ++i) {
			pSrc = &src->m_pImgU16[i * wStep];
			for(j = leftX; j <= rightX; ++j) {
				if(pSrc[j] == (*label_num))
					pSrc[j] = BLACK_16;
			}
		}
		(*label_num)--;
	} else {
		int cnt[2];

		object[(*label_num)].areaSize = areaSize;
		object[(*label_num)].left.x = leftX;
		object[(*label_num)].right.x = rightX;
		// ������ ���Ϸ� flip�Ǿ� �־ �ݴ�� ����
		object[(*label_num)].top.y = bottomY;
		object[(*label_num)].bottom.y = topY;
		object[(*label_num)].width = rightX - leftX + 1;
		object[(*label_num)].height = topY - bottomY + 1;

		cnt[0] = cnt[1] = 0;
		object[(*label_num)].left.y = 0;
		object[(*label_num)].right.y = 0;
		for(i = bottomY; i <= topY; ++i) {
			pSrc = &src->m_pImgU16[i * wStep];
			if(pSrc[leftX] == (*label_num)) {
				cnt[0]++;
				object[(*label_num)].left.y += i;
			}
			if(pSrc[rightX] == (*label_num)) {
				cnt[1]++;
				object[(*label_num)].right.y += i;
			}
		}
		object[(*label_num)].left.y /= cnt[0];
		object[(*label_num)].right.y /= cnt[1];
		
		cnt[0] = cnt[1] = 0;
		object[(*label_num)].top.x = 0;
		object[(*label_num)].bottom.x = 0;
		for(j = leftX; j <= rightX; ++j) {
			if(src->m_pImgU16[bottomY * wStep + j] == (*label_num)) {
				cnt[0]++;
				object[(*label_num)].top.x += j;
			}
			if(src->m_pImgU16[topY * wStep + j] == (*label_num)) {
				cnt[1]++;
				object[(*label_num)].bottom.x += j;
			}			
		}
		object[(*label_num)].top.x /= cnt[0];
		object[(*label_num)].bottom.x /= cnt[1];

		cnt[0] = cnt[1] = 0;
		for(i = bottomY; i <= topY; ++i) {
			pSrc = &src->m_pImgU16[i * wStep];
			for(j = leftX; j <= rightX; ++j) {
				if(pSrc[j] == (*label_num)) {
					cnt[0] += j;
					cnt[1] += i;
				}
			}
		}
		object[(*label_num)].center.x = cnt[0] / areaSize;
		object[(*label_num)].center.y = cnt[1] / areaSize;
	}

	return ;
}
