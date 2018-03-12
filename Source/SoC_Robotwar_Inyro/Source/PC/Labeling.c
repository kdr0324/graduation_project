#include "stdafx.h"

#include "Labeling.h"

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

/* 라벨번호를 매기는 함수 */
static void labeling(IMG_INFO *src, const int x, const int y,
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

/* 특정 라벨값의 번호의 시작위치를 정하는 함수 */
int CallLabel(IMG_INFO *src, OBJECT *object, const int sizeFilter, const int MAX_LABEL)
{
	int width = src->m_nWidth;
	int imageSize = src->m_nImageSize;
	U8 *pSrc = src->m_pImgU8;
	int i, x, y;
	int labelNum = 1;

	// 물체의 정보 초기화
	memset(&object[1], 0, sizeof(OBJECT) * MAX_LABEL);
	for(i = 0; i < imageSize; ++i) {
		if(pSrc[i] == WHITE_8) {
			pSrc[i] = labelNum;
			x = i % width;
			y = i / width;

			/* 번호 매기는 함수 호출 */
			labeling(src, x, y, &labelNum, object, sizeFilter);
			/* 라벨번호 증가 */
			labelNum++;
		}
	}

	for(i = imageSize - 1; i >= 0; --i) {
		if(pSrc[i] != BLACK_8) {
			pSrc[i] = WHITE_8;
		}
	}

	qsort((void *)&object[1], labelNum - 1, sizeof(OBJECT), area_sort);

	return labelNum;
}

static void labeling(IMG_INFO *src, const int x, const int y, int *label_num, OBJECT *object, const int sizeFilter)
{
	int width = src->m_nWidth;
	int height = src->m_nHeight;
	int i, j, k;
	U8 *pSrc = src->m_pImgU8;
	int labelState;			// 라벨링이 되고 있는지의 정보를 확인

	int areaSize = 1, leftX = x, rightX = x, bottomY = y, topY = y;

	for(i = y; i < height; ++i) {		
		for(j = x; j < width; ++j) {
			labelState = 8;
			pSrc = &src->m_pImgU8[i * width + j];
			if(pSrc[0] == (*label_num)) {
				for(k = 7; k >= 0; --k) {
					if((i + ptr[k].y) >= 0 && (i + ptr[k].y) < height && (j + ptr[k].x) >= 0 && (j + ptr[k].x) < width) {
						if(pSrc[ptr[k].y * width + ptr[k].x] == WHITE_8) {
							pSrc[ptr[k].y * width + ptr[k].x] = (*label_num);

							/* 특정영역의 상, 하, 좌, 우 값을 설정 */
							if(bottomY > (i + ptr[k].y))
								bottomY = i + ptr[k].y;
							if(topY < (i + ptr[k].y))
								topY = i + ptr[k].y;
							if(leftX > (j + ptr[k].x))
								leftX = j + ptr[k].x;
							if(rightX < (j + ptr[k].x))
								rightX = j + ptr[k].x;

							/* 물체의 사이즈를 측정 */
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
	}

	if(areaSize < sizeFilter) {
		/* 라벨 영역이라고 취급하지 않으면 BACKWORD로 변경 */
		for(i = bottomY; i <= topY; ++i) {
			pSrc = &src->m_pImgU8[i * width];
			for(j = leftX; j <= rightX; ++j) {
				if(pSrc[j] == (*label_num))
					pSrc[j] = BLACK_8;
			}
		}
		(*label_num)--;
	} else {
		int cnt[2];

		object[(*label_num)].areaSize = areaSize;
		object[(*label_num)].left.x = leftX;
		object[(*label_num)].right.x = rightX;
		// 영상이 상하로 flip되어 있어서 반대로 저장
		object[(*label_num)].top.y = bottomY;
		object[(*label_num)].bottom.y = topY;
		object[(*label_num)].width = rightX - leftX + 1;
		object[(*label_num)].height = topY - bottomY + 1;

		cnt[0] = cnt[1] = 0;
		object[(*label_num)].left.y = 0;
		object[(*label_num)].right.y = 0;
		for(i = bottomY; i <= topY; ++i) {
			pSrc = &src->m_pImgU8[i * width];
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
			if(src->m_pImgU8[bottomY * width + j] == (*label_num)) {
				cnt[0]++;
				object[(*label_num)].top.x += j;
			}
			if(src->m_pImgU8[topY * width + j] == (*label_num)) {
				cnt[1]++;
				object[(*label_num)].bottom.x += j;
			}			
		}
		object[(*label_num)].top.x /= cnt[0];
		object[(*label_num)].bottom.x /= cnt[1];

		cnt[0] = cnt[1] = 0;
		for(i = bottomY; i <= topY; ++i) {
			pSrc = &src->m_pImgU8[i * width];
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
