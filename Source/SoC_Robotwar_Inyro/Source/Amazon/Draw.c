#include "stdafx.h"

#include "Draw.h"
#include "LUT.h"
#include "Memory.h"
#include "Number.h"

int DrawCrossLine(IMG_INFO *buf, const int x, const int y, const int thick, const U16 COLOR)
{
	int i, j;
	int t = thick >> 1;
	int thick_1 = thick - 1;
	int nWidth = buf->m_nWidth;
	int nHeight = buf->m_nHeight;
	int nWStep = buf->m_nWStep;
	U16 *pBuf;
	int index;
	int nX, nY;

	nX = x - t;
	nY = y - 5;
	index = (y + 5) * nWStep + nX;
	for(i = 10; i >= 0; --i) {
		if(nY + i < nHeight && nY + i >= 0) {
			pBuf = &buf->m_pImgU16[index];			//출력 영상의 이미지에 접근할 때 이렇게 사용한다.
			for(j = thick_1; j >= 0; --j) {
				if(nX + j < nWidth && nX + j >= 0) {
					pBuf[j] = COLOR;				//왜 10개씩??
				}
			}
		}
		index -= nWStep;
	}

	nX = x - 5;
	nY = y - t;
	index = (y + t) * nWStep + nX;
	for(i = thick_1; i >= 0; --i) {
		if(nY + i < nHeight && nY + i >= 0) {
			pBuf = &buf->m_pImgU16[index];
			for(j = 10; j >= 0; --j) {
				if(nX + j < nWidth && nX + j >= 0) {
					pBuf[j] = COLOR;
				}
			}
		}
		index -= nWStep;
	}

	return 1;
}

//size 는 짝수
int DrawCrossLine_T(IMG_INFO *buf, const int x, const int y, const int size, const int thick, const U16 COLOR)
{
	int i, j;
	int t = thick >> 1;
	int thick_1 = thick - 1;
	int nWidth = buf->m_nWidth;
	int nHeight = buf->m_nHeight;
	int nWStep = buf->m_nWStep;
	int half_size = size >> 1;
	U16 *pBuf;
	int index;
	int nX, nY;

	nX = x - t;
	nY = y - half_size;
	index = (y + half_size) * nWStep + nX;
	for (i = size; i >= 0; --i) {
		if (nY + i < nHeight && nY + i >= 0) {
			pBuf = &buf->m_pImgU16[index];			//출력 영상의 이미지에 접근할 때 이렇게 사용한다.
			for (j = thick; j >= 0; --j) {
				if (nX + j < nWidth && nX + j >= 0) {
					pBuf[j] = COLOR;				//왜 size개씩??
				}
			}
		}
		index -= nWStep;
	}

	nX = x - half_size;
	nY = y - t;
	index = (y + t) * nWStep + nX;
	for (i = thick; i >= 0; --i) {
		if (nY + i < nHeight && nY + i >= 0) {
			pBuf = &buf->m_pImgU16[index];
			for (j = size; j >= 0; --j) {
				if (nX + j < nWidth && nX + j >= 0) {
					pBuf[j] = COLOR;
				}
			}
		}
		index -= nWStep;
	}

	return 1;
}

