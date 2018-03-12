#include "stdafx.h"

#include "Taekwon.h"

#include "../Common/Bitmap.h"

#include "../Amazon/amazon2_sdk.h"
#include "../Amazon/graphic_api.h"

#include "../Amazon/Memory.h"
#include "TaekwonComm.h"
#include "../Amazon/ImageConvert.h"
#include "../Amazon/Draw.h"

#define _TEST_ 0
#define MAX_SEPARATOR 11
#define FRONT_GO 1

#define START_MODE 3

int max_idx, max_val, left, right, cx, robot_width, var;

int back_walk_count, head_count, punch_count, attack_count, is_attacked, kicks;
int go_front = 1;
int image_num;

static int area_sort(const void *a, const void *b)
{
	int one = ((OBJECT *)a)->left.x;
	int two = ((OBJECT *)b)->left.x;

	if (one > two) {
		return 1;
	} else if (one < two) {
		return -1;
	} else {
		return 0;
	}
}

static int line_sort(const void *a, const void *b)
{
	int one = ((int *)a)[1];
	int two = ((int *)b)[1];

	if (one < two) {
		return 1;
	} else {
		return -1;
	}
}

int taekwon_main(void)
{
	FILE *fp = NULL;
	int ret = 0, head = 0;
	int closed = 0, closed_cnt = 7;

	TaekwonMemoryAllocate();
#ifndef _WIN32		
	usleep(500000);	// 1초간 쉰다.
#endif // WIN32

	if (imageSaveMode) {
		FILE *fp_im = fopen("/mnt/f0/num.txt", "rt");
		fscanf(fp_im, "%d", &image_num);
		fclose(fp_im);
		fp_im = fopen("/mnt/f0/num.txt", "wt");
		fprintf(fp_im, "%d", image_num + 1000);
		fclose(fp_im);
	}
	
#if START_MODE == 1
	taekwon_motion(TAEKWON_START_MODE_1, SAFE_NO);
	taekwon_motion(TAEKWON_TURN_45, SAFE_NO);
#elif START_MODE == 2
	taekwon_motion(TAEKWON_START_MODE_2, SAFE_NO);
#else
	taekwon_motion(TAEKWON_START_MODE_3, SAFE_NO);
#endif
	
	while (1) {
		//head = 0;
		switch (head) {

		case 1:
			taekwon_motion(TAEKWON_HEAD_45_LEFT, SAFE_NO);
			break;
		case 2:
#if 0
			taekwon_motion(TAEKWON_TURN_45 + RIGHT_MOTION, SAFE_NO);
			head = 1;
#else
			taekwon_motion(TAEKWON_HEAD_45_RIGHT, SAFE_NO);
#ifndef _WIN32
			usleep(100000);
#endif
#endif
			break;
		case 3:
			taekwon_motion(TAEKWON_BASIC_POS, SAFE_NO);
			if (head_count % 6 == 0) {
				if (go_front) {
					--go_front ;
					taekwon_motion(TAEKWON_LONG_WALK, SAFE_NO);
				} else {
					++go_front;
					taekwon_motion(TAEKWON_LONG_BACK_WALK, SAFE_NO);
					taekwon_motion(TAEKWON_LONG_BACK_WALK, SAFE_NO);
				}
			}
			head = 0;
			break;
		}

#ifndef _WIN32		
		// 영상을 불러옴
		read_fpga_video_data(original->m_pImgU16);
		draw_img_from_buffer_separator(original->m_pImgU16, LEFT_TOP_SCREEN); // 태권에서는 180도 돌아가있음
#endif // WIN32

#if 1
		// 카메라 가리기 실행
#ifdef _WIN32
		ret = IsClosed(NULL);
#else
		ret = IsClosed();
#endif
		if (ret || closed) {
			if (ret == 1) {
				head = 0;
				back_walk_count = closed = 0;
				go_front = 1;
				head_count = 0;
#ifndef _WIN32
				usleep(500000);				
#endif // _WIN32
				// 정명(MODE_1), 좌(MODE_2), 우(MODE_3)에서 중앙으로 가게함
#if START_MODE == 1
				taekwon_motion(TAEKWON_START_MODE_1, SAFE_NO);
				taekwon_motion(TAEKWON_TURN_45, SAFE_NO);
#elif START_MODE == 2
				taekwon_motion(TAEKWON_START_MODE_2, SAFE_NO);
#else
				taekwon_motion(TAEKWON_START_MODE_3, SAFE_NO);
#endif
			} else {
				closed = 1;
				head = 3;
			}
#ifndef _WIN32
			flip();
#endif
			if (closed_cnt < 0) {
				closed = 0;
			}
			--closed_cnt;

			continue;
		}
#endif
		closed_cnt = 7;
		// 영상을 저장함
		if (imageSaveMode) {
			sprintf(g_filename, "/mnt/f0/image/taekwon_%07d.raw", (int)image_num++);
			SaveRawRGB565(g_filename, original);
		}

		ret = FindRobot(fp, NULL);

		//head = 1;
#ifndef _WIN32
		draw_img_from_buffer_separator(original->m_pImgU16, RIGHT_TOP_SCREEN);
		draw_img_from_buffer_separator(print[0]->m_pImgU16, LEFT_BOTTOM_SCREEN);
		draw_img_from_buffer_separator(print[1]->m_pImgU16, RIGHT_BOTTOM_SCREEN);
		flip();
#endif // WIN32
		
		//continue;
		printf("%d\n", ret);
		if (!ret) {
			//로봇 없는 경우
			++head;		// 0은 정면, 1은 왼쪽, 2는 오른쪽은 본다.
			++head_count;
			continue;
		} else {

			//로봇 있는 경우
			if (head == 1) {
				//head left
				Head_Left();
			}
			else if (head == 2) {
				//head right
				Head_Right();
			}
			else if (head == 0) {
				//head front
				Head_Front();
			}

		}

		if (attack_count % 4 == 0 && is_attacked == 1) {
#ifndef _WIN32
			usleep(50000);
#endif
#if FRONT_GO
			taekwon_motion(TAEKWON_ROUNDING_FRONT, SAFE_NO);
#else
			taekwon_motion(TAEKWON_ROUNDING_BACK, SAFE_NO);
#endif
#ifndef _WIN32
			usleep(150000);
#endif
		}
		is_attacked = 0;
		head = 0;
		head_count = 0;
	}

	TaekwonMemoryRelease();
	return 0;
}


