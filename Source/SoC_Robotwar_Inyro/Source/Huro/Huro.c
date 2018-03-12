#include "stdafx.h"

#include "Huro.h"

#include "../Common/Bitmap.h"

#ifndef _WIN32
#include "../Amazon/amazon2_sdk.h"
#include "../Amazon/graphic_api.h"
#endif // _WIN32

#include "../Amazon/Memory.h"
#include "HuroComm.h"
#include "../Amazon/ImageConvert.h"

#include "AlignPosition.h"
#include "Barricade.h"
#include "RedStair.h"
#include "Mine.h"
#include "Hurdle.h"
#include "Stair.h"
#include "Golf.h"
#include "YellowHole.h"

#define _TEST_ 0

int image_num;

static int SaveImageWithMotion(void)
{
	int motion_num;
	int frame_num = 0;
	char name[1024];

	printf("Call Motion Num (exit 0): ");
	scanf("%d", &motion_num);
	getchar();
	huro_motion(motion_num, SAFE_LOT);
	while (1) {
		// Save Bitamp
		printf("Press Enter Key");
		getchar();
		printf("%d\n", image_num);
		// Save Bitamp
#ifndef _WIN32
		read_fpga_video_data(original->m_pImgU16);
		draw_img_from_buffer_separator(original->m_pImgU16, FULL_SCREEN);
		flip();
#endif
		sprintf(name, "/mnt/f0/image/%07d.raw", image_num++);
		SaveRawRGB565(name, original);
	}

	return 1;
}

static int MissionThresholdCatcher(void)
{
	int motion_num;
	int mission_n = 0;
	int frame_num = 0;
	char name[1024];

	printf("Call Motion Num (exit 0): ");
	scanf("%d", &motion_num);

	printf("0: Align\n");
	printf("1: RedStairup\n");
	printf("2: RedStairAlign\n");
	printf("3: MineDetect\n");
	printf("4: Hurdle\n");
	printf("5: StairUp\n");
	printf("6: GreenBridge\n");
	printf("7: StairDown\n");
	printf("8: BallDetect\n");
	printf("9: Golf_B\n");
	printf("10: Golf_A\n");
	printf("11: YellowHoleUp\n");
	printf("12: YellowHoleAling\n");
	printf("13: YellowHoleDown\n");

	printf("Call Mission Num (exit 0): ");
	scanf("%d", &mission_n);

	huro_motion(motion_num, SAFE_LOT);
	while (1) {
#ifndef _WIN32
		read_fpga_video_data(original->m_pImgU16);
		draw_img_from_buffer_separator(original->m_pImgU16, LEFT_TOP_SCREEN);
#endif
		switch (mission_n) {
		case 0:
			AlignPosition(NULL, NULL);
			break;
		case 1:
			RedStair(NULL, NULL);
			break;
		case 2:
			RedStairAlign(NULL, NULL);
			break;
		case 3:
			MineDetect(NULL, NULL);
			break;
		case 4:
			Hurdle(NULL, NULL);
			break;
		case 5:
			StairUp(NULL, NULL);
			break;
		case 6:
			GreenBridge(NULL, NULL);
			break;
		case 7:
			StairDown(NULL, NULL);
			break;
		case 8:
			BallDetect(NULL, NULL);
			break;
		case 9:
			Golf_B(NULL, NULL);
			break;
		case 10:
			Golf_H(NULL, NULL);
			break;
		case 11:
			YellowHoleUp(NULL, NULL);
			break;
		case 12:
			YellowHoleAlign(NULL, NULL);
			break;
		case 13:
			YellowHoleDown2(NULL, NULL);
			break;
		case 14:
			huro_motion(HEAD_DOWN_80, SAFE_NO);
#ifndef _WIN32
			read_fpga_video_data(original->m_pImgU16);
			draw_img_from_buffer_separator(original->m_pImgU16, LEFT_TOP_SCREEN);
#endif
			Golf_B(NULL, NULL);
			huro_motion(BASIC_OFF, SAFE_NO);
#ifndef _WIN32
			read_fpga_video_data(original->m_pImgU16);
			draw_img_from_buffer_separator(original->m_pImgU16, LEFT_TOP_SCREEN);
#endif
			Golf_H(NULL, NULL);


#ifndef _WIN32
			usleep(500000);
#endif
		}

#ifndef _WIN32
		draw_img_from_buffer_separator(view->m_pImgU16, RIGHT_TOP_SCREEN);
		draw_img_from_buffer_separator(print[0]->m_pImgU16, LEFT_BOTTOM_SCREEN);
		draw_img_from_buffer_separator(print[1]->m_pImgU16, RIGHT_BOTTOM_SCREEN);
		flip();
#endif
	}

	return 1;
}


// 이미지 결과 출력용
static int mission_00(char mode[4], int cnt);
static int mission_00_turn(char mode[4], int cnt);
static int mission_01(void);
static int mission_02(void);
static int mission_03(void);
static int mission_04(void);
static int mission_05(void);
static int mission_06(void);
static int mission_07(void);
static int mission_08(void);

int huro_main(void)
{
	//! 보드 환경에서 시작할 때 5초 대기
#ifndef _WIN32
	sleep(2);
#endif

	HuroMemoryAllocate();

	if (imageSaveMode) {
		FILE *fp_im = fopen("/mnt/f0/num.txt", "rt");
		fscanf(fp_im, "%d", &image_num);
		fclose(fp_im);
		fp_im = fopen("/mnt/f0/num.txt", "wt");
		fprintf(fp_im, "%d", image_num + 1000);
		fclose(fp_im);
	}

	//huro_mission();
	mission_01();
	mission_02();
	mission_03();
	mission_04();
	mission_05();
	mission_06();
	mission_07();
	mission_08();

	HuroMemoryRelease();

	return 1;
}

int huro_mission(void)
{
	int mission_num;

	HuroMemoryAllocate();

	if (imageSaveMode) {
		FILE *fp_im = fopen("/mnt/f0/num.txt", "rt");
		fscanf(fp_im, "%d", &image_num);
		fclose(fp_im);
		fp_im = fopen("/mnt/f0/num.txt", "wt");
		fprintf(fp_im, "%d", image_num + 1000);
		fclose(fp_im);
	}

	// scanf로 미션 번호 입력
	if (imageSaveMode) {
		printf("Enter Mission Num (0~8, 99: Save image with motion): ");
	}
	else {
		printf("Enter Mission Num (0~8): ");
	}
	scanf("%d", &mission_num);

	//! 보드 환경에서 시작할 때 5초 대기
#ifndef _WIN32
	if (mission_num > 10)
		sleep(1);
	else
		sleep(3);
#endif

	// swtich로 미션 실행
	switch (mission_num) {
	case 0:
		mission_00("wt", 10);
		break;
	case 1:
		mission_01();
		break;
	case 2:
		mission_02();
		break;
	case 3:
		mission_03();
		break;
	case 4:
		mission_04();
		break;
	case 5:
		mission_05();
		break;
	case 6:
		mission_06();
		break;
	case 7:
		mission_07();
		break;
	case 8:
		mission_08();
		break;
#if PRINT_MODE
	case 98:
		if (noMotionMode == 0)
			MissionThresholdCatcher();
		break;
#endif
	case 99:
		if (imageSaveMode && noMotionMode == 0)
			SaveImageWithMotion();
		break;
	default:
		break;
	}

	HuroMemoryRelease();

	return 1;
}

