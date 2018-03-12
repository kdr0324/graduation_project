#include "stdafx.h"

#include "Tracking.h"
#include "../PC/Color.h"
#include "../PC/Draw.h"
#include "../PC/Labeling.h"

//static int ObjectExtract()

int TrackingMain(IMG_INFO *src, void *buf)
{
	TRACKINGINFO *st_buf = (TRACKINGINFO *)buf;

	IMG_INFO *HSI = ConvertColorFormat(src, BGR2HSI);
	IMG_INFO *hue = ExtractChannel(HSI, 2);
	IMG_INFO *saturation = ExtractChannel(HSI, 1);
	IMG_INFO *intensity = ExtractChannel(HSI, 0);

	OBJECT object[4][6];
	//OBJECT new_window[4];
	COLORTABLE ct[4] = { COLORTABLE(255, 0, 0), COLORTABLE(0, 255, 0),COLORTABLE(0, 0, 255),COLORTABLE(255,255,0) };
	int is_tracking_bit[4] = { 0x1, 0x2, 0x4, 0x8 };
	/*st_buf->m_n_tracking_cnt = 4;
	st_buf->m_n_result_img_cnt = 4;*/

#if defined(_OPENMP)
#pragma omp parallel for
#endif	
	for (int i = 0; i < st_buf->m_n_tracking_cnt; i++) {
		st_buf->m_st_hsi[i].hImg = hue;
		st_buf->m_st_hsi[i].sImg = saturation;
		st_buf->m_st_hsi[i].iImg = intensity;
	}


#if defined(_OPENMP)
#pragma omp parallel for
#endif	
	for (int i = 0; i < st_buf->m_n_tracking_cnt; i++) {
		IMG_INFO *binary = Binary_HSI(&st_buf->m_st_hsi[i]);
		IMG_INFO *result;
		int a = st_buf->m_n_is_tracking & is_tracking_bit[i];
		
		//Tracking
		if (a == is_tracking_bit[i]) {
			result = CAMShift(&st_buf->m_st_hsi[i], binary, &(st_buf->m_st_roi[i]));

			if (!result) {
				st_buf->m_n_is_tracking = st_buf->m_n_is_tracking ^ is_tracking_bit[i];
				result = Binary_HSI(&st_buf->m_st_hsi[i]);
				st_buf->m_st_roi[i].areaSize = 0;
				st_buf->mutex.lock();
				st_buf->isTracking = 0;
				st_buf->mutex.unlock();
			}
		}
		else if(!st_buf->isTracking){ //Recognition
			//Opening 연산
			IMG_INFO *erosion_1 = ErosionBinary(binary);
			result = DilationBinary(erosion_1);

			//Labeling 연산
			int label_num = CallLabel(binary, object[i], 7000, 5);

			//일정 크기 이상일시 Tracking 실시
			if (object[i][1].areaSize > 7000) {
				st_buf->mutex.lock();
				if(!st_buf->isTracking) 
					st_buf->isTracking = i+1;
				st_buf->mutex.unlock();
				if (st_buf->isTracking != i+1) { 
					object[i][1].areaSize = 0; break; 
				}
				st_buf->m_n_is_tracking |= is_tracking_bit[i]; // Tracking Start
				memcpy(&(st_buf->m_st_roi[i]), &(object[i][1]), sizeof(OBJECT)); //Object -> ROI
				
				
				//Get Histogram
				Histogram(&st_buf->m_st_hsi[i], result, &(st_buf->m_st_roi[i]));
			}
			else {
				st_buf->m_st_roi[i].areaSize = 0;
			}
			//메모리 해제
			ReleaseImageInfo(&erosion_1);
		}
		else {
			result = Binary_HSI(&st_buf->m_st_hsi[i]);
		}
		//결과 이미지
		*(st_buf->m_img_result[i]) = CloneImageInfo(result, D_U8);
		//메모리 해제
		ReleaseImageInfo(&binary);
		ReleaseImageInfo(&result);
	}

	//roi를 기준으로 사각형을 그림
	for (int i = 0; i < st_buf->m_n_tracking_cnt; i++) {
		if (st_buf->m_st_roi[i].areaSize != 0) {
			RECT2D rt(st_buf->m_st_roi[i].left.x, st_buf->m_st_roi[i].top.y, st_buf->m_st_roi[i].width,	st_buf->m_st_roi[i].height );
			DrawRect(src, rt, ct[i], 2);
		}
	}
	//H,S,I 이미지 해제
	ReleaseImageInfo(&HSI);
	ReleaseImageInfo(&hue);
	ReleaseImageInfo(&saturation);
	ReleaseImageInfo(&intensity);

	return 0;
}

//캠시프트
IMG_INFO* CAMShift(HSIVALUE *hsv, IMG_INFO *binary, OBJECT *roi) {

	OBJECT new_window;
	memcpy(&new_window, roi, sizeof(OBJECT));
	Resizing(&new_window, 100., hsv); // 윈도우 크기를 input img size에 맞춰줌
	Histogram(hsv, binary, &new_window);  // get histogram for input img
	IMG_INFO *backprojected_img = BackProjection(&new_window, roi, binary ,hsv); // backprojection (input/model )
	U8 *pSrc = backprojected_img->m_pImgU8;
	POINT2D mean = GetMean(pSrc, roi, hsv);  // Get Mean in Backprojected roi
	if (!CheckFail(mean, roi)) //if mean is 0 then Tracking Fail
		return 0;
	MoveWindow(binary->m_nWidth, binary->m_nHeight, roi, mean); // roi move to mean
	if(!ResizeWindow(pSrc, roi, hsv)) return 0; //Resize window

	return backprojected_img;
}

