#include "stdafx.h"

#include "Common\Global.h"

#include "Amazon\Config.h"
#ifndef _WIN32
#include "Amazon\amazon2_sdk.h"
#include "Amazon\graphic_api.h"
#endif // WIN32
#include "Amazon\uart_api.h"
#include "Amazon\amazon2_timer_api.h"
#include "Amazon\Memory.h"

#include "Huro\Huro.h"
#include "Huro\HuroComm.h"
#include "Taekwon\Taekwon.h"
#include "Taekwon\TaekwonComm.h"
#include "Amazon\ImageConvert.h"
#include "Amazon\imagePixel.h"

#define AMAZON2_GRAPHIC_VERSION		"v0.5"000000000000000000000000

#define BIO_PRM_UART_SPEED 57600
#define MF_UART_SPEED 4800

static int show_help(char *);

int (*SoC_Robotwar_Main)();
int imageSaveMode = 0;

int main(int argc, char **argv)
{
	int selectMode = 0;
	int isFirstOptionSeleted = 0;
	//	printf("SoC Robot War Start\n", argc);

	CommonMemoryAllocate();

#ifndef _WIN32
	if (open_graphic() < 0) {
		return -1;
	}
	printf("Graphic Open\n");

	direct_camera_display_off();
	if (direct_camera_display_stat() > 0) {
		return -1;
	}
	printf("Camera Open\n");

	clear_screen();
	flip();
	clear_screen();
#endif // WIN32

#ifndef _WIN32
	if (uart_open() < 0) {
		return -1;
	} else {
		// 버퍼 초기화
		uart_txbuf_clear(UART1);
		uart_rxbuf_clear(UART1);
	}
	printf("UART ON\n");	

#endif // WIN32

	while((selectMode = getopt(argc, argv, "tThHmMcCiInN")) != -1) {
		switch (selectMode) {
		case 't':
		case 'T':
			// Taekwon
			uart_config(UART1, BIO_PRM_UART_SPEED, 8, UART_PARNONE, 1);
			SoC_Robotwar_Main = taekwon_main;
			isFirstOptionSeleted++;
			break;
		case 'h':
		case 'H':			
			// Huro Competition
			uart_config(UART1, MF_UART_SPEED, 8, UART_PARNONE, 1);
			SoC_Robotwar_Main = huro_main;
			isFirstOptionSeleted++;
			break;
		case 'm':
		case 'M':
			// Mission Test
			uart_config(UART1, MF_UART_SPEED, 8, UART_PARNONE, 1);
			SoC_Robotwar_Main = huro_mission;
			isFirstOptionSeleted++;
			break;
		case 'c':
		case 'C':
			// Continuous Mission Test
			uart_config(UART1, MF_UART_SPEED, 8, UART_PARNONE, 1);
			SoC_Robotwar_Main = huro_continuous_mission;
			isFirstOptionSeleted++;
			break;
		case 'i':
		case 'I':
			// Image Capture Mode On
			imageSaveMode = 1;
			break;
		case 'n':
		case 'N':
			// Don't Use Motion
			noMotionMode = 1;
			break;
		default:
			show_help(argv[0]);
			break;
		}
	}

	if (isFirstOptionSeleted == 1) {
		SoC_Robotwar_Main();
	} else {
		show_help(argv[0]);
	}

#ifndef _WIN32
	close_graphic();
#endif // WIN32

	return 0;
}

int show_help(char *name)
{
	printf("Usage: ./%s [OPTION]\n", name);
	printf("First Options (Just Only One)\n");
	printf(" t : Taekwon\n");
	printf(" h : Huro Competition\n");
	printf(" m : Huro Mission Test\n");
	printf(" c : Continuous Mission Test\n");

	printf("Second Options\n");
	printf(" i : Capture Image\n");
	printf(" n : No Motion\n");	

	printf("Example\n");
	printf("Usage: ./%s -t -i -n\n", name);
	printf("Taekwon Mode, Image Capture Mode, No Motion Mode ON\n");

	return 0;
}
