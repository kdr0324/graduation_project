#ifndef _POINT_H_
#define _POINT_H_

typedef struct _tag2DPoint {
	int x;
	int y;
#ifdef __cplusplus
	_tag2DPoint()
	{
	}

	_tag2DPoint(int x1, int x2)
	{
		x = x1, y = x2;
	}
#endif //__cplusplus
}POINT2D;

typedef struct _tag3DPoint {
	int x;
	int y;
	int z;
#ifdef __cplusplus
	_tag3DPoint()
	{
	}

	_tag3DPoint(int x1, int x2, int x3)
	{
		x = x1, y = x2, z = x3;
	}
#endif //__cplusplus
}POINT3D;

typedef struct _tagRect2DPoint {
	int startX;
	int startY;
	int width;
	int height;
#ifdef __cplusplus
	_tagRect2DPoint()
	{
	}

	_tagRect2DPoint(int x1, int x2, int w1, int w2)
	{
		startX = x1, startY = x2, width = w1, height = w2;
	}

	_tagRect2DPoint(POINT2D p1, POINT2D p2)
	{
		startX = p1.x < p2.x ? p1.x : p2.x;
		startY = p1.y < p2.y ? p1.y : p2.y; 
		width = abs(p2.x - p1.x); 
		height = abs(p2.y - p1.y);
	}
#endif //__cplusplus
}RECT2D;

#endif // _POINT_H_