void MoveWindow(const int width, const int height, OBJECT *window, POINT2D center)
{
	/*
	x1                     x2
	------------------------y2
	|                      |
	|                      |
	|                      |
	|                      |
	------------------------y1
	*/
	int x1 = center.x - window->width / 2, x2 = center.x + window->width / 2; //window rect( x1, x2, y1, y2 )
	int y1 = center.y - window->height / 2, y2 = center.y + window->height / 2;
	if (x1 < 0) x1 = 0; // not over frame
	if (y1 < 0) y1 = 0;
	if (x2 > width) x2 = width - 1;
	if (y2 > height) y2 = height - 1;

	window->top.y = y1;
	window->bottom.y = y2;
	window->left.x = x1;
	window->right.x = x2;
	window->center.x = center.x;
	window->center.y = center.y;
}

void Histogram(HSIVALUE *hsv, IMG_INFO *binary, OBJECT *window)
{
	U16 *pH = hsv->hImg->m_pImgU16;
	U16 *pS = hsv->sImg->m_pImgU16;
	U16 *pI = hsv->iImg->m_pImgU16;
	U8 * pB = binary->m_pImgU8;
	int wStep = hsv->iImg->m_nWStep;
	int histo_index;

	for (int i = 0; i < 360; i++) window->histo[i] = 0.0; //hist init H Value (0-359)

	for (int i = window->top.y; i < window->bottom.y; i++) {
		int index = i * wStep;
		for (int j = window->left.x; j < window->right.x; j++) {
			if (pB[index + j] == WHITE_8) // if binary white then check histo
			{
				histo_index = pH[index + j];
				window->histo[histo_index] += 1;
			}
		}
	}
	for (int i = 0; i < 360; i++) window->histo[i] /= window->height * window->width; //histogram nomalization
}

IMG_INFO *BackProjection(OBJECT *in_img, OBJECT *model_img, IMG_INFO *binary, HSIVALUE *hsv)
{

	IMG_INFO *dest = CloneImageInfo(hsv->iImg, D_U8);

	U16 *pH = hsv->hImg->m_pImgU16;
	U16 *pS = hsv->sImg->m_pImgU16;
	U16 *pI = hsv->iImg->m_pImgU16;
	U8 *pB = binary->m_pImgU8;
	U8 *pDest = dest->m_pImgU8;

	int wStep = dest->m_nWStep;
	int histo_index;
	double value;
	memset(pDest, BLACK_8, sizeof(U8)* dest->m_nImageSize); //memset return image


	for (int i = 0; i < binary->m_nHeight; i++)
	{
		int index = i * wStep;
		for (int j = 0; j < binary->m_nWidth; j++)
		{
			histo_index = pH[index + j]; // calc histo_index
			if (!in_img->histo[histo_index] || pB[index + j] == BLACK_8)  // in_img pixel in histo and binary img isn't black
				continue;
			value = MIN((model_img->histo[histo_index] / in_img->histo[histo_index]), 1.0); // value 1.0 or less than

			if (value == 1.0) pDest[index + j] = WHITE_8; // if value 1.0 than pDest white
		}
	}
	return dest; // return backprojected img
}

POINT2D GetMean(U8 *src, OBJECT *window, HSIVALUE *hsv)
{
	int wStep = hsv->iImg->m_nWidth;
	int mX = 0, mY = 0, cnt = 1;
	double object_area;

	for (int i = window->top.y; i < window->bottom.y; i++) {
		int index = i * wStep;
		for (int j = window->left.x; j < window->right.x; j++) {
			if (src[index + j] == WHITE_8) { //if backprojected img white
				mX += j; // calc mean
				mY += i;
				cnt++; // get count
			}
		}
	}

	mX = (int)(mX / (cnt + 10e-7));
	mY = (int)(mY / (cnt + 10e-7));
	
	return POINT2D(mX, mY);
}

int ResizeWindow(U8 *src, OBJECT *window, HSIVALUE *hsv)
{

	int wStep = hsv->iImg->m_nWidth;
	int  cnt = 0;
	double object_area;
	//count in moved window
	for (int i = window->top.y; i < window->bottom.y; i++) {
		int index = i * wStep;
		for (int j = window->left.x; j < window->right.x; j++) {
			if (src[index + j] == WHITE_8) {
				cnt++;
			}
		}
	}

	//size change
	object_area = (double)cnt / window->areaSize;
	if (object_area > 0.6 ) Resizing(window, 1.1, hsv);
	else if (object_area < 0.5 && window->areaSize > 8000) Resizing(window, 0.9, hsv);
	else if (object_area < 0.005) return 0;
	
	return 1;

}

void Resizing(OBJECT *window, double rate, HSIVALUE *hsv)
{
	window->height = (int)(window->height * rate);
	window->width = (int)(window->width * rate);
	if (window->height <= 0) window->height = 1;
	if (window->width <= 0) window->width = 1;
	if (window->height >= hsv->iImg->m_nHeight) window->height = hsv->iImg->m_nHeight;
	if (window->width >= hsv->iImg->m_nWidth) window->width = hsv->iImg->m_nWidth;
	window->areaSize = window->height * window->width;
	MoveWindow(hsv->iImg->m_nWidth, hsv->iImg->m_nHeight, window, POINT2D(window->center.x, window->center.y));
}

int CheckFail(POINT2D mean, OBJECT * window) 
{
	int dX = mean.x - window->center.x; //delta X
	int dY = mean.y - window->center.y; //delta Y
										 
	//윈도우가 이상한 곳에 걸렸을 시 종료 조건
	if (abs(dX) < 10 && abs(dY) < 10)
	{
		window->lost_object++;
	}
	else
	{
		window->lost_object = 1;
	}
	if (window->lost_object > 40 ) return 0;
	if (!mean.x && !mean.y) return 0;

	return 1;
}