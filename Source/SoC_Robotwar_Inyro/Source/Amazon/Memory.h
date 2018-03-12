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

// 공용 메모리
extern IMG_INFO *original; // rgb565 이미지 저장
extern IMG_INFO *crop_original; // rgb565 이미지 저장
extern IMG_INFO *hImg, *sImg, *iImg; // H, S, I
extern IMG_INFO *binary; // 이진영상
extern IMG_INFO *edgeImage; // 8bits
extern IMG_INFO *noise, *dilation;	// 노이즈제거, 팽창
extern IMG_INFO *imgLabel; // 라벨링 결과 저장
extern IMG_INFO *integral;
extern IMG_INFO *integral_32;
extern IMG_INFO *view;	// LCD에 출력하기 위한 용도
#if PRINT_MODE
extern IMG_INFO *print[2]; // LCD에 숫자를 출력하기 위한 용도
#endif
extern IMG_INFO *g_rgb888; // rgb888 이미지 저장
extern IMG_INFO *saveImage; // 24bits 컬러 이미지로 저장하기 위한 용도, PC에서만 사용할 예정

 // 휴로 전용 메모리
extern IMG_INFO *ballDetect;
extern IMG_INFO *holeDetect;
extern OBJECT *holeLabels;

// 태권 전용 메모리
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
