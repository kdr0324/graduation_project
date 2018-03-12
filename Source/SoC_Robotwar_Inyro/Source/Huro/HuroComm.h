#ifndef	_HURO_COMM_H_
#define	_HURO_COMM_H_

#include "../Amazon/uart_api.h"

#define	HURO_OK	'A'
#define	HURO_NG	'B'

#define LEFT_MOTION 0
#define RIGHT_MOTION 1

enum MOTION {
	/*
	-------------------- �ּ� ��Ģ --------------------
	- ��� ����
	- ��� ��ȣ, ��� �̸�, (���)
	- ���� ��� �ּ� : //# -> �ٲ� ���, (�ݺ� Ƚ��)
	- 2016 �޷� ��� : �� �Ʒ� ��ġ
	---------------------------------------------------
	- huro.c�� �ִ� ������ ���� ��ǵ��� //# �� �ٿ��� �ٲ� �ڵ� �ؿ� �ּ����� �־��
	*/
	// 1, �⺻ �ڼ�, (�� 45�� �Ʒ�)
	// �⺻ ���� ���
	BASIC_OFF = 11, // 1, �⺻ �ڼ�, (�� 45�� �Ʒ�)
	
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

	SAFETY_OFF, // 2, ����ȭ �ڼ�, (�� ����)

	// ��� �׽�Ʈ (3�� ~ 10�� ���)

	// ���� ���� ���
	//HALF_WALK = 11, // 11, ���� ����ݺ�, (���� �� ���)
	//HALF_HALF_WALK,
	//BELLBELL_WALK, // 13, ���� ��������
	//STAIR_BELLBELL_WALK, // 14, ��� ���� ���� ����
	//WALK, // 15, ���� ����
	//RUN, // 16, ���� �޸���
	//SIDE_MOVE_10 = 17, // 17, ���� ������ 10
	//SIDE_MOVE_20 = 19, // 19, ���� ������ 20 (17, ������ ������ 20)
	//SIDE_MOVE_50 = 21, // 21, ���� ������ 50 (19, ������ ������ 50)
	//SIDE_MOVE_70 = 23, // 23, ���� ������ 70 (21, ������ ������ 70)
	//BACK_LITTLE_WALK = 25, // 24, ���� ���� ����
	//BACK_BELLBELL_HALF_WALK = 26,
	//BACK_WALK = 27,

	//// ȸ�� ���� ���
	//TURN_5 = 28, // 28, ���� �� 5, (26, ������ �� 5)
	//TURN_20 = 30, // 30, ���� �� 20, (28, ������ �� 20)
	//TURN_30 = 32, // 32, ���� �� 30, (30, ������ �� 30)
	//TURN_45 = 34, // 34, ���� �� 45, (32, ������ �� 45)

	//// Ư�� ���� ���
	//STAIR_UP_2 = 36, // 36, ��� �޹� ������ 2cm,
	//STAIR_DOWN_2, // 37, ��� ������ �������� 2m
	//STAIR_UP_4, // 38, ��� �޹� ������ 4cm
	//STAIR_DOWN_4, // 39, ��� �������� 4cm
	//GOLF, // 40, ������ ������
	//HURDLE, // 41, ��� �ѱ�

	//// �� ���� ���
	//HIDE_ARM_HEAD_RIGHT_90_DOWN_45, // 42, ���� ���� �� ���������� 90 �Ʒ� 45
	//HEAD_DOWN_70, // 43, �� �Ʒ��� 70
	//HEAD_DOWN_50, // 44, �� �Ʒ��� 70
	//HEAD_DOWN_30, // 45, �� �Ʒ��� 30
	//HEAD_DOWN_0 // 46, �� ����

	/*
	// ���� ���
	WALK_50_1, //# -> WALK (1��)
	WALK_50_2, //# -> WALK (2��)
	RUN_50_3, //# -> RUN (1��)
	RUN_50_5, //# -> RUN (2��)
	RUN_50_7, //# -> RUN (3��)
	TURN_15, //# -> TURN_5 (3��)
	AFTER_WALK_SIDE20, //# -> SIDE_MOVE_20 (1��)
	HIDE_ARM_SIDE_MOVE_20, //# -> SIDE_MOVE_20 (1��)
	HIDE_ARM_SIDE_MOVE_50, //# -> SIDE_MOVE_50 (1��)
	HALF_WALK_LEFT, //# -> HALF_WALK (1��)
	HIDE_ARM_TURN_5, //# -> TURN_5 (1��)
	HIDE_ARM_TURN_20 //# -> TURN_20 (1��)
	*/


	/* ------------------------------------------------------------------------------------------------------------------------ */


	// 2016 Huro ���

	/* BASIC_OFF = 1,
	
	HALF_WALK = 4,
	// ���� 1������ ������.
	BELLBELL_WALK = 5,

	// �Ϲ����� ���� ���
	WALK_50_1,
	WALK_50_2,
	
	// �ٸ������� ���� ������ ���� ���ؼ� �����޸��� 50 �󺧵��� �߰�
	RUN_50_3,
	RUN_50_5,
	RUN_50_7,

	// ������ �̵��� �ּ����� ȣ��(��, �ִ��� �߾Ӷ��� ���߱�)
	SIDE_MOVE_20 = 11,
	SIDE_MOVE_50 = 13,
	SIDE_MOVE_70 = 15,
	BACK_LITTLE_WALK = 17,

	HIDE_ARM_SIDE_MOVE_20 = 18,

	// ū ���� �ʿ� ����
	TURN_5 = 20,
	TURN_15 = 22,
	TURN_30 = 24,
	TURN_45 = 26,
	
	//���ȵڷ�_��
	HIDE_ARM_TURN_5 = 28,
	HIDE_ARM_TURN_20 = 30,

	// Ư�� ���
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
