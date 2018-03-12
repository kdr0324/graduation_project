#include "stdafx.h"

#include "Golf.h"

#ifdef _WIN32
#include "../Common/Bitmap.h"
#endif // WIN32

#include "../Amazon/Memory.h"
#include "../Amazon/ImageConvert.h"
#include "../Amazon/Draw.h"
#include "HuroMissonConfig.h"

static int hole_sort(const void *a, const void *b)
{
	int one = ((int *)a)[1];
	int two = ((int *)b)[1];

	if (one > two) {
		return 1;
	}
	else {
		return -1;
	}
}


// 공을 찾으러 가는 함수
int BallDetect(FILE *fp, const char *name)
{
	int ret = 0;
	int labelCnt;
	int bx, by;

	const int IGNORE_MIN_X = 10;
	const int IGNORE_MAX_X = 170;
	const int IGNORE_MIN_Y = 5;
	const int IGNORE_MAX_Y = 115;

	vHS.h.max = ORANGE_H_MAX;
	vHS.h.min = ORANGE_H_MIN;
	vHS.s.max = 255;
	vHS.s.min = ORANGE_S_MIN;

	memset(view->m_pImgU16, 0x0000, sizeof(U16)* AMAZON_WIDTH * AMAZON_HEIGHT);

#if PRINT_MODE
	memset(print[0]->m_pImgU16, 0xFFFF, sizeof(U16)* AMAZON_WIDTH * AMAZON_HEIGHT);
	memset(print[1]->m_pImgU16, 0xFFFF, sizeof(U16)* AMAZON_WIDTH * AMAZON_HEIGHT);
#endif

	CropImage(original, crop_original, 10, 5, 170, 115);
	RGB565toHS(crop_original, hImg, sImg);
	binary_HS(&vHS, binary, ORANGE_16);

	// 노이즈 제거
	medianBinary(binary, noise, ORANGE_16);
	dilationBinary(noise, dilation, ORANGE_16);
	labelCnt = CallLabel(dilation, imgLabel, ORANGE_16, g_object, 30, 30);
	
	if(labelCnt == 1) {
		if (fp != NULL)
			fprintf(fp, ",X,X");
		// 옆으로 틀어졌거나 공을 찼다.
		ret = BALL_DETECT_IS_NOT_BALL;
	} else {
		if (fp != NULL)
			fprintf(fp, ",%d,%d", g_object[1].bottom.x, g_object[1].bottom.y);
		bx = g_object[1].center.x + IGNORE_MIN_X;
		by = g_object[1].bottom.y + IGNORE_MIN_Y;

		DrawCrossLine(dilation, g_object[1].center.x, g_object[1].center.y, 3, GREEN_16);
		// 세세하게 조절 할 필요 없어 다음에 더함
		if (by < 30) {
			ret = BALL_DETECT_RUN_20;
		}
		else if (by < 90) {
			ret = BALL_DETECT_RUN_10;
		}
		else if (by < 100) {
			ret = BALL_DETECT_WALK;
		}
		else if(bx < 80) {              //80, 125
			ret = BALL_DETECT_LEFT_MOTION | BALL_DETECT_SIDE_MOVE;
		}
		else if(bx > 100) {
			ret = BALL_DETECT_SIDE_MOVE;
		} 
		else {
			ret = BALL_DETECT_END;
		}

#if PRINT_MODE
		printResult(bx, 0, 0);
		printResult(by, 1, 0);
		printResult(ret, 0, 1);
#endif
	}

#ifdef _WIN32
	RGB565toRGB888(dilation, saveImage);
	sprintf(g_filename, "%s_01_result.bmp", name);
	SaveBitmap(g_filename, saveImage);
#endif

	if (fp != NULL)
		fprintf(fp, ",0x%04X\n", ret);

	CopyImageForView(dilation, view, IGNORE_MIN_X, IGNORE_MIN_Y);

	return ret;
}

