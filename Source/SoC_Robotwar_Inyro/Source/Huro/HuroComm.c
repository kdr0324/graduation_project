#include "stdafx.h"

#include "HuroComm.h"


#include <stdio.h>
#include <stdlib.h>
#ifndef _WIN32
#include <unistd.h>
#endif // WIN32

// 메탈파이터는 1바이트로만 통신한다.
#define	COMM_RETRY_CNT	15

// Retry value - A: Success, B: Failure
int huro_motion(unsigned char number, int is_sleep)
{
	int i;
	unsigned char send = number;
	unsigned char recv = -1;

#if PRINT_MODE
	printf("%s: motion = %d\n", __FUNCTION__, number);
#endif

	if (noMotionMode)
		return 0;

#ifndef _WIN32
	if(uart1_buffer_write(&send, 1) < 0) {
		return -1;
	}
#endif // WIN32

// 수신 - 15초동안 받아보고 안되면 통신 실패
	for(i = 0; i < COMM_RETRY_CNT; i++) {
#ifndef _WIN32
		usleep(100000);
		if(uart1_buffer_read(&recv, 1) == 0) {
			if(is_sleep == SAFE_LOT) {
				usleep(700000);	// 안정화를 위한 시간 1.4초
			} else if(is_sleep == SAFE_LITTLE) {
				usleep(300000);		// 안정화를 위한 시간 0.6초
			} else {
				usleep(100000);		// 기본 동작 후 시간 0.2초
			}
#if PRINT_MODE
			printf("%s: return = %d\n", __FUNCTION__, recv);
#endif				
		}
		// usleep(500000);
#endif // WIN32
		switch(recv) {
		case HURO_OK:
			return recv;

		case HURO_NG:		// 카운트를 초기화 후 로봇이 일어날 때 까지 기다린다.
			i = 0;
			break;

		default:
			break;
		}
	}
	return recv;
}
