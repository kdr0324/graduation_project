#include "stdafx.h"

#include "Hurdle.h"

#ifdef _WIN32
#include "../Common/Bitmap.h"
#endif // WIN32

#include "../Amazon/Memory.h"
#include "../Amazon/ImageConvert.h"
#include "../Amazon/Draw.h"
#include "HuroMissonConfig.h"

int Hurdle(FILE *fp, const char *name)
{
	int i, ret = 0,j;
	int labelNum;
	int index = 0;
	int center = 0;
	U16 *pSrc;
	const int FORWARD = BLUE_16;

	// 고개 각도에 따라 THRESHOLD 변환
	const int THRESHOLD = 300;

	// 세팅 값 설정
	vHS.h.max = BLUE_H_MAX;
	vHS.h.min = BLUE_H_MIN;
	vHS.s.max = 255;
	vHS.s.min = BLUE_S_MIN;

	center = 0;

	// I 변환
	RGB565toHS(original, hImg, sImg);			// 16bits, 8bits 영상 버퍼
	binary_HS(&vHS, binary, FORWARD);
	medianBinary(binary, noise, FORWARD);
	dilationBinary(noise, binary, FORWARD);

	for (i = 0; i < AMAZON_HEIGHT; ++i) {
		index = i * AMAZON_WIDTH;
		pSrc = &binary->m_pImgU16[index];
		for (j = 30; j >= 0; --j) {
			pSrc[j] = BLACK_16;
		}
		for (j = AMAZON_WIDTH-1; j >= AMAZON_WIDTH - 30; --j) {
			pSrc[j] = BLACK_16;
		}
	}

#ifdef _WIN32
	RGB565toRGB888(binary, saveImage);
	sprintf(g_filename, "%s_02_result.bmp", name);
	SaveBitmap(g_filename, saveImage);
#endif

	labelNum = CallLabel(binary, imgLabel, FORWARD, g_object, THRESHOLD, 150);

	if(labelNum != 1) {
		for(i = 1; i < labelNum; ++i) {
			center += g_object[i].center.x;
			DrawCrossLine(binary, g_object[i].center.x, g_object[i].center.y, 3, YELLOW_16);
		}
		center /= labelNum - 1;

		if(center) {
			DrawCrossLine(binary, center, 60, 3, MAGENTA_16);
		}
		if (fp != NULL)
			fprintf(fp, ",%d,%d", center, labelNum - 1);
		ret = HURDLE_BELL_BELL;
	} else {
		if (fp != NULL)
			fprintf(fp, ",X,X");
		ret = HURDLE_END;
	}

#ifdef _WIN32
	RGB565toRGB888(binary, saveImage);
	sprintf(g_filename, "%s_01_result.bmp", name);
	SaveBitmap(g_filename, saveImage);
#endif

	if (fp != NULL)
		fprintf(fp, ",0x%04X\n", ret);
	return ret;
}
