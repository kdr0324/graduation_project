#include "stdafx.h"

#include "Recognition.h"
#include "../PC/Color.h"
#include "../PC/Draw.h"

int RecognitionMain(IMG_INFO *src, void *buf)
{
	IMG_INFO *HSI = ConvertColorFormat(src, BGR2HSI);
	IMG_INFO *hue = ExtractChannel(HSI, 0);
	IMG_INFO *saturation = ExtractChannel(HSI, 1);
	IMG_INFO *intensity = ExtractChannel(HSI, 2);

	COLORTABLE ct_red(255, 0, 0);
	
	DrawRect(src, RECT2D(100, 100, 100, 100), ct_red, 5);

	ReleaseImageInfo(&HSI);
	ReleaseImageInfo(&hue);
	ReleaseImageInfo(&saturation);
	ReleaseImageInfo(&intensity);

	return 0;
}