// 막았으면 1, 아니면 0
#ifdef _WIN32
int IsClosed(const char *name)
#else
int IsClosed(void)
#endif
{
	const int MAX_THRESHOLD = 3500000;
	const int MIN_THRESHOLE = 1100000;
	int i, area_sum, ret = 0;

#if PRINT_MODE
	memset(print[0]->m_pImgU16, 0xFFFF, sizeof(U16) * AMAZON_WIDTH * AMAZON_HEIGHT);
	memset(print[1]->m_pImgU16, 0xFFFF, sizeof(U16) * AMAZON_WIDTH * AMAZON_HEIGHT);
#endif

#ifdef _WIN32
	RGB565toRGB888(original, saveImage);
	sprintf(g_filename, "%s_00_original.bmp", name);
	SaveBitmap(g_filename, saveImage);
#endif

	RGB565toI(original, iImg);
	GetIntegralImage(iImg, integral_32);
	area_sum = GetAreaSum32(integral_32, 2, 2, 177, 117);

	printResult(area_sum / 10000, 0, 0);
	printResult(area_sum % 10000, 1, 0);

#ifdef _WIN32
	FilpImage(print[0], view);
	RGB565toRGB888(view, saveImage);
	sprintf(g_filename, "%s_02_result_1.bmp", name);

	SaveBitmap(g_filename, saveImage);
#endif

	if (area_sum > MAX_THRESHOLD)
		ret = 1;
	else if (area_sum < MIN_THRESHOLE)
		ret = 2;

	return ret;
}