int huro_continuous_mission(void)
{
	int mission_num;

	HuroMemoryAllocate();

	if (imageSaveMode) {
		FILE *fp_im = fopen("/mnt/f0/num.txt", "rt");
		fscanf(fp_im, "%d", &image_num);
		fclose(fp_im);
		fp_im = fopen("/mnt/f0/num.txt", "wt");
		fprintf(fp_im, "%d", image_num + 1000);
		fclose(fp_im);
	}

	// scanf로 미션 번호 입력
	printf("Enter Start Mission Num (1~8): ");
	scanf("%d", &mission_num);

	//! 보드 환경에서 시작할 때 2초 대기
#ifndef _WIN32
	sleep(3);
#endif

	huro_motion(BASIC_OFF, SAFE_NO);

	// swtich로 미션 실행
	switch (mission_num) {
	case 1:
		mission_01();
	case 2:
		mission_02();
	case 3:
		mission_03();
	case 4:
		mission_04();
	case 5:
		mission_05();
	case 6:
		mission_06();
	case 7:
		mission_07();
	case 8:
		mission_08();
	default:
		break;
	}

	HuroMemoryRelease();

	return 1;
}

static int mission_00(char mode[4], int cnt)
{
	FILE *fp = NULL;
	int ret, DIR_MODE;

	if (imageSaveMode) {
		fp = fopen("/mnt/f0/result/mission_00.csv", mode);
		fprintf(fp, "AlignPosition\n");
	}
	huro_motion(HEAD_RIGHT_90_DOWN_35, SAFE_NO);					// 팔 뒤로 고개 위아래 호출
	while (cnt > 0) {
		// 좌우 이동, 회전의 경우 팔과 머리를 고정 한 채로 움직인다.
		//huro_motion(HEAD_RIGHT_90_DOWN_35, SAFE_NO);					// 팔 뒤로 고개 위아래 호출

#ifndef _WIN32
		read_fpga_video_data(original->m_pImgU16);
		draw_img_from_buffer_separator(original->m_pImgU16, LEFT_TOP_SCREEN);		// 영상 4분할 출력 모드
#endif // WIN32

		if (imageSaveMode) {
			fprintf(fp, "mission_00_Align_%07d", (int)image_num);
			sprintf(g_filename, "/mnt/f0/image/mission_00_Align_%07d.bmp", (int)image_num++);
			RGB565toRGB888(original, saveImage);
			SaveBitmap(g_filename, saveImage);
		}

		ret = AlignPosition(fp, NULL);
#ifndef _WIN32
		draw_img_from_buffer_separator(view->m_pImgU16, RIGHT_TOP_SCREEN);		// 영상 4분할 출력 모드
#if PRINT_MODE
		draw_img_from_buffer_separator(print[0]->m_pImgU16, LEFT_BOTTOM_SCREEN);
		draw_img_from_buffer_separator(print[1]->m_pImgU16, RIGHT_BOTTOM_SCREEN);
#endif
		flip();
#endif // WIN32

		if (ret == ALIGN_POSITION_END || ret == -1)
			break;

		if (ret & ALIGN_POSITION_SIDE_70) {
			huro_motion(SIDE_MOVE_70, SAFE_LITTLE);
			continue;
		}

		if (ret & ALIGN_POSITION_LEFT_MOTION)
			DIR_MODE = LEFT_MOTION;
		else
			DIR_MODE = RIGHT_MOTION;

		if (ret & ALIGN_POSITION_TURN_20) {
			huro_motion(TURN_15 + DIR_MODE, SAFE_LITTLE);
		}
		else if (ret & ALIGN_POSITION_TURN_5) {
			huro_motion(TURN_15 + DIR_MODE, SAFE_LITTLE);
			//huro_motion(TURN_5 + DIR_MODE, SAFE_LOT);
		}
		else if (ret & ALIGN_POSITION_TRIPLE_SIDE_50) {
			huro_motion(SIDE_MOVE_50 + DIR_MODE, SAFE_NO);
			huro_motion(SIDE_MOVE_50 + DIR_MODE, SAFE_NO);
			huro_motion(SIDE_MOVE_50 + DIR_MODE, SAFE_LITTLE);
		}
		else if (ret & ALIGN_POSITION_DOUBLE_SIDE_50) {
			huro_motion(SIDE_MOVE_50 + DIR_MODE, SAFE_NO);
			huro_motion(SIDE_MOVE_50 + DIR_MODE, SAFE_LITTLE);
		}
		else if (ret & ALIGN_POSITION_SINGLE_SIDE_50) {
			huro_motion(SIDE_MOVE_50 + DIR_MODE, SAFE_LITTLE);
		}
		else if (ret & ALIGN_POSITION_SINGLE_SIDE_20) {
			huro_motion(SIDE_MOVE_10 + DIR_MODE, SAFE_LITTLE);
		}
		cnt--;
	}

	if (imageSaveMode)
		fclose(fp);

	huro_motion(BASIC_OFF, SAFE_LITTLE);

	return 0;
}

static int mission_00_turn(char mode[4], int cnt)
{
	FILE *fp = NULL;
	int ret, DIR_MODE;

	if (imageSaveMode) {
		fp = fopen("/mnt/f0/result/mission_00.csv", mode);
		fprintf(fp, "AlignPosition\n");
	}
	huro_motion(HEAD_RIGHT_90_DOWN_35, SAFE_NO);					// 팔 뒤로 고개 위아래 호출
	while (cnt > 0) {
		// 좌우 이동, 회전의 경우 팔과 머리를 고정 한 채로 움직인다.
		//huro_motion(HEAD_RIGHT_90_DOWN_35, SAFE_NO);					// 팔 뒤로 고개 위아래 호출

#ifndef _WIN32
		read_fpga_video_data(original->m_pImgU16);
		draw_img_from_buffer_separator(original->m_pImgU16, LEFT_TOP_SCREEN);		// 영상 4분할 출력 모드
#endif // WIN32

		if (imageSaveMode) {
			fprintf(fp, "mission_00_Align_Turn_%07d", (int)image_num);
			sprintf(g_filename, "/mnt/f0/image/mission_00_Align_Turn_%07d.bmp", (int)image_num++);
			RGB565toRGB888(original, saveImage);
			SaveBitmap(g_filename, saveImage);
		}

		ret = AlignPositionTurn(fp, NULL);
#ifndef _WIN32
		draw_img_from_buffer_separator(view->m_pImgU16, RIGHT_TOP_SCREEN);		// 영상 4분할 출력 모드
#if PRINT_MODE
		draw_img_from_buffer_separator(print[0]->m_pImgU16, LEFT_BOTTOM_SCREEN);
		draw_img_from_buffer_separator(print[1]->m_pImgU16, RIGHT_BOTTOM_SCREEN);
#endif
		flip();
#endif // WIN32

		if (ret == ALIGN_POSITION_END || ret == -1)
			break;

		if (ret & ALIGN_POSITION_LEFT_MOTION)
			DIR_MODE = LEFT_MOTION;
		else
			DIR_MODE = RIGHT_MOTION;

		if (ret & ALIGN_POSITION_TURN_20) {
			huro_motion(TURN_15 + DIR_MODE, SAFE_LITTLE);
		} else if (ret & ALIGN_POSITION_TURN_5) {
			huro_motion(TURN_5 + DIR_MODE, SAFE_LITTLE);
			//huro_motion(TURN_5 + DIR_MODE, SAFE_LOT);
		}
		cnt--;
	}

	if (imageSaveMode)
		fclose(fp);

	huro_motion(BASIC_OFF, SAFE_NO);

	return 0;
}

