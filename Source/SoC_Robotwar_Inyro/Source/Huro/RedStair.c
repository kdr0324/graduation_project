#include "stdafx.h"

#include "RedStair.h"

#ifdef _WIN32
#include "../Common/Bitmap.h"
#endif // WIN32

#include "../Amazon/Memory.h"
#include "../Amazon/ImageConvert.h"
#include "../Amazon/Draw.h"
#include "HuroMissonConfig.h"

int RedStair(FILE *fp, const char *name)
{
	int i, index;
	int left_x=0, right_x=0, mx=0;
	int h_diff, top, bot;
	int ret = 0;
	int nWidth, nHeight, nWStep;
	const U16 FORWARD = RED_16;
	const int X_MIN = 10, X_MAX = 170, Y_MIN = 0, Y_MAX = 80;
	const int Y_THRESOLD = 55; // 40;
	U16 *pSrc;	

#if PRINT_MODE
	memset(print[0]->m_pImgU16, 0xFFFF, sizeof(U16)* AMAZON_WIDTH * AMAZON_HEIGHT);
	memset(print[1]->m_pImgU16, 0xFFFF, sizeof(U16)* AMAZON_WIDTH * AMAZON_HEIGHT);
#endif

	// 세팅 값 설정
	vHS.h.max = RED_H_MAX;
	vHS.h.min = RED_H_MIN;
	vHS.s.max = 255;
	vHS.s.min = RED_S_MIN;

	CropImage(original, crop_original, X_MIN, Y_MIN, X_MAX, Y_MAX);
	RGB565toHS(crop_original, hImg, sImg);			// 16bits, 16bits, 8bits 영상 버퍼	
	binary_HS(&vHS, binary, FORWARD);
	medianBinary(binary, noise, FORWARD);		// Binary Median Filter를 이용하여 노이즈 제거
	dilationBinary(noise, dilation, FORWARD);
	
	nWidth = dilation->m_nWidth;
	nHeight = dilation->m_nHeight;
	nWStep = dilation->m_nWStep;
	index = Y_THRESOLD * nWStep;
	pSrc = &dilation->m_pImgU16[index];

	//right_x
	for (i = 0; i < nWidth - 4 ; ++i) {
		if (pSrc[i] != FORWARD)
			continue;
		
		if (pSrc[++i] != FORWARD)
			continue;
		
		if (pSrc[++i] != FORWARD)
			continue;

		if (pSrc[++i] != FORWARD)
			continue;

		if (pSrc[++i] != FORWARD)
			continue;

		right_x = i;
		DrawCrossLine_T(dilation, i, Y_THRESOLD, 6, 2, YELLOW_16);
		break;
		
	}

	//left_x
	for (i = nWidth - 1; i >= 4; --i) {
		if (pSrc[i] != FORWARD)
			continue;

		if (pSrc[--i] != FORWARD)
			continue;

		if (pSrc[--i] != FORWARD)
			continue;

		if (pSrc[--i] != FORWARD)
			continue;

		if (pSrc[--i] != FORWARD)
			continue;
		
		left_x = i;
		DrawCrossLine_T(dilation, i, Y_THRESOLD, 6, 2, BLUE_16);
		break;
	}

	if (right_x && left_x) {
		mx = X_MIN + ((left_x + right_x) >> 1);
		DrawCrossLine_T(dilation, mx - X_MIN, Y_THRESOLD, 6, 2, GREEN_16);

		if (mx < 75) {
			ret |= RED_STAIR_SIDE_MOVE_70;
		} else if (mx < 81) {
			ret |= RED_STAIR_SIDE_MOVE_30;
		} else if (mx < 85) {
			ret |= RED_STAIR_SIDE_MOVE_10;
		} else if (mx > 103) {
			ret |= RED_STAIR_SIDE_MOVE_70 | RED_STAIR_RIGHT_MOTION;
		} else if (mx > 97) {
			ret |= RED_STAIR_SIDE_MOVE_30 | RED_STAIR_RIGHT_MOTION;
		} else if (mx > 93) {
			ret |= RED_STAIR_SIDE_MOVE_10 | RED_STAIR_RIGHT_MOTION;
		}
		
	}
	else {
		ret |= RED_STAIR_WALK;
	}

#if PRINT_MODE
	printResult(mx, 0, 0);
#endif	

	CopyImageForView(dilation, view, X_MIN, Y_MIN);

#ifdef _WIN32
	RGB565toRGB888(view, saveImage);
	sprintf(g_filename, "%s_00_result.bmp", name);
	SaveBitmap(g_filename, saveImage);

	RGB565toRGB888(original, saveImage);
	sprintf(g_filename, "%s_01_result.bmp", name);
	SaveBitmap(g_filename, saveImage);
#endif

	return ret;
}

