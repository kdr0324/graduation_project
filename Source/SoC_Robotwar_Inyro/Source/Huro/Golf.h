#ifndef _GOLF_H_
#define _GOLF_H_

#include "../Amazon/Config.h"

#define BALL_DETECT_END							0x00000000
#define BALL_DETECT_IS_NOT_BALL					0x00000001
#define BALL_DETECT_LEFT_MOTION					0x00000002
#define BALL_DETECT_SIDE_MOVE					0x00000004
#define BALL_DETECT_WALK						0x00000008
#define BALL_DETECT_RUN_10						0x00000010
#define BALL_DETECT_RUN_20						0x00000020

#define GOLF_END								0x00000000

#define GOLF_LEFT_MOTION						0x00000001
#define GOLF_SIDE_MOVE							0x00000002
#define GOLF_TURN_5								0x00000004
#define GOLF_TURN_15							0x00000008
#define GOLF_RUN_10								0x00000010
#define GOLF_RUN_5								0x00000020
#define GOLF_HALF_WALK							0x00000040
#define GOLF_NOT_FOUND							0x00000080
#define GOLF_SIDE_MOVE_30						0x00000100

int BallDetect(FILE *fp, const char *name);
int Golf_B(FILE *fp, const char *name);
int Golf_H(FILE *fp, const char *name);

#endif // _GOLF_H_