int FindRobot(FILE *fp, const char *name)
{
	int ret = 0;
	const int IGNORE_X_MIN = 25; // 20
	const int IGNORE_X_MAX = 160; // 155
	const int IGNORE_Y_MAX = 70;
	const int IGNORE_Y_MIN = 5;

	const int FORWARD = YELLOW_16;
	const int BLACK_I_MAX = 110;		// 경기장 밖을 너무 잡으면 100으로
	const int BLACK_S_MIN = 0;
	const int BLACK_S_MAX = 50;

	const int DIFF = 25;
	const int MAX_VAR = 80;

#if 1
	// 상대방 카메라가 오른쪽을 보고 있을 때
	const int THRES_LEFT_TURN = 95;
	const int THRES_RIGHT_TURN = 55;
#else
	// 상대방 카메라가 왼쪽을 보고 있을 때
	const int THRES_LEFT_TURN = 85;
	const int THRES_RIGHT_TURN = 45;
#endif

	int profile_tmp[AMAZON_WIDTH] = { 0 };

	int gauss[7] = { 1, 4, 5, 6, 5, 4, 1 };
	int profile_x[AMAZON_WIDTH] = { 0 };

	int nWidth, nWidth_1, nHeight, nHeight_1, nWStep;
	int i, j, k, index;
	POINT2D pt[2];
	int tmp, sum, avg, squre_sum, var;	// 표준 편차 계산용
	U16 *pBuf;


#if PRINT_MODE
	memset(print[0]->m_pImgU16, 0xFFFF, sizeof(U16) * AMAZON_WIDTH * AMAZON_HEIGHT);
	memset(print[1]->m_pImgU16, 0xFFFF, sizeof(U16) * AMAZON_WIDTH * AMAZON_HEIGHT);
#endif

#ifdef _WIN32
	RGB565toRGB888(original, saveImage);
	sprintf(g_filename, "%s_00_original.bmp", name);
	SaveBitmap(g_filename, saveImage);
#endif
	CropImage(original, crop_original, IGNORE_X_MIN, IGNORE_Y_MIN, IGNORE_X_MAX, IGNORE_Y_MAX);
	nWidth = crop_original->m_nWidth;
	nWidth_1 = nWidth - 1;
	nHeight = crop_original->m_nHeight;
	nHeight_1 = nHeight - 1;
	nWStep = crop_original->m_nWStep;

#ifdef _WIN32
	RGB565toRGB888(crop_original, saveImage);
	sprintf(g_filename, "%s_01_crop.bmp", name);
	SaveBitmap(g_filename, saveImage);
#endif

	RGB565toSI(crop_original, sImg, iImg);
	vSI.s.max = BLACK_S_MAX, vSI.s.min = BLACK_S_MIN;
	vSI.i.max = BLACK_I_MAX, vSI.i.min = 0;
	binary_SI(&vSI, binary, FORWARD);

#ifdef _WIN32
	RGB565toRGB888(binary, saveImage);
	sprintf(g_filename, "%s_02_binary.bmp", name);
	SaveBitmap(g_filename, saveImage);
#endif


	//medianBinary(binary, noise, FORWARD);
	dilationBinary(binary, dilation, FORWARD);

	//GetBinaryCount(binary, integral);

	memset(view->m_pImgU16, 0xFFFF, sizeof(U16) * AMAZON_WIDTH * AMAZON_HEIGHT);
	SetImageInfo(view, nWidth, nHeight, 1, D_U16);


	for (i = nHeight_1; i >= 0; --i) {
		index = i * nWStep;
		pBuf = &dilation->m_pImgU16[index];
		for (j = nWidth_1; j >= 0; --j) {
			if (pBuf[j] == FORWARD)
				profile_tmp[j]++;
		}
	}

	for (k = 0; k < 5; k++) {
		for (j = 0; j < nWidth; j++) {
			sum = tmp = 0;
			for (i = -3; i < 4; i++) {
				if (j + i >= 0 && j + i < nWidth) {
					sum += gauss[i + 3];
					tmp += profile_tmp[j + i] * gauss[i + 3];
				}
			}
			profile_x[j] = tmp / sum;
		}

		for (j = 0; j < nWidth; j++) {
			sum = tmp = 0;
			for (i = -3; i < 4; i++) {
				if (j + i >= 0 && j + i < nWidth) {
					sum += gauss[i + 3];
					tmp += profile_x[j + i] * gauss[i + 3];
				}
			}
			profile_tmp[j] = tmp / sum;
		}
	}
	for (j = 0; j < nWidth; j++) {
		sum = tmp = 0;
		for (i = -3; i < 4; i++) {
			if (j + i >= 0 && j + i < nWidth) {
				sum += gauss[i + 3];
				tmp += profile_tmp[j + i] * gauss[i + 3];
			}
		}
		profile_x[j] = tmp / sum;
	}

	pBuf = view->m_pImgU16;
	sum = 0;
	squre_sum = 0;
	max_idx = 0;
	for (j = 0; j < nWidth; j++) {
		index = j;
		sum += profile_x[j];
		squre_sum += profile_x[j] * profile_x[j];
		if (profile_x[j] > profile_x[max_idx])
			max_idx = j;
		for (i = 0; i < profile_x[j] && i < nHeight; i++) {
			pBuf[index] = BLUE_16;
			index += nWStep;
		}
	}

#ifdef _WIN32
	RGB565toRGB888(view, saveImage);
	sprintf(g_filename, "%s_04_profile_x.bmp", name);
	SaveBitmap(g_filename, saveImage);
#endif

	sum = SQ_5(sum);
	avg = sum / nWidth;
	squre_sum /= nWidth;
	avg *= avg;
	avg = DSQ_10(avg);
	var = squre_sum - avg;
	printResult(squre_sum, 0, 0);
	printResult(var, 1, 0);

#ifdef _WIN32
	FilpImage(print[0], view);
	RGB565toRGB888(view, saveImage);
	sprintf(g_filename, "%s_05_result_1.bmp", name);
	SaveBitmap(g_filename, saveImage);
#endif

	if (var > MAX_VAR) {
		max_val = profile_x[max_idx];
		DrawCrossLine(dilation, max_idx, nHeight - 1 - max_val, 5, RED_16);

		left = 0;
		for (j = max_idx; j >= 0; --j) {
			if (max_val - profile_x[j] > DIFF) {
				left = j;
				break;
			}
		}

		right = nWidth - 1;
		for (j = max_idx; j < nWidth; ++j) {
			if (max_val - profile_x[j] > DIFF) {
				right = j;
				break;
			}
		}

		tmp = 0;
		for (j = left; j <= right; ++j) {
			tmp += profile_x[j];
		}
		tmp /= (right - left) + 1;

		pt[0].x = pt[1].x = left;
		pt[0].y = 0, pt[1].y = nHeight;
		DrawLine(dilation, pt, GREEN_16);
		pt[0].x = pt[1].x = right;
		pt[0].y = 0, pt[1].y = nHeight;
		DrawLine(dilation, pt, GREEN_16);

		cx = (left + right) >> 1;
		robot_width = right - left + 1;

		DrawCrossLine(dilation, cx, nHeight - 1 - tmp, 5, MAGENTA_16);

		printResult(max_val, 0, 0);
		printResult(cx, 1, 0);
		printResult(left, 0, 1);
		printResult(right, 1, 1);
#ifdef _WIN32
		FilpImage(print[0], view);
		RGB565toRGB888(view, saveImage);
		sprintf(g_filename, "%s_05_result_1.bmp", name);
		SaveBitmap(g_filename, saveImage);
#endif

		// 로봇 거리 : max_val, right-left+1 
		// 로봇 중심 : cx, right-left+1
		// 1순위 : cx, right-left+1
		// 2순위 : 
		ret = TAEKWON_ROBOT_EXIST;
	} else {
		ret = TAEKWON_ROBOT_NOT;
	}

	//ret = TAEKWON_ROBOT_NOT;
#ifdef _WIN32
	RGB565toRGB888(dilation, saveImage);
	sprintf(g_filename, "%s_06_max.bmp", name);
	SaveBitmap(g_filename, saveImage);
#endif

	//return 0;

	CopyImageForView(dilation, original, IGNORE_X_MIN, IGNORE_Y_MIN);

	return ret;
}