static int mission_01(void)
{
	FILE *fp = NULL;
	int ret, is_open = 0;

	if (imageSaveMode) {
		fp = fopen("/mnt/f0/result/mission_01.csv", "wt");
		fprintf(fp, "BarricadeUp\n");
	}

	huro_motion(BASIC_OFF, SAFE_NO);
	while (1) {
#ifndef _WIN32		
		read_fpga_video_data(original->m_pImgU16);
		draw_img_from_buffer_separator(original->m_pImgU16, LEFT_TOP_SCREEN);		// 영상 4분할 출력 모드
#endif // WIN32
		if (imageSaveMode) {
			fprintf(fp, "mission_01_BarricadeUp_%07d", (int)image_num);
			sprintf(g_filename, "/mnt/f0/image/mission_01_BarricadeUp_%07d.raw", (int)image_num++);
			SaveRawRGB565(g_filename, original);
		}

		ret = BarricadeUp(fp, NULL);
		if (ret >= 3) {
			is_open = 1;
		}
#ifndef _WIN32
		draw_img_from_buffer_separator(noise->m_pImgU16, RIGHT_TOP_SCREEN);
		flip();
#endif // WIN32

		if (is_open && ret == 1) { // 바리게이트가 올라간 조건
								   // 다음 미션인 게이트가 존재해서 먼거리는 달릴수 없음.
								   // huro_motion(HEAD_FRONT, SAFE_LOT);		// 고개 정면 모션을 콜한다.
#ifndef _WIN32
			usleep(500000);
#endif
			huro_motion(RUN_40, SAFE_LITTLE);
			huro_motion(RUN_20, SAFE_LITTLE);
			huro_motion(WALK, SAFE_LITTLE);
			break;
		}
	}
	if (imageSaveMode)
		fclose(fp);

	return 0;
}

static int mission_02(void)
{
	FILE *fp = NULL;
	int cnt, ret, DIR_MODE;

	// 빨간 계단 올라가기
	if (imageSaveMode) {
		fp = fopen("/mnt/f0/result/mission_02.csv", "wt");
		fprintf(fp, "RedStair\n");
	}


	while (1) {
		// huro_motion(HEAD_DOWN_20, SAFE_NO);	// 기본자세로 변경


#ifndef _WIN32		
		read_fpga_video_data(original->m_pImgU16);
		draw_img_from_buffer_separator(original->m_pImgU16, LEFT_TOP_SCREEN);		// 영상 4분할 출력 모드
#endif // WIN32
		
		if (imageSaveMode) {
			printf("ImageSaveMode\n");
			fprintf(fp, "mission_02_RedStair_%07d", (int)image_num);
			sprintf(g_filename, "/mnt/f0/image/mission_02_RedStair_%07d.bmp", (int)image_num++);
			RGB565toRGB888(original, saveImage);
			SaveBitmap(g_filename, saveImage);
		}
		ret = RedStair(fp, NULL);		

#ifndef _WIN32
		CopyImageForView(dilation, original, 0, 0);
		draw_img_from_buffer_separator(original->m_pImgU16, RIGHT_TOP_SCREEN);		// 영상 4분할 출력 모드
#if PRINT_MODE
		draw_img_from_buffer_separator(print[0]->m_pImgU16, LEFT_BOTTOM_SCREEN);
		//draw_img_from_buffer_separator(print[1]->m_pImgU16, RIGHT_BOTTOM_SCREEN);
#endif
		flip();
#endif // WIN32

		//! 반복문 탈출하고 전진 종종걸음 후 올라가는 것
		if (ret == RED_STAIR_END) {	// 가깝다 올라갈 준비하자
			break;
		}


		//! 빨간 계단이 발견되지 않았으니 걸어야하는 것

		if (ret & RED_STAIR_RIGHT_MOTION) {
			DIR_MODE = RIGHT_MOTION;
		}
		else {
			DIR_MODE = LEFT_MOTION;
		}

		if (ret & RED_STAIR_WALK) {
			huro_motion(WALK, SAFE_LITTLE);
		} else if (ret & RED_STAIR_SIDE_MOVE_10) {
			huro_motion(SIDE_MOVE_10 + DIR_MODE, SAFE_NO);
			huro_motion(STAIR_BELLBELL_SHORT, SAFE_LITTLE);
		}
		else if (ret & RED_STAIR_SIDE_MOVE_30) {
			huro_motion(SIDE_MOVE_30 + DIR_MODE, SAFE_NO);
			huro_motion(STAIR_BELLBELL_SHORT, SAFE_LITTLE);
		}
		else if (ret & RED_STAIR_SIDE_MOVE_70) {
			huro_motion(SIDE_MOVE_70 + DIR_MODE, SAFE_NO);
			huro_motion(STAIR_BELLBELL_SHORT, SAFE_LITTLE);
		}
	}

	// 계단 올라가기 시작
	huro_motion(STAIR_BELLBELL_SHORT, SAFE_LITTLE);
	// 계단 올라가자
	huro_motion(STAIR_UP_4, SAFE_LOT);
	//huro_motion(SIDE_MOVE_70 + RIGHT_MOTION, SAFE_LITTLE);
	//huro_motion(TURN_15 + RIGHT_MOTION, SAFE_LITTLE);
	//huro_motion(RUN_10, SAFE_LITTLE);

	if (imageSaveMode) {
		fclose(fp);
		fp = fopen("/mnt/f0/result/mission_02.csv", "at");
		fprintf(fp, "RedStairAlign\n");
	}

	while (1) {
		huro_motion(HEAD_DOWN_80, SAFE_NO);
#ifndef _WIN32		
		read_fpga_video_data(original->m_pImgU16);
		draw_img_from_buffer_separator(original->m_pImgU16, LEFT_TOP_SCREEN);		// 영상 4분할 출력 모드
#endif // WIN32
		if (imageSaveMode) {
			fprintf(fp, "mission_02_RedStairAlign_%07d", (int)image_num);
			sprintf(g_filename, "/mnt/f0/image/mission_02_RedStairAlign_%07d.bmp", (int)image_num++);
			RGB565toRGB888(original, saveImage);
			SaveBitmap(g_filename, saveImage);
		}

		ret = RedStairAlign(fp, NULL);

#ifndef _WIN32
		draw_img_from_buffer_separator(dilation->m_pImgU16, RIGHT_TOP_SCREEN);
#if PRINT_MODE		
		draw_img_from_buffer_separator(print[0]->m_pImgU16, LEFT_BOTTOM_SCREEN);
		draw_img_from_buffer_separator(print[1]->m_pImgU16, RIGHT_BOTTOM_SCREEN);
#endif
		flip();
#endif // WIN32

		if (ret == RED_STAIR_ALIGN_END)
			break;

		if (ret & RED_STAIR_ALIGN_LEFT_MOTION)
			DIR_MODE = LEFT_MOTION;
		else
			DIR_MODE = RIGHT_MOTION;

		if (ret & RED_STAIR_ALIGN_TURN_30)			
			huro_motion(TURN_30 + DIR_MODE, SAFE_NO);
		else if (ret & RED_STAIR_ALIGN_TURN_15)
			huro_motion(TURN_15 + DIR_MODE, SAFE_LITTLE);
		else if (ret & RED_STAIR_ALIGN_RUN) 
			huro_motion(RUN_15, SAFE_LOT);
		else if (ret & RED_STAIR_ALIGN_WALK)
			huro_motion(WALK, SAFE_LITTLE);
		else if(ret & RED_STAIR_ALIGN_HALF_WALK)
			huro_motion(HALF_WALK, SAFE_LOT);
	}
	if (imageSaveMode)
		fclose(fp);

	huro_motion(STAIR_DOWN_4, SAFE_LITTLE);
	mission_00("wt", 10);
	
	return 0;
}