int RedStair2(FILE *fp, const char *name)
{
	int i, j, k, base_y, base_x, index;
	int theta, distance, diff, line_num;
	int ret = 0, mx = 0, cnt = 0;
	const U16 FORWARD = RED_16;
	const int MARGIN = 20;
	const int MINIMUMLINES = 10;
	const int tan_THRESHOLD = 3;
	U16 *pSrc;
	const int CTS = 2;
	POINT2D pts[(MARGIN * 2 + 1) * CTS] = { {0, 0} };
	int pt_num, ct;

#if PRINT_MODE
	memset(print[0]->m_pImgU16, 0xFFFF, sizeof(U16)* AMAZON_WIDTH * AMAZON_HEIGHT);
	memset(print[1]->m_pImgU16, 0xFFFF, sizeof(U16)* AMAZON_WIDTH * AMAZON_HEIGHT);
#endif

	// 세팅 값 설정
	vH.h.max = RED_H_MAX;
	vH.h.min = RED_H_MIN;

	RGB565toH(original, hImg);			// 16bits, 16bits, 8bits 영상 버퍼
	binary_H(&vH, binary, FORWARD);
	medianBinary(binary, noise, FORWARD);		// Binary Median Filter를 이용하여 노이즈 제거
	dilationBinary(noise, dilation, FORWARD);

	pt_num = 0;
	pSrc = dilation->m_pImgU16;
	for (j = AMAZON_HALF_WIDTH - MARGIN; j <= AMAZON_HALF_WIDTH + MARGIN; j++) {
		index = (AMAZON_HEIGHT - 4) * AMAZON_WIDTH + j;
		ct = CTS;
		for (i = AMAZON_HEIGHT - 4; i >= 0 && ct > 0; --i) {
			if (pSrc[index] == FORWARD) {
				pSrc[index] = YELLOW_16;
				pts[pt_num].x = j;
				pts[pt_num++].y = i;
				ct--;
			}
			index -= AMAZON_WIDTH;
		}
	}
	LineDectector(pts, pt_num, FORWARD);

	line_num = 0;
	memset(hough_lines, 0, sizeof(HOUGH_LINE_INFO) * MAX_THETA_HOUGH * MAX_DISTANCE_HOUGH * 2);
	for (i = MAX_THETA_HOUGH - 1; i >= 0; --i) {
		for (j = MAX_DISTANCE_HOUGH * 2 - 1; j >= 0; --j) {
			if (hough_counts[i][j] >= MINIMUMLINES) {
				hough_lines[line_num].theta = i;
				hough_lines[line_num].distance = j - MAX_DISTANCE_HOUGH;
				hough_lines[line_num].hough_counts = hough_counts[i][j];
				line_num++;
			}
		}
	}
	qsort(hough_lines, line_num, sizeof(HOUGH_LINE_INFO), hough_line_sort);
	for (k = 0; k < 1 && k < line_num; k++) {
		DrawLineWithThetaNDistance(dilation, hough_lines[k].theta, hough_lines[k].distance, MAGENTA_16);
		DrawLineWithThetaNDistance(original, hough_lines[k].theta, hough_lines[k].distance, MAGENTA_16);
	}

	theta = hough_lines[0].theta;
	distance = hough_lines[0].distance;

	base_y = -SQ_10(distance) / cos_lookup[theta];

	if (base_y > 90) {
		index = 10 * AMAZON_WIDTH;
		for (i = AMAZON_WIDTH - 1; i >= 0; --i) {
			if (dilation->m_pImgU16[index + i] == FORWARD) {
				mx += i;
				cnt++;
			}
		}

		if (cnt > 10) {
			//mx /= cnt;
			mx = 90;
		} else {
			mx = 90;
		}
	} else {
		mx = 90;
	}	

	if (theta > 90)
		theta = theta - 180;

	if (fp != NULL) {
		fprintf(fp, ",%d,%d,%d", theta, base_y, mx);
	}

#if PRINT_MODE
	printResult(theta, 0, 0);
	printResult(base_y, 1, 0);
	printResult(mx, 0, 1);
#endif

	//if (theta < -tan_THRESHOLD || theta > tan_THRESHOLD) {
	//	if (theta >= tan_THRESHOLD * 4) {
	//		ret = RED_STAIR_TURN_15 | RED_STAIR_RIGHT_MOTION;
	//	} else if (theta > tan_THRESHOLD) {
	//		ret = RED_STAIR_TURN_5 | RED_STAIR_RIGHT_MOTION;
	//	} else if (theta <= -(tan_THRESHOLD * 4)) {
	//		ret = RED_STAIR_TURN_15;
	//	} else {
	//		ret = RED_STAIR_TURN_5;
	//	}

	//} else {
	//	if (mx < 80) {
	//		ret = RED_STAIR_SIDE_50;
	//	} else if (mx > 100) {
	//		ret = RED_STAIR_SIDE_50 | RED_STAIR_RIGHT_MOTION;
	//	} else if (base_y < 45) {
	//		ret |= RED_STAIR_RUN_5;
	//	} else if (base_y < 70) {
	//		ret |= RED_STAIR_RUN_3;
	//	} else if (base_y < 95) { // 100       기존: 105
	//		ret |= RED_STAIR_WALK;
	//	} else {
	//		ret = RED_STAIR_END;
	//	}
	//}

#ifdef _WIN32
	RGB565toRGB888(dilation, saveImage);
	sprintf(g_filename, "%s_01_result.bmp", name);
	SaveBitmap(g_filename, saveImage);
	RGB565toRGB888(original, saveImage);
	sprintf(g_filename, "%s_02_result.bmp", name);
	SaveBitmap(g_filename, saveImage);
#endif

	return ret;
}


