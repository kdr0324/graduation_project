#ifndef _TAEKWON_H_
#define _TAEKWON_H_

#include "../Amazon/Config.h"

// 로봇이 존재하지 않는다.
#define TAEKWON_ROBOT_NOT						0x00000000

#define TAEKWON_ROBOT_EXIST						0x00000001

// 로봇이 먼 것 같다.
#define TAEKWON_ROBOT_FAR						0x00000002

// 로봇이 가까이 있다. (이동 후 공격)
#define TAEKWON_ROBOT_NEAR						0x00000004

// 공격 거리에 있다.
#define TAEKWON_ROBOT_ATTACK_1					0x00000008
#define TAEKWON_ROBOT_ATTACK_2					0x00000010
#define TAEKWON_ROBOT_ATTACK_3					0x00000020
#define TAEKWON_ROBOT_ATTACK_4					0x00000040
#define TAEKWON_ROBOT_ATTACK_5					0x00000080
#define TAEKWON_ROBOT_ATTACK_6					0x00000100

// 로봇이 왼쪽 혹은 오른쪽에 있는거 같다.
#define TAEKWON_ROBOT_LEFT						0x00000200
#define TAEKWON_ROBOT_RIGHT						0x00000400

int taekwon_main(void);

#ifdef _WIN32
int IsClosed(const char *name);
#else
int IsClosed(void);
#endif

int FindRobot(FILE *fp, const char *name);
int Head_Left();
int Head_Right();
int Head_Front();

#endif // _TAEKWON_H_