static int mission_03(void)
{
	// Mine 함수 반환
	int ret;
	// 왼쪽, 오른쪽 모션 판단하는 변수
	int DIR_MODE = 1;
	// 방향성 검사
	int cnt=0;

	huro_motion(RUN_40, SAFE_LOT);
	mission_00("at", 7);
	huro_motion(SIDE_MOVE_50 + LEFT_MOTION, SAFE_LITTLE);
	huro_motion(RUN_35, SAFE_LITTLE);
	huro_motion(HALF_WALK, SAFE_LITTLE);
	huro_motion(SIDE_MOVE_50 + RIGHT_MOTION, SAFE_LITTLE);
	mission_00("at", 7);
#if 0
	huro_motion(SIDE_MOVE_50 + RIGHT_MOTION, SAFE_LITTLE);
	huro_motion(SIDE_MOVE_70 + RIGHT_MOTION, SAFE_LITTLE);
	huro_motion(RUN_20, SAFE_LITTLE);	
	huro_motion(SIDE_MOVE_50 + LEFT_MOTION, SAFE_LITTLE);
	huro_motion(SIDE_MOVE_70 + LEFT_MOTION, SAFE_LITTLE);
#else
	huro_motion(SIDE_MOVE_30 + RIGHT_MOTION, SAFE_LITTLE);
	huro_motion(MINE_RUN + LEFT_MOTION, SAFE_LITTLE);
#endif
	mission_00("at", 7);
	huro_motion(RUN_50, SAFE_LITTLE);

	return 0;

	huro_motion(SIDE_MOVE_70 + RIGHT_MOTION, SAFE_LITTLE);
	huro_motion(SIDE_MOVE_70 + RIGHT_MOTION, SAFE_LITTLE);
	huro_motion(RUN_50, SAFE_LITTLE);
	huro_motion(SIDE_MOVE_70 + LEFT_MOTION, SAFE_LITTLE);
	mission_00("at", 7);
	huro_motion(RUN_40, SAFE_LITTLE);


	huro_motion(SIDE_MOVE_50 + LEFT_MOTION, SAFE_LITTLE);
	huro_motion(SIDE_MOVE_70 + LEFT_MOTION, SAFE_LITTLE);
	huro_motion(RUN_50, SAFE_LITTLE);
	huro_motion(SIDE_MOVE_50 + RIGHT_MOTION, SAFE_LITTLE);
	huro_motion(SIDE_MOVE_70 + RIGHT_MOTION, SAFE_LITTLE);
	mission_00("at", 7);

	huro_motion(SIDE_MOVE_70 + RIGHT_MOTION, SAFE_LITTLE);
	huro_motion(SIDE_MOVE_70 + RIGHT_MOTION, SAFE_LITTLE);
	huro_motion(RUN_50, SAFE_LITTLE);
	huro_motion(SIDE_MOVE_70 + LEFT_MOTION, SAFE_LITTLE);
	mission_00("at", 7);
	huro_motion(RUN_40, SAFE_LITTLE);


	return 0;


	FILE *fp = NULL;
	if (imageSaveMode) {
		fp = fopen("/mnt/f0/result/mission_03.csv", "wt");
		fprintf(fp, "MineDetect\n");
	}

	while (cnt < 100) {
		huro_motion(HEAD_DOWN_80, SAFE_NO);
#ifndef _WIN32
		read_fpga_video_data(original->m_pImgU16);
		draw_img_from_buffer_separator(original->m_pImgU16, LEFT_TOP_SCREEN);
#endif
		if (imageSaveMode) {
			fprintf(fp, "mission_03_MineDetect_%07d", (int)image_num);
			sprintf(g_filename, "/mnt/f0/image/mission_03_MineDetect_%07d.raw", (int)image_num++);
			SaveRawRGB565(g_filename, original);
		}

		ret = MineDetect(fp, NULL);
#ifndef _WIN32
		draw_img_from_buffer_separator(original->m_pImgU16, RIGHT_TOP_SCREEN);		// 영상 4분할 출력 모드
#if PRINT_MODE
		draw_img_from_buffer_separator(print[0]->m_pImgU16, LEFT_BOTTOM_SCREEN);
		draw_img_from_buffer_separator(print[1]->m_pImgU16, RIGHT_BOTTOM_SCREEN);
#endif
		flip();
#endif // WIN32

		if (ret == MINE_DETECT_IS_NOT_MINE) {
			huro_motion(RUN_15, SAFE_LITTLE);
			cnt += 15;
			mission_00_turn("at", 3);
		} else if (ret & MINE_DETECT_LEFT_MOTION) {
			DIR_MODE = 0;
			if (ret & MINE_DETECT_SIDE_MOVE_30) {
				huro_motion(SIDE_MOVE_30 + DIR_MODE, SAFE_LITTLE);
			} else {
				huro_motion(SIDE_MOVE_70 + DIR_MODE, SAFE_LITTLE);
			}
		} else {
			DIR_MODE = 1;
			
			if (ret & MINE_DETECT_MINE_RUN) {
				huro_motion(MINE_RUN, SAFE_LITTLE);
				cnt += 20;
				mission_00("at", 10);
			} else if (ret & MINE_DETECT_QUARTER_WALK) {
				huro_motion(QUARTER_WALK, SAFE_LITTLE);
				cnt += 2;
			} else if (ret & MINE_DETECT_HALF_WALK) {
				huro_motion(HALF_WALK, SAFE_LITTLE);
				cnt += 4;
			} else if (ret & MINE_DETECT_WALK) {
				huro_motion(WALK, SAFE_LITTLE);
				cnt += 7;
			} else if (ret & MINE_DETECT_SIDE_MOVE_30) {
				huro_motion(SIDE_MOVE_30 + DIR_MODE, SAFE_LITTLE);				
			} else if (ret & MINE_DETECT_SIDE_MOVE_10) {
				huro_motion(SIDE_MOVE_10 + DIR_MODE, SAFE_LITTLE);
			} else {
				huro_motion(SIDE_MOVE_70 + DIR_MODE, SAFE_LITTLE);
			}
		}
	}

	if (imageSaveMode) {
		fprintf(fp, "MineDetect&Hurdle\n");
	}

	while (1) {
		huro_motion(BASIC_OFF, SAFE_NO);
#ifndef _WIN32
		read_fpga_video_data(original->m_pImgU16);
		draw_img_from_buffer_separator(original->m_pImgU16, LEFT_TOP_SCREEN);
#endif
		if (imageSaveMode) {
			fprintf(fp, "mission_03_Hurdle_%07d", (int)image_num);
			sprintf(g_filename, "/mnt/f0/image/mission_03_Hurdle_%07d.raw", (int)image_num++);
			SaveRawRGB565(g_filename, original);
		}
		ret = Hurdle(fp, NULL);
#ifndef _WIN32
		draw_img_from_buffer_separator(binary->m_pImgU16, RIGHT_TOP_SCREEN);		// 영상 4분할 출력 모드
		flip();
#endif // WIN32

		if (ret == HURDLE_END) {
			break;
		} else {
			huro_motion(HEAD_DOWN_80, SAFE_NO);
#ifndef _WIN32
			read_fpga_video_data(original->m_pImgU16);
			draw_img_from_buffer_separator(original->m_pImgU16, LEFT_TOP_SCREEN);
#endif
			if (imageSaveMode) {
				fprintf(fp, "mission_03_MineDetect_%07d", (int)image_num);
				sprintf(g_filename, "/mnt/f0/image/mission_03_MineDetect_%07d.raw", (int)image_num++);
				SaveRawRGB565(g_filename, original);
			}
			ret = MineDetect(fp, NULL);
#ifndef _WIN32
			draw_img_from_buffer_separator(original->m_pImgU16, RIGHT_TOP_SCREEN);		// 영상 4분할 출력 모드
			flip();
#endif // WIN32

			if (ret == MINE_DETECT_IS_NOT_MINE) {
				mission_00("at", 3);
				huro_motion(RUN_15, SAFE_LITTLE);
			} else if (ret & MINE_DETECT_LEFT_MOTION) {
				DIR_MODE = 0;
				if (ret & MINE_DETECT_SIDE_MOVE_30) {
					huro_motion(SIDE_MOVE_30 + DIR_MODE, SAFE_LITTLE);
				} else {
					huro_motion(SIDE_MOVE_70 + DIR_MODE, SAFE_LITTLE);
				}
			} else {
				DIR_MODE = 1;
				if (ret & MINE_DETECT_MINE_RUN) {
					huro_motion(MINE_RUN, SAFE_LITTLE);
				} else if (ret & MINE_DETECT_QUARTER_WALK) {
					huro_motion(QUARTER_WALK, SAFE_LITTLE);
				} else if (ret & MINE_DETECT_HALF_WALK) {
					huro_motion(HALF_WALK, SAFE_LITTLE);
				} else if (ret & MINE_DETECT_WALK) {
					huro_motion(WALK, SAFE_LITTLE);
				} else if (ret & MINE_DETECT_SIDE_MOVE_30) {
					huro_motion(SIDE_MOVE_30 + DIR_MODE, SAFE_LITTLE);
				} else if (ret & MINE_DETECT_SIDE_MOVE_10) {
					huro_motion(SIDE_MOVE_10 + DIR_MODE, SAFE_LITTLE);
				} else {
					huro_motion(SIDE_MOVE_70 + DIR_MODE, SAFE_LITTLE);
				}
			}
		} 
	}
	huro_motion(WALK, SAFE_LITTLE);
	huro_motion(WALK, SAFE_LITTLE);
	huro_motion(STAIR_BELLBELL_WALK, SAFE_NO);
	huro_motion(HURDLE, SAFE_LOT);
	huro_motion(RUN_20, SAFE_LOT);
	huro_motion(TURN_45, SAFE_LITTLE);
	huro_motion(TURN_45, SAFE_LOT);
	mission_00("at", 10);
	huro_motion(RUN_40, SAFE_LOT);
	mission_00("at", 5);
	huro_motion(RUN_40, SAFE_LOT);
	//huro_motion(WALK, SAFE_LITTLE);

	if (imageSaveMode)
		fclose(fp);

	return 0;
}