int RedStairAlign(FILE *fp, const char *name)
{
	int i, j, k, base_y, index;
	int theta, distance, line_num;
	int ret = 0, mx = 0, cnt = 0;
	const U16 FORWARD = RED_16;
	const int MARGIN = 25;
	const int MINIMUMLINES = 20;
	const int THETA_THRESHOLD = 5;
	const int THEATA_MAX_THRESHOLD = 20;
	const int X_MIN = 15, X_MAX = 170, Y_MIN = 10, Y_MAX = 90;
	int nWidth, nHeight, nWStep;
	U16 *pSrc;
	const int CTS = 2;
	POINT2D pts[(MARGIN * 2 + 1) * CTS] = { { 0, 0 } };
	int pt_num, ct;

#if PRINT_MODE
	memset(print[0]->m_pImgU16, 0xFFFF, sizeof(U16)* AMAZON_WIDTH * AMAZON_HEIGHT);
	memset(print[1]->m_pImgU16, 0xFFFF, sizeof(U16)* AMAZON_WIDTH * AMAZON_HEIGHT);
#endif


	// 세팅 값 설정
	CropImage(original, crop_original, X_MIN, Y_MIN, X_MAX, Y_MAX);
	nWidth = crop_original->m_nWidth;
	nHeight = crop_original->m_nHeight;
	nWStep = crop_original->m_nWStep;

	RGB565toH(crop_original, hImg);

	vH.h.max = RED_H_MAX; vH.h.min = RED_H_MIN;
	binary_H(&vH, binary, FORWARD);
	medianBinary(binary, noise, FORWARD);
	dilationBinary(noise, dilation, FORWARD);

	pt_num = 0;
	pSrc = dilation->m_pImgU16;
	for (j = 0; j < MARGIN; j++) {
		index = 4 * nWStep + j;
		ct = CTS;
		for (i = 4; i < nHeight && ct > 0; ++i) {
			if (pSrc[index] == FORWARD) {
				pSrc[index] = YELLOW_16;
				pts[pt_num].x = j;
				pts[pt_num++].y = i;
				ct--;
			}
			index += nWStep;
		}

		index = 4 * nWStep + nWidth - 1 - j;
		ct = CTS;
		for (i = 4; i < nHeight && ct > 0; ++i) {
			if (pSrc[index] == FORWARD) {
				pSrc[index] = YELLOW_16;
				pts[pt_num].x = nWidth - 1 - j;
				pts[pt_num++].y = i;
				ct--;
			}
			index += nWStep;
		}
	}
	LineDectector(pts, pt_num, FORWARD);

	line_num = 0;
	memset(hough_lines, 0, sizeof(HOUGH_LINE_INFO) * MAX_THETA_HOUGH * MAX_DISTANCE_HOUGH * 2);
	for (i = MAX_THETA_HOUGH - 1; i >= 0; --i) {
		for (j = MAX_DISTANCE_HOUGH * 2 - 1; j >= 0; --j) {
			if (hough_counts[i][j] >= MINIMUMLINES) {
				hough_lines[line_num].theta = i;
				hough_lines[line_num].distance = j - MAX_DISTANCE_HOUGH;
				hough_lines[line_num].hough_counts = hough_counts[i][j];
				line_num++;
			}
		}
	}
	qsort(hough_lines, line_num, sizeof(HOUGH_LINE_INFO), hough_line_sort);
	for (k = 0; k < 1 && k < line_num; k++) {
		DrawLineWithThetaNDistance(dilation, hough_lines[k].theta, hough_lines[k].distance, MAGENTA_16);
		DrawLineWithThetaNDistance(crop_original, hough_lines[k].theta, hough_lines[k].distance, MAGENTA_16);
	}

	theta = hough_lines[0].theta;
	distance = hough_lines[0].distance;
	//if (45 <= theta && theta <= 135) // 에러임
	//	return -1;
	base_y = -SQ_10(distance) / cos_lookup[theta];
	if (theta > 90)
		theta = theta - 180;

#if PRINT_MODE
	printResult(theta, 0, 0);
	printResult(base_y, 1, 0);
#endif

	if (line_num != 0) {
		if (fp != NULL)
			fprintf(fp, ",,%d,%d", theta, base_y);
		if (theta <= -THETA_THRESHOLD || theta >= THETA_THRESHOLD) {
			if (theta >= THEATA_MAX_THRESHOLD) {
				ret = RED_STAIR_ALIGN_TURN_30 | RED_STAIR_ALIGN_LEFT_MOTION;
			} else if (theta >= THETA_THRESHOLD * 3) {
				ret = RED_STAIR_ALIGN_TURN_30;
			} else if (theta >= THETA_THRESHOLD) {
				ret = RED_STAIR_ALIGN_TURN_15;
			} else if (theta <= -THEATA_MAX_THRESHOLD) {
				ret = RED_STAIR_ALIGN_TURN_30;
			} else if (theta <= -(THETA_THRESHOLD * 3)) {
				ret = RED_STAIR_ALIGN_TURN_30 | RED_STAIR_ALIGN_LEFT_MOTION;
			} else {
				ret = RED_STAIR_ALIGN_TURN_15 | RED_STAIR_ALIGN_LEFT_MOTION;
			}
		} else if (base_y <= 55) {
			if (base_y <= 30) {
				ret = RED_STAIR_ALIGN_WALK;
			}
			else {
				ret = RED_STAIR_ALIGN_HALF_WALK;
			}
		} else {
			ret = RED_STAIR_ALIGN_END;
		}
	} else {
		if (fp != NULL)
			fprintf(fp, ",X,X", tan, base_y);
		ret = RED_STAIR_ALIGN_END;
	}

	CopyImageForView(dilation, original, X_MIN, Y_MIN);

#ifdef _WIN32
	RGB565toRGB888(dilation, saveImage);
	sprintf(g_filename, "%s_02_result.raw", name);
	SaveBitmap(g_filename, saveImage);
	RGB565toRGB888(original, saveImage);
	sprintf(g_filename, "%s_03_result.raw", name);
	SaveBitmap(g_filename, saveImage);
#endif
	if (fp != NULL)
		fprintf(fp, ",0x%04X\n", ret);

	return ret;
}
