#ifndef	_RGB_H_
#define	_RGB_H_

#include "../Common/ImageManagement.h"

extern U8 pal[256][4];

int RGB565toRGB888(IMG_INFO *rgb565, IMG_INFO *rgb888);
int RGB888toRGB565(IMG_INFO *rgb888, IMG_INFO *rgb565);
int RGB565toHSI(IMG_INFO *rgb565, IMG_INFO *hImg, IMG_INFO *sImg, IMG_INFO *iImg);
int RGB565toHS(IMG_INFO *rgb565, IMG_INFO *hImg, IMG_INFO *sImg);
int RGB565toHI(IMG_INFO *rgb565, IMG_INFO *hImg, IMG_INFO *iImg);
int RGB565toSI(IMG_INFO *rgb565, IMG_INFO *sImg, IMG_INFO *iImg);
int RGB565toH(IMG_INFO *rgb565, IMG_INFO *hImg);
int RGB565toS(IMG_INFO *rgb565, IMG_INFO *sImg);
int RGB565toI(IMG_INFO *rgb565, IMG_INFO *iImg);
int MergeOriginal(IMG_INFO *result, IMG_INFO *original);
int DiffOriginal(IMG_INFO *result, IMG_INFO *original);
int GrayToRGB565(IMG_INFO *gray, IMG_INFO *rgb565);

#endif //	_RGB_H_
