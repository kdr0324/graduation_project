#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *memcpy(void *dest, const void *src, size_t n);
void *memset(void *s, int c, size_t n);

typedef signed char		S8;
typedef signed short	S16;
typedef signed int		S32;

typedef unsigned char	U8;
typedef unsigned short	U16;
typedef unsigned int	U32;

typedef volatile unsigned char	vU8;
typedef volatile unsigned short	vU16;
typedef volatile unsigned int	vU32;

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

#ifndef CLIP
#define CLIP(x) ((x) > 255 ? 255 : (x) < 0 ? 0 :(x))
#endif // CLIP

#ifndef ABS
#define ABS(x) ((x) > 0 ? (x) : (-x))
#endif // ABSOLUTE

#define BLACK_8 (U8)0x00
#define WHITE_8 (U8)0xFF

#endif // _GLOBAL_H_
