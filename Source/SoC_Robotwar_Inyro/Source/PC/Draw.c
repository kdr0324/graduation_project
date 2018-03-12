#include "stdafx.h"

#include "Draw.h"

int DrawLine(IMG_INFO *src, POINT2D const p[2], COLORTABLE const table, const int thick)
{
	int xs, ys, xe, ye;
	int i, j, k;
	int index, index1;

	int channels = src->m_nChannels;
	int width = src->m_nWidth;
	int height = src->m_nHeight;
	int wStep = src->m_nWStep;
	int t = thick >> 1;

	double a;
	double invA;

	const int R = 2, G = 1, B = 0;

	int gray = (table.red + table.green + table.blue) / 3;

	if((src->m_nDepth != D_U8) || ((channels != 1) && (channels != 3))) {
		return 0;
	}
	
	if(p[0].y == p[1].y && p[0].y >= 0 && p[0].y < height) {
		if(p[0].x < p[1].x) {
			xs = p[0].x;
			xe = p[1].x;
		} else {
			xs = p[1].x;
			xe = p[0].x;
		}
		
		xs = MAX(xs, 0);
		xe = MIN(xe, width - 1);
		index = (p[0].y - t - 1) * wStep;
		for(k = -t; k <= t; k++) {
			index += wStep;
			if(p[0].y + k < 0 || p[0].y + k >= height)
				continue;
			
			index1 = index + xs * channels;
			for(j = xs; j <= xe; ++j) {			
				if(channels == 1) {
					src->m_pImgU8[index1] = gray;
				} else {
					src->m_pImgU8[index1 + R] = table.red;
					src->m_pImgU8[index1 + G] = table.green;
					src->m_pImgU8[index1 + B] = table.blue;
				}
				index1 += channels;
			}			
		}

		return 1;
	}

	if(p[0].x == p[1].x && p[0].x >= 0 && p[0].x < width) {
		if(p[0].y < p[1].y) {
			ys = p[0].y;
			ye = p[1].y;
		} else {
			ys = p[1].y;
			ye = p[0].y;
		}

		ys = MAX(ys, 0);
		ye = MIN(ye, height - 1);
		index = ys * wStep;
		for(i = ys; i <= ye; ++i) {
			index1 = index + (p[0].x - t - 1) * channels;

			for(k = -t; k <= t; ++k) {
				index1 += channels;
				if(p[0].x + k < 0 || p[0].x + k >= width)
					continue;

				if(channels == 1) {
					src->m_pImgU8[index1] = gray;
				} else {					
					src->m_pImgU8[index1 + R] = table.red;
					src->m_pImgU8[index1 + G] = table.green;
					src->m_pImgU8[index1 + B] = table.blue;
				}
				
			}
			index += wStep;
		}

		return 1;
	}	

	a = (double)(p[0].y - p[1].y) / (p[0].x - p[1].x);

	if((a > -1) && (a < 1)) {							// 인접한 'x'의 값에 대해 같은 'y'값이 존재
		if(p[0].x < p[1].x) {
			xs = p[0].x, xe = p[1].x, ys = p[0].y, ye = p[1].y;
		} else {
			xs = p[1].x, xe = p[0].x, ys = p[1].y, ye = p[0].y;
		}

		for(j = xs; j <= xe; ++j) {
			i = (int)((a * (j - xs) + 0.5));			// 'x'증가에 따른 'y'변화량 계산
			i += ys;									// 변화량에 'y'의 초기값을 더해줌

			for(k = -t; k <= t; ++k) {					// 두께만큼 반복
				if(i + k < 0 || i + k >= height)
					continue;
				index1 = (i + k) * wStep + j * channels;
				if(channels == 1) {
					src->m_pImgU8[index1] = gray;
				} else {
					src->m_pImgU8[index1 + R] = table.red;
					src->m_pImgU8[index1 + G] = table.green;
					src->m_pImgU8[index1 + B] = table.blue;
				}
			}
		}
	} else {											// 인접한 'y'의 값에 대해 같은 'x'값이 존재	
		invA = 1.0 / a;
		if(p[0].y < p[1].y) {
			xs = p[0].x, xe = p[1].x, ys = p[0].y, ye = p[1].y;
		} else {
			xs = p[1].x, xe = p[0].x, ys = p[1].y, ye = p[0].y;
		}

		index = ys * wStep;
		for(i = ys; i <= ye; ++i) {
			j = (int)(invA * (i - ys) + 0.5);	// 'y'증가에 따른 'x'변화량 계산
			j += xs;							// 변화량에 'x'의 초기값을 더해줌

			index1 = index + (j - t - 1) * channels;
			for(k = -t; k <= t; ++k) {
				index1 += channels;
				if((j + k < 0) || (j + k >= width)) {
					continue;
				}
				if(channels == 1) {
					src->m_pImgU8[index1] = gray;
				} else {
					src->m_pImgU8[index1 + R] = table.red;
					src->m_pImgU8[index1 + G] = table.green;
					src->m_pImgU8[index1 + B] = table.blue;
				}
				
			}
			index += wStep;
		}
	}

	return 1;
}

int DrawRect(IMG_INFO *src, RECT2D const r, COLORTABLE const table, const int thick)
{
	int t = thick >> 1;
	int x1 = r.startX, x2 = r.startX + r.width;
	int y1 = r.startY, y2 = r.startY + r.height;

	//Taekwon 용 사각형 무조건 그려지게 수정
	if (x1 <= t) x1 = t+1;
	if (y1 <= t) y1 = t+1;
	if (x2 >= src->m_nWidth-t) x2 = src->m_nWidth - t-1;
	if (y2 >= src->m_nHeight-t) y2 = src->m_nHeight - t-1;
	//
	
	POINT2D left[2] = {{x1 + t, y1}, {x1 + t, y2}};
	POINT2D right[2] = {{x2 - t, y1}, {x2 - t, y2}};
	POINT2D bottom[2] = {{x1, y1 + t}, {x2, y1 + t}};
	POINT2D top[2] = {{x1, y2 - t}, {x2, y2 - t}};

	

	DrawLine(src, left, table, thick);
	DrawLine(src, right, table, thick);
	DrawLine(src, bottom, table, thick);
	DrawLine(src, top, table, thick);

	return 1;
}