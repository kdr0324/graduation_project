#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "Config.h"
#include "LUT.h"
#include "Line.h"
#include "ImagePixel.h"
#include "Labeling.h"

extern HSIVALUE vHSI;
extern HSVALUE vHS;
extern HIVALUE vHI;
extern SIVALUE vSI;
extern HVALUE vH;
extern SVALUE vS;
extern IVALUE vI;

// ���� �޸�
extern IMG_INFO *original; // rgb565 �̹��� ����
extern IMG_INFO *crop_original; // rgb565 �̹��� ����
extern IMG_INFO *hImg, *sImg, *iImg; // H, S, I
extern IMG_INFO *binary; // ��������
extern IMG_INFO *edgeImage; // 8bits
extern IMG_INFO *noise, *dilation;	// ����������, ��â
extern IMG_INFO *imgLabel; // �󺧸� ��� ����
extern IMG_INFO *integral;
extern IMG_INFO *integral_32;
extern IMG_INFO *view;	// LCD�� ����ϱ� ���� �뵵
#if PRINT_MODE
extern IMG_INFO *print[2]; // LCD�� ���ڸ� ����ϱ� ���� �뵵
#endif
extern IMG_INFO *g_rgb888; // rgb888 �̹��� ����
extern IMG_INFO *saveImage; // 24bits �÷� �̹����� �����ϱ� ���� �뵵, PC������ ����� ����

 // �޷� ���� �޸�
extern IMG_INFO *ballDetect;
extern IMG_INFO *holeDetect;
extern OBJECT *holeLabels;

// �±� ���� �޸�
extern IMG_INFO *blocks[2];

extern OBJECT *g_object;

extern char g_filename[2048];

#ifdef __cplusplus
extern "C" {
#endif
	void CommonMemoryAllocate(void);
	void HuroMemoryAllocate(void);
	void TaekwonMemoryAllocate(void);
	void CommonMemoryRelease(void);
	void HuroMemoryRelease(void);
	void TaekwonMemoryRelease(void);
	void CalcLinear(const POINT2D *p, int *a0, int *a1, const int cntPoint);
	int line_sort(const void *a, const void *b);
	int hough_line_sort(const void *a, const void *b);

#ifdef __cplusplus
}
#endif

#endif // _MEMORY_H_