int DrawLine(IMG_INFO *src, POINT2D const p[2], const U16 COLOR)
{
	int xs, ys, xe, ye;
	int i, j, k;
	int index, index1;

	int width = src->m_nWidth;
	int height = src->m_nHeight;
	int nWStep = src->m_nWStep;
	int t = 1;

	U16 *pSrc = src->m_pImgU16;

	int a, invA;

	if(p[0].y == p[1].y) {
		if(p[0].x < p[1].x) {
			xs = p[0].x;
			xe = p[1].x;
		} else {
			xs = p[1].x;
			xe = p[0].x;
		}

		if(xs < 0)
			xs = 0;
		if(xe >= width)
			xe = width - 1;

		index = (p[0].y - t) * nWStep;
		for(k = -t; k <= t; k++) {			
			index1 = index + xs;
			index += nWStep;
			if(p[0].y + k < 0 || p[0].y + k >= height) {
				continue;
			}
			for(j = xs; j <= xe; ++j) {			
				pSrc[index1] = COLOR;
				++index1;
			}

		}

		return 1;
	}

	if(p[0].x == p[1].x) {
		if(p[0].y < p[1].y) {
			ys = p[0].y;
			ye = p[1].y;
		} else {
			ys = p[1].y;
			ye = p[0].y;
		}

		if(ys < 0)
			ys = 0;
		if(ye >= height)
			ye = height - 1;

		index = ys * nWStep;
		for(i = ys; i <= ye; ++i) {
			index1 = index + (p[0].x - t);
			index += nWStep;

			for(k = -t; k <= t; ++k) {
				if(p[0].x + k < 0 || p[0].x >= width) {
					index1 += 1;
					continue;
				}
				pSrc[index1] = COLOR;
				++index1;
			}

		}

		return 1;
	}	

	a = (SQ_10(p[0].y - p[1].y)) / (p[0].x - p[1].x);	// SHIFT 10 bits -> double 삭제

	// SHIFT 연산을 이용했기 때문에 기울기는 1024배 증가
	if((a > -1024) && (a < 1024)) {							// 인접한 'x'의 값에 대해 같은 'y'값이 존재
		if(p[0].x < p[1].x) {
			xs = p[0].x, xe = p[1].x, ys = p[0].y, ye = p[1].y;
		} else {
			xs = p[1].x, xe = p[0].x, ys = p[1].y, ye = p[0].y;
		}

		if(xs < 0) {
			ys -= DSQ_10(a * xs - 512);		// (a * xs - 0.5)을 SHIFT 연산에 맞게 변환
			xs = 0;

		}
		if(xe >= width)
			xe = width - 1;


		for(j = xs; j <= xe; ++j) {
			// a * (j - xs) + 0.5
			i = DSQ_10((a * (j - xs) + 512));		// 'x'증가에 따른 'y'변화량 계산
			i += ys;								// 변화량에 'y'의 초기값을 더해줌

			for(k = -t; k <= t; ++k) {					// 두께만큼 반복
				if((i + k < 0) || (i + k >= height)) {
					continue;
				}

				index1 = (i + k) * nWStep + j;
				pSrc[index1] = COLOR;
			}
		}
	} else {											// 인접한 'y'의 값에 대해 같은 'x'값이 존재	
		invA = SQ_20(1) / a;				// a가 이미 SQ_10을 진행한 상태
		if(p[0].y < p[1].y) {
			xs = p[0].x, xe = p[1].x, ys = p[0].y, ye = p[1].y;
		} else {
			xs = p[1].x, xe = p[0].x, ys = p[1].y, ye = p[0].y;
		}

		if(ys < 0) {
			xs -= DSQ_10(invA * ys - 512);				// invA * ys - 0.5 변환
			ys = 0;
		}
		if(ye >= height)
			ye = height - 1;

		index = ys * nWStep;
		for(i = ys; i <= ye; ++i) {
			// invA * (i - ys) + 0.5
			j = DSQ_10(invA * (i - ys) + 512);	// 'y'증가에 따른 'x'변화량 계산
			j += xs;							// 변화량에 'x'의 초기값을 더해줌

			index1 = index + (j - t);
			for(k = -t; k <= t; ++k) {
				if((j + k < 0) || (j + k >= width)) {
					++index1;
					continue;
				}

				pSrc[index1] = COLOR;
				++index1;
			}
			index += nWStep;
		}
	}

	return 1;
}

int DrawLineWithThetaNDistance(IMG_INFO *src, int theta, int distance, const U16 COLOR)
{
	POINT2D p[2];

	// y = _tan[theta] * x + d * sec[theta]
	if (theta == 90) {
		p[0].x = distance, p[1].x = distance;
		p[0].y = 0, p[1].y = src->m_nHeight;
	} else {
		p[0].x = 0, p[1].x = src->m_nWidth;
		p[0].y = SQ_10(-distance) / cos_lookup[theta];
		p[1].y = p[0].y + (src->m_nWidth * sin_lookup[theta] / cos_lookup[theta]);
	}

	return DrawLine(src, p, COLOR);
}

#if PRINT_MODE
void printResult(int num, const int line, const int viewNum)
{
	int i, j, k;
	int digit;
	int index, index2;
	int nWidth = print[viewNum]->m_nWidth;
	U16 *pView, *pNum;

	pView = &print[viewNum]->m_pImgU16[(line * 60 + 1) * AMAZON_WIDTH];
	index = 1;
	if (num < 0) {
		pNum = &number[16240];
		index2 = 0;
		for (i = 0; i < 58; ++i) {
			for (j = 0; j < 28; ++j) {
				pView[index + j] = pNum[index2++];
			}
			index += AMAZON_WIDTH;
		}
		num = -num;
	}

	for (k = 4; k >= 0; --k) {
		digit = num % 10;
		num /= 10;
		index = k * 30 + 31;
		pNum = &number[digit * 1624];
		index2 = 0;
		for (i = 0; i < 58; ++i) {
			for (j = 0; j < 28; ++j) {
				pView[index + j] = pNum[index2++];
			}
			index += AMAZON_WIDTH;
		}
	}
}
#endif