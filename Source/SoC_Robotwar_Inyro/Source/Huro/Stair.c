#include "stdafx.h"

#include "Stair.h"

#ifdef _WIN32
#include "../Common/Bitmap.h"
#endif // WIN32

#include "../Amazon/Memory.h"
#include "../Amazon/ImageConvert.h"
#include "../Amazon/Draw.h"
#include "HuroMissonConfig.h"

int StairUp(FILE *fp, const char *name)
{
	// 변수 선언
	int i, labelNum;
	int ret = 0;

	// 고개 각도에 따라 THRESHOLD 변환
	const int THRESHOLD = 200;
	const U16 FORWARD = GREEN_16;

	// 세팅 값 설정
	vHS.h.max = GREEN_H_MAX;
	vHS.h.min = GREEN_H_MIN;
	vHS.s.max = 255;
	vHS.s.min = GREEN_S_MIN;


#if PRINT_MODE
	memset(print[0]->m_pImgU16, 0xFFFF, sizeof(U16)* AMAZON_WIDTH * AMAZON_HEIGHT);
	memset(print[1]->m_pImgU16, 0xFFFF, sizeof(U16)* AMAZON_WIDTH * AMAZON_HEIGHT);
#endif


	RGB565toHS(original, hImg, sImg);			// 16bits, 16bits, 8bits 영상 버퍼
	binary_HS(&vHS, binary, FORWARD);
	medianBinary(binary, noise, FORWARD);		// Binary Median Filter를 이용하여 노이즈 제거

#ifdef _WIN32
	RGB565toRGB888(binary, saveImage);
	sprintf(g_filename, "%s_01_binary.bmp", name);
	SaveBitmap(g_filename, saveImage);
	RGB565toRGB888(noise, saveImage);
	sprintf(g_filename, "%s_02_filtered.bmp", name);
	SaveBitmap(g_filename, saveImage);
#endif

	labelNum = CallLabel(noise, imgLabel, FORWARD, g_object, THRESHOLD, 255);		// 라벨 갯수 + 1을 리턴

	// 라벨이 1개 이상이다.																					
	if(labelNum >= 2) {
		// 라벨영역 중심에 십자가를 표시
		for(i = labelNum - 1; i >= 1; --i) {
			DrawCrossLine(noise, g_object[i].center.x, g_object[i].bottom.y, 3, YELLOW_16);
		}

#ifdef _WIN32
		RGB565toRGB888(noise, saveImage);
		sprintf(g_filename, "%s_03_result.bmp", name);
		SaveBitmap(g_filename, saveImage);
#endif
	}

	// 계단 오르기 위한 초석
	if(labelNum == 1) {
		if (fp != NULL)
			fprintf(fp, ",X,X");
		// 초록이 보이지 않는다. 앞으로 가자
		ret = STAIR_UP_RUN_20;
	} else {
		if (fp != NULL)
			fprintf(fp, ",%d,%d", g_object[1].center.x, g_object[1].bottom.y);
		// 너무 측면이야 조금 중앙으로 가자

		if (g_object[1].bottom.y < 50) {	// 너무 멀어 , 30
			ret |= STAIR_UP_RUN_20;
		} else if (g_object[1].bottom.y < 80) {	// 좀더 가자  , 75
			ret |= STAIR_UP_RUN_15;
		} else if (g_object[1].bottom.y < 90) {	// 좀더 가자  , 75
			ret |= STAIR_UP_RUN_10;
		} else if (g_object[1].bottom.y < 110) {	// 좀더 가자  , 75
			ret |= STAIR_UP_RUN_5;
		} else if (g_object[1].center.x < 89) { //원래 70     ,,  89~97
			ret = STAIR_UP_LEFT_MOTION | STAIR_UP_SIDE_MOVE;
		} else if (g_object[1].center.x > 97) {	//원래 115
			ret = STAIR_UP_SIDE_MOVE;
		} else {	// 갈 필요 없다. 종종하고 올라가자
			ret |= STAIR_UP_END;
		}

#if PRINT_MODE
		printResult(g_object[1].center.x, 0, 0);
		printResult(g_object[1].bottom.y, 1, 0);
		printResult(ret, 0, 1);
#endif

	}
	if (fp != NULL)
		fprintf(fp, ",0x%04X\n", ret);
	return ret;
}

