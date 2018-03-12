#include "stdafx.h"

#include "Line.h"

#include "Config.h"
#include "LUT.h"

// src: 16bits image
U16 **hough_counts;
HOUGH_LINE_INFO *hough_lines;

int HoughLineDectector(IMG_INFO *src, const U16 FORWARD)
{
	int nWidth = src->m_nWidth;
	int nHeight = src->m_nHeight;
	int nWidth_1 = nWidth - 1;
	int nHeight_1 = nHeight - 1;
	int index, i, j;
	int d, th;
	U16 *pSrc;

	for (th = MAX_THETA_HOUGH - 1; th >= 0; --th)
		memset(hough_counts[th], 0, sizeof(U16) * MAX_DISTANCE_HOUGH * 2);

	for (i = nHeight_1; i >= 0; --i) {
		index = i * nWidth;
		pSrc = &src->m_pImgU16[index];
		for (j = nWidth_1; j >= 0; --j) {
			if (pSrc[j] == FORWARD) {
				for (th = 179; th >= 0; --th) {
					d = (j * sin_lookup[th] - i * cos_lookup[th]) + 512;
					hough_counts[th][DSQ_10(d) + MAX_DISTANCE_HOUGH]++;
				}
			}
		}
	}

	return 1;
}

int LineDectector(POINT2D *pts, const int point_num, const U16 FORWARD)
{
	int i, x, y;
	int d, th;

	for (th = MAX_THETA_HOUGH - 1; th >= 0; --th)
		memset(hough_counts[th], 0, sizeof(U16) * MAX_DISTANCE_HOUGH * 2);

	for (i = point_num - 1; i >= 0; --i) {
		y = pts[i].y;
		x = pts[i].x;

		for (th = MAX_THETA_HOUGH - 1; th >= 0; --th) {
			d = (x * sin_lookup[th] - y * cos_lookup[th]);
			hough_counts[th][DSQ_10(d) + MAX_DISTANCE_HOUGH]++;
		}
	}

	return 1;
}

int hough_line_sort(const void *a, const void *b)
{
	int one = ((HOUGH_LINE_INFO *)a)->hough_counts;
	int two = ((HOUGH_LINE_INFO *)b)->hough_counts;

	if (one < two) {
		return 1;
	} else {
		return -1;
	}
}
