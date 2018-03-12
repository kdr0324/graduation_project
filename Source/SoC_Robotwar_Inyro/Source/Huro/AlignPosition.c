#include "stdafx.h"

#include "AlignPosition.h"

#ifdef _WIN32
#include "../Common/Bitmap.h"
#endif // WIN32

#include "../Amazon/Memory.h"
#include "../Amazon/ImageConvert.h"
#include "../Amazon/Draw.h"
#include "HuroMissonConfig.h"

static int hough_line_sort_align(const void *a, const void *b)
{
	int one = ((HOUGH_LINE_INFO *)a)->hough_counts;
	int two = ((HOUGH_LINE_INFO *)b)->hough_counts;

	if (one < two) {
		return 1;
	}
	else if (one > two) {
		return -1;
	} else {
		one = ((HOUGH_LINE_INFO *)a)->distance;
		two = ((HOUGH_LINE_INFO *)b)->distance;
		if (one < two) {
			return 1;
		} else {
			return -1;
		}
	}
}

int AlignPosition(FILE *fp, const char *name)
{
	int i, j, k, base_y, base_x, index;
	int theta, distance, diff, line_num;
	int ret = 0;
	int edgeCount;
	const int THETA_THRESHOLD = 4;
	const int Y_BASELINE = 70;
	const int Y_THRESHOLD = 5;

	const int MINIMUMPOINTS = 15;
	const int IGNORE_X_MIN = 20;
	const int IGNORE_X_MAX = 65;
	const int IGNORE_Y_MIN = 10;
	const int IGNORE_Y_MAX = 118;
	const int MINIMUM_THETA = 50;
	const int MINIMUM_DISTANCE = 20;

	//영상처리용 변수
	int nWidth, nWStep;
	int nWidth_1;
	int nHeight;
	int nHeight_1;
	U16 *pSrc;
	const U16 FORWARD = YELLOW_16;
	const int CTS = 2; // 4
	POINT2D pts[(IGNORE_X_MAX - IGNORE_X_MIN) * CTS] = { { 0, 0 } };
	int pt_num, ct;

#ifdef _WIN32
	RGB565toRGB888(original, saveImage);
	sprintf(g_filename, "%s_01_original.bmp", name);
	SaveBitmap(g_filename, saveImage);
#endif

#if PRINT_MODE
	memset(print[0]->m_pImgU16, 0xFFFF, sizeof(U16)* AMAZON_WIDTH * AMAZON_HEIGHT);
	memset(print[1]->m_pImgU16, 0xFFFF, sizeof(U16)* AMAZON_WIDTH * AMAZON_HEIGHT);
#endif

	memset(view->m_pImgU16, 0xFFFF, sizeof(U16) * AMAZON_WIDTH * AMAZON_HEIGHT);

#if 0
	RGB565toI(original, iImg);

	vI.i.max = 255; vI.i.min = 100;
	vI.iImg = edgeImage;
	Laplacian(iImg, edgeImage);

	for (i = nHeight_1; i >= 0; --i) {
		index = i * nWidth;
		for (j = IGNORE_X_MIN; j >= 0; --j) {
			edgeImage->m_pImgU8[index + j] = BLACK_8;
		}
		for (j = nWidth_1; j >= IGNORE_X_MAX; --j) {
			edgeImage->m_pImgU8[index + j] = BLACK_8;
		}
	}
	for (i = IGNORE_Y_MIN; i >= 0; --i) {
		index = i * nWidth;
		for (j = nWidth_1; j >= 0; --j) {
			edgeImage->m_pImgU8[index + j] = BLACK_8;
		}
	}

	for (i = nHeight_1; i >= IGNORE_Y_MAX; --i) {
		index = i * nWidth;
		for (j = nWidth_1; j >= 0; --j) {
			edgeImage->m_pImgU8[index + j] = BLACK_8;
		}
	}

	binary_I(&vI, binary, FORWARD);
	vI.iImg = iImg;
	medianBinary(binary, noise, FORWARD);
	dilationBinary(noise, dilation, FORWARD);
#else
	CropImage(original, crop_original, IGNORE_X_MIN, IGNORE_Y_MIN, IGNORE_X_MAX, IGNORE_Y_MAX);
#ifdef _WIN32
	RGB565toRGB888(crop_original, saveImage);
	sprintf(g_filename, "%s_02_crop.bmp", name);
	SaveBitmap(g_filename, saveImage);
#endif
	RGB565toSI(crop_original, sImg, iImg);
	vSI.s.max = BLACK_S_MAX; vSI.s.min = 0;// 60
	vSI.i.max = BLACK_I_MAX; vSI.i.min = 0;// 80

	binary_SI(&vSI, binary, FORWARD);
	medianBinary(binary, noise, FORWARD);
	dilationBinary(noise, dilation, FORWARD);
#endif

	pSrc = dilation->m_pImgU16;
	nWidth = dilation->m_nWidth;
	nWStep = dilation->m_nWStep;
	nHeight = dilation->m_nHeight;
	nHeight_1 = nHeight - 1;
	pt_num = 0;
	for (j = nWidth - 1; j >= 0; --j) {
		index = nHeight_1 * nWStep + j;
		ct = CTS;
		for (i = nHeight_1; i >= 0 && ct > 0; --i) {
			if (pSrc[index] == FORWARD) {
				pts[pt_num].x = j;
				pts[pt_num++].y = i;
				ct--;
			}
			index -= nWStep;
		}
	}
	LineDectector(pts, pt_num, FORWARD);

	line_num = 0;
	memset(hough_lines, 0, sizeof(HOUGH_LINE_INFO) * MAX_THETA_HOUGH * MAX_DISTANCE_HOUGH * 2);
	for (i = MAX_THETA_HOUGH - 1; i >= 0; --i) {
		for (j = MAX_DISTANCE_HOUGH * 2 - 1; j >= 0; --j) {
			if (hough_counts[i][j] > MINIMUMPOINTS) {
				hough_lines[line_num].theta = i;
				hough_lines[line_num].distance = j - MAX_DISTANCE_HOUGH;
				hough_lines[line_num].hough_counts = hough_counts[i][j];
				line_num++;
			}
		}
	}
	qsort(hough_lines, line_num, sizeof(HOUGH_LINE_INFO), hough_line_sort_align);

	for (k = 0; k < 1 && k < line_num; k++) {
		DrawLineWithThetaNDistance(dilation, hough_lines[k].theta, hough_lines[k].distance, MAGENTA_16);
		DrawLineWithThetaNDistance(crop_original, hough_lines[k].theta, hough_lines[k].distance, MAGENTA_16);
	}

	theta = hough_lines[0].theta;
	distance = hough_lines[0].distance;
	base_y = -SQ_10(distance) / cos_lookup[theta];
	if (theta > 90)
		theta = theta - 180;
	theta = theta - 5;

#if PRINT_MODE
	printResult(theta, 0, 0);
	printResult(base_y, 1, 0);
	printResult(ret, 0, 1);
#endif

	CopyImageForView(crop_original, original, IGNORE_X_MIN, IGNORE_Y_MIN);
	CopyImageForView(dilation, view, IGNORE_X_MIN, IGNORE_Y_MIN);

#ifdef _WIN32
	RGB565toRGB888(dilation, saveImage);
	sprintf(g_filename, "%s_03_result.bmp", name);
	SaveBitmap(g_filename, saveImage);

	RGB565toRGB888(original, saveImage);
	sprintf(g_filename, "%s_04_result.bmp", name);
	SaveBitmap(g_filename, saveImage);
#endif

	//return 0;

	if (fp != NULL)
		fprintf(fp, ",%d,%d", theta, base_y);

	if (theta <= -THETA_THRESHOLD || theta >= THETA_THRESHOLD) {
		// 기울기를 이용해서 회전량을 구한다.
		// 기울기가 양수인경우 오른쪽 턴 실행
		// 기울기가 음수인경우 왼쪽 턴 실행
		if (theta >= THETA_THRESHOLD * 3) {
			ret = ALIGN_POSITION_TURN_20;
		}
		else if (theta >= THETA_THRESHOLD) {
			ret = ALIGN_POSITION_TURN_20;
		}
		else if (theta <= -(THETA_THRESHOLD * 3)) {
			ret = ALIGN_POSITION_LEFT_MOTION | ALIGN_POSITION_TURN_20;
		}
		else {
			ret = ALIGN_POSITION_LEFT_MOTION | ALIGN_POSITION_TURN_20;
		}
	} else if (base_y < Y_BASELINE -  Y_THRESHOLD || base_y > Y_BASELINE + Y_THRESHOLD) {
		// Y 절편으로 경계로부터의 거리를 구한다
		// Y 절편이 크면 왼쪽으로 이동
		// Y 절편이 작으면 오른쪽으로 이동
		if (base_y < Y_BASELINE - 6 * Y_THRESHOLD) {
			ret = ALIGN_POSITION_TRIPLE_SIDE_50;
		}
		else if (base_y > Y_BASELINE + 6 * Y_THRESHOLD) {
			ret = ALIGN_POSITION_LEFT_MOTION | ALIGN_POSITION_TRIPLE_SIDE_50;
		}
		else if (base_y < Y_BASELINE - 4 * Y_THRESHOLD) {
			ret = ALIGN_POSITION_DOUBLE_SIDE_50;
		}
		else if (base_y > Y_BASELINE + 4 * Y_THRESHOLD) {
			ret = ALIGN_POSITION_LEFT_MOTION | ALIGN_POSITION_DOUBLE_SIDE_50;
		}
		else if (base_y < Y_BASELINE - 3 * Y_THRESHOLD) {
			ret = ALIGN_POSITION_SINGLE_SIDE_50;
		}
		else if (base_y > Y_BASELINE + Y_THRESHOLD) {
			ret = ALIGN_POSITION_LEFT_MOTION | ALIGN_POSITION_SINGLE_SIDE_50;
		}
		else if (base_y < Y_BASELINE - Y_THRESHOLD) {
			ret = ALIGN_POSITION_SINGLE_SIDE_20;
		}
		else {
			ret = ALIGN_POSITION_LEFT_MOTION | ALIGN_POSITION_SINGLE_SIDE_20;
		}
	} else {
		ret = ALIGN_POSITION_END;
	}

#ifdef _WIN32
	RGB565toRGB888(dilation, saveImage);
	sprintf(g_filename, "%s_03_result.bmp", name);
	SaveBitmap(g_filename, saveImage);

	RGB565toRGB888(original, saveImage);
	sprintf(g_filename, "%s_04_result.bmp", name);
	SaveBitmap(g_filename, saveImage);
#endif

	if (fp != NULL)
		fprintf(fp, ",%d", ret);

	return ret;
}

