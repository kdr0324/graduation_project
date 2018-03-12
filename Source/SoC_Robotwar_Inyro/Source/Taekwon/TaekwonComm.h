#ifndef _TAEKWON_COMM_H_
#define _TAEKWON_COMM_H_

#include "../Amazon/uart_api.h"

#define START_CODE    0xFF
#define START_CODE1   0x55
#define Hdata	      0x00
#define Hdata1        0xFF

#define RIGHT_MOTION	1

enum TAEKOWN_MOTION {
	//�ڼ�
	TAEKWON_POWER_INIT_POS = 1,	//�����ʱ�
	TAEKWON_BASIC_POS,			//�⺻�ڼ�
	TAEKWON_STAND_POS,			//�����ڼ�

		//ȸ��
	TAEKWON_TURN_10 = 6, //�� , ��
	TAEKWON_TURN_30 = 8,
	TAEKWON_TURN_45 = 10,

	//�Ӹ� ȸ��
	TAEKWON_HEAD_45_LEFT = 14,	//��7 �� 8
	TAEKWON_HEAD_45_RIGHT = 15,	//��7 �� 8

	//�ȱ�
	TAEKWON_LONG_BACK_WALK = 20,	
	TAEKWON_BACK_WALK = 21,
	TAEKWON_WALK = 22,
	TAEKWON_LONG_WALK = 23,

	//����
	TAEKWON_PUNCH = 30,				// ���ڸ����� ġ��
	TAEKWON_PUNCH2 = 31, //39,			// ������ ������ �ߴ� ġ��
	TAEKWON_STING = 32,				// ���
	TAEKWON_HEAD_PUNCH = 33,		// �Ӹ�ġ��
	TAEKWON_SHORT_KICK = 34,		// ������
	TAEKWON_LONG_KICK = 35,			// ū ������
	TAEKWON_FRONT_ATTACK = 36,		// ������ ��ġ��
	TAEKWON_BACK_ATTACK = 37,		// �ڷ� ��ġ��
	TAEKWON_FRONT_KICK = 38,
	
	TAEKWON_START_MODE_1 = 41,
	TAEKWON_START_MODE_2= 43,
	TAEKWON_START_MODE_3= 45,

	TAEKWON_ROUNDING_FRONT = 51,
	TAEKWON_ROUNDING_BACK = 55,

	TAEKWON_SHORT_BACK_IN_LEFT_HEAD = 57,
	TAEKWON_SHORT_FRONT_IN_RIGHT_HEAD = 59,	
};

int taekwon_motion(unsigned char number, int is_sleep);

#endif // _TAEKWON_COMM_H_
