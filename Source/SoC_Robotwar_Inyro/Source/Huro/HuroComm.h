#ifndef	_HURO_COMM_H_
#define	_HURO_COMM_H_

#include "../Amazon/uart_api.h"

#define	HURO_OK	'A'
#define	HURO_NG	'B'

#define LEFT_MOTION 0
#define RIGHT_MOTION 1

enum MOTION {
	/*
	-------------------- 주석 규칙 --------------------
	- 모션 구분
	- 모션 번호, 모션 이름, (비고)
	- 삭제 모션 주석 : //# -> 바꾼 모션, (반복 횟수)
	- 2016 휴로 모션 : 맨 아래 위치
	---------------------------------------------------
	- huro.c에 있는 없어진 예전 모션들은 //# 을 붙여서 바꾼 코드 밑에 주석으로 넣어둠
	*/
	// 1, 기본 자세, (고개 45도 아래)
	// 기본 관련 모션
	BASIC_OFF = 11, // 1, 기본 자세, (고개 45도 아래)
	
	//HEAD
	HEAD_DOWN_0 = 12,
	HEAD_DOWN_20,
	HEAD_DOWN_80,
	HEAD_RIGHT_90_DOWN_35,
	HEAD_DOWN_50,
	HEAD_DOWN_60,
	HEAD_RIGHT_90_DOWN_55,

	
	//SPECIAL = 19,
	
	// MINE RUN
	MINE_RUN = 20,

	// STAIR
	STAIR_UP_2 = 21,
	STAIR_DOWN_2,
	STAIR_UP_4,
	STAIR_DOWN_4 = 19, //24

	//GOLF
	GOLF = 25,

	//HURDLE
	HURDLE,

	//STAIR
	STAIR_BELLBELL_WALK = 27,
	STAIR_BELLBELL_BACK_WALK,
	STAIR_BELLBELL_SHORT,
	TRAP = 30,

	//RUN
	RUN_5=31,
	RUN_10,
	RUN_15,
	RUN_20,
	RUN_25,
	RUN_30,
	RUN_35,
	RUN_40,
	RUN_45,
	RUN_50,
	

	//WALK
	QUARTER_WALK = 41,
	HALF_WALK = 42,
	WALK = 43,


	//TURN
	TURN_5 = 46, // 46 LEFT, 47 RIGHT
	TURN_15 = 48, // 48 LEFT, 49 RIGHT
	TURN_30 = 50,  // 50 LEFT, 51 RIGHT
	TURN_45 = 52,  // 52 LEFT, 53 RIGHT

	//SIDE_WALK
	SIDE_MOVE_10 = 56, // 56 LEFT, 57 RIGHT
	SIDE_MOVE_30 = 58, // 58 LEFT, 59 RIGHT
	SIDE_MOVE_50 = 60, // 60 LEFT, 61 RIGHT
	SIDE_MOVE_70 = 62, // 62 LEFT, 63 RIGHT

	SAFETY_OFF, // 2, 안정화 자세, (고개 정면)

	// 모션 테스트 (3번 ~ 10번 모션)

	// 보행 관련 모션
	//HALF_WALK = 11, // 11, 전진 보행반보, (골프 때 사용)
	//HALF_HALF_WALK,
	//BELLBELL_WALK, // 13, 전진 종종걸음
	//STAIR_BELLBELL_WALK, // 14, 계단 전진 종종 걸음
	//WALK, // 15, 전진 보행
	//RUN, // 16, 전진 달리기
	//SIDE_MOVE_10 = 17, // 17, 왼쪽 옆으로 10
	//SIDE_MOVE_20 = 19, // 19, 왼쪽 옆으로 20 (17, 오른쪽 옆으로 20)
	//SIDE_MOVE_50 = 21, // 21, 왼쪽 옆으로 50 (19, 오른쪽 옆으로 50)
	//SIDE_MOVE_70 = 23, // 23, 왼쪽 옆으로 70 (21, 오른쪽 옆으로 70)
	//BACK_LITTLE_WALK = 25, // 24, 후진 종종 걸음
	//BACK_BELLBELL_HALF_WALK = 26,
	//BACK_WALK = 27,