int AlignPositionTurn(FILE *fp, const char *name)
{
	int i, j, k, base_y, base_x, index;
	int theta, distance, diff, line_num;
	int ret = 0;
	int edgeCount;
	const int THETA_THRESHOLD = 4;

	const int MINIMUMPOINTS = 15;
	const int IGNORE_X_MIN = 30;
	const int IGNORE_X_MAX = 65;
	const int IGNORE_Y_MIN = 10;
	const int IGNORE_Y_MAX = 118;
	const int MINIMUM_THETA = 50;
	const int MINIMUM_DISTANCE = 20;

	//영상처리용 변수
	int nWidth, nWStep;
	int nWidth_1;
	int nHeight;
	int nHeight_1;
	U16 *pSrc;
	const U16 FORWARD = YELLOW_16;
	const int CTS = 2; // 4
	POINT2D pts[(IGNORE_X_MAX - IGNORE_X_MIN) * CTS] = {{0, 0}};
	int pt_num, ct;

#if PRINT_MODE
	memset(print[0]->m_pImgU16, 0xFFFF, sizeof(U16)* AMAZON_WIDTH * AMAZON_HEIGHT);
	memset(print[1]->m_pImgU16, 0xFFFF, sizeof(U16)* AMAZON_WIDTH * AMAZON_HEIGHT);
#endif

	memset(view->m_pImgU16, 0xFFFF, sizeof(U16) * AMAZON_WIDTH * AMAZON_HEIGHT);


	CropImage(original, crop_original, IGNORE_X_MIN, IGNORE_Y_MIN, IGNORE_X_MAX, IGNORE_Y_MAX);
	RGB565toSI(crop_original, sImg, iImg);
	vSI.s.max = 60; vSI.s.min = 0;// 60
	vSI.i.max = 80; vSI.i.min = 0;// 80
	binary_SI(&vSI, binary, FORWARD);
	medianBinary(binary, noise, FORWARD);
	dilationBinary(noise, dilation, FORWARD);

	pSrc = dilation->m_pImgU16;
	nWidth = dilation->m_nWidth;
	nWStep = dilation->m_nWStep;
	nHeight = dilation->m_nHeight;
	nHeight_1 = nHeight - 1;
	pt_num = 0;
	for (j = nWidth - 1; j >= 0; --j) {
		index = nHeight_1 * nWStep + j;
		ct = CTS;
		for (i = nHeight_1; i >= 0 && ct > 0; --i) {
			if (pSrc[index] == FORWARD) {
				pts[pt_num].x = j;
				pts[pt_num++].y = i;
				ct--;
			}
			index -= nWStep;
		}
	}
	LineDectector(pts, pt_num, FORWARD);

	line_num = 0;
	memset(hough_lines, 0, sizeof(HOUGH_LINE_INFO) * MAX_THETA_HOUGH * MAX_DISTANCE_HOUGH * 2);
	for (i = MAX_THETA_HOUGH - 1; i >= 0; --i) {
		for (j = MAX_DISTANCE_HOUGH * 2 - 1; j >= 0; --j) {
			if (hough_counts[i][j] > MINIMUMPOINTS) {
				hough_lines[line_num].theta = i;
				hough_lines[line_num].distance = j - MAX_DISTANCE_HOUGH;
				hough_lines[line_num].hough_counts = hough_counts[i][j];
				line_num++;
			}
		}
	}
	qsort(hough_lines, line_num, sizeof(HOUGH_LINE_INFO), hough_line_sort_align);

	for (k = 0; k < 1 && k < line_num; k++) {
		DrawLineWithThetaNDistance(dilation, hough_lines[k].theta, hough_lines[k].distance, MAGENTA_16);
		DrawLineWithThetaNDistance(crop_original, hough_lines[k].theta, hough_lines[k].distance, MAGENTA_16);
	}

	theta = hough_lines[0].theta;
	if (theta > 90)
		theta = theta - 180;
	theta = theta - 5;

#if PRINT_MODE
	printResult(theta, 0, 0);
	printResult(ret, 1, 0);
#endif

	CopyImageForView(crop_original, original, IGNORE_X_MIN, IGNORE_Y_MIN);
	CopyImageForView(dilation, view, IGNORE_X_MIN, IGNORE_Y_MIN);

#ifdef _WIN32
	RGB565toRGB888(dilation, saveImage);
	sprintf(g_filename, "%s_03_result.bmp", name);
	SaveBitmap(g_filename, saveImage);

	RGB565toRGB888(original, saveImage);
	sprintf(g_filename, "%s_04_result.bmp", name);
	SaveBitmap(g_filename, saveImage);
#endif

	//return 0;

	if (fp != NULL)
		fprintf(fp, ",%d,%d", theta, base_y);

	if (theta <= -THETA_THRESHOLD || theta >= THETA_THRESHOLD) {
		// 기울기를 이용해서 회전량을 구한다.
		// 기울기가 양수인경우 오른쪽 턴 실행
		// 기울기가 음수인경우 왼쪽 턴 실행
		if (theta >= THETA_THRESHOLD * 3) {
			ret = ALIGN_POSITION_TURN_20;
		} else if (theta >= THETA_THRESHOLD) {
			ret = ALIGN_POSITION_TURN_5;
		} else if (theta <= -(THETA_THRESHOLD * 3)) {
			ret = ALIGN_POSITION_LEFT_MOTION | ALIGN_POSITION_TURN_20;
		} else {
			ret = ALIGN_POSITION_LEFT_MOTION | ALIGN_POSITION_TURN_5;
		}
	} else {
		ret = ALIGN_POSITION_END;
	}

#ifdef _WIN32
	RGB565toRGB888(dilation, saveImage);
	sprintf(g_filename, "%s_03_result.bmp", name);
	SaveBitmap(g_filename, saveImage);

	RGB565toRGB888(original, saveImage);
	sprintf(g_filename, "%s_04_result.bmp", name);
	SaveBitmap(g_filename, saveImage);
#endif

	if (fp != NULL)
		fprintf(fp, ",%d", ret);

	return ret;
}
