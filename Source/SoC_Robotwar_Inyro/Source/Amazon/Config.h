#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _WIN32
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#endif // WIN32

#define BLACK_16	0x0000
#define BLUE_16		0x001F
#define GREEN_16	0x07E0
#define CYAN_16		0x07FF
#define RED_16		0xF800
#define MAGENTA_16	0xF81F
#define ORANGE_16	0xFC00
#define YELLOW_16	0xFFE0
#define WHITE_16	0xFFFF

#ifndef SQ_5
#define SQ_5(x) ((x) << 5)
#endif // SQ_5

#ifndef SQ_10
#define SQ_10(x) ((x) << 10)
#endif // SQ_10

#ifndef SQ_15
#define SQ_15(x) ((x) << 15)
#endif // SQ_15

#ifndef SQ_20
#define SQ_20(x) ((x) << 20)
#endif // SQ_20

#ifndef DSQ_5
#define DSQ_5(x) ((x) >> 5)
#endif // DSQ_5

#ifndef DSQ_10
#define DSQ_10(x) ((x) >> 10)
#endif // DSQ_10

#ifndef DSQ_15
#define DSQ_15(x) ((x) >> 15)
#endif // DSQ_15

#ifndef DSQ_20
#define DSQ_20(x) ((x) >> 20)
#endif // DSQ_20

#define PRINT_MODE 1

// 영상 길이
#define AMAZON_HALF_WIDTH 90
#define	AMAZON_WIDTH	180
#define AMAZON_HALF_HEIGHT 60
#define	AMAZON_HEIGHT	120
#define AMAZON_IMAGESIZE	21600

#define SAFE_NO 0
#define SAFE_LITTLE 1
#define SAFE_LOT 2

extern int imageSaveMode;

#endif // _CONFIG_H_
