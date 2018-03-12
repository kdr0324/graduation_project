#include "stdafx.h"



#include <stdio.h>
#include <stdlib.h>
#ifndef _WIN32
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <errno.h>
#endif // _WIN32

#include "uart_api.h"

static int __uart_dev = -1;
int noMotionMode = 0;

#ifndef _WIN32
int uart_open(void)
{
    int handle;

    if((handle = open(UART_DEV_NAME, O_RDWR)) < 0) {
        printf("Open Error %s\n", UART_DEV_NAME);
        return -1;
    }

    __uart_dev = handle;

    return 0;
}

void uart_close(void)
{
    close(__uart_dev);
}

int uart_config(int ch, int baud, int bits, int parity, int stops)
{
    struct uart_config uart_conf;

    uart_conf.uart_ch = ch;
    uart_conf.baud = baud;
    uart_conf.bits = bits;
    uart_conf.parity = parity;
    uart_conf.stops = stops;

    return ioctl(__uart_dev, UART_SET_CONFIG, &uart_conf);
}

int uart_tx_buf_full(int ch)
{
    return ioctl(__uart_dev, UART_TXBUF_FULL, ch);
}

int uart_write(int ch, U8 *ubuf, int size)
{
    struct uart_data uart_d;

    uart_d.uart_ch = ch;
    uart_d.buf_size = size;
    uart_d.uart_buf = ubuf;

    return ioctl(__uart_dev, UART_DATA_WRITE, &uart_d);
}

int uart_rx_level(int ch)
{
    return ioctl(__uart_dev, UART_RXBUF_LEVEL, ch);
}

int uart_read(int ch, U8 *ubuf, int size)
{
    struct uart_data uart_d;

    uart_d.uart_ch = ch;
    uart_d.buf_size = size;
    uart_d.uart_buf = ubuf;

    return ioctl(__uart_dev, UART_DATA_READ, &uart_d);
}

int uart_txbuf_clear(int ch)
{
	return ioctl(__uart_dev, UART_TXBUF_CLEAR, ch);
}

int uart_rxbuf_clear(int ch)
{
	return ioctl(__uart_dev, UART_RXBUF_CLEAR, ch);
}

int uart1_buffer_write(U8 *buf, int size)
{
	int ret;

	int retry_cnt = 0;
	const int retry_max = 3;

	// TX ���۰� �������� 3���� ��ٸ�
	while(retry_cnt < retry_max) {
		if(uart_tx_buf_full(UART1) == 1) {
			usleep(10000);
			retry_cnt++;
		} else {
			break;
		}
	}

	// ������ ���۰� ��� ���� ������ ���۸� ����.
	if(retry_cnt >= retry_max) { 
		printf("%s: clear buffer\n", __FUNCTION__);
		uart_txbuf_clear(UART1);
	}
	
	ret = uart_write(UART1, buf, size);
	if(ret >= 0) {
		return 0;
	} else {
		printf("%s: Maybe UART Buffer is Full!\n", __FUNCTION__);
		return -1;
	}	
}

int uart1_buffer_read(U8 *buf, int size)
{
	const int retry_max = 5;

	int rx_len=0;
	int rx_cnt=0;
	int retry_cnt = 0;

	while(retry_cnt < retry_max) {
		rx_len = uart_rx_level(UART1);
		if(rx_len <= 0) {
			usleep(100000);
			retry_cnt++;
			continue;
		}

		if(rx_cnt + rx_len < size) {
			uart_read(UART1, &buf[rx_cnt], rx_len);
			rx_cnt += rx_len;
		} else if(rx_cnt + rx_len == size) {
			uart_read(UART1, &buf[rx_cnt], rx_len);
			break;
		} else { // ���ۿ� �� ���� ��ŭ�� �ް� �������� ������.
			uart_read(UART1, &buf[rx_cnt], size - rx_cnt);
			break;
		}
	} // while

	if(retry_cnt < retry_max) {
		return 0;
	} else {
		return -1;
	}
}

#endif
