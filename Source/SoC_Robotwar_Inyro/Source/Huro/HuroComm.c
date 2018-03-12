#include "stdafx.h"

#include "HuroComm.h"


#include <stdio.h>
#include <stdlib.h>
#ifndef _WIN32
#include <unistd.h>
#endif // WIN32

// ��Ż�����ʹ� 1����Ʈ�θ� ����Ѵ�.
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

// ���� - 15�ʵ��� �޾ƺ��� �ȵǸ� ��� ����
	for(i = 0; i < COMM_RETRY_CNT; i++) {
#ifndef _WIN32
		usleep(100000);
		if(uart1_buffer_read(&recv, 1) == 0) {
			if(is_sleep == SAFE_LOT) {
				usleep(700000);	// ����ȭ�� ���� �ð� 1.4��
			} else if(is_sleep == SAFE_LITTLE) {
				usleep(300000);		// ����ȭ�� ���� �ð� 0.6��
			} else {
				usleep(100000);		// �⺻ ���� �� �ð� 0.2��
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

		case HURO_NG:		// ī��Ʈ�� �ʱ�ȭ �� �κ��� �Ͼ �� ���� ��ٸ���.
			i = 0;
			break;

		default:
			break;
		}
	}
	return recv;
}