int Head_Left() {
	int ret;

	if (max_val > 48 && robot_width > 50) {
		//돌고 때림
		if (cx > 70) {
			++kicks;
			if (kicks % 7 == 0) {
				taekwon_motion(TAEKWON_FRONT_KICK, SAFE_NO);
#ifndef _WIN32
				usleep(100000);
#endif
			}
			else
				taekwon_motion(TAEKWON_FRONT_ATTACK, SAFE_NO);
			taekwon_motion(TAEKWON_LONG_BACK_WALK, SAFE_NO);
			taekwon_motion(TAEKWON_TURN_45, SAFE_NO);
			return 0;
		} else if (cx > 50) {
			taekwon_motion(TAEKWON_LONG_BACK_WALK, SAFE_NO);
#ifndef _WIN32
			usleep(100000);
#endif
			taekwon_motion(TAEKWON_TURN_30, SAFE_NO);
		} else if (cx > 30){
			taekwon_motion(TAEKWON_LONG_BACK_WALK, SAFE_NO);
#ifndef _WIN32
			usleep(100000);
#endif
			taekwon_motion(TAEKWON_TURN_10, SAFE_NO);
		} else {
			taekwon_motion(TAEKWON_LONG_BACK_WALK, SAFE_NO);
			return 0;
		}

		if (max_val > 58 && robot_width > 80) {
			taekwon_motion(TAEKWON_LONG_KICK, SAFE_NO);
		} else if (max_val > 57 && robot_width > 90) {
			taekwon_motion(TAEKWON_LONG_KICK, SAFE_NO);
		} else if (max_val > 55 && cx < 62 && cx >58) {
			++attack_count;
			is_attacked = 1;
			taekwon_motion(TAEKWON_HEAD_PUNCH, SAFE_NO);
		} else if (max_val > 48 && robot_width > 50) {
			++punch_count;
			if (punch_count % 6 == 0) {
				++attack_count;
				is_attacked = 1;
				taekwon_motion(TAEKWON_SHORT_KICK, SAFE_NO);
			} else if(punch_count % 5 == 0) {
				++attack_count;
				is_attacked = 1;
				taekwon_motion(TAEKWON_SHORT_KICK, SAFE_NO);
			} else if (punch_count % 3 == 0) {
				++attack_count;
				is_attacked = 1;
				taekwon_motion(TAEKWON_SHORT_KICK, SAFE_NO);
			} else {
				taekwon_motion(TAEKWON_SHORT_KICK, SAFE_NO);
			}
		}

	} else {
		if (cx > 70) {
			taekwon_motion(TAEKWON_TURN_45, SAFE_NO);
		} else if (cx > 50) {
			taekwon_motion(TAEKWON_TURN_30, SAFE_NO);
		} else if (cx > 30) {
			taekwon_motion(TAEKWON_TURN_10, SAFE_NO);
		} else {
			taekwon_motion(TAEKWON_LONG_BACK_WALK, SAFE_NO);
		}
	}
	return 0;
}


int Head_Right() {
	// 상대방 카메라가 오른쪽을 보고 있을 때	
	if (max_val > 48 && robot_width > 50) {
		//돌고 때림
		taekwon_motion(TAEKWON_BACK_ATTACK, SAFE_NO);
		taekwon_motion(TAEKWON_LONG_BACK_WALK, SAFE_NO);
		if (cx > 70) {
			taekwon_motion(TAEKWON_TURN_30 + RIGHT_MOTION, SAFE_NO);
			return 0;
		} else if (cx > 50) {
			taekwon_motion(TAEKWON_TURN_45 + RIGHT_MOTION, SAFE_NO);
		} else {
			taekwon_motion(TAEKWON_LONG_BACK_WALK, SAFE_NO);
#ifndef _WIN32
			usleep(100000);
#endif
			taekwon_motion(TAEKWON_TURN_45 + RIGHT_MOTION, SAFE_NO);
			return 0;
		} 
	} else {
		if (cx > 70) {
			taekwon_motion(TAEKWON_TURN_30 + RIGHT_MOTION, SAFE_NO);
			return 0;
		} else if (cx > 50) {
			taekwon_motion(TAEKWON_TURN_45 + RIGHT_MOTION, SAFE_NO);
		} else {
			taekwon_motion(TAEKWON_TURN_45 + RIGHT_MOTION, SAFE_NO);
			return 0;
		}
	}
	return 0;
}

