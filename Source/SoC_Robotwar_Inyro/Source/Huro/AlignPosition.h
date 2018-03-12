#ifndef _ALIGN_POSITION_H_
#define _ALIGN_POSITION_H_

#include "../Amazon/Config.h"

#define ALIGN_POSITION_END						0x00000000
#define ALIGN_POSITION_LEFT_MOTION				0x00000001
#define ALIGN_POSITION_TURN_20					0x00000002
#define ALIGN_POSITION_TURN_5					0x00000004
#define ALIGN_POSITION_TRIPLE_SIDE_50			0x00000008
#define ALIGN_POSITION_DOUBLE_SIDE_50			0x00000010
#define ALIGN_POSITION_SINGLE_SIDE_50			0x00000020
#define ALIGN_POSITION_SINGLE_SIDE_20			0x00000040
#define ALIGN_POSITION_SIDE_70					0x00000080

int AlignPosition(FILE *fp, const char *name);
int AlignPositionTurn(FILE *fp, const char *name);

#endif // _ALIGN_POSITION_H_