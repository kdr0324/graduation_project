#ifndef _DRAW_H_
#define _DRAW_H_

#include "Config.h"
#include "../Common/ImageManagement.h"
#include "../Common/Point.h"
#include "LUT.h"

// 두께는 1, 3, 5만 사용
int DrawCrossLine(IMG_INFO *buf, const int x, const int y, const int thick, const U16 COLOR);
int DrawCrossLine_T(IMG_INFO *buf, const int x, const int y, const int size, const int thick, const U16 COLOR);
int DrawLine(IMG_INFO *src, POINT2D const p[2], const U16 COLOR);
int DrawLineWithThetaNDistance(IMG_INFO *src, int theta, int distance, const U16 COLOR);
void printResult(int num, const int line, const int viewNum);

#endif // _DRAW_H_
