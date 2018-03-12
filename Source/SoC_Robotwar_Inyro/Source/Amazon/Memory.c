#include "stdafx.h"

#include "Memory.h"

// 공용 메모리
HSIVALUE vHSI;
HSVALUE vHS;
HIVALUE vHI;
SIVALUE vSI;
HVALUE vH;
SVALUE vS;
IVALUE vI;

IMG_INFO *original; // rgb565 이미지 저장
IMG_INFO *crop_original; // rgb565 이미지 저장
IMG_INFO *hImg, *sImg, *iImg; // H, S, I
IMG_INFO *binary; // 이진영상
IMG_INFO *edgeImage; // 8bits
IMG_INFO *noise, *dilation;	// 노이즈제거, 팽창
IMG_INFO *imgLabel; // 라벨링 결과 저장
OBJECT *g_object;
IMG_INFO *integral;
IMG_INFO *integral_32;
IMG_INFO *view;	// LCD에 출력하기 위한 용도
#if PRINT_MODE
IMG_INFO *print[2]; // LCD에 숫자를 출력하기 위한 용도
#endif
IMG_INFO *g_rgb888; // rgb888 이미지 저장
IMG_INFO *saveImage; // 24bits 컬러 이미지로 저장하기 위한 용도, PC에서만 사용할 예정

// 휴로 전용 메모리
IMG_INFO *ballDetect;
IMG_INFO *holeDetect;
OBJECT *holeLabels;

// 태권 전용 메모리
IMG_INFO *blocks[2];

char g_filename[2048];

void CommonMemoryAllocate(void)
{
	int i;

	OpenLookUpTable();
	hough_counts = (U16 **)malloc(sizeof(U16 *) * MAX_THETA_HOUGH);
	for (i = MAX_THETA_HOUGH - 1; i >= 0; --i)
		hough_counts[i] = (U16 *)malloc(sizeof(U16) * MAX_DISTANCE_HOUGH * 2);
	hough_lines = (HOUGH_LINE_INFO *)malloc(sizeof(HOUGH_LINE_INFO) * MAX_THETA_HOUGH * MAX_DISTANCE_HOUGH * 2);

	original = CreateImageInfo(AMAZON_WIDTH, AMAZON_HEIGHT, 1, D_U16);
	crop_original = CreateImageInfo(AMAZON_WIDTH, AMAZON_HEIGHT, 1, D_U16);
	hImg = CreateImageInfo(AMAZON_WIDTH, AMAZON_HEIGHT, 1, D_U16);
	sImg = CreateImageInfo(AMAZON_WIDTH, AMAZON_HEIGHT, 1, D_U8);
	iImg = CreateImageInfo(AMAZON_WIDTH, AMAZON_HEIGHT, 1, D_U8);
	binary = CreateImageInfo(AMAZON_WIDTH, AMAZON_HEIGHT, 1, D_U16);
	edgeImage = CreateImageInfo(AMAZON_WIDTH, AMAZON_HEIGHT, 1, D_U8);
	noise = CreateImageInfo(AMAZON_WIDTH, AMAZON_HEIGHT, 1, D_U16);
	dilation = CreateImageInfo(AMAZON_WIDTH, AMAZON_HEIGHT, 1, D_U16);
	imgLabel = CreateImageInfo(AMAZON_WIDTH, AMAZON_HEIGHT, 1, D_U16);
	g_object = (OBJECT*)malloc(sizeof(OBJECT) * MAX_LABEL_COUNT);
	integral = CreateImageInfo(AMAZON_WIDTH, AMAZON_HEIGHT, 1, D_U16);
	integral_32 = CreateImageInfo(AMAZON_WIDTH, AMAZON_HEIGHT, 1, D_U32);
	view = CreateImageInfo(AMAZON_WIDTH, AMAZON_HEIGHT, 1, D_U16);	

	g_rgb888 = CreateImageInfo(AMAZON_WIDTH, AMAZON_HEIGHT, 3, D_U8);
	saveImage = CreateImageInfo(AMAZON_WIDTH, AMAZON_HEIGHT, 3, D_U8);

#if PRINT_MODE
	print[0] = CreateImageInfo(AMAZON_WIDTH, AMAZON_HEIGHT, 1, D_U16);
	print[1] = CreateImageInfo(AMAZON_WIDTH, AMAZON_HEIGHT, 1, D_U16);
#endif

	vHSI.hImg = vHS.hImg = vHI.hImg = vH.hImg = hImg;
	vHSI.sImg = vHS.sImg = vSI.sImg = vS.sImg = sImg;
	vHSI.iImg = vHI.iImg = vSI.iImg = vI.iImg = iImg;
}

