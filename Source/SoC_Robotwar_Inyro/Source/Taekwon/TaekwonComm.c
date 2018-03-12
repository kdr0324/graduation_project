#include "stdafx.h"

#include "TaekwonComm.h"

#include <stdio.h>
#include <stdlib.h>
#ifndef _WIN32
#include <unistd.h>
#endif // WIN32

// CM-530은 2바이트로 통신
#define	COMM_RETRY_CNT	10

// 정상 시간
int TAEKWON_MOTION_TIME[100] = {
	0, 10, 3, 2, 0, 0, 4, 4, 13, 13,
	4, 4, 0, 0, 3, 3, 0, 0, 0, 0,
	3, 2, 2, 3, 3, 0, 0, 0, 0, 0,
	4, 11, 14, 14, 14, 95, 11, 10, 21, 7,
	0, 35 ,0, 59, 0, 59, 0, 10,0,0, // 41번 4회 30, 5회 35,  6회 40
	0, 26, 0, 0, 0, 25, 0, 28, 0, 30,
};

// 0.1초씩 감소
//int TAEKWON_MOTION_TIME[100] = {
//	0, 10, 3, 2, 0, 0, 3, 3, 13, 13,
//	4, 4, 0, 0, 3, 3, 0, 0, 0, 0,
//	2, 1, 1, 2, 2, 0, 0, 0, 0, 0,
//	3, 10, 13, 13, 13, 94, 12, 11, 13, 6,
//	0, 30 ,0, 41, 0, 20, 0, 10,0,0,
//	0, 26, 0, 0, 0, 15
//};

int send_packet(unsigned char data)
{
	unsigned char packet[6] = { 0, };

	packet[0] = START_CODE;	// Start Byte -> 0xff
	packet[1] = START_CODE1; // Start Byte1 -> 0x55
	packet[2] = data;
	packet[3] = ~data;
	packet[4] = Hdata;  // 0x00
	packet[5] = Hdata1; // 0xff

#ifndef _WIN32
	if (uart1_buffer_write(packet, 6) < 0) {
		return -1;
	}
#endif	
	return 0;
}

int recv_packet()
{	
	unsigned short recv_data;
	unsigned char checksum;
	unsigned char packet[6] = { 0, };

#ifndef _WIN32
	if (uart1_buffer_read(packet, 6) == 0) {
		if (packet[0] == 0xff && packet[1] == 0x55) {
			checksum = ~packet[3];
			if (packet[2] == checksum) {
				checksum = ~packet[5];
				if (packet[4] == checksum) {
					recv_data = (unsigned short)((packet[4] << 8) & 0xff00);
					recv_data += packet[2];
					if (recv_data == 'A')
						return 0;
					else
						return -1;
				}
			}
		}
	}	
#endif
	return -1;
	
}

// Retry value - 0: Success, -1: Failure
int taekwon_motion(unsigned char number, int is_sleep)
{
	int i;
	unsigned char send = number;
	int time = TAEKWON_MOTION_TIME[send] * 50000;

#if PRINT_MODE
	printf("%s: motion = %d\n", __FUNCTION__, number);
#endif

	if (noMotionMode)
		return 0;

	if (send_packet(send) < 0) {
		// 모션 실행 실패
		return -1;
	}

#ifndef _WIN32
	usleep(time);
#endif // WIN32	


#if 0
	// 수신 - 10초동안 받아보고 안되면 통신 실패
	for (i = 0; i < COMM_RETRY_CNT; i++) {
#ifndef _WIN32
		//sleep(1);
		usleep(100000);
		if (recv_packet() == 0) {
			if (is_sleep == SAFE_LOT) {
				usleep(700000);		// 안정화를 위한 시간 0.7초
			}
			else if (is_sleep == SAFE_LITTLE) {
				usleep(300000);		// 안정화를 위한 시간 0.3초
			}
			else {
				//usleep(time);		// 기본 동작 후 시간 0.1초
			}
			return 0;
		}
		
#endif // WIN32		
	}
#endif
	return -1;
}