	//// 회전 관련 모션
	//TURN_5 = 28, // 28, 왼쪽 턴 5, (26, 오른쪽 턴 5)
	//TURN_20 = 30, // 30, 왼쪽 턴 20, (28, 오른쪽 턴 20)
	//TURN_30 = 32, // 32, 왼쪽 턴 30, (30, 오른쪽 턴 30)
	//TURN_45 = 34, // 34, 왼쪽 턴 45, (32, 오른쪽 턴 45)

	//// 특수 동작 모션
	//STAIR_UP_2 = 36, // 36, 계단 왼발 오르기 2cm,
	//STAIR_DOWN_2, // 37, 계단 오른발 내려오기 2m
	//STAIR_UP_4, // 38, 계단 왼발 오르기 4cm
	//STAIR_DOWN_4, // 39, 계단 내려오기 4cm
	//GOLF, // 40, 오른발 공차기
	//HURDLE, // 41, 허들 넘기

	//// 고개 관련 모션
	//HIDE_ARM_HEAD_RIGHT_90_DOWN_45, // 42, 양팔 위로 고개 오른쪽으로 90 아래 45
	//HEAD_DOWN_70, // 43, 고개 아래로 70
	//HEAD_DOWN_50, // 44, 고개 아래로 70
	//HEAD_DOWN_30, // 45, 고개 아래로 30
	//HEAD_DOWN_0 // 46, 고개 정면

	/*
	// 삭제 모션
	WALK_50_1, //# -> WALK (1번)
	WALK_50_2, //# -> WALK (2번)
	RUN_50_3, //# -> RUN (1번)
	RUN_50_5, //# -> RUN (2번)
	RUN_50_7, //# -> RUN (3번)
	TURN_15, //# -> TURN_5 (3번)
	AFTER_WALK_SIDE20, //# -> SIDE_MOVE_20 (1번)
	HIDE_ARM_SIDE_MOVE_20, //# -> SIDE_MOVE_20 (1번)
	HIDE_ARM_SIDE_MOVE_50, //# -> SIDE_MOVE_50 (1번)
	HALF_WALK_LEFT, //# -> HALF_WALK (1번)
	HIDE_ARM_TURN_5, //# -> TURN_5 (1번)
	HIDE_ARM_TURN_20 //# -> TURN_20 (1번)
	*/


	/* ------------------------------------------------------------------------------------------------------------------------ */


	// 2016 Huro 모션

	/* BASIC_OFF = 1,
	
	HALF_WALK = 4,
	// 전진 1보보다 빠르다.
	BELLBELL_WALK = 5,

	// 일반적인 보행 모션
	WALK_50_1,
	WALK_50_2,
	
	// 다리위에서 보다 빠르게 가기 위해서 전진달리기 50 라벨들을 추가
	RUN_50_3,
	RUN_50_5,
	RUN_50_7,

	// 옆으로 이동은 최소한의 호출(즉, 최대한 중앙라인 맞추기)
	SIDE_MOVE_20 = 11,
	SIDE_MOVE_50 = 13,
	SIDE_MOVE_70 = 15,
	BACK_LITTLE_WALK = 17,

	HIDE_ARM_SIDE_MOVE_20 = 18,

	// 큰 턴은 필요 없음
	TURN_5 = 20,
	TURN_15 = 22,
	TURN_30 = 24,
	TURN_45 = 26,
	
	//양팔뒤로_턴
	HIDE_ARM_TURN_5 = 28,
	HIDE_ARM_TURN_20 = 30,

	// 특수 모션
	STAIR_UP_2 = 32, 
	STAIR_DOWN_2, 
	STAIR_UP_4, 
	STAIR_DOWN_4, 
	GOLF, 
	STAIR_BELLBELL_WALK, 
	HIDE_ARM_HEAD_RIGHT_90_DOWN_45, 
	HURDLE, 

	HEAD_FRONT = 40,
	HEAD_DOWN_30,
	HEAD_DOWN_45,
	HEAD_DOWN_50,
	HEAD_DOWN_60,
	HEAD_DOWN_70,
	HEAD_DOWN_80,
	HIDE_ARM_SIDE_MOVE_50 = 47,
	AFTER_WALK_SIDE20 = 49,
	HEAD_SIDE_35 = 51,
	HALF_WALK_LEFT = 53 */
};

int huro_motion(unsigned char number, int is_sleep);

#endif // _HURO_COMM_H_