static int mission_04(void)
{
	FILE *fp = NULL;
	int ret = 0;
	while (1) {
		huro_motion(BASIC_OFF, SAFE_NO);
#ifndef _WIN32
		read_fpga_video_data(original->m_pImgU16);
		draw_img_from_buffer_separator(original->m_pImgU16, LEFT_TOP_SCREEN);
#endif
		if (imageSaveMode) {
			fprintf(fp, "mission_03_Hurdle_%07d", (int)image_num);
			sprintf(g_filename, "/mnt/f0/image/mission_03_Hurdle_%07d.raw", (int)image_num++);
			SaveRawRGB565(g_filename, original);
		}
		ret = Hurdle(fp, NULL);
#ifndef _WIN32
		draw_img_from_buffer_separator(binary->m_pImgU16, RIGHT_TOP_SCREEN);		// 영상 4분할 출력 모드
		flip();
#endif // WIN32
		if (ret == HURDLE_END) {
			break;
		} else {
			huro_motion(WALK, SAFE_NO);

		}
	}
	
	huro_motion(STAIR_BELLBELL_WALK, SAFE_NO);
	huro_motion(HURDLE, SAFE_LOT);
	huro_motion(RUN_20, SAFE_LOT);
	huro_motion(TURN_45, SAFE_LITTLE);
	huro_motion(TURN_30, SAFE_LOT);
	huro_motion(TURN_30, SAFE_LOT);
	mission_00("at", 10);
	huro_motion(RUN_40, SAFE_LOT);
	mission_00("at", 5);
	huro_motion(RUN_40, SAFE_LOT);

	//Hurdle
	return 0;
}

