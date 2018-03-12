#ifndef _LINE_H_
#define _LINE_H_

#include "../Common/ImageManagement.h"
#include "../Common/Point.h"

#define MAX_THETA_HOUGH 180
#define MAX_DISTANCE_HOUGH 220

typedef struct tagHoughLineInfo {
	short theta;
	short distance;	
	short hough_counts;
} HOUGH_LINE_INFO;

extern U16 **hough_counts;
extern HOUGH_LINE_INFO *hough_lines;

#ifdef __cplusplus
extern "C" {
#endif
	
	int HoughLineDectector(IMG_INFO *src, const U16 FORWARD);
	int LineDectector(POINT2D *pts, const int point_num, const U16 FORWARD);
	int hough_line_sort(const void *a, const void *b);

#ifdef __cplusplus
}
#endif

#endif // _LINE_H_
