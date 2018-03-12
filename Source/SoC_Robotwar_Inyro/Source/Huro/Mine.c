/**
 @title : Mine.c
 @author : 임현 (010 - 9019 - 3799)
 @brief : 지뢰 미션 코드 파일
*/

/*
	------------------------ 용어 정리 -------------------------
	- RGB565 : R 5비트, G 5비트, B 5비트
	- RGB888 : R 8비트, G 8비트, B 8비트
	- g_object : 라벨링해서 제일 크게 나온 값
	------------------------------------------------------------
*/

// 미리 컴파일된 헤더 파일
#include "stdafx.h" // 정확한 개념을 이해하지 못 하겠음 (모든 cpp에서 시작할 때 쓴다함)

// Mine.c의 헤더 파일
#include "Mine.h"

// 윈도우 환경에 Bitmap 파일을 열고 저장하기 위한 헤더 파일
#ifdef _WIN32
#include "../Common/Bitmap.h"
#endif // WIN32

#include "../Amazon/Memory.h"
#include "../Amazon/ImageConvert.h"
#include "../Amazon/Draw.h"
#include "HuroMissonConfig.h"

int MineDetect(FILE *fp, const char *name)
{
	int ret = 0;

	int i, label_cnt, cx, cy;

	const U16 FORWARD = YELLOW_16;
	const int IGNORE_X_MIN = 50;
	const int IGNORE_X_MAX = 130;
	const int IGNORE_Y_MIN = 2;
	const int IGNORE_Y_MAX = 78;
	const int MINIMUM_SIZE = 20;

#if PRINT_MODE
	memset(print[0]->m_pImgU16, 0xFFFF, sizeof(U16)* AMAZON_WIDTH * AMAZON_HEIGHT);
	memset(print[1]->m_pImgU16, 0xFFFF, sizeof(U16)* AMAZON_WIDTH * AMAZON_HEIGHT);
#endif

	CropImage(original, crop_original, IGNORE_X_MIN, IGNORE_Y_MIN, IGNORE_X_MAX, IGNORE_Y_MAX);
	RGB565toSI(crop_original, sImg, iImg);
	vSI.s.max = 60; vSI.s.min = 0;// 60
	vSI.i.max = 80; vSI.i.min = 0;// 80
	binary_SI(&vSI, binary, FORWARD);
	dilationBinary(binary, dilation, FORWARD);
	label_cnt = CallLabel(dilation, imgLabel, FORWARD, g_object, MINIMUM_SIZE, 10);
	if (label_cnt >= 2) {
		cx = g_object[1].center.x;
		cy = g_object[1].center.y;
		DrawCrossLine(dilation, cx, cy, 3, BLUE_16);
	} else {
		cx = cy = 0;
	}

#if PRINT_MODE
	printResult(cx, 0, 0);
	printResult(cy, 1, 0);
#endif

	CopyImageForView(dilation, original, IGNORE_X_MIN, IGNORE_Y_MIN);

#ifdef _WIN32
	RGB565toRGB888(original, saveImage);
	sprintf(g_filename, "%s_00_original.bmp", name);
	SaveBitmap(g_filename, saveImage);

	RGB565toRGB888(crop_original, saveImage);
	sprintf(g_filename, "%s_01_cropped.bmp", name);
	SaveBitmap(g_filename, saveImage);

	RGB565toRGB888(binary, saveImage);
	sprintf(g_filename, "%s_02_binary.bmp", name);
	SaveBitmap(g_filename, saveImage);

	RGB565toRGB888(dilation, saveImage);
	sprintf(g_filename, "%s_03_dilation.bmp", name);
	SaveBitmap(g_filename, saveImage);
#endif

	if (cy < 5 || g_object[1].width > 30) {
		ret = MINE_DETECT_IS_NOT_MINE;
	} else if (cy < 25) {
		ret = MINE_DETECT_WALK;
	} else if (cy < 45) {
		ret = MINE_DETECT_HALF_WALK;
	} else if (cy < 58) {
		ret = MINE_DETECT_QUARTER_WALK;
	} else {
		if (cx < 10)
			ret = MINE_DETECT_LEFT_MOTION | MINE_DETECT_SIDE_MOVE_70;
		else if(cx < 25)
			ret = MINE_DETECT_LEFT_MOTION | MINE_DETECT_SIDE_MOVE_30;
		else if (cx < 38)
			ret = MINE_DETECT_LEFT_MOTION | MINE_DETECT_SIDE_MOVE_10;
		else if (cx > 70)
			ret = MINE_DETECT_SIDE_MOVE_70;
		else if (cx > 55)
			ret =  MINE_DETECT_SIDE_MOVE_30;
		else if (cx > 42)
			ret = MINE_DETECT_SIDE_MOVE_10;
		else
			ret = MINE_DETECT_MINE_RUN;
	}

	return ret; // 지뢰 찾기 결과를 반환
}