int Head_Front() {
	//max_val, width, left, right
	if (cx < 35) {
		++back_walk_count;
		if (max_val > 55) {
			if (back_walk_count % 4 == 0)
				taekwon_motion(TAEKWON_LONG_BACK_WALK, SAFE_NO);
			else
				taekwon_motion(TAEKWON_BACK_WALK, SAFE_NO);
		}
		taekwon_motion(TAEKWON_TURN_10 + RIGHT_MOTION, SAFE_NO);
	}
	else if (cx > 85) {
		++back_walk_count;
		if (max_val > 55) {
			if (back_walk_count % 4 == 0)
				taekwon_motion(TAEKWON_LONG_BACK_WALK, SAFE_NO);
			else
				taekwon_motion(TAEKWON_BACK_WALK, SAFE_NO);
		}
		taekwon_motion(TAEKWON_TURN_10, SAFE_NO);
	}
	
	if (max_val > 50 && robot_width > 50) {
		if (max_val > 58 && robot_width > 80) {
			taekwon_motion(TAEKWON_LONG_KICK, SAFE_NO);
		} else if (max_val > 57 && robot_width > 90) {
			taekwon_motion(TAEKWON_LONG_KICK, SAFE_NO);
		} else if (max_val > 55 && robot_width > 50) {
			++attack_count;
			is_attacked = 1;
			taekwon_motion(TAEKWON_SHORT_KICK, SAFE_NO);
		} else if (max_val > 55 && cx < 60 && cx >50) {
			++attack_count;
			is_attacked = 1;
			taekwon_motion(TAEKWON_HEAD_PUNCH, SAFE_NO);
		} else if (max_val > 50 && robot_width > 50) {
			++punch_count;
			if (punch_count % 3 == 0) {
				++attack_count;
				is_attacked = 1;
				taekwon_motion(TAEKWON_SHORT_KICK, SAFE_NO);
			} else if (punch_count % 5 == 0) {
				++attack_count;
				is_attacked = 1;
				taekwon_motion(TAEKWON_SHORT_KICK, SAFE_NO);
			} else if (punch_count % 2 == 0) {
				++attack_count;
				is_attacked = 1;
				taekwon_motion(TAEKWON_SHORT_KICK, SAFE_NO);
			} else {
				taekwon_motion(TAEKWON_SHORT_KICK, SAFE_NO);
			}
		}
	}
	else if(max_val > 50) {
		taekwon_motion(TAEKWON_WALK, SAFE_NO);
	} else {
		taekwon_motion(TAEKWON_LONG_WALK, SAFE_NO);
	}
	return 0;
}
