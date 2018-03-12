#ifndef _STAIR_H_
#define _STAIR_H_

#include "../Amazon/Config.h"

#define STAIR_UP_END									0x00000000
#define STAIR_UP_LEFT_MOTION							0x00000001
#define STAIR_UP_SIDE_MOVE								0x00000002
#define STAIR_UP_RUN_5									0x00000004
#define STAIR_UP_RUN_10									0x00000008
#define STAIR_UP_RUN_15									0x00000010
#define STAIR_UP_RUN_20									0x00000020

#define GREEN_BRIDGE_END							0x00000000
#define GREEN_BRIDGE_LEFT_MOTION					0x00000001
#define GREEN_BRIDGE_SIDE_MOVE_10					0x00000002
#define GREEN_BRIDGE_SIDE_MOVE_30					0x00000004
#define GREEN_BRIDGE_SIDE_TURN_15					0x00000008
#define GREEN_BRIDGE_SIDE_TURN_30					0x00000010
#define GREEN_BRIDGE_GO								0x00000020

#define STAIR_DOWN_END								0x00000000
#define STAIR_DOWN_LEFT_MOTION						0x00000001
#define STAIR_DOWN_TURN_5							0x00000002
#define STAIR_DOWN_TURN_15							0x00000004
#define STAIR_DOWN_DOUBLE_HALF_WALK					0x00000008
#define STAIR_DOWN_HALF_WALK						0x00000010
#define STAIR_DOWN_HALF_HALF_WALK					0x00000020
#define STAIR_DOWN_MISSION_END_FLAG					0x00000040
#define STAIR_DOWN_LITTLE_BACK						0x00000080

int StairUp(FILE *fp, const char *name);
int GreenBridge(FILE *fp, const char *name);
int StairDown(FILE *fp, const char *name);
int StairDown(FILE *fp, const char *name);

#endif // _STAIR_H_
