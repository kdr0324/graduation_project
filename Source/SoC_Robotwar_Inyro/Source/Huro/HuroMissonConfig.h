#ifndef _HURO_MISSION_CONFIG_H_
#define _HURO_MISSION_CONFIG_H_

#include "../Amazon/Config.h"

// ����� ��� H MIN, H MAX ��ġ			40 65
// �б�   ��� H MIN, H MAX ��ġ			35 65
// ����� ��� S MIN						100
// �б�   ��� S MIN						65
#define	YELLOW_H_MIN	17		// 35 / 2
#define	YELLOW_H_MAX	40		// 65 / 2
#define	YELLOW_S_MIN	50		// ���� 80

#define	YELLOW_HOLE_H_MIN	17		// 35 / 2  //���� , 35, 65, 70
#define	YELLOW_HOLE_H_MAX	40		// 65 / 2
#define	YELLOW_HOLE_S_MIN	70		//���� 80

// ����� �Ķ� H MIN, H MAX ��ġ			205 235
// �б� ��Ʈ��								195	225
// ����� �Ķ� S MIN						150
// �б� ��Ʈ��								100
#define	BLUE_H_MIN	90		// 180 / 2
#define	BLUE_H_MAX	135		// 270 / 2
#define	BLUE_S_MIN	45

// ����� �ʷ� H MIN, H MAX ��ġ			150 180
// �б� ��Ʈ��								130 170
// ����� �ʷ� S MIN						150 
// �б� ��Ʈ��								60
#define	GREEN_H_MIN 50	// 100 / 2		// ���� 130
#define	GREEN_H_MAX	85	// 170 / 2		// ���� 170
#define	GREEN_S_MIN	50		// ���� 60

// ����� ������ H MIN, H MAX ��ġ			355 25
// ����� ������ S MIN						100
#define	ORANGE_H_MIN	172		// 355 / 2
#define	ORANGE_H_MAX	12		// 25 / 2
#define	ORANGE_S_MIN	100

// ����� ���� H MIN, H MAX ��ġ			355 25
// �б� ��Ʈ��								340 20
// -> S ���� �ʹ� Ʀ
#define RED_H_MIN 165	// 330 / 2
#define RED_H_MAX 20	// 30 / 2
#define RED_S_MIN 20


//09.03 �� 95,135 ���� why?

// ����� ���� H MIN, H MAX ��ġ			35 75
// ����� ���� S MAX						60
#define	HOLE_H_MIN	95		// 190 / 2		//���� 35  //180 220 // 195 235    
#define	HOLE_H_MAX	135		// 270 / 2
#define	HOLE_S_MIN	80		// ���� 45
//195, 225, 100 ���� ���� ������
// �� ���� Ȧ�� �Ķ��� �츦 �������� �� ����

// ����� ���� I MAX, S MAX ��ġ			80 100
// �б�	  ���� I MAX, S MAX ��ġ			80 70
#define	BLACK_I_MAX	80 //���� 60
#define BLACK_S_MAX 60

// ����� �Ͼ� I MIN, S MAX ��ġ			180 30
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
