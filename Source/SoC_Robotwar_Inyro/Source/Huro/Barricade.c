#include "stdafx.h"

#include "Barricade.h"

#ifdef _WIN32
#include "../Common/Bitmap.h"
#endif // WIN32

#include "../Amazon/Memory.h"
#include "../Amazon/ImageConvert.h"
#include "../Amazon/Draw.h"
#include "HuroMissonConfig.h"

int BarricadeUp(FILE *fp, const char *name)
{
	// 변수 선언
	int i;
	int labelNum; // 최대 라벨 번호 + 1

	// 고개 각도에 따라 THRESHOLD 변환
	const int THRESHOLD = 200;

	// 세팅 값 설정
	vHS.h.max = YELLOW_H_MAX;
	vHS.h.min = YELLOW_H_MIN;
	vHS.s.max = 255;
	vHS.s.min = YELLOW_S_MIN;

	RGB565toHS(original, hImg, sImg);			// RGB565를 이용해서 H, S 값 추출
	binary_HS(&vHS, binary, YELLOW_16);			// H, S를 이용해서 이진화 진행
	medianBinary(binary, noise, YELLOW_16);		// Binary Median Filter를 이용하여 노이즈 제거

	labelNum = CallLabel(noise, imgLabel, YELLOW_16, g_object, THRESHOLD, 255);		// 라벨 갯수 + 1을 리턴

	if (fp != NULL)
		fprintf(fp, ",%d", labelNum - 1);

#ifdef _WIN32
	RGB565toRGB888(binary, saveImage);
	sprintf(g_filename, "%s_01_binary.bmp", name);
	SaveBitmap(g_filename, saveImage);
	RGB565toRGB888(noise, saveImage);
	sprintf(g_filename, "%s_02_noise.bmp", name);
	SaveBitmap(g_filename, saveImage);
#endif

	// 라벨이 2개 이상이다.
	if (labelNum >= 2) {		
		// 라벨영역 중심에 십자가를 표시
		for (i = labelNum - 1; i >= 1; --i) {
			DrawCrossLine(noise, g_object[i].center.x, g_object[i].center.y, 3, RED_16);
		}

#ifdef _WIN32
		RGB565toRGB888(noise, saveImage);
		sprintf(g_filename, "%s_03_result.bmp", name);
		SaveBitmap(g_filename, saveImage);
#endif
	}

	if (fp != NULL)
		fprintf(fp, ",0x%0x02X\n", labelNum);

	return labelNum;	
}

int BarricadeSide(FILE *fp, const char *name)
{
	// 변수 선언
	int i;
	int labelNum; // 최대 라벨 번호 + 1

	// 고개 각도에 따라 THRESHOLD 변환
	const int THRESHOLD = 150;

	// 세팅 값 설정
	vHS.h.max = YELLOW_H_MAX;
	vHS.h.min = YELLOW_H_MIN;
	vHS.s.max = 255;
	vHS.s.min = YELLOW_S_MIN;

	RGB565toHS(original, hImg, sImg);			// RGB565를 이용해서 H, S 값 추출
	binary_HS(&vHS, binary, YELLOW_16);			// H, S를 이용해서 이진화 진행
	medianBinary(binary, noise, YELLOW_16);		// Binary Median Filter를 이용하여 노이즈 제거

	labelNum = CallLabel(noise, imgLabel, YELLOW_16, g_object, THRESHOLD, 255);		// 라벨 갯수 + 1을 리턴

	if (fp != NULL)
		fprintf(fp, ",%d", labelNum - 1);

#ifdef _WIN32
	RGB565toRGB888(binary, saveImage);
	sprintf(g_filename, "%s_01_binary.bmp", name);
	SaveBitmap(g_filename, saveImage);
	RGB565toRGB888(noise, saveImage);
	sprintf(g_filename, "%s_02_noise.bmp", name);
	SaveBitmap(g_filename, saveImage);
#endif

	// 라벨이 2개 이상이다.
	if (labelNum >= 3) {
		// 라벨영역 중심에 십자가를 표시
		for (i = labelNum - 1; i >= 1; --i) {
			DrawCrossLine(noise, g_object[i].center.x, g_object[i].center.y, 3, RED_16);
		}

#ifdef _WIN32
		RGB565toRGB888(noise, saveImage);
		sprintf(g_filename, "%s_03_result.bmp", name);
		SaveBitmap(g_filename, saveImage);
#endif
	}

	if (fp != NULL)
		fprintf(fp, ",0x%0x02X\n", labelNum);

	return labelNum;
}

