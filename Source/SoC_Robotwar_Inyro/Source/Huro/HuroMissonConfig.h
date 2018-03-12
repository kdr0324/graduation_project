#ifndef _HURO_MISSION_CONFIG_H_
#define _HURO_MISSION_CONFIG_H_

#include "../Amazon/Config.h"

// 경기장 노랑 H MIN, H MAX 수치			40 65
// 학교   노랑 H MIN, H MAX 수치			35 65
// 경기장 노랑 S MIN						100
// 학교   노랑 S MIN						65
#define	YELLOW_H_MIN	17		// 35 / 2
#define	YELLOW_H_MAX	40		// 65 / 2
#define	YELLOW_S_MIN	50		// 원래 80

#define	YELLOW_HOLE_H_MIN	17		// 35 / 2  //원래 , 35, 65, 70
#define	YELLOW_HOLE_H_MAX	40		// 65 / 2
#define	YELLOW_HOLE_S_MIN	70		//원래 80

// 경기장 파랑 H MIN, H MAX 수치			205 235
// 학교 세트장								195	225
// 경기장 파랑 S MIN						150
// 학교 세트장								100
#define	BLUE_H_MIN	90		// 180 / 2
#define	BLUE_H_MAX	135		// 270 / 2
#define	BLUE_S_MIN	45

// 경기장 초록 H MIN, H MAX 수치			150 180
// 학교 세트장								130 170
// 경기장 초록 S MIN						150 
// 학교 세트장								60
#define	GREEN_H_MIN 50	// 100 / 2		// 원래 130
#define	GREEN_H_MAX	85	// 170 / 2		// 원래 170
#define	GREEN_S_MIN	50		// 원래 60

// 경기장 오렌지 H MIN, H MAX 수치			355 25
// 경기장 오렌지 S MIN						100
#define	ORANGE_H_MIN	172		// 355 / 2
#define	ORANGE_H_MAX	12		// 25 / 2
#define	ORANGE_S_MIN	100

// 경기장 빨강 H MIN, H MAX 수치			355 25
// 학교 세트장								340 20
// -> S 값이 너무 튐
#define RED_H_MIN 165	// 330 / 2
#define RED_H_MAX 20	// 30 / 2
#define RED_S_MIN 20


//09.03 에 95,135 였음 why?

// 경기장 구멍 H MIN, H MAX 수치			35 75
// 경기장 구멍 S MAX						60
#define	HOLE_H_MIN	95		// 190 / 2		//원래 35  //180 220 // 195 235    
#define	HOLE_H_MAX	135		// 270 / 2
#define	HOLE_S_MIN	80		// 원래 45
//195, 225, 100 원래 골프 수정전
// 위 값은 홀의 파랑색 띠를 기준으로 한 값임

// 경기장 검정 I MAX, S MAX 수치			80 100
// 학교	  검정 I MAX, S MAX 수치			80 70
#define	BLACK_I_MAX	80 //원래 60
#define BLACK_S_MAX 60

// 경기장 하양 I MIN, S MAX 수치			180 30
#define	WHITE_I_MIN	170
#define	WHITE_S_MAX	30

enum HURO_MISSION {
	MISSION_END = 0,
	MISSION_BARRICADE_UP = 0,
	MISSION_STAIR_RED,
	MISSION_HURDLE,
	MISSION_GATE_1,
	MISSION_BRIDGE_GREEN,
	MISSION_GOLF,
	MISSION_GATE_2,
	MISSION_ACTIVATE_TRAP_CARD,
	MISSION_BARRICADE_BACK,
	MISSION_ALIGN_POS
};

#endif // _HURO_MISSION_CONFIG_H_