// 공을 차기전 공을 찾는 함수
int Golf_B(FILE *fp, const char *name) 
{
	int ret = 0, i, j;
	int ballCnt, bx, by;
	int dis_baseline = 68, dis_THRESHOLD = 4, dis_DOUBLE_THRESHOLD=12, dis_TRIPLE_THRESHOLD = 20;  // 원래 102
	const int IGNORE_X_MIN = 5;
	const int IGNORE_X_MAX = 180;
	const int IGNORE_Y_MIN = 2;
	const int IGNORE_Y_MAX = 78;

	memset(view->m_pImgU16, 0x0000, sizeof(U16)* AMAZON_WIDTH * AMAZON_HEIGHT);

#if PRINT_MODE
	memset(print[0]->m_pImgU16, 0xFFFF, sizeof(U16)* AMAZON_WIDTH * AMAZON_HEIGHT);
	memset(print[1]->m_pImgU16, 0xFFFF, sizeof(U16)* AMAZON_WIDTH * AMAZON_HEIGHT);
#endif

	CropImage(original, crop_original, IGNORE_X_MIN, IGNORE_Y_MIN, IGNORE_X_MAX, IGNORE_Y_MAX);
	RGB565toHS(crop_original, hImg, sImg);
	vHS.h.max = ORANGE_H_MAX; vHS.h.min = ORANGE_H_MIN;
	vHS.s.max = 255; vHS.s.min = ORANGE_S_MIN;
	binary_HS(&vHS, ballDetect, ORANGE_16);
	medianBinary(ballDetect, noise, ORANGE_16);
	dilationBinary(noise, ballDetect, ORANGE_16);
	ballCnt = CallLabel(ballDetect, imgLabel, ORANGE_16, g_object, 70, 30);

	CopyImageForView(ballDetect, view, IGNORE_X_MIN, IGNORE_Y_MIN);
	
	if (ballCnt >= 2) {
		bx = g_object[1].center.x;
		by = g_object[1].center.y;
		DrawCrossLine(ballDetect, bx, by, 3, YELLOW_16);

#if PRINT_MODE
		printResult(bx, 0, 1);
#endif

		if (bx < dis_baseline - dis_TRIPLE_THRESHOLD) {
			ret = GOLF_SIDE_MOVE_30 | GOLF_LEFT_MOTION;
		} else if (bx > dis_baseline + dis_TRIPLE_THRESHOLD) {
			ret = GOLF_SIDE_MOVE_30;
		} else if (by < 15) {
			ret = GOLF_RUN_10;
		}
		else if (by < 25) {
			ret = GOLF_RUN_5;
		}
		else if (by < 40) {
			ret = GOLF_HALF_WALK;
		}
		else if (bx < dis_baseline - dis_DOUBLE_THRESHOLD) {
			ret = GOLF_SIDE_MOVE_30 | GOLF_LEFT_MOTION;
		}
		else if (bx < dis_baseline - dis_THRESHOLD) {
			ret = GOLF_SIDE_MOVE | GOLF_LEFT_MOTION;
		}
		else if (bx > dis_baseline + dis_DOUBLE_THRESHOLD) {
			ret = GOLF_SIDE_MOVE_30;
		}
		else if (bx > dis_baseline + dis_THRESHOLD) {
			ret = GOLF_SIDE_MOVE;
		}

		else {
			ret = GOLF_END;
		}
	}
	else {
		ret = GOLF_NOT_FOUND;
	}

	return ret;
}

// 공을 차기전 홀을 찾는 함수
int Golf_H(FILE *fp, const char *name)
{
	int ret = 0, i, hole_cnt;
	const int FORWARD = BLUE_16;
	const int DIFF_THRESHOLD_TWO_MIN = -20;			// -16		// 이거보다 작으면 오른쪽으로 회전
	const int DIFF_THRESHOLD_MIN = -16;			// -16		// 이거보다 작으면 오른쪽으로 회전
	const int DIFF_THRESHOLD_MAX = -14;			// -14		// 이거보다 크면 왼쪽으로 회전
	const int DIFF_THRESHOLD_TWO_MAX = -10;			// -14		// 이거보다 크면 왼쪽으로 회전
	int hx, hy, bx, by, diff;
	
	vHS.h.max = HOLE_H_MAX;
	vHS.h.min = HOLE_H_MIN;
	vHS.s.max = 255;
	vHS.s.min = HOLE_S_MIN;
	
	RGB565toHS(original, hImg, sImg);
	binary_HS(&vHS, binary, FORWARD);
	dilationBinary(binary, dilation, FORWARD);
	hole_cnt = CallLabel(dilation, imgLabel, FORWARD, holeLabels, 30, 50);		// 라벨 갯수 + 1을 리턴

	if (hole_cnt >= 2) {
		hx = holeLabels[1].center.x;
		hy = holeLabels[1].center.y;
		bx = g_object[1].center.x;
		by = g_object[1].center.y;
		diff = bx - hx;

		DrawCrossLine(dilation, hx, hy, 3, RED_16);
		for(i = 2; i < hole_cnt; i++)
			DrawCrossLine(dilation, holeLabels[i].center.x, holeLabels[i].center.y, 3, YELLOW_16);

#if PRINT_MODE
		printResult(g_object[1].center.x, 0, 1);		
		printResult(diff, 1, 1);
#endif
		if (diff < DIFF_THRESHOLD_MIN) {
			if (diff < DIFF_THRESHOLD_TWO_MIN) {
				ret = GOLF_TURN_15;
			}
			else {
				ret = GOLF_TURN_5;
			}
			
		} else if (diff > DIFF_THRESHOLD_MAX) {
			if (diff > DIFF_THRESHOLD_TWO_MAX) {
				ret = GOLF_TURN_15 | GOLF_LEFT_MOTION;
			}
			else {
				ret = GOLF_TURN_5 | GOLF_LEFT_MOTION;
			}
		}
	} else {
		ret = GOLF_NOT_FOUND;
	}



#ifdef _WIN32
	RGB565toRGB888(dilation, saveImage);
	sprintf(g_filename, "%s_01_result.bmp", name);
	SaveBitmap(g_filename, saveImage);
#endif

	MergeOriginal(view, dilation);

	return ret;
}