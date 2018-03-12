#include "stdafx.h"

#include "YellowHole.h"

#ifdef _WIN32
#include "../Common/Bitmap.h"
#endif // WIN32

#include "../Amazon/Memory.h"

#include "../Amazon/ImageConvert.h"
#include "../Amazon/Draw.h"
#include "HuroMissonConfig.h"

int YellowHoleUp(FILE *fp, const char *name)
{
	int i, j, k, base_y, base_x, index;
	int theta, distance, diff, line_num;
	int nWidth, nHeight, nWStep;
	int ret = 0, mx = 0, cnt = 0;
	const U16 FORWARD = YELLOW_16;
	const int MARGIN = 25;
	const int MINIMUMLINES = 20;
	const int THETA_THRESHOLD = 5;
	const int THEATA_MAX_THRESHOLD = 20;
	const int MINIMUM_THETA = 50;
	const int MINIMUM_DISTANCE = 20;
	const int X_MIN = 15, X_MAX = 170, Y_MIN = 10, Y_MAX = 90;
	U16 *pSrc;
	const int CTS = 2;
	POINT2D pts[(MARGIN * 2 + 1) * CTS] = { { 0, 0 } };
	int pt_num, ct;
	int labelNum;

	const int THRESHOLD = 300;
	const int MINIMUMPOINTS = 10;

#if PRINT_MODE
	memset(print[0]->m_pImgU16, 0xFFFF, sizeof(U16)* AMAZON_WIDTH * AMAZON_HEIGHT);
	memset(print[1]->m_pImgU16, 0xFFFF, sizeof(U16)* AMAZON_WIDTH * AMAZON_HEIGHT);
#endif

	// 세팅 값 설정
	vHS.h.max = YELLOW_HOLE_H_MAX;
	vHS.h.min = YELLOW_HOLE_H_MIN;
	vHS.s.max = 255;
	vHS.s.min = YELLOW_HOLE_S_MIN;

	CropImage(original, crop_original, X_MIN, Y_MIN, X_MAX, Y_MAX);
	nWidth = crop_original->m_nWidth;
	nHeight = crop_original->m_nHeight;
	nWStep = crop_original->m_nWStep;

	RGB565toHS(crop_original, hImg, sImg);			// 16bits, 16bits, 8bits 영상 버퍼
	binary_HS(&vHS, binary, FORWARD);
	medianBinary(binary, noise, FORWARD);		// Binary Median Filter를 이용하여 노이즈 제거
	dilationBinary(noise, dilation, FORWARD);

	labelNum = CallLabel(dilation, imgLabel, FORWARD, g_object, THRESHOLD, 255);		// 라벨 갯수 + 1을 리턴

	pt_num = 0;
	pSrc = dilation->m_pImgU16;
	for (j = 0; j < MARGIN; j++) {
		index = (nHeight - 1) * nWStep + j;
		ct = CTS;
		for (i = nHeight-1; i>=  0 && ct > 0; --i) {
			if (pSrc[index] == FORWARD) {
				//pSrc[index] = MAGENTA_16;
				pts[pt_num].x = j;
				pts[pt_num++].y = i;
				ct--;
			}
			index -= nWStep;
		}

		index = (nHeight - 1) * nWStep + nWidth - 1 - j;
		ct = CTS;
		for (i = nHeight - 1; i >= 0 && ct > 0; --i) {
			if (pSrc[index] == FORWARD) {
				//pSrc[index] = MAGENTA_16;
				pts[pt_num].x = nWidth - 1 - j;
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
			if (hough_counts[i][j] >= MINIMUMPOINTS) {
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

	if (line_num != 0) {
		theta = hough_lines[0].theta;
		distance = hough_lines[0].distance;
		
		base_y = -SQ_10(distance) / cos_lookup[theta];
		if (theta > 90)
			theta = theta - 180;

#if PRINT_MODE
		printResult(theta, 0, 0);
		printResult(base_y, 1, 0);
#endif

		if (fp != NULL)
			fprintf(fp, ",%d,%d", theta, base_y);

		if (theta < -THETA_THRESHOLD || theta > THETA_THRESHOLD) {
			if (theta > THETA_THRESHOLD) {
				ret = YELLOW_HOLE_UP_TURN_10 | YELLOW_HOLE_UP_RIGHT_MOTION;
			}
			else if (theta < -THETA_THRESHOLD) {
				ret = YELLOW_HOLE_UP_TURN_10;
			}
		}
		else {
			if (base_y < 30) {
				ret |= YELLOW_HOLE_UP_RUN_10;
			}
			else if (base_y < 50) {
				ret |= YELLOW_HOLE_UP_WALK;
			}
			else if (base_y < 65) { // 100       기존: 105
				ret |= YELLOW_HOLE_UP_HALF_WALK;
			}
			else {
				ret = YELLOW_HOLE_UP_END;
			}
		}
	}
	else {
		ret |= YELLOW_HOLE_UP_RUN_20;
	}

	CopyImageForView(dilation, view, X_MIN, Y_MIN);
#ifdef _WIN32
	RGB565toRGB888(dilation, saveImage);
	sprintf(g_filename, "%s_02_result.bmp", name);
	SaveBitmap(g_filename, saveImage);
	RGB565toRGB888(original, saveImage);
	sprintf(g_filename, "%s_03_result.bmp", name);
	SaveBitmap(g_filename, saveImage);
#endif

	if (fp != NULL)
		fprintf(fp, ",0x%04X\n", ret);

	return ret;
}


int YellowHoleUp2(FILE *fp, const char *name)
{
	int i, j, k, base_y, base_x, index;
	int theta, distance, diff, line_num;
	int nWidth, nHeight, nWStep;
	int ret = 0, mx = 0, cnt = 0, bot_y;
	const U16 FORWARD = YELLOW_16;
	const int MARGIN = 15;
	const int MINIMUMLINES = 20;
	const int THETA_THRESHOLD = 5;
	const int THEATA_MAX_THRESHOLD = 20;
	const int MINIMUM_THETA = 50;
	const int MINIMUM_DISTANCE = 20;
	const int X_MIN = 15, X_MAX = 170, Y_MIN = 30, Y_MAX = 115;
	U16 *pSrc;
	const int CTS = 2;
	POINT2D pts[(MARGIN * 2 + 1) * CTS] = { { 0, 0 } };
	int pt_num, ct;
	int labelNum;
	int center = (X_MAX - X_MIN) / 2;

	const int THRESHOLD = 300;
	const int MINIMUMPOINTS = 10;

#if PRINT_MODE
	memset(print[0]->m_pImgU16, 0xFFFF, sizeof(U16)* AMAZON_WIDTH * AMAZON_HEIGHT);
	memset(print[1]->m_pImgU16, 0xFFFF, sizeof(U16)* AMAZON_WIDTH * AMAZON_HEIGHT);
#endif

	// 세팅 값 설정
	vHS.h.max = YELLOW_HOLE_H_MAX;
	vHS.h.min = YELLOW_HOLE_H_MIN;
	vHS.s.max = 255;
	vHS.s.min = YELLOW_HOLE_S_MIN;

	CropImage(original, crop_original, X_MIN, Y_MIN, X_MAX, Y_MAX);
	nWidth = crop_original->m_nWidth;
	nHeight = crop_original->m_nHeight;
	nWStep = crop_original->m_nWStep;

	RGB565toHS(crop_original, hImg, sImg);			// 16bits, 16bits, 8bits 영상 버퍼
	binary_HS(&vHS, binary, FORWARD);
	medianBinary(binary, noise, FORWARD);		// Binary Median Filter를 이용하여 노이즈 제거
	dilationBinary(noise, dilation, FORWARD);

	labelNum = CallLabel(dilation, imgLabel, FORWARD, g_object, THRESHOLD, 255);		// 라벨 갯수 + 1을 리턴

	pt_num = 0;
	pSrc = dilation->m_pImgU16;
	for (j = center-MARGIN; j < center+MARGIN; j++) {
		index = (nHeight - 1) * nWStep + j;
		ct = CTS;
		for (i = nHeight - 1; i >= 0 && ct > 0; --i) {
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
			if (hough_counts[i][j] >= MINIMUMPOINTS) {
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

	if (line_num != 0 && labelNum > 1) {
		theta = hough_lines[0].theta;
		distance = hough_lines[0].distance;

		base_y = -SQ_10(distance) / cos_lookup[theta];
		if (theta > 90)
			theta = theta - 180;

		mx = g_object[1].center.x;
		bot_y = g_object[1].bottom.y;

		DrawCrossLine(dilation, mx, bot_y, 3, RED_16);

#if PRINT_MODE
		printResult(theta, 0, 0);
		printResult(base_y, 1, 0);
		printResult(mx, 0, 1);
		printResult(bot_y, 1, 1);
#endif

		if (fp != NULL)
			fprintf(fp, ",%d,%d", theta, base_y);

		if (theta < -THETA_THRESHOLD || theta > THETA_THRESHOLD) {
			if (theta > THETA_THRESHOLD) {
				ret = YELLOW_HOLE_UP_TURN_10 | YELLOW_HOLE_UP_RIGHT_MOTION;
			} else if (theta < -THETA_THRESHOLD) {
				ret = YELLOW_HOLE_UP_TURN_10;
			}
		} else if (mx < 74 || mx > 82) {
			if (mx < 68) {
				ret = YELLOW_HOLE_UP_SIDE_50;
			} else if (mx < 74) {
				ret = YELLOW_HOLE_UP_SIDE_30;
			} else if (mx > 88) {
				ret = YELLOW_HOLE_UP_SIDE_50 | YELLOW_HOLE_UP_RIGHT_MOTION;
			} else if (mx > 82) {
				ret = YELLOW_HOLE_UP_SIDE_30 | YELLOW_HOLE_UP_RIGHT_MOTION;
			}
		} else {
			if (bot_y < 82) { // 100       기존: 105
				ret |= YELLOW_HOLE_UP_RUN_10;
			} else {
				ret = YELLOW_HOLE_UP_END;
			}
		}
	} else {
		ret |= YELLOW_HOLE_UP_RUN_20;
	}

	CopyImageForView(dilation, view, X_MIN, Y_MIN);
#ifdef _WIN32
	RGB565toRGB888(dilation, saveImage);
	sprintf(g_filename, "%s_02_result.bmp", name);
	SaveBitmap(g_filename, saveImage);
	RGB565toRGB888(original, saveImage);
	sprintf(g_filename, "%s_03_result.bmp", name);
	SaveBitmap(g_filename, saveImage);
#endif

	if (fp != NULL)
		fprintf(fp, ",0x%04X\n", ret);

	return ret;
}


int YellowHoleAlign(FILE *fp, const char *name)
{
	int i, j, k, base_y, base_x, index;
	int theta, distance, diff, line_num;
	int nWidth, nHeight, nWStep;
	int right_x=0, left_x=0, center_x=0, center_y = 0;
	int ret = 0, mx = 0, cnt = 0;
	const U16 FORWARD = YELLOW_16;
	const int MARGIN = 25;
	const int MINIMUMLINES = 20;
	const int THETA_THRESHOLD = 5;
	const int MAX_THETA_THRESHOLD = 30;
	const int THEATA_MAX_THRESHOLD = 20;
	const int LEFT_X_MIN_THRESHOLD = 54;
	const int RIGHT_X_MAX_THRESHOLD = 70;
	const int LEFT_X_THRESHOLD = 59;
	const int RIGHT_X_THRESHOLD = 65;
	const int MINIMUM_THETA = 50;
	const int MINIMUM_DISTANCE = 20;
	const int X_MIN = 25, X_MAX = 155, Y_MIN = 10, Y_MAX = 90;

	U16 *pSrc;
	const int CTS = 2;
	POINT2D pts[(MARGIN * 2 + 1) * CTS] = { { 0, 0 } };
	int pt_num, ct;
	int labelNum;

	const int THRESHOLD = 400;
	const int MINIMUMPOINTS = 10;

#if PRINT_MODE
	memset(print[0]->m_pImgU16, 0xFFFF, sizeof(U16)* AMAZON_WIDTH * AMAZON_HEIGHT);
	memset(print[1]->m_pImgU16, 0xFFFF, sizeof(U16)* AMAZON_WIDTH * AMAZON_HEIGHT);
#endif
	memset(view->m_pImgU16, 0x0000, sizeof(U16)* AMAZON_WIDTH * AMAZON_HEIGHT);
	// 세팅 값 설정
	vHS.h.max = YELLOW_HOLE_H_MAX;
	vHS.h.min = YELLOW_HOLE_H_MIN;
	vHS.s.max = 255;
	vHS.s.min = YELLOW_HOLE_S_MIN;

	CropImage(original, crop_original, X_MIN, Y_MIN, X_MAX, Y_MAX);
	nWidth = crop_original->m_nWidth;
	nHeight = crop_original->m_nHeight;
	nWStep = crop_original->m_nWStep;

	RGB565toHS(crop_original, hImg, sImg);			// 16bits, 16bits, 8bits 영상 버퍼
	binary_HS(&vHS, binary, FORWARD);
	InverseImage(binary, FORWARD);
	medianBinary(binary, noise, FORWARD);		// Binary Median Filter를 이용하여 노이즈 제거
	dilationBinary(noise, dilation, FORWARD);
	
	labelNum = CallLabel2(dilation, imgLabel, FORWARD, g_object, THRESHOLD, 255);		// 라벨 갯수 + 1을 리턴

	center_y = g_object[1].center.y;

	pt_num = 0;
	pSrc = dilation->m_pImgU16;
	for (j = 40; j <= 70; j++) {
		index = (center_y) * nWStep + j;
		ct = CTS;
		for (i = center_y; i < nHeight && ct > 0; ++i) {
			if (pSrc[index] != FORWARD) {
				pSrc[index] = MAGENTA_16;
				pts[pt_num].x = j;
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
			if (hough_counts[i][j] >= MINIMUMPOINTS) {
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

	if (line_num != 0 && labelNum > 1) {
		theta = hough_lines[0].theta;
		distance = hough_lines[0].distance;

		base_y = -SQ_10(distance) / cos_lookup[theta];
		if (theta > 90)
			theta = theta - 180;

		center_x = g_object[1].center.x;

		DrawCrossLine(dilation, center_x, g_object[1].center.y , 3, RED_16);

#if PRINT_MODE
		printResult(theta, 0, 0);
		printResult(base_y, 1, 0);
		printResult(center_x, 0, 1);
#endif

		if (fp != NULL)
			fprintf(fp, ",%d,%d", theta, base_y);

		if (theta < -THETA_THRESHOLD || theta > THETA_THRESHOLD) {
			if (theta > MAX_THETA_THRESHOLD) {
				ret = YELLOW_HOLE_ALIGN_TURN_10;
			} else if (theta > THETA_THRESHOLD) {
				ret = YELLOW_HOLE_ALIGN_TURN_10 | YELLOW_HOLE_ALIGN_RIGHT_MOTION;
			} else if (theta < -MAX_THETA_THRESHOLD) {
				ret = YELLOW_HOLE_ALIGN_TURN_10 | YELLOW_HOLE_ALIGN_RIGHT_MOTION;
			} else if (theta < -THETA_THRESHOLD) {
				ret = YELLOW_HOLE_ALIGN_TURN_10;
			}
		} 
		else if (center_x < LEFT_X_THRESHOLD || center_x > RIGHT_X_THRESHOLD) {
			if (center_x > RIGHT_X_MAX_THRESHOLD) {
				ret = YELLOW_HOLE_ALIGN_SIDE_30 | YELLOW_HOLE_ALIGN_RIGHT_MOTION;
			} else if (center_x > RIGHT_X_THRESHOLD) {
				ret = YELLOW_HOLE_ALIGN_SIDE_10 | YELLOW_HOLE_ALIGN_RIGHT_MOTION;
			} else if (center_x < LEFT_X_MIN_THRESHOLD) {
				ret = YELLOW_HOLE_ALIGN_SIDE_30;
			} else if (center_x < LEFT_X_THRESHOLD) {
				ret = YELLOW_HOLE_ALIGN_SIDE_10;
			}
		}
		else {
			if (base_y < 40) { // 40) { // 100       기존: 105
				ret |= YELLOW_HOLE_ALIGN_QUARTER_WALK;
			} else {
				ret = YELLOW_HOLE_ALIGN_END;
			}
		}
	} else {
		ret |= YELLOW_HOLE_ALIGN_HALF_WALK;
	}

	CopyImageForView(dilation, view, X_MIN, Y_MIN);
#ifdef _WIN32
	RGB565toRGB888(dilation, saveImage);
	sprintf(g_filename, "%s_02_result.bmp", name);
	SaveBitmap(g_filename, saveImage);
	RGB565toRGB888(original, saveImage);
	sprintf(g_filename, "%s_03_result.bmp", name);
	SaveBitmap(g_filename, saveImage);
#endif

	if (fp != NULL)
		fprintf(fp, ",0x%04X\n", ret);

	return ret;
}

int YellowHoleDown(FILE *fp, const char *name)
{
	int i, j;
	int ret = 0;
	int cntPoint = 0;
	int start, end;
	const U16 FORWARD = YELLOW_16;
	int tan_THRESHOLD = 102;		//tan기준 5.7도(102)

	int tan, _y;

	const int LINEPOINT = 30;
	const int MINIMUMPOINT = 10;

	POINT2D lineP[10000], drawP[2];
	POINT2D leftP[2] = { { 90 - (LINEPOINT >> 1), 0 },{ 90 - (LINEPOINT >> 1), 179 } };
	POINT2D rightP[2] = { { 90 + (LINEPOINT >> 1), 0 },{ 90 + (LINEPOINT >> 1), 179 } };

#if PRINT_MODE
	memset(print[0]->m_pImgU16, 0xFFFF, sizeof(U16)* AMAZON_WIDTH * AMAZON_HEIGHT);
	memset(print[1]->m_pImgU16, 0xFFFF, sizeof(U16)* AMAZON_WIDTH * AMAZON_HEIGHT);
#endif


	// 한번만 호출 한다.
	cntPoint = 0;
	// 세팅 값 설정
	vHS.h.max = YELLOW_HOLE_H_MAX;
	vHS.h.min = YELLOW_HOLE_H_MIN;
	vHS.s.max = 255;
	vHS.s.min = YELLOW_HOLE_S_MIN;

	RGB565toHS(original, hImg, sImg);			// 16bits, 16bits, 8bits 영상 버퍼
	binary_HS(&vHS, binary, FORWARD);
	medianBinary(binary, noise, FORWARD);		// Binary Median Filter를 이용하여 노이즈 제거
	dilationBinary(noise, dilation, FORWARD);

#ifdef _WIN32
	RGB565toRGB888(dilation, saveImage);
	sprintf(g_filename, "%s_01_binary.bmp", name);
	SaveBitmap(g_filename, saveImage);
#endif

	for(j = 75; j < 105; j++) {//범위 줄임 -> 20-80
		for(i = 11; i <= AMAZON_HEIGHT - 6; ++i) {
			if(dilation->m_pImgU16[i * AMAZON_WIDTH + j] == FORWARD) {
				break;
			}
		}
		lineP[cntPoint].x = j;
		lineP[cntPoint++].y = i;
	}

	qsort(lineP, LINEPOINT, sizeof(POINT2D), line_sort);

	start = 0;
	for(i = 0; i < LINEPOINT; ++i) {
		if(lineP[i].y < 15) {
			start = i + 1;
		} else {
			break;
		}
	}

	end = LINEPOINT - 1;
	for(i = LINEPOINT - 1; i >= 0; --i) {
		if(lineP[i].y > 110) {
			end = i - 1;
		} else {
			break;
		}
	}

	if(end - start > MINIMUMPOINT) {
		CalcLinear(&lineP[start], &_y, &tan, end - start + 1);

		drawP[0].x = 1, drawP[0].y = (_y) + DSQ_10(tan * 1);
		drawP[1].x = 179, drawP[1].y = (_y) + DSQ_10(tan * 179);

		DrawLine(dilation, leftP, MAGENTA_16);
		DrawLine(dilation, rightP, MAGENTA_16);
		DrawLine(dilation, drawP, CYAN_16);

#ifdef _WIN32
		DrawLine(original, leftP, MAGENTA_16);
		DrawLine(original, rightP, MAGENTA_16);
		DrawLine(original, drawP, CYAN_16);
#endif

		if (fp != NULL)
			fprintf(fp, ",%d,%d,%d", end - start, tan, _y);
		if(tan <= -tan_THRESHOLD || tan >= tan_THRESHOLD) {
			if(tan >= tan_THRESHOLD * 3) {
				ret = YELLOW_HOLE_DOWN_TURN_15;
			} else if(tan >= tan_THRESHOLD) {
				ret = YELLOW_HOLE_DOWN_TURN_5;
			} else if(tan <= -(tan_THRESHOLD * 3)) {
				ret = YELLOW_HOLE_DOWN_TURN_15 | YELLOW_HOLE_DOWN_LEFT_MOTION;
			} else if (tan <= -tan_THRESHOLD ){
				ret = YELLOW_HOLE_DOWN_TURN_5 | YELLOW_HOLE_DOWN_LEFT_MOTION;
			}
		} else if(_y < 95) {  //90 (60, 70)
			if(_y <= 75) {
				ret = YELLOW_HOLE_DOWN_SINGLE_HALF_WALK | YELLOW_HOLE_DOWN_MISSION_END_FLAG;
			}
			else {
				ret = YELLOW_HOLE_DOWN_HALF_HALF_WALK;
			}
		} else {
			ret = YELLOW_HOLE_DOWN_END;
		}

#if PRINT_MODE
		printResult(tan, 0, 0);
		printResult(_y, 1, 0);
		printResult(ret, 0, 1);
#endif

	} else {
		if (fp != NULL)
			fprintf(fp, ",%d,X,X", end - start);
		ret = YELLOW_HOLE_DOWN_END;
	}

#ifdef _WIN32
	RGB565toRGB888(dilation, saveImage);
	sprintf(g_filename, "%s_02_result.bmp", name);
	SaveBitmap(g_filename, saveImage);
	RGB565toRGB888(original, saveImage);
	sprintf(g_filename, "%s_03_result.bmp", name);
	SaveBitmap(g_filename, saveImage);
#endif

	if (fp != NULL)
		fprintf(fp, ",0x%04X\n", ret);
	return ret;
}

int YellowHoleDown2(FILE *fp, const char *name)
{
	int i, j, k, base_y, base_x, index;
	int theta, distance, diff, line_num;
	int ret = 0, mx = 0, cnt = 0;
	const U16 FORWARD = YELLOW_16;
	const int MARGIN = 30;
	const int MINIMUMLINES = 12;
	const int THETA_THRESHOLD = 3;
	const int THEATA_MAX_THRESHOLD = 20;
	U16 *pSrc;
	const int CTS = 2;
	POINT2D pts[(MARGIN * 2 + 1) * CTS] = { { 0, 0 } };
	int pt_num, ct;

#if PRINT_MODE
	memset(print[0]->m_pImgU16, 0xFFFF, sizeof(U16)* AMAZON_WIDTH * AMAZON_HEIGHT);
	memset(print[1]->m_pImgU16, 0xFFFF, sizeof(U16)* AMAZON_WIDTH * AMAZON_HEIGHT);
#endif

	// 세팅 값 설정
	vHS.h.max = YELLOW_HOLE_H_MAX;
	vHS.h.min = YELLOW_HOLE_H_MIN;
	vHS.s.max = 255;
	vHS.s.min = YELLOW_HOLE_S_MIN;

	RGB565toHS(original, hImg, sImg);			// 16bits, 16bits, 8bits 영상 버퍼
	binary_HS(&vHS, binary, FORWARD);
	medianBinary(binary, noise, FORWARD);		// Binary Median Filter를 이용하여 노이즈 제거
	dilationBinary(noise, dilation, FORWARD);

	pt_num = 0;
	pSrc = dilation->m_pImgU16;
	for (j = AMAZON_HALF_WIDTH - MARGIN; j <= AMAZON_HALF_WIDTH + MARGIN; j++) {
		index = 4 * AMAZON_WIDTH + j;
		ct = CTS;
		for (i = 4; i < AMAZON_HEIGHT - 2 && ct > 0; ++i) {
			if (pSrc[index] == FORWARD) {
				pSrc[index] = RED_16;
				pts[pt_num].x = j;
				pts[pt_num++].y = i;
				ct--;
			}
			index += AMAZON_WIDTH;
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
			fprintf(fp, ",%d,%d", theta, base_y);

	if (theta <= -THETA_THRESHOLD || theta >= THETA_THRESHOLD) {
			if (theta >= THETA_THRESHOLD * 3) {
				ret = YELLOW_HOLE_DOWN_TURN_15;
			}
			else if (theta >= THETA_THRESHOLD) {
				ret = YELLOW_HOLE_DOWN_TURN_5;
			}
			else if (theta <= -(THETA_THRESHOLD * 3)) {
				ret = YELLOW_HOLE_DOWN_TURN_15 | YELLOW_HOLE_DOWN_LEFT_MOTION;
			}
			else if (theta <= -THETA_THRESHOLD) {
				ret = YELLOW_HOLE_DOWN_TURN_5 | YELLOW_HOLE_DOWN_LEFT_MOTION;
			}
		}
		else if (base_y < 98) {  //90 (60, 70)
			if (base_y <= 75) {
				ret = YELLOW_HOLE_DOWN_SINGLE_HALF_WALK | YELLOW_HOLE_DOWN_MISSION_END_FLAG;
			}
			else {
				ret = YELLOW_HOLE_DOWN_HALF_HALF_WALK;
			}
		}
		else {
			ret = YELLOW_HOLE_DOWN_END;
		}

#if PRINT_MODE
		printResult(theta, 0, 0);
		printResult(base_y, 1, 0);
		printResult(ret, 0, 1);
#endif
	}
	else {
		if (fp != NULL)
			fprintf(fp, ",X,X");
		ret = YELLOW_HOLE_DOWN_END;
	}

#ifdef _WIN32
	RGB565toRGB888(dilation, saveImage);
	sprintf(g_filename, "%s_02_result.bmp", name);
	SaveBitmap(g_filename, saveImage);
	RGB565toRGB888(original, saveImage);
	sprintf(g_filename, "%s_03_result.bmp", name);
	SaveBitmap(g_filename, saveImage);
#endif

	if (fp != NULL)
		fprintf(fp, ",0x%04X\n", ret);
	return ret;
}

