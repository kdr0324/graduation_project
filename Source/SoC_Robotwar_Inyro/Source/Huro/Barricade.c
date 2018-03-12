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
	// ���� ����
	int i;
	int labelNum; // �ִ� �� ��ȣ + 1

	// �� ������ ���� THRESHOLD ��ȯ
	const int THRESHOLD = 200;

	// ���� �� ����
	vHS.h.max = YELLOW_H_MAX;
	vHS.h.min = YELLOW_H_MIN;
	vHS.s.max = 255;
	vHS.s.min = YELLOW_S_MIN;

	RGB565toHS(original, hImg, sImg);			// RGB565�� �̿��ؼ� H, S �� ����
	binary_HS(&vHS, binary, YELLOW_16);			// H, S�� �̿��ؼ� ����ȭ ����
	medianBinary(binary, noise, YELLOW_16);		// Binary Median Filter�� �̿��Ͽ� ������ ����

	labelNum = CallLabel(noise, imgLabel, YELLOW_16, g_object, THRESHOLD, 255);		// �� ���� + 1�� ����

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

	// ���� 2�� �̻��̴�.
	if (labelNum >= 2) {		
		// �󺧿��� �߽ɿ� ���ڰ��� ǥ��
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
	// ���� ����
	int i;
	int labelNum; // �ִ� �� ��ȣ + 1

	// �� ������ ���� THRESHOLD ��ȯ
	const int THRESHOLD = 150;

	// ���� �� ����
	vHS.h.max = YELLOW_H_MAX;
	vHS.h.min = YELLOW_H_MIN;
	vHS.s.max = 255;
	vHS.s.min = YELLOW_S_MIN;

	RGB565toHS(original, hImg, sImg);			// RGB565�� �̿��ؼ� H, S �� ����
	binary_HS(&vHS, binary, YELLOW_16);			// H, S�� �̿��ؼ� ����ȭ ����
	medianBinary(binary, noise, YELLOW_16);		// Binary Median Filter�� �̿��Ͽ� ������ ����

	labelNum = CallLabel(noise, imgLabel, YELLOW_16, g_object, THRESHOLD, 255);		// �� ���� + 1�� ����

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

	// ���� 2�� �̻��̴�.
	if (labelNum >= 3) {
		// �󺧿��� �߽ɿ� ���ڰ��� ǥ��
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

