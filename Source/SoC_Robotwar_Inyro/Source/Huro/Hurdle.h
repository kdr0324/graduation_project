#ifndef _HURDLE_H_
#define _HURDLE_H_

#include "../Amazon/Config.h"

#define HURDLE_END								0x00000000
#define HURDLE_BELL_BELL						0x00000001

int Hurdle(FILE *fp, const char *name);

#endif // _HURDLE_H_