void HuroMemoryAllocate(void)
{
	// 휴로를 위한 메모리 세팅
	ballDetect = CreateImageInfo(AMAZON_WIDTH, AMAZON_HEIGHT, 1, D_U16);
	holeDetect = CreateImageInfo(AMAZON_WIDTH, AMAZON_HEIGHT, 1, D_U16);
	holeLabels = (OBJECT *)malloc(sizeof(OBJECT) * MAX_LABEL_COUNT);
}

void TaekwonMemoryAllocate(void)
{
	int i;
	blocks[0] = CreateImageInfo(AMAZON_WIDTH, AMAZON_HEIGHT, 1, D_U16);
	blocks[1] = CreateImageInfo(AMAZON_WIDTH, AMAZON_HEIGHT, 1, D_U16);
}

void CommonMemoryRelease(void)
{
	int i;
	CloseLookUpTable();
	for (i = MAX_THETA_HOUGH - 1; i >= 0; --i)
		free(hough_counts[i]);
	free(hough_counts);
	free(hough_lines);

	ReleaseImageInfo(&original);
	ReleaseImageInfo(&crop_original);
	ReleaseImageInfo(&hImg);
	ReleaseImageInfo(&sImg);
	ReleaseImageInfo(&iImg);
	ReleaseImageInfo(&binary);
	ReleaseImageInfo(&edgeImage);
	ReleaseImageInfo(&noise);
	ReleaseImageInfo(&dilation);
	ReleaseImageInfo(&imgLabel);
	free(g_object);
	ReleaseImageInfo(&integral);
	ReleaseImageInfo(&integral_32);
	ReleaseImageInfo(&view);
#if PRINT_MODE
	ReleaseImageInfo(&print[0]);
	ReleaseImageInfo(&print[1]);
#endif
	ReleaseImageInfo(&g_rgb888);

	ReleaseImageInfo(&saveImage);

}

void HuroMemoryRelease(void)
{
	ReleaseImageInfo(&ballDetect);
	ReleaseImageInfo(&holeDetect);
	free(holeLabels);
}

void TaekwonMemoryRelease(void)
{
	ReleaseImageInfo(&blocks[0]);
	ReleaseImageInfo(&blocks[1]);
}

void CalcLinear(const POINT2D *p, int *a0, int *a1, const int cntPoint)
{
	int i = 0;
	int xi, yi;
	int sum_xi = 0, sum_yi = 0, sum_xi_yi = 0, sum_xi_square = 0;

	for(i = 0; i < cntPoint; i++) {
		xi = p[i].x;
		yi = p[i].y;

		sum_xi += xi;
		sum_yi += yi;
		sum_xi_yi += xi * yi;
		sum_xi_square += xi * xi;
	}

	if(cntPoint * sum_xi_square - sum_xi * sum_xi != 0) {
		*a0 = (sum_xi_square * sum_yi - sum_xi_yi * sum_xi) / (cntPoint * sum_xi_square - sum_xi * sum_xi);
		*a1 = SQ_10((cntPoint * sum_xi_yi - sum_xi * sum_yi)) / (cntPoint * sum_xi_square - sum_xi * sum_xi);
	} else {
		*a1 = (abs(p[cntPoint - 1].y - p[0].y)) * 8;
		*a0 = -SQ_10((p[0].x * (*a1)));
	}
}

int line_sort(const void *a, const void *b)
{
	int one = ((POINT2D *)a)->y;
	int two = ((POINT2D *)b)->y;

	if(one < two) {
		return -1;
	} else if(one > two) {
		return 1;
	} else {
		return 0;
	}
}
