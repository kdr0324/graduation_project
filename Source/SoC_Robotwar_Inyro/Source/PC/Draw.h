#ifndef _DRAW_H_
#define _DRAW_H_

#include "../Common/Global.h"
#include "../Common/ImageManagement.h"
#include "../Common/Point.h"

typedef struct _tagColorTable {
	UCHAR red;
	UCHAR green;
	UCHAR blue;
#ifdef __cplusplus
	_tagColorTable()
	{	}

	_tagColorTable(UCHAR r, UCHAR g, UCHAR b)
	{
		red = r, green = g, blue = b;
	}
#endif //__cplusplus
} COLORTABLE;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

	int DrawLine(IMG_INFO *src, POINT2D const p[2], COLORTABLE const table, const int thick);
	int DrawRect(IMG_INFO *src, RECT2D const r, COLORTABLE const table, const int thick);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _DRAW_H_