int GreenBridge(FILE *fp, const char *name)
{
	// 변수 선언
	int i, j, index, index2;
	int h_diff;
	int top, bot;
	int left_top_x, left_bot_x;
	int right_top_x, right_bot_x;
	int left_diff, right_diff, center_bot_x;
	int labelNum;
	int ret = 0;
	const int X_MIN = 5, X_MAX = 180, Y_MIN = 0, Y_MAX = 80;
	
	// 고개 각도에 따라 THRESHOLD 변환
	const int THRESHOLD = 400;
	const int NO_COMPENSATION = 700;
	const U16 FORWARD = GREEN_16;

#if PRINT_MODE
	memset(print[0]->m_pImgU16, 0xFFFF, sizeof(U16)* AMAZON_WIDTH * AMAZON_HEIGHT);
	memset(print[1]->m_pImgU16, 0xFFFF, sizeof(U16)* AMAZON_WIDTH * AMAZON_HEIGHT);
#endif


	// 세팅 값 설정
	vHS.h.max = GREEN_H_MAX;
	vHS.h.min = GREEN_H_MIN;
	vHS.s.max = 255;
	vHS.s.min = GREEN_S_MIN;

	CropImage(original, crop_original, X_MIN, Y_MIN, X_MAX, Y_MAX);
	RGB565toHS(crop_original, hImg, sImg);			// 16bits, 16bits, 8bits 영상 버퍼	
	binary_HS(&vHS, binary, FORWARD);
	medianBinary(binary, noise, FORWARD);		// Binary Median Filter를 이용하여 노이즈 제거
	dilationBinary(noise, dilation, FORWARD);

	labelNum = CallLabel(dilation, imgLabel, FORWARD, g_object, THRESHOLD, 255);		// 라벨 갯수 + 1을 리턴

	// 라벨이 1개 이상이다.
	if(labelNum >= 2) {
		// 제일 큰 영역의 중심점을 표시한다.
		for (i = 1; i < labelNum; ++i) {
			DrawCrossLine(dilation, g_object[i].center.x, g_object[i].center.y, 3, YELLOW_16);
		}
	}

	// labelNum이 1인 경우는 무한 루프 돌린다.
	// 테스트 할 때 빠지는 것을 감안 하기 위해서
	if(labelNum >= 2) {
		// 기어 내려갈 수준이다.
		if(g_object[1].areaSize < NO_COMPENSATION) {
			ret = GREEN_BRIDGE_END;
		} else {
			// 옆으로 휘는 것 보정하기 - 위쪽 영역이 꽉 찼을 때만 유효
			if(g_object[1].top.y < 30) {
				left_top_x = left_bot_x = right_top_x = right_bot_x = -1;

				h_diff = g_object[1].height / 10;
				top = g_object[1].top.y + h_diff;
				bot = g_object[1].bottom.y - h_diff;

				index = top * crop_original->m_nWStep;
				index2 = bot * crop_original->m_nWStep;

				//라벨링 영역의 사각형 꼭지점 구하기 - 왼쪽 위아래
				for(j = g_object[1].center.x; j >= 0; --j) {
					if(dilation->m_pImgU16[index + j] == FORWARD) {
						left_top_x = j;
					}
					if(dilation->m_pImgU16[index2 + j] == FORWARD) {
						left_bot_x = j;
					}
				}
				if(left_top_x == -1) {
					left_top_x = g_object[1].center.x;
				}
				if(left_bot_x == -1) {
					left_bot_x = g_object[1].center.x;
				}

				//라벨링 영역의 사각형 꼭지점 구하기 - 오른쪽 위아래
				for(j = g_object[1].center.x; j < crop_original->m_nWStep; ++j) {
					if(dilation->m_pImgU16[index + j] == FORWARD) {
						right_top_x = j;
					}
					if(dilation->m_pImgU16[index2 + j] == FORWARD) {
						right_bot_x = j;
					}
				}
				if(right_top_x == -1) {
					right_top_x = g_object[1].center.x;
				}
				if(right_bot_x == -1) {
					right_bot_x = g_object[1].center.x;
				}

				left_diff = abs(left_top_x - left_bot_x);
				right_diff = abs(right_top_x - right_bot_x);
				center_bot_x = (right_bot_x + left_bot_x) / 2;
				
				DrawCrossLine(dilation, left_top_x, top, 3, YELLOW_16);
				DrawCrossLine(dilation, right_top_x, top, 3, YELLOW_16);
				DrawCrossLine(dilation, left_bot_x, bot, 3, YELLOW_16);
				DrawCrossLine(dilation, right_bot_x, bot, 3, YELLOW_16);
				DrawCrossLine(dilation, center_bot_x, bot, 3, YELLOW_16);
				

				if (center_bot_x < 70) { //오른쪽으로 너무 갔어	//원래 65
					ret |= GREEN_BRIDGE_SIDE_MOVE_30 | GREEN_BRIDGE_LEFT_MOTION;
				} else if (center_bot_x > 100) {
					ret |= GREEN_BRIDGE_SIDE_MOVE_30;
				} else if (right_diff - left_diff > 20) {	//오른쪽으로 휘었어	//원래 30
					ret = GREEN_BRIDGE_SIDE_TURN_30 | GREEN_BRIDGE_LEFT_MOTION;
				} else if(right_diff - left_diff > 8) {	//오른쪽으로 휘었어	//원래 30
					ret = GREEN_BRIDGE_SIDE_TURN_15 | GREEN_BRIDGE_LEFT_MOTION;
				} else if (left_diff - right_diff > 20) { //왼쪽으로 휘었어	//원래 30
					ret = GREEN_BRIDGE_SIDE_TURN_30;
				} else if(left_diff - right_diff > 8) { //왼쪽으로 휘었어	//원래 30
					ret = GREEN_BRIDGE_SIDE_TURN_15;
				} else if (center_bot_x < 82) { //오른쪽으로 너무 갔어	//원래 65
					ret |= GREEN_BRIDGE_SIDE_MOVE_10 | GREEN_BRIDGE_LEFT_MOTION;
				} else if (center_bot_x > 88) {	// //왼쪽으로 많이 갔어	//원래 100
					ret |= GREEN_BRIDGE_SIDE_MOVE_10;
				}  else {
					ret |= GREEN_BRIDGE_GO;
				}
				if (fp != NULL)
					fprintf(fp, ",%d,%d", left_diff, right_diff);
				if (fp != NULL)
					fprintf(fp, ",%d", g_object[1].center.x);

#if PRINT_MODE
				printResult(left_diff - right_diff, 0, 0);
				printResult(right_diff - left_diff, 1, 0);
				printResult(center_bot_x, 0, 1);
#endif
			} else {
				if (fp != NULL)
					fprintf(fp, ",X,X");
				if (g_object[1].center.x < 70) { //오른쪽으로 너무 갔어	//원래 65
					ret |= GREEN_BRIDGE_SIDE_MOVE_30 | GREEN_BRIDGE_LEFT_MOTION;
				} else if (g_object[1].center.x < 82) { //오른쪽으로 너무 갔어	//원래 65
					ret |= GREEN_BRIDGE_SIDE_MOVE_10 | GREEN_BRIDGE_LEFT_MOTION;
				} else if (g_object[1].center.x > 88) {	// //왼쪽으로 많이 갔어	//원래 100
					ret |= GREEN_BRIDGE_SIDE_MOVE_10;
				} else if (g_object[1].center.x > 100) {
					ret |= GREEN_BRIDGE_SIDE_MOVE_30;
				} else {
					ret |= GREEN_BRIDGE_GO;
				}
				if (fp != NULL)
					fprintf(fp, ",%d", g_object[1].center.x);
			}			
		}
	} else {
		ret = GREEN_BRIDGE_END;
		if (fp != NULL)
			fprintf(fp, ",X,X,X");
	}
	CopyImageForView(dilation, view, X_MIN, Y_MIN);

#ifdef _WIN32
	RGB565toRGB888(dilation, saveImage);
	sprintf(g_filename, "%s_01_result.bmp", name);
	SaveBitmap(g_filename, saveImage);
#endif

	if (fp != NULL)
		fprintf(fp, ",0x%04X\n", ret);
	return ret;
}

int StairDown(FILE *fp, const char *name)
{
	int i, j, k, base_y, index;
	int theta, distance, line_num;
	int ret = 0, mx = 0, cnt = 0;
	const U16 FORWARD = YELLOW_16;
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

	RGB565toSI(crop_original, sImg, iImg);
	vSI.s.max = 100; vSI.s.min = 0;
	vSI.i.max = 80; vSI.i.min = 0;
	binary_SI(&vSI, binary, FORWARD);
	medianBinary(binary, noise, FORWARD);
	dilationBinary(noise, dilation, FORWARD);

	pt_num = 0;
	pSrc = dilation->m_pImgU16;
	for (j = 0; j < MARGIN; j++) {
		index = 4 * nWStep + j;
		ct = CTS;
		for (i = 0; i < nHeight && ct > 0; ++i) {
			if (pSrc[index] == FORWARD) {
				pSrc[index] = RED_16;
				pts[pt_num].x = j;
				pts[pt_num++].y = i;
				ct--;
			}
			index += nWStep;
		}

		index = 4 * nWStep + nWidth - 1 - j;
		ct = CTS;
		for (i = 0; i < nHeight && ct > 0; ++i) {
			if (pSrc[index] == FORWARD) {
				pSrc[index] = RED_16;
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
			fprintf(fp, ",%d,%d", theta, base_y);

		if (theta <= -THETA_THRESHOLD || theta >= THETA_THRESHOLD) {
			if (theta >= THETA_THRESHOLD * 3) {
				ret = STAIR_DOWN_TURN_15;
			}
			else if (theta >= THETA_THRESHOLD) {
				ret = STAIR_DOWN_TURN_5;
			}
			else if (theta <= -(THETA_THRESHOLD * 3)) {
				ret = STAIR_DOWN_TURN_15 | STAIR_DOWN_LEFT_MOTION;
			}
			else {
				ret = STAIR_DOWN_TURN_5 | STAIR_DOWN_LEFT_MOTION;
			}

			//발이 검은 맨 끝에 걸치면 105
			//그 뒤에 선은 90
		}
		else if (base_y < 52) {
			ret = STAIR_DOWN_HALF_WALK;		
		}
		else {
			ret = STAIR_DOWN_END;
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
		ret = STAIR_DOWN_END;
	}

#ifdef _WIN32
	RGB565toRGB888(original, saveImage);
	sprintf(g_filename, "%s_00_original.bmp", name);
	SaveBitmap(g_filename, saveImage);
	RGB565toRGB888(dilation, saveImage);
	sprintf(g_filename, "%s_01_dilation.bmp", name);
	SaveBitmap(g_filename, saveImage);
	RGB565toRGB888(crop_original, saveImage);
	sprintf(g_filename, "%s_02_cropped.bmp", name);
	SaveBitmap(g_filename, saveImage);
#endif

	CopyImageForView(dilation, original, X_MIN, Y_MIN);

	if (fp != NULL)
		fprintf(fp, ",0x%04X\n", ret);
	return ret;
}