static int mission_05(void)
{
	FILE *fp = NULL;
	int cnt, ret;
	int DIR_MODE;
	// 영상 입력 후 기본영상 출력

	// 계단 올라가기
	cnt = 0;
	if (imageSaveMode) {
		fp = fopen("/mnt/f0/result/mission_05.csv", "wt");
		fprintf(fp, "StairUp\n");
		fprintf(fp, "이미지 이름,bot_x,bot_y,반환값\n");
	}

	while (cnt < 3) {
#ifndef _WIN32		
		read_fpga_video_data(original->m_pImgU16);
		draw_img_from_buffer_separator(original->m_pImgU16, LEFT_TOP_SCREEN);		// 영상 4분할 출력 모드
#endif // WIN32
		if (imageSaveMode) {
			fprintf(fp, "mission_05_StairUp_%07d", (int)image_num);
			sprintf(g_filename, "/mnt/f0/image/mission_05_StairUp_%07d.bmp", (int)image_num++);
			RGB565toRGB888(original, saveImage);
			SaveBitmap(g_filename, saveImage);
		}
		ret = StairUp(fp, NULL);
#ifndef _WIN32
		draw_img_from_buffer_separator(noise->m_pImgU16, RIGHT_TOP_SCREEN);		// 영상 4분할 출력 모드
#if PRINT_MODE
		draw_img_from_buffer_separator(print[0]->m_pImgU16, LEFT_BOTTOM_SCREEN);
		draw_img_from_buffer_separator(print[1]->m_pImgU16, RIGHT_BOTTOM_SCREEN);
#endif
		flip();
#endif // WIN32
		if (ret == STAIR_UP_END)
			break;
		else {
			if (ret & STAIR_UP_LEFT_MOTION) {
				DIR_MODE = 0;
				huro_motion(SIDE_MOVE_30 + DIR_MODE, SAFE_LITTLE);
				huro_motion(STAIR_BELLBELL_SHORT, SAFE_NO);
			} else {
				DIR_MODE = 1;
				if (ret & STAIR_UP_RUN_20) {
					huro_motion(RUN_20, SAFE_LITTLE);
				} else if (ret & STAIR_UP_RUN_15) {
					huro_motion(RUN_15, SAFE_LITTLE);
				} else if (ret & STAIR_UP_RUN_10) {
					huro_motion(WALK, SAFE_LITTLE);
				} else if (ret & STAIR_UP_RUN_5) {
					huro_motion(HALF_WALK, SAFE_LITTLE);
				} else {
					huro_motion(SIDE_MOVE_30 + DIR_MODE, SAFE_LITTLE);
					huro_motion(STAIR_BELLBELL_SHORT, SAFE_NO);
				}
			}
		}
	}
	//huro_motion(HALF_HALF_WALK, SAFE_NO);

	huro_motion(STAIR_BELLBELL_WALK, SAFE_LOT);
	// 계단 올라가자
	huro_motion(STAIR_UP_2, SAFE_LOT);
	//huro_motion(WALK, SAFE_LITTLE);
	huro_motion(HALF_WALK, SAFE_LITTLE);
	huro_motion(HALF_WALK, SAFE_LITTLE);

	// 계단 올라가기 종료

	// 계단 건너기		
	cnt = 0;
	if (imageSaveMode) {
		fclose(fp);
		fp = fopen("/mnt/f0/result/mission_05.csv", "at");
		fprintf(fp, "GreenBride\n");
		fprintf(fp, "이미지 이름,왼쪽 기울기,오른쪽 기울기,중심,반환값\n");
	}

	while (cnt < 5) {
		huro_motion(HEAD_DOWN_80, SAFE_LITTLE);		// 고개 아래 모션을 콜한다.
#ifndef _WIN32
		read_fpga_video_data(original->m_pImgU16);
		draw_img_from_buffer_separator(original->m_pImgU16, LEFT_TOP_SCREEN);
#endif
		if (imageSaveMode) {
			fprintf(fp, "mission_05_GreenBridge_%07d", (int)image_num);
			sprintf(g_filename, "/mnt/f0/image/mission_05_GreenBridge_%07d.bmp", (int)image_num++);
			RGB565toRGB888(original, saveImage);
			SaveBitmap(g_filename, saveImage);
		}
		ret = GreenBridge(fp, NULL);
#ifndef _WIN32
		draw_img_from_buffer_separator(view->m_pImgU16, RIGHT_TOP_SCREEN);
#if PRINT_MODE
		draw_img_from_buffer_separator(print[0]->m_pImgU16, LEFT_BOTTOM_SCREEN);
		draw_img_from_buffer_separator(print[1]->m_pImgU16, RIGHT_BOTTOM_SCREEN);
#endif
		flip();
#endif
		if (ret == GREEN_BRIDGE_END)
			break;
		else {
			if (ret & GREEN_BRIDGE_LEFT_MOTION) {
				DIR_MODE = 0;
			} else{
				DIR_MODE = 1;
			}

			if (ret & GREEN_BRIDGE_SIDE_MOVE_30) {
				huro_motion(SIDE_MOVE_30 + DIR_MODE, SAFE_LITTLE);
			} else if (ret & GREEN_BRIDGE_SIDE_MOVE_10) {
				huro_motion(SIDE_MOVE_10 + DIR_MODE, SAFE_LITTLE);
			} else if (ret & GREEN_BRIDGE_SIDE_TURN_30) {
				huro_motion(TURN_30 + DIR_MODE, SAFE_LITTLE);
			} else if (ret & GREEN_BRIDGE_SIDE_TURN_15) {
				huro_motion(TURN_15 + DIR_MODE, SAFE_LITTLE);
			} else {
				if (cnt < 1) {
					huro_motion(RUN_25, SAFE_LOT);
					cnt++;
				} else if (cnt < 4) {
					huro_motion(RUN_20, SAFE_LOT);
					cnt++;
				} 
				else {
					huro_motion(WALK, SAFE_LOT);
					cnt++;
				}
			}
		} 
	}

	// 계단 내려가기
	cnt = 0;
	if (imageSaveMode) {
		fclose(fp);
		fp = fopen("/mnt/f0/result/mission_05.csv", "at");
		fprintf(fp, "StairDown\n");
	}

	while (cnt < 5) {
		huro_motion(HEAD_DOWN_80, SAFE_NO);					// 팔 뒤로 고개 위아래 호출
#ifndef _WIN32
		read_fpga_video_data(original->m_pImgU16);
		draw_img_from_buffer_separator(original->m_pImgU16, LEFT_TOP_SCREEN);		// 영상 4분할 출력 모드
#endif
		if (imageSaveMode) {
			fprintf(fp, "mission_05_StairDown_%07d", (int)image_num);
			sprintf(g_filename, "/mnt/f0/image/mission_05_StairDown_%07d.bmp", (int)image_num++);
			RGB565toRGB888(original, saveImage);
			SaveBitmap(g_filename, saveImage);
		}

		ret = StairDown(fp, NULL);
#ifndef _WIN32
		draw_img_from_buffer_separator(original->m_pImgU16, RIGHT_TOP_SCREEN);		// 영상 4분할 출력 모드
#if PRINT_MODE
		draw_img_from_buffer_separator(print[0]->m_pImgU16, LEFT_BOTTOM_SCREEN);
		draw_img_from_buffer_separator(print[1]->m_pImgU16, RIGHT_BOTTOM_SCREEN);
#endif
		flip();
#endif // WIN32

		if (ret == STAIR_DOWN_END)
			break;

		if (ret & STAIR_DOWN_LEFT_MOTION) {
			DIR_MODE = LEFT_MOTION;
		}
		else {
			DIR_MODE = RIGHT_MOTION;
		}

		if (ret & STAIR_DOWN_TURN_5) {
			huro_motion(TURN_15 + DIR_MODE, SAFE_LITTLE);
		}
		else if (ret & STAIR_DOWN_TURN_15) {
			huro_motion(TURN_15 + DIR_MODE, SAFE_LITTLE);
		}		
		else if (ret & STAIR_DOWN_HALF_WALK) {
			huro_motion(HALF_WALK, SAFE_LOT);
			cnt++;
		}
	}
	if (imageSaveMode)
		fclose(fp);
	//huro_motion(QUARTER_WALK, SAFE_LITTLE);

	huro_motion(STAIR_DOWN_2, SAFE_LOT);
	huro_motion(TURN_45 + RIGHT_MOTION, SAFE_LITTLE);
	mission_00("at", 10);
	huro_motion(RUN_40, SAFE_LOT);	
	mission_00("at", 10);

	return 0;
}

