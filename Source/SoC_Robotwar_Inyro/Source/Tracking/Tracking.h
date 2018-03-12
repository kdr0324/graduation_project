#ifndef _TRACKING_H_
#define _TRACKING_H_

#include "../Common/ImageManagement.h"
#include "../PC/ImagePixel.h"
#include "../PC/Labeling.h"
#include "../PC/Semaphore.h"

typedef struct _tagTrackingInfo {
	int m_n_tracking_cnt;
	HSIVALUE *m_st_hsi;
	int m_n_result_img_cnt;
	IMG_INFO ***m_img_result;

	//김대룡 수정 관심영역 ROI, 추적 중인지 아닌지 결정하는 is tracking
	OBJECT *m_st_roi;
	int m_n_is_tracking; //8:y, 4:b, 2:g, 1:r

	//Semaphore
	const char* mutexName = "isTracking";
	KSemaphore mutex;
	int isTracking = 0;
	//

} TRACKINGINFO;



int TrackingMain(IMG_INFO *src, void *buf);
IMG_INFO* CAMShift(HSIVALUE *hsv,IMG_INFO *binary, OBJECT *roi);
void MoveWindow(const int width, const int height, OBJECT *roi, POINT2D center);
void Histogram(HSIVALUE * hsv, IMG_INFO *binary,OBJECT *window);
IMG_INFO *BackProjection(OBJECT *r1, OBJECT *r2, IMG_INFO *binary,HSIVALUE *hsv);
POINT2D GetMean(U8* src, OBJECT *window, HSIVALUE *hsv);
int ResizeWindow(U8* src, OBJECT *window, HSIVALUE *hsv);
void Resizing(OBJECT *window, double rate, HSIVALUE *hsv);
int CheckFail(POINT2D mean, OBJECT * roi);
#endif // _TRACKING_H_