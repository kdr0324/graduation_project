/**
 @title : Mine.h
 @author : 임현 (010 - 9019 - 3799)
 @since : 2017 - 07 - 05
 @brief : 지뢰 미션 헤더 파일
*/

#ifndef _MINE_H_
// 컴파일 확인 변수로 추정
#define _MINE_H_

// Config.h : 컴퓨터의 환경을 설정
#include "../Amazon/Config.h"

// 지뢰를 찾았는데 없음
#define MINE_DETECT_IS_NOT_MINE		0x00000000

#define MINE_DETECT_LEFT_MOTION		0x00000001

#define MINE_DETECT_MINE_RUN		0x00000002
#define MINE_DETECT_QUARTER_WALK	0x00000004
// 지뢰가 적당히 보임
#define MINE_DETECT_HALF_WALK		0x00000008
// 지뢰를 찾으려고 반걸음
#define MINE_DETECT_WALK			0x00000010
// 작게
#define MINE_DETECT_SIDE_MOVE_10		0x00000020
#define MINE_DETECT_SIDE_MOVE_30		0x00000040
// 크게
#define MINE_DETECT_SIDE_MOVE_70		0x00000080

int MineDetect(FILE *fp, const char *name);

#endif // _MINE_H_