static int mission_06(void)
{
	int ret, DIR_MODE, cnt = 0, end=0;
	int pre = 0;
	int diff;
	
	FILE *fp = NULL;
	if (imageSaveMode) {
		fp = fopen("/mnt/f0/result/mission_06.csv", "wt");
		fprintf(fp, "BallDetect\n");
	}

	while (1) {
#ifndef _WIN32
		read_fpga_video_data(original->m_pImgU16);
		draw_img_from_buffer_separator(original->m_pImgU16, LEFT_TOP_SCREEN);		// 영상 4분할 출력 모드
#endif
		if (imageSaveMode) {
			fprintf(fp, "mission_06_BallDetect_%07d", (int)image_num);
			sprintf(g_filename, "/mnt/f0/image/mission_06_BallDetect_%07d.bmp", (int)image_num++);
			RGB565toRGB888(original, saveImage);
			SaveBitmap(g_filename, saveImage);
		}
		ret = BallDetect(fp, NULL);
#ifndef _WIN32
		draw_img_from_buffer_separator(view->m_pImgU16, RIGHT_TOP_SCREEN);		// 영상 4분할 출력 모드
#if PRINT_MODE
		draw_img_from_buffer_separator(print[0]->m_pImgU16, LEFT_BOTTOM_SCREEN);
		draw_img_from_buffer_separator(print[1]->m_pImgU16, RIGHT_BOTTOM_SCREEN);
#endif
		flip();
#endif // WIN32

		if (ret == BALL_DETECT_END) {
			break;
		}
		if (ret & BALL_DETECT_LEFT_MOTION) {
			DIR_MODE = LEFT_MOTION;
		}
		else {
			DIR_MODE = RIGHT_MOTION;
		}
		// 옆으로 틀어졌건 공을 찬 경우 공이 없음
		if (ret & BALL_DETECT_IS_NOT_BALL) {
			// 공이 있었는데 움직여서 사라짐
			if (pre == 1) {
				pre = 2;
				break;
			}
			// 공이 없음
			else {
				pre = 1;
				huro_motion(WALK, SAFE_LITTLE);
			}
		}
		//공이 있는 경우
		else {
			pre = 1;
			if (ret & BALL_DETECT_RUN_20) {
				huro_motion(RUN_20, SAFE_LITTLE);
			}
			else if (ret & BALL_DETECT_RUN_10) {
				huro_motion(WALK, SAFE_LITTLE);
			}
			else if (ret & BALL_DETECT_WALK) {
				huro_motion(WALK, SAFE_LITTLE);
			}
			else if (ret & BALL_DETECT_SIDE_MOVE) {
				huro_motion(SIDE_MOVE_30 + DIR_MODE, SAFE_LITTLE);
			}
		}
	}
	mission_00_turn("at", 10);
	huro_motion(SIDE_MOVE_50 + RIGHT_MOTION, SAFE_LITTLE);
	//huro_motion(TURN_30 + RIGHT_MOTION, SAFE_LITTLE);
	//huro_motion(WALK, SAFE_LITTLE);

	if (imageSaveMode) {
		fclose(fp);
		fp = fopen("/mnt/f0/result/mission_06.csv", "at");
		fprintf(fp, "Golf\n");
		fprintf(fp, "이미지 이름,X 변화량,공 중심,리턴 값\n");
	}
	while (/*pre != 2 && */cnt < 2  && end < 20) {
		huro_motion(HEAD_DOWN_80, SAFE_NO);
		end++;
#ifndef _WIN32
		read_fpga_video_data(original->m_pImgU16);
		draw_img_from_buffer_separator(original->m_pImgU16, LEFT_TOP_SCREEN);		// 영상 4분할 출력 모드
#endif
		if (imageSaveMode) {
			fprintf(fp, "mission_06_Golf_B_%07d", (int)image_num);
			sprintf(g_filename, "/mnt/f0/image/mission_06_Golf_B_%07d.bmp", (int)image_num++);
			RGB565toRGB888(original, saveImage);
			SaveBitmap(g_filename, saveImage);
		}

		//Ball_Detect Head_80
		ret = Golf_B(fp, NULL);

		if (ret != GOLF_END) {
			//  공 위치 잡기
			if (ret & GOLF_LEFT_MOTION) {
				DIR_MODE = 0;
				huro_motion(SIDE_MOVE_10 + DIR_MODE, SAFE_LITTLE);
			} else {
				DIR_MODE = 1;
				if (ret & GOLF_RUN_10) {
					huro_motion(RUN_10, SAFE_LITTLE);
				} else if (ret & GOLF_RUN_5) {
					huro_motion(WALK, SAFE_LITTLE);
				} else if (ret & GOLF_HALF_WALK) {
					huro_motion(HALF_WALK, SAFE_LITTLE);
				} else if (ret & GOLF_SIDE_MOVE) {
					huro_motion(SIDE_MOVE_10 + DIR_MODE, SAFE_LITTLE);
				}
				else if (ret & GOLF_SIDE_MOVE_30) {
					huro_motion(SIDE_MOVE_30 + DIR_MODE, SAFE_LITTLE);
				}
				else if (ret & GOLF_NOT_FOUND) {
					huro_motion(WALK, SAFE_LITTLE);
					cnt++;
				}
			}
#ifndef _WIN32
			draw_img_from_buffer_separator(view->m_pImgU16, LEFT_BOTTOM_SCREEN);
			flip();
#endif
			continue;
		}

		//너무 높음 낮추자
		huro_motion(BASIC_OFF, SAFE_NO);

#ifndef _WIN32
		read_fpga_video_data(original->m_pImgU16);
		draw_img_from_buffer_separator(original->m_pImgU16, RIGHT_TOP_SCREEN);		// 영상 4분할 출력 모드
#endif

		if (imageSaveMode) {
			fprintf(fp, "mission_06_Golf_H_%07d", (int)image_num);
			sprintf(g_filename, "/mnt/f0/image/mission_06_Golf_H_%07d.bmp", (int)image_num++);
			RGB565toRGB888(original, saveImage);
			SaveBitmap(g_filename, saveImage);
		}

		ret = Golf_H(fp, NULL);

#ifndef _WIN32
		draw_img_from_buffer_separator(view->m_pImgU16, LEFT_BOTTOM_SCREEN);		// 영상 4분할 출력 모드
#if PRINT_MODE
		draw_img_from_buffer_separator(print[1]->m_pImgU16, RIGHT_BOTTOM_SCREEN);
#endif
		flip();
#endif // WIN32

		// 골프 킥
		if (ret == GOLF_END) {
			break;
		}
		// 홀 없는 경우
		// 모션 좌우 체크
		else if (ret & GOLF_LEFT_MOTION) {
			DIR_MODE = LEFT_MOTION;
			huro_motion(TURN_5 + DIR_MODE, SAFE_LITTLE);
		}
		else {
			DIR_MODE = RIGHT_MOTION;
			if (ret & GOLF_NOT_FOUND) {
				huro_motion(TURN_15 + DIR_MODE, SAFE_LITTLE);
				cnt++;
			}
			else if (ret & GOLF_TURN_5) {
				huro_motion(TURN_5 + DIR_MODE, SAFE_LITTLE);
			}
			else if (ret & GOLF_TURN_15) {
				huro_motion(TURN_5 + DIR_MODE, SAFE_LITTLE);
				huro_motion(TURN_5 + DIR_MODE, SAFE_LITTLE);
			}
		}
	}

	if (imageSaveMode)
		fclose(fp);
	huro_motion(QUARTER_WALK, SAFE_LITTLE);
	huro_motion(GOLF, SAFE_LOT);
	huro_motion(RUN_10, SAFE_LITTLE);	
	huro_motion(TURN_45, SAFE_NO);
	huro_motion(TURN_45, SAFE_NO);
	//huro_motion(TURN_45, SAFE_NO);
	huro_motion(TURN_30, SAFE_LITTLE);
	mission_00("at", 10);
	huro_motion(RUN_50, SAFE_LOT);
	mission_00("at", 10);
	huro_motion(RUN_50, SAFE_LOT);	
	mission_00("at", 10);

	return 0;
}

static int mission_07(void)
{
	int cnt, ret, DIR_MODE;
	FILE *fp = NULL;
	if (imageSaveMode) {
		fp = fopen("/mnt/f0/result/mission_07.csv", "wt");
		fprintf(fp, "YellowHoleUp\n");
	}
	
	while (1) {
		//huro_motion(HEAD_DOWN_80, SAFE_NO);
#ifndef _WIN32
		read_fpga_video_data(original->m_pImgU16);
		draw_img_from_buffer_separator(original->m_pImgU16, LEFT_TOP_SCREEN);		// 영상 4분할 출력 모드
#endif

		if (imageSaveMode) {
			fprintf(fp, "mission_07_YellowHoleUp_%07d", (int)image_num);
			sprintf(g_filename, "/mnt/f0/image/mission_07_YellowHoleUp_%07d.bmp", (int)image_num++);
			RGB565toRGB888(original, saveImage);
			SaveBitmap(g_filename, saveImage);
		}
		ret = YellowHoleUp2(fp, NULL);

#ifndef _WIN32
		draw_img_from_buffer_separator(view->m_pImgU16, RIGHT_TOP_SCREEN);		// 영상 4분할 출력 모드
#if PRINT_MODE
		draw_img_from_buffer_separator(print[0]->m_pImgU16, LEFT_BOTTOM_SCREEN);
		draw_img_from_buffer_separator(print[1]->m_pImgU16, RIGHT_BOTTOM_SCREEN);
#endif
		flip();
#endif // WIN32

		if (ret == YELLOW_HOLE_UP_END) {	// 너무 멀어
			break;
		}

		if (ret & YELLOW_HOLE_UP_RIGHT_MOTION) {
			DIR_MODE = RIGHT_MOTION;
		}
		else {
			DIR_MODE = LEFT_MOTION;
		}

		if (ret & YELLOW_HOLE_UP_TURN_10) {
			huro_motion(TURN_15 + DIR_MODE, SAFE_LITTLE);
		}
		else if (ret & YELLOW_HOLE_UP_SIDE_30) {
			huro_motion(SIDE_MOVE_30 + DIR_MODE, SAFE_LITTLE);
		}
		else if (ret & YELLOW_HOLE_UP_SIDE_50) {
			huro_motion(SIDE_MOVE_50 + DIR_MODE, SAFE_LITTLE);
		}
		else if (ret & YELLOW_HOLE_UP_RUN_20) {
			huro_motion(RUN_20, SAFE_LITTLE);
		}
		else if (ret & YELLOW_HOLE_UP_RUN_10) {
			huro_motion(WALK, SAFE_LITTLE);
		}
		else if (ret & YELLOW_HOLE_UP_WALK) {
			huro_motion(WALK, SAFE_LITTLE);
		}
		else if (ret & YELLOW_HOLE_UP_HALF_WALK) {
			huro_motion(HALF_WALK, SAFE_LITTLE);
		}
	}
	huro_motion(WALK, SAFE_LITTLE);
	huro_motion(HALF_WALK, SAFE_LITTLE);
	huro_motion(HALF_WALK, SAFE_LITTLE);
	huro_motion(STAIR_BELLBELL_WALK, SAFE_LOT);
	// 계단 올라가자
	huro_motion(STAIR_UP_2, SAFE_LOT);
	//huro_motion(WALK, SAFE_LITTLE);
	huro_motion(HALF_WALK, SAFE_LITTLE);
	huro_motion(HALF_WALK, SAFE_LITTLE);
	huro_motion(HALF_WALK, SAFE_LITTLE);


	//함정정렬
	if (imageSaveMode) {
		fclose(fp);
		fp = fopen("/mnt/f0/result/mission_07.csv", "at");
		fprintf(fp, "YellowHoleAlign\n");
	}

	while (1) {
		huro_motion(HEAD_DOWN_80, SAFE_NO);

#ifndef _WIN32
		read_fpga_video_data(original->m_pImgU16);
		draw_img_from_buffer_separator(original->m_pImgU16, LEFT_TOP_SCREEN);		// 영상 4분할 출력 모드
#endif
		if (imageSaveMode) {
			fprintf(fp, "mission_07_YellowHoleAlign_%07d", (int)image_num);
			sprintf(g_filename, "/mnt/f0/image/mission_07_YellowHoleAlign_%07d.bmp", (int)image_num++);
			RGB565toRGB888(original, saveImage);
			SaveBitmap(g_filename, saveImage);
		}
		ret = YellowHoleAlign(fp, NULL);
#ifndef _WIN32
		draw_img_from_buffer_separator(view->m_pImgU16, RIGHT_TOP_SCREEN);		// 영상 4분할 출력 모드
#if PRINT_MODE
		draw_img_from_buffer_separator(print[0]->m_pImgU16, LEFT_BOTTOM_SCREEN);
		draw_img_from_buffer_separator(print[1]->m_pImgU16, RIGHT_BOTTOM_SCREEN);
#endif
		flip();
#endif // WIN32

		if (ret == YELLOW_HOLE_ALIGN_END) {	// 너무 멀어
			break;
		}

		if (ret & YELLOW_HOLE_ALIGN_RIGHT_MOTION) {
			DIR_MODE = RIGHT_MOTION;
		} else {
			DIR_MODE = LEFT_MOTION;
		}

		if (ret & YELLOW_HOLE_ALIGN_TURN_10) {
			huro_motion(TURN_15 + DIR_MODE, SAFE_LITTLE);
		}
		else if (ret & YELLOW_HOLE_ALIGN_SIDE_30) {
			huro_motion(SIDE_MOVE_30 + DIR_MODE, SAFE_LITTLE);
		}
		else if (ret & YELLOW_HOLE_ALIGN_SIDE_10) {
			huro_motion(SIDE_MOVE_10 + DIR_MODE, SAFE_LITTLE);
		}
		else if (ret & YELLOW_HOLE_ALIGN_RUN_20) {
			huro_motion(RUN_20, SAFE_LITTLE);
		} else if (ret & YELLOW_HOLE_ALIGN_RUN_10) {
			huro_motion(RUN_10, SAFE_LITTLE);
		} else if (ret & YELLOW_HOLE_ALIGN_WALK) {
			huro_motion(WALK, SAFE_LITTLE);
		} else if (ret & YELLOW_HOLE_ALIGN_HALF_WALK) {
			huro_motion(HALF_WALK, SAFE_LITTLE);
		} else if (ret & YELLOW_HOLE_ALIGN_QUARTER_WALK) {
			huro_motion(QUARTER_WALK, SAFE_LITTLE);
		}
	}
	//huro_motion(HALF_WALK, SAFE_LITTLE);
	huro_motion(TRAP, SAFE_LOT);
	return 0;
}

static int mission_08(void)
{
	FILE *fp = NULL;
	int ret, is_open = 0;

	if (imageSaveMode) {
		fp = fopen("/mnt/f0/result/mission_08.csv", "wt");
		fprintf(fp, "BarricadeSide\n");
	}

	huro_motion(RUN_40, SAFE_LITTLE);		// 고개 아래 모션을 콜한다.
	mission_00("at", 10);
	huro_motion(RUN_40, SAFE_LITTLE);		// 고개 아래 모션을 콜한다.
	mission_00("at", 10);
	huro_motion(RUN_40, SAFE_LITTLE);		// 고개 아래 모션을 콜한다.
	mission_00("at", 10);

	 huro_motion(BASIC_OFF, SAFE_LITTLE);		// 고개 아래 모션을 콜한다.
	while (1) {
#ifndef _WIN32		
		read_fpga_video_data(original->m_pImgU16);
		draw_img_from_buffer_separator(original->m_pImgU16, LEFT_TOP_SCREEN);		// 영상 4분할 출력 모드
#endif // WIN32
		if (imageSaveMode) {
			fprintf(fp, "mission_08_BarricadeSide_%07d", (int)image_num);
			sprintf(g_filename, "/mnt/f0/image/mission_08_BarricadeSide_%07d.bmp", (int)image_num++);
			RGB565toRGB888(original, saveImage);
			SaveBitmap(g_filename, saveImage);
		}
		ret = BarricadeSide(fp, NULL);
		if (ret >= 3) {
			is_open = 1;
		}
#ifndef _WIN32
		draw_img_from_buffer_separator(noise->m_pImgU16, RIGHT_TOP_SCREEN);
		flip();
#endif // WIN32

		if (is_open && ret == 1) { // 바리게이트가 올라간 조건
								   // 다음 미션인 게이트가 존재해서 먼거리는 달릴수 없음.
			huro_motion(RUN_50, SAFE_LITTLE);
			huro_motion(RUN_50, SAFE_LITTLE);
			break;
		}
	}
	if (imageSaveMode)
		fclose(fp);

	return 0;
}

