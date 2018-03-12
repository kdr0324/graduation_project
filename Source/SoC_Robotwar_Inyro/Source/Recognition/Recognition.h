#ifndef _RECOGNITION_H_
#define _RECOGNITION_H_

#include "../Common/ImageManagement.h"

int RecognitionMain(IMG_INFO *src, void *buf);

typedef struct _tagRecognitionInfo {
	int m_n_result_img_cnt;
	IMG_INFO ***m_img_result;
} RECOGNITIONINFO;

#endif // _RECOGNITION_H_