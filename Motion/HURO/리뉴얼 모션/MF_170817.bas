'******** 메탈파이터 기본형 프로그램 ********

DIM I AS BYTE
DIM GRADIENT_VAL AS BYTE
DIM A AS BYTE
DIM A_old AS BYTE
DIM Send AS BYTE
DIM MOTOR_ONOFF AS BYTE
DIM GYRO_ONOFF AS BYTE
DIM GRADIENT_FB AS INTEGER
DIM GRADIENT_LR AS INTEGER
DIM DELAY_TIME AS BYTE
DIM DELAY_TIME2 AS BYTE
DIM GRADIENT_CHECK_COUNT AS BYTE
DIM FALLDOWN_CHECK AS BYTE

DIM 기울기센서측정여부 AS BYTE

'*** GYRO Setting  ***********************************
GOSUB GYRO_INIT
GOSUB GYRO_MID

'**** 기울기센서포트 설정

CONST 앞뒤기울기AD포트 = 0
CONST 좌우기울기AD포트 = 1
CONST 기울기확인시간 = 10  'ms

CONST min = 61			'뒤로넘어졌을때
CONST max = 107			'앞으로넘어졌을때
CONST COUNT_MAX = 20
CONST 하한전압 = 103	'약6V전압

PTP SETON 				'단위그룹별 점대점동작 설정
PTP ALLON				'전체모터 점대점 동작 설정

DIR G6A, 1, 0, 0, 1, 0, 0		'모터0~5번
DIR G6B, 1, 1, 1, 1, 1, 1		'모터6~11번
DIR G6C, 0, 0, 0, 0, 0, 0		'모터12~17번
DIR G6D, 0, 1, 1, 0, 1, 0		'모터18~23번


'***** 초기선언 *********************************
MOTOR_ONOFF = 0
GRADIENT_CHECK_COUNT = 0
FALLDOWN_CHECK = 0
기울기센서측정여부 = 1

'****초기위치 *****************************
OUT 52, 0
GOSUB MOTOR_ON
HIGHSPEED SETOFF
SPEED 5
GOSUB GYRO_INIT
GOSUB 전원초기자세
'GOSUB 안정화자세
GOSUB BASIC

GOTO MAIN
'************************************************


'************ 자이로 ****************************

GYRO_INIT:
    '    GYRODIR G6A, 0, 0, 0, 0, 1
    '    GYRODIR G6D, 1, 0, 0, 0, 0
    GYRODIR G6A, 0, 0, 1, 0, 0
    GYRODIR G6D, 1, 0, 1, 0, 0
    GYRODIR G6B, 1, 0, 0, 0, 0
    GYRODIR G6C, 1, 0, 0, 0, 0

    GYROSENSE G6A, 200, 150, 30, 150, 0
    GYROSENSE G6D, 200, 150, 30, 150, 0

    RETURN
    '***********************************************
GYRO_MAX:

    'GYROSENSE G6A, 255, 255, 255, 255
    'GYROSENSE G6D, 255, 255, 255, 255
    GYROSENSE G6A, 250, 180, 30, 180, 0
    GYROSENSE G6D, 250, 180, 30, 180, 0

    RETURN
    '***********************************************
GYRO_MID:
    'GYROSENSE G6A, 255, 100, 100, 100
    'GYROSENSE G6D, 255, 100, 100, 100,
    GYROSENSE G6A, 200, 150, 30, 150, 0
    GYROSENSE G6D, 200, 150, 30, 150, 0
    RETURN
    '***********************************************
    '***********************************************
GYRO_MIN:
    GYROSENSE G6A, 200, 100, 30, 100, 0
    GYROSENSE G6D, 200, 100, 30, 100, 0
    'GYROSENSE G6A, 50, 50, 50, 50
    'GYROSENSE G6D, 50, 50, 50, 50
    RETURN
    '***********************************************
    'GYRO_ST:
    '    GYROSENSE G6A, 100, 30, 30, 30,
    '    GYROSENSE G6D, 100, 30, 30, 30,
    '    RETURN

    '***********************************************
GYRO_ON:
    'GYROSET G6A, 2, 1, 1, 1,
    'GYROSET G6D, 2, 1, 1, 1,
    GYROSET G6A, 4, 3, 3, 3,
    GYROSET G6D, 4, 3, 3, 3,
    GYRO_ONOFF = 1
    RETURN
    '***********************************************
GYRO_OFF:
    GYROSET G6A, 0, 0, 0, 0, 0
    GYROSET G6D, 0, 0, 0, 0, 0
    GYRO_ONOFF = 0
    RETURN

    '************************************************

MOTOR_FAST_ON:
    MOTOR G6B
    MOTOR G6C
    MOTOR G6A
    MOTOR G6D

    MOTOR_ONOFF = 0
    RETURN
    '************************************************
MOTOR_ON:
    GOSUB MOTOR_GET

    MOTOR G6B
    DELAY 50
    MOTOR G6C
    DELAY 50
    MOTOR G6A
    DELAY 50
    MOTOR G6D

    MOTOR_ONOFF = 0
    GOSUB START_SOUND
    RETURN

    '************************************************
    '전포트서보모터사용설정
MOTOR_OFF:
    MOTOROFF G6B
    MOTOROFF G6C
    MOTOROFF G6A
    MOTOROFF G6D
    MOTOR_ONOFF = 1	
    GOSUB MOTOR_GET	
    GOSUB END_SOUND
    RETURN
    '************************************************

    '위치값피드백
MOTOR_GET:
    GETMOTORSET G6A, 1, 1, 1, 1, 1, 0
    GETMOTORSET G6B, 1, 1, 1, 0, 0, 1
    GETMOTORSET G6C, 1, 1, 1, 0, 0, 0
    GETMOTORSET G6D, 1, 1, 1, 1, 1, 0
    RETURN

    '************************************************
All_motor_Reset:

    MOTORMODE G6A, 1, 1, 1, 1, 1
    MOTORMODE G6D, 1, 1, 1, 1, 1
    MOTORMODE G6B, 1, 1, 1, , , 1
    MOTORMODE G6C, 1, 1, 1

    RETURN
    '************************************************
All_motor_mode2:

    MOTORMODE G6A, 2, 2, 2, 2, 2
    MOTORMODE G6D, 2, 2, 2, 2, 2
    MOTORMODE G6B, 2, 2, 2, , , 2
    MOTORMODE G6C, 2, 2, 2

    RETURN
    '************************************************
All_motor_mode3:

    MOTORMODE G6A, 3, 3, 3, 3, 3
    MOTORMODE G6D, 3, 3, 3, 3, 3
    MOTORMODE G6B, 3, 3, 3, , , 3
    MOTORMODE G6C, 3, 3, 3

    RETURN
    '************************************************

Leg_motor_mode1:
    MOTORMODE G6A, 1, 1, 1, 1, 1
    MOTORMODE G6D, 1, 1, 1, 1, 1
    RETURN
    '************************************************
Leg_motor_mode2:
    MOTORMODE G6A, 2, 2, 2, 2, 2
    MOTORMODE G6D, 2, 2, 2, 2, 2
    RETURN

    '************************************************
Leg_motor_mode3:
    MOTORMODE G6A, 3, 3, 3, 3, 3
    MOTORMODE G6D, 3, 3, 3, 3, 3
    RETURN
    '************************************************
Leg_motor_mode4:
    MOTORMODE G6A, 3, 2, 2, 1, 3
    MOTORMODE G6D, 3, 2, 2, 1, 3
    RETURN
    '************************************************
Leg_motor_mode5:
    MOTORMODE G6A, 3, 2, 2, 1, 2
    MOTORMODE G6D, 3, 2, 2, 1, 2
    RETURN
    '************************************************
    '************************************************
Arm_motor_mode1:
    MOTORMODE G6B, 1, 1, 1
    MOTORMODE G6C, 1, 1, 1
    RETURN
    '************************************************
Arm_motor_mode2:
    MOTORMODE G6B, 2, 2, 2
    MOTORMODE G6C, 2, 2, 2
    RETURN
    '************************************************
Arm_motor_mode3:
    MOTORMODE G6B, 3, 3, 3
    MOTORMODE G6C, 3, 3, 3
    RETURN

    '************************************************
    '*******기본자세관련*****************************
    '************************************************
전원초기자세:
    'GOSUB GYRO_OFF
    GOSUB GYRO_ON
    MOVE G6A, 95, 70, 145, 100, 105, 100
    MOVE G6D, 95, 70, 145, 100, 105, 100
    MOVE G6B, 100, 45, 90, 100, 100, 100
    MOVE G6C, 100, 45, 90, 100, 135, 100
    WAIT

    RETURN
    '************************************************
안정화자세:

    GOSUB GYRO_OFF
    '    MOVE G6A, 100, 70, 145, 100, 99, 100
    '    MOVE G6D, 100, 72, 145, 99, 100, 100
    '    MOVE G6B, 100, 30, 80, 100, 100, 100
    '    MOVE G6C, 100, 30, 80, 100, 100, 100
    '    WAIT
    MOVE G6A, 98, 70, 145, 98, 100, 100
    MOVE G6D, 98, 70, 145, 98, 100, 100
    MOVE G6B, 100, 30, 80, 100, 100, 100
    MOVE G6C, 100, 30, 80, 100, 135, 100
    WAIT

    RETURN
    '************************************************
자이로기본자세:
    GOSUB GYRO_ON
    'MOVE G6A, 98, 76, 145, 93, 101, 100
    'MOVE G6D, 99, 75, 145, 93, 101,
    MOVE G6A, 98, 76, 145, 98, 101, 100
    MOVE G6D, 99, 78, 143, 98, 100, 100
    MOVE G6B, 100, 30, 80, 100, 100, 100
    MOVE G6C, 100, 30, 80, 100, 100, 100
    WAIT

    GOSUB GYRO_OFF

    RETURN
    '************************************************	
BASIC:
    GOSUB GYRO_OFF
    MOVE G6A, 100, 70, 145, 100, 100
    MOVE G6D, 100, 70, 145, 100, 100
    MOVE G6B, 100, 30, 80, , , 100
    MOVE G6C, 100, 30, 80, , 135
    WAIT

    RETURN
    '************************************************

RX_EXIT:
    ERX 4800, A, MAIN

    GOTO RX_EXIT
    '************************************************

GOSUB_RX_EXIT:
    ERX 4800, A, GOSUB_RX_EXIT2
    GOTO GOSUB_RX_EXIT

GOSUB_RX_EXIT2:
    RETURN
    '************************************************

    '************************************************
    '******* 피에조 소리 관련 ***********************
    '************************************************
START_SOUND:
    TEMPO 220
    MUSIC "O23EAB7EA>3#C"''"O32B2>D1#F#F 2E2D<1BB 3B>2D2#f2#f2D2E2D<1B"
    RETURN
    '************************************************
END_SOUND:
    TEMPO 220
    MUSIC "O38GD<BGD<BG"
    RETURN
    '************************************************
ERROR_SOUND:
    TEMPO 180
    MUSIC "O13A"
    DELAY 300

    RETURN
    '************************************************
BEEP_SOUND:
    TEMPO 180
    MUSIC "A"
    DELAY 300

    RETURN
    '************************************************


앞뒤기울기측정:
    FOR i = 0 TO COUNT_MAX
        A = AD(앞뒤기울기AD포트)	'기울기 앞뒤
        IF A > 250 OR A < 5 THEN RETURN
        IF A > MIN AND A < MAX THEN RETURN
        DELAY 기울기확인시간
    NEXT i

    IF A < MIN THEN GOSUB 기울기앞
    IF A > MAX THEN GOSUB 기울기뒤

    RETURN
    '**************************************************
기울기앞:
    GRADIENT_VAL = AD(앞뒤기울기AD포트)
    'IF A < MIN THEN GOSUB 앞으로일어나기
    'IF GRADIENT_VAL < MIN THEN  GOSUB 뒤로일어나기
    RETURN

기울기뒤:
    GRADIENT_VAL = AD(앞뒤기울기AD포트)
    'IF A > MAX THEN GOSUB 뒤로일어나기
    'IF GRADIENT_VAL > MAX THEN GOSUB 앞으로일어나기
    RETURN
    '**************************************************

좌우기울기측정:
    FOR i = 0 TO COUNT_MAX
        GRADIENT_VAL = AD(좌우기울기AD포트)	'기울기 좌우
        IF GRADIENT_VAL > 250 OR GRADIENT_VAL < 5 THEN RETURN
        IF GRADIENT_VAL > MIN AND GRADIENT_VAL < MAX THEN RETURN
        DELAY 기울기확인시간
    NEXT i

    IF GRADIENT_VAL < MIN OR GRADIENT_VAL > MAX THEN
        SPEED 8
        MOVE G6B, 140, 40, 80
        MOVE G6C, 140, 40, 80
        WAIT
        GOSUB BASIC	
        RETURN

    ENDIF
    RETURN
    '**************************************************

MOTOR_ONOFF_LED:
    IF MOTOR_ONOFF = 1  THEN
        OUT 52, 1
        DELAY 200
        OUT 52, 0
        DELAY 200
    ENDIF
    RETURN
    '**************************************************

    '************************************************
    '************ 2017 MOTION ***********************
    '************************************************

    '**************************************************
HEAD_DOWN_0:
    GOSUB GYRO_OFF
    MOVE G6A, 100, 70, 145, 100, 100, 100
    MOVE G6D, 100, 70, 145, 100, 100, 100
    MOVE G6B, 100, 30, 80, 100, 100, 100
    MOVE G6C, 100, 30, 80, 100, 100, 100
    WAIT

    RETURN
    '**************************************************
HEAD_DOWN_20:
    GOSUB GYRO_OFF
    MOVE G6A, 100, 70, 145, 100, 100, 100
    MOVE G6D, 100, 70, 145, 100, 100, 100
    MOVE G6B, 100, 30, 80, 100, 100, 100
    MOVE G6C, 100, 30, 80, 100, 120, 100
    WAIT

    RETURN
    '**************************************************
HEAD_DOWN_80:
    GOSUB GYRO_OFF
    MOVE G6A, 100, 70, 145, 100, 100, 100
    MOVE G6D, 100, 70, 145, 100, 100, 100
    MOVE G6B, 100, 30, 80, 100, 100, 100
    MOVE G6C, 100, 30, 80, 100, 180, 100
    WAIT

    RETURN
    '**************************************************
HEAD_DOWN_35_SIDE_90:
    GOSUB GYRO_OFF
    MOVE G6A, 100, 70, 145, 100, 100, 100
    MOVE G6D, 100, 71, 145, 100, 100, 100
    MOVE G6B, 100, 30, 80, , 100, 190
    MOVE G6C, 100, 30, 80, , 135, 100
    WAIT

    RETURN
    '**************************************************
MINE_RUN_READY:
    GOSUB All_motor_mode3
    HIGHSPEED SETOFF
    SPEED 15
    GOSUB BASIC
    DELAY 50
    SPEED 7

    '다리 벌리기
    MOVE G6A, 90, 70, 145, 105, 110
    MOVE G6D, 90, 70, 145, 105, 110
    WAIT
    DELAY 100

    SPEED 15
    HIGHSPEED SETON

    '다리 기울이기
    MOVE G6A, 80, 70, 145, 105, 110
    MOVE G6D, 100, 70, 145, 105, 110
    WAIT

    '왼발들기
    MOVE G6A, 80, 80, 125, 115, 110
    MOVE G6D, 100, 70, 145, 105, 110
    MOVE G6C, 80
    MOVE G6B, 120
    WAIT

    '다리 벌리기
    MOVE G6A, 90, 40, 175, 105, 110
    MOVE G6D, 90, 70, 145, 105, 110
    WAIT

    RETURN
    '**************************************************
MINE_RUNNING:

    '다리기울이기 (오른쪽)
    MOVE G6A, 80, 40, 175, 105, 110
    MOVE G6D, 100, 70, 145, 105, 110
    WAIT

    '오른발들기
    MOVE G6D, 80, 80, 125, 115, 110
    MOVE G6A, 100, 70, 145, 105, 110
    MOVE G6B, 80
    MOVE G6C, 120
    WAIT

    '다리 벌리기
    MOVE G6D, 90, 40, 175, 105, 110
    MOVE G6A, 90, 70, 145, 105, 110
    WAIT

    '다리기울이기 (왼쪽)
    MOVE G6D, 80, 40, 175, 105, 110
    MOVE G6A, 100, 70, 145, 105, 110
    WAIT

    '왼발들기
    MOVE G6A, 80, 80, 125, 115, 110
    MOVE G6D, 100, 70, 145, 105, 110
    MOVE G6C, 80
    MOVE G6B, 120
    WAIT

    '다리 벌리기
    MOVE G6A, 90, 70, 145, 105, 110
    MOVE G6D, 90, 70, 145, 105, 110
    WAIT

    RETURN
    '**************************************************
MINE_RUN_END:
    '다리기울이기 (오른쪽)
    MOVE G6A, 80, 40, 175, 105, 110
    MOVE G6D, 100, 70, 145, 105, 110
    WAIT

    '오른발들기
    MOVE G6D, 80, 80, 125, 115, 110
    MOVE G6A, 100, 70, 145, 105, 110
    MOVE G6B, 80
    MOVE G6C, 120
    WAIT

    '다리 벌리기
    MOVE G6D, 90, 40, 175, 105, 110
    MOVE G6A, 90, 70, 145, 105, 110
    WAIT

    GOSUB BASIC
    RETURN
    '**************************************************
MINE_RUN:
    GOSUB MINE_RUN_READY
    FOR i = 0 TO 10
        GOSUB MINE_RUNNING
    NEXT i
    GOSUB MINE_RUN_END
    RETURN
    '**************************************************
    '**************************************************
RUN_READY_D:
    GOSUB All_motor_mode3
    HIGHSPEED SETOFF
    SPEED 15
    ' GOSUB BASIC
    DELAY 50
    SPEED 7

    '다리 벌리기
    MOVE G6A, 98, 70, 145, 105, 102
    MOVE G6D, 98, 70, 145, 105, 102
    WAIT
    DELAY 100

    SPEED 15
    HIGHSPEED SETON

    '다리 기울이기
    MOVE G6A, 88, 70, 145, 105, 102
    MOVE G6D, 108, 70, 145, 105, 102
    WAIT

    '왼발들기
    MOVE G6A, 88, 80, 125, 115, 102
    MOVE G6D, 108, 70, 145, 105, 102
    MOVE G6C, 80
    MOVE G6B, 120
    WAIT

    '다리 벌리기
    MOVE G6A, 98, 40, 175, 105, 102
    MOVE G6D, 98, 70, 145, 105, 102
    WAIT

    RETURN
    '**************************************************
RUNNING_D:

    '다리기울이기 (오른쪽)
    MOVE G6A, 88, 40, 175, 105, 102
    MOVE G6D, 108, 70, 145, 105, 102
    WAIT

    '오른발들기
    MOVE G6D, 88, 80, 125, 115, 102
    MOVE G6A, 108, 70, 145, 105, 102
    MOVE G6B, 80
    MOVE G6C, 120
    WAIT

    '다리 벌리기
    MOVE G6D, 98, 40, 175, 105, 102
    MOVE G6A, 98, 70, 145, 105, 102
    WAIT

    '다리기울이기 (왼쪽)
    MOVE G6D, 88, 40, 175, 105, 102
    MOVE G6A, 108, 70, 145, 105, 102
    WAIT

    '왼발들기
    MOVE G6A, 88, 80, 125, 115, 102
    MOVE G6D, 108, 70, 145, 105, 102
    MOVE G6C, 80
    MOVE G6B, 120
    WAIT

    '다리 벌리기
    MOVE G6A, 98, 70, 145, 105, 102
    MOVE G6D, 98, 70, 145, 105, 102
    WAIT

    RETURN
    '**************************************************
RUN_END_D:
    '다리기울이기 (오른쪽)
    MOVE G6A, 88, 40, 175, 105, 102
    MOVE G6D, 108, 70, 145, 105, 102
    WAIT

    '오른발들기
    MOVE G6D, 88, 80, 125, 115, 102
    MOVE G6A, 108, 70, 145, 105, 102
    MOVE G6B, 80
    MOVE G6C, 120
    WAIT

    '다리 벌리기
    MOVE G6D, 98, 40, 175, 105, 102
    MOVE G6A, 98, 70, 145, 105, 102
    WAIT

    GOSUB HEAD_DOWN_80'BASIC
    RETURN
    '**************************************************
RUN_15_D:
    GOSUB RUN_READY_D
    FOR i = 0 TO 10
        GOSUB RUNNING_D
    NEXT i
    GOSUB RUN_END_D
    RETURN

    '**************************************************

Walk_Ready:
    GOSUB Leg_motor_mode3
    SPEED 4
    MOVE G6A, 88,  74, 144,  95, 110
    MOVE G6D,108,  76, 146,  93,  96
    MOVE G6B,100
    MOVE G6C,100
    WAIT

    SPEED 10'
    MOVE G6A, 90, 90, 120, 105, 110,100
    MOVE G6D,110,  76, 147,  93,  96,100
    MOVE G6B,90
    MOVE G6C,110
    WAIT

    SPEED 10
    MOVE G6A, 86,  56, 145, 115, 110
    MOVE G6D,108,  76, 147,  93,  96
    WAIT


    SPEED 4
    GOSUB Leg_motor_mode3

    MOVE G6A,110,  76, 147, 93,  96
    MOVE G6D,86, 100, 145,  69, 110
    WAIT

    RETURN
    '*******************************

Walking:
    SPEED 10
    MOVE G6A,110,  76, 147,  93, 96,100
    MOVE G6D,90, 90, 120, 105, 110,100
    MOVE G6B,110
    MOVE G6C,90
    WAIT

    SPEED 10
    MOVE G6D, 86,  56, 145, 115, 110
    MOVE G6A,108,  76, 147,  93,  96
    WAIT

    SPEED 4
    MOVE G6D,110,  76, 147, 93,  96
    MOVE G6A,86, 100, 145,  69, 110
    WAIT

    SPEED 10
    MOVE G6A, 86,  56, 145, 115, 110
    MOVE G6D,108,  76, 147,  93,  96
    WAIT

    SPEED 4
    GOSUB Leg_motor_mode3
    MOVE G6A,110,  76, 147, 93,  96
    MOVE G6D,86, 100, 145,  69, 110
    WAIT

    RETURN

    '*******************************

Walk_Left_End:
    SPEED 10
    MOVE G6A, 90, 100, 100, 115, 110,100
    MOVE G6D,112,  76, 146,  93,  96,100
    MOVE G6B,90
    MOVE G6C,110
    WAIT
    HIGHSPEED SETOFF

    SPEED 4
    MOVE G6D, 106,  76, 146,  93,  96,100		
    MOVE G6A,  88,  71, 152,  91, 106,100
    MOVE G6C, 100
    MOVE G6B, 100
    WAIT	

    SPEED 2
    GOSUB HEAD_DOWN_80

    RETURN

Walk_Right_End:
    SPEED 10
    MOVE G6A,110,  76, 146,  93, 96,100
    MOVE G6D,90, 100, 100, 115, 112,100
    MOVE G6B,110
    MOVE G6C,90
    WAIT
    HIGHSPEED SETOFF

    SPEED 8
    MOVE G6A, 106,  76, 146,  93,  96,100		
    MOVE G6D,  88,  71, 152,  91, 106,100
    MOVE G6B, 100
    MOVE G6C, 100
    WAIT

    SPEED 2
    GOSUB HEAD_DOWN_80

    RETURN

Walk_H:
    GOSUB HEAD_DOWN_80
    GOSUB Walk_Ready
    GOSUB Walking
    GOSUB Walk_Right_End

    RETURN

Half_Walk_H:
    GOSUB HEAD_DOWN_80
    GOSUB Walk_Ready
    GOSUB Walk_Right_End

    RETURN
    '**********************************************
Q_Walk_Ready_H:
    GOSUB Leg_motor_mode3
    SPEED 4
    MOVE G6A, 88,  74, 144,  95, 110
    MOVE G6D,108,  76, 146,  93,  96
    MOVE G6B,100
    MOVE G6C,100
    WAIT

    SPEED 7
    MOVE G6A, 92, 90, 120, 105, 113,100
    MOVE G6D,110,  76, 147,  93,  96,100
    MOVE G6B,90
    MOVE G6C,110
    WAIT

    SPEED 7
    MOVE G6A, 88,  61, 145, 110, 110
    MOVE G6D,108,  76, 147,  93,  96
    WAIT

    SPEED 4
    GOSUB Leg_motor_mode3

    MOVE G6A,105,  76, 147, 93,  96
    MOVE G6D, 89, 90, 145,  79, 112
    WAIT

    RETURN

Q_Walk_End_H:
    SPEED 7
    MOVE G6A,109,  76, 146,  93, 96,100
    MOVE G6D,90, 92, 125, 100, 114,100
    MOVE G6B,110
    MOVE G6C,90
    WAIT
    HIGHSPEED SETOFF

    '    SPEED 7
    '    MOVE G6A, 106,  76, 146,  93,  96,100		
    '    MOVE G6D,  88,  71, 152,  91, 106,100
    '    MOVE G6B, 100
    '    MOVE G6C, 100
    '    WAIT

    SPEED 2
    GOSUB HEAD_DOWN_80

    RETURN

Q_Walk:
    GOSUB HEAD_DOWN_80
    GOSUB Q_Walk_Ready_H
    GOSUB Q_Walk_End_H

    RETURN

    '**********************************************
Rolling_Hurdle:
    GOSUB GYRO_OFF
    GOSUB Leg_motor_mode3


    SPEED 15			'고개 숙이며 팔벌리기

    MOVE G6A,  98,  73, 145,  96, 100, 100
    MOVE G6D, 100,  75, 144,  94,  98, 100
    MOVE G6B,  100,  100,  81, 100, 100, 100
    MOVE G6C,  100,  100,  81, 100, 185,
    WAIT

    SPEED 15			'팔벌리기
    MOVE G6A,  98,  73, 145,  96, 100, 100
    MOVE G6D, 100,  75, 144,  94,  98, 100
    MOVE G6B,  188,  100,  81, 100, 100, 100
    MOVE G6C,  190,  100,  81, 100, 185, 100
    WAIT

    SPEED 15			'팔앞으로 나란히
    MOVE G6A,  98,  73, 145,  96, 100, 100
    MOVE G6D, 100,  75, 144,  94,  98, 100
    MOVE G6B,  183,  40,  70, 100, 100, 100
    MOVE G6C,  185,  40,  70, 100, 185, 100
    WAIT

    'SPEED 6			'무릎살짝굽히기
    'MOVE G6A, 100,  80, 127, 110, 100, 100
    'MOVE G6D, 102,  80, 124, 111,  98, 100
    'WAIT

    SPEED 3 		'허리숙이기
    MOVE G6A, 100,  90, 127, 160, 100, 100
    MOVE G6D, 102,  94, 124, 161,  98, 100
    WAIT

    SPEED 15
    MOVE G6A,100,  56, 110,  26, 100, 100
    MOVE G6D,100,  71, 177, 162, 100, 100
    MOVE G6B,170,  40,  70
    MOVE G6C,170,  40,  70
    WAIT

    SPEED 15
    MOVE G6A,100,  60, 110,  15, 100, 100
    MOVE G6D,100,  70, 120, 30, 100, 100
    MOVE G6B,170,  40,  70
    MOVE G6C,170,  40,  70
    WAIT

    SPEED 15
    MOVE G6A,100,  60, 110,  15, 100, 100
    MOVE G6D,100,  60, 110,  15, 100, 100
    MOVE G6B,190,  40,  70
    MOVE G6C,190,  40,  70
    WAIT
    DELAY 50

    SPEED 15 '다리 돌아가기
    MOVE G6A,100, 110, 70,  65, 100, 100
    MOVE G6D,100, 110, 70,  65, 100, 100
    MOVE G6B,190, 160, 115
    MOVE G6C,190, 160, 115
    WAIT

    SPEED 15
    MOVE G6A,100, 170,  70,  15, 100, 100
    MOVE G6D,100, 170,  70,  15, 100, 100
    MOVE G6B,190, 155, 120
    MOVE G6C,190, 155, 120
    WAIT

    SPEED 12
    MOVE G6A,100, 136,  35, 80, 100,
    MOVE G6D,100, 136,  35, 80, 100,
    WAIT

    SPEED 12
    MOVE G6A,100, 165,  70, 30, 100,
    MOVE G6D,100, 165,  70, 30, 100,
    WAIT

    GOSUB Leg_motor_mode3
    SPEED 10
    MOVE G6A,100, 165,  45, 90, 100,
    MOVE G6D,100, 165,  45, 90, 100,
    MOVE G6B,130,  50,  60
    MOVE G6C,130,  50,  60
    WAIT

    GOSUB BASIC
    RETURN


Rolling_Trap:
    GOSUB GYRO_ON
    GOSUB Leg_motor_mode3

    '앉기
    SPEED 10
    MOVE G6A,100, 143,  28, 142, 100, 100
    MOVE G6D,100, 143,  28, 142, 100, 100
    MOVE G6B,100,  30,  80
    MOVE G6C,100,  30,  80
    WAIT

    '팔뻗기
    SPEED 10
    MOVE G6B,  183,  60,  70, 100, 100, 100
    MOVE G6C,  185,  60,  70, 100, 185, 100
    WAIT


    SPEED 10
    MOVE G6A,100, 143,  28, 162, 100, 100
    MOVE G6D,100, 143,  28, 162, 100, 100
    WAIT

    '    SPEED 10
    '    MOVE G6A,100, 143,  78, 142, 100, 100
    '    MOVE G6D,100, 143,  78, 142, 100, 100
    '    MOVE G6B,  173,  55,  70, 100, 100, 100
    '    MOVE G6C,  175,  55,  70, 100, 185, 100
    '    WAIT
    '    DELAY 500


    SPEED 10
    MOVE G6A,100, 123,  128, 132, 100, 100
    MOVE G6D,100, 123,  128, 132, 100, 100
    MOVE G6B,  173,  55,  70, 100, 100, 100
    MOVE G6C,  175,  55,  70, 100, 185, 100
    WAIT
    DELAY 500

    '다리올리기
    SPEED 15
    HIGHSPEED SETON
    MOVE G6A,100,  56, 110,  26, 100, 100
    MOVE G6D,100,  71, 177, 162, 100, 100
    MOVE G6B,  173,  55,  70, 100, 100, 100
    MOVE G6C,  175,  55,  70, 100, 185, 100
    WAIT

    'HIGHSPEED SETON
    'SPEED 10
    MOVE G6A,100,  60, 110,  15, 100, 100
    MOVE G6D,100,  70, 120, 30, 100, 100
    WAIT


    'SPEED 10
    MOVE G6A,100,  60, 103,  13, 100, 100
    MOVE G6D,100,  60, 100,  13, 100, 100
    MOVE G6B,  173,  60,  70, 100, 100, 100
    MOVE G6C,  175,  60,  70, 100, 185, 100


    '   SPEED 10
    '   MOVE G6B,  163,  60,  70, 100, 100, 100
    '   MOVE G6C,  165,  60,  70, 100, 185, 100
    '   WAIT

    HIGHSPEED SETOFF

    '    SPEED 15 '다리 돌아가기
    '    MOVE G6A,100, 110, 70,  65, 100, 100
    '    MOVE G6D,100, 110, 70,  65, 100, 100

    '    MOVE G6A,100, 170,  70,  15, 100, 100ㅊ

    '    GOSUB 기본자세
    GOSUB GYRO_OFF
    RETURN


제자리걸음:
    GOSUB All_motor_mode3
    HIGHSPEED SETOFF
    SPEED 15
    GOSUB BASIC
    DELAY 50
    SPEED 12

    '다리 벌리기
    MOVE G6A, 98, 70, 145, 100, 102
    MOVE G6D, 98, 70, 145, 100, 102
    WAIT
    DELAY 100

    HIGHSPEED SETON
    FOR i = 0 TO 4
        '다리 기울이기
        MOVE G6A, 88, 70, 145, 100, 102
        MOVE G6D, 108, 70, 145, 100, 102
        WAIT

        '왼발들기
        MOVE G6A, 88, 80, 125, 110, 102
        MOVE G6D, 108, 70, 145, 100, 102
        MOVE G6C, 80
        MOVE G6B, 120
        WAIT

        '다리 벌리기
        MOVE G6A, 98, 70, 145, 100, 102
        MOVE G6D, 98, 70, 145, 100, 102
        WAIT

        '다리 기울이기
        MOVE G6D, 88, 70, 145, 100, 102
        MOVE G6A, 108, 70, 145, 100, 102
        WAIT

        '오른발들기
        MOVE G6D, 88, 80, 125, 110, 102
        MOVE G6A, 108, 70, 145, 100, 102
        MOVE G6B, 80
        MOVE G6C, 120
        WAIT

        '다리 벌리기
        MOVE G6A, 98, 70, 145, 100, 102
        MOVE G6D, 98, 70, 145, 100, 102
        WAIT
    NEXT i

    '다리벌린자세
    MOVE G6A, 95, 70, 145, 100, 102
    MOVE G6D, 95, 70, 145, 100, 102
    WAIT

    GOSUB BASIC

    RETURN

    '**************************************************

계단올라가기4CM:
    HIGHSPEED SETOFF
    GOSUB GYRO_OFF
    GOSUB All_motor_mode3

    SPEED 4
    MOVE G6A, 100, 120, 95, 100, 100
    MOVE G6D, 100, 120, 95, 100, 100
    MOVE G6B, 120, 30, 80, , , 100
    MOVE G6C, 120, 30, 80, , 100
    WAIT

    SPEED 4
    MOVE G6A, 100, 120, 95, 100, 100
    MOVE G6D, 100, 120, 95, 100, 100
    MOVE G6B, 140, 30, 80, , , 100
    MOVE G6C, 140, 30, 80, , 170
    WAIT


    SPEED 3
    MOVE G6A, 100, 80, 145, 160, 100
    MOVE G6D, 100, 80, 145, 160, 100
    MOVE G6B, 170, 30, 80, , , 100
    MOVE G6C, 170, 30, 80, , 170
    WAIT

    SPEED 3
    MOVE G6A, 100, 130, 100, 150, 100
    MOVE G6D, 100, 130, 100, 150, 100
    MOVE G6B, 130, 30, 80, , , 100
    MOVE G6C, 130, 30, 80, , 170
    WAIT

    GOSUB Arm_motor_mode1
    SPEED 5
    MOVE G6A, 100, 80, 145, 160, 100
    MOVE G6D, 100, 80, 145, 160, 100
    MOVE G6B, 145, 50, 60, , , 100
    MOVE G6C, 145, 50, 60, , 170
    WAIT

    SPEED 10
    MOVE G6A, 100, 80, 145, 160, 100
    MOVE G6D, 100, 60, 130, 40, 100
    MOVE G6B, 175, 50, 60, , , 100
    MOVE G6C, 175, 50, 60, , 170
    WAIT

    SPEED 10
    MOVE G6A, 100, 80, 100, 20, 100
    MOVE G6D, 100, 60, 100, 20, 100
    MOVE G6B, 175, 50, 60, , , 100
    MOVE G6C, 175, 50, 60, , 170
    WAIT

    HIGHSPEED SETON
    SPEED 15
    MOVE G6A, 100, 60, 100, 20, 100
    MOVE G6D, 100, 60, 100, 20, 100
    MOVE G6B, 185, 20, 80, , , 100
    MOVE G6C, 185, 20, 80, , 185
    WAIT
    HIGHSPEED SETOFF

    GOSUB All_motor_Reset
    SPEED 10
    MOVE G6A, 100, 140, 60, 40, 100
    MOVE G6D, 100, 140, 60, 40, 100
    MOVE G6B, 185, 140, 100, , , 100
    MOVE G6C, 185, 140, 100, , 185
    WAIT

    MOVE G6A, 100, 160, 60, 40, 100
    MOVE G6D, 100, 160, 60, 40, 100
    MOVE G6B, 185, 180, 110, , , 100
    MOVE G6C, 185, 180, 110, , 185
    WAIT

    MOVE G6A, 100, 150, 40, 120, 100
    MOVE G6D, 100, 150, 40, 120, 100
    MOVE G6B, 185, 20, 90, , , 100
    MOVE G6C, 185, 20, 90, , 185

    GOSUB All_motor_Reset
    SPEED 5
    GOSUB BASIC
    RETURN

Rise_Up_Back:
    HIGHSPEED SETOFF
    SPEED 15
    GOSUB BASIC

    MOVE G6A,90, 130, 120,  80, 110, 100
    MOVE G6D,90, 130, 120,  80, 110, 100
    MOVE G6B,150, 160,  10, 100, 100, 100
    MOVE G6C,150, 160,  10, 100, 100, 100
    WAIT

    MOVE G6B,170, 140,  10, 100, 100, 100
    MOVE G6C,170, 140,  10, 100, 100, 100
    WAIT

    MOVE G6B,185,  20, 70,  100, 100, 100
    MOVE G6C,185,  20, 70,  100, 100, 100
    WAIT

    SPEED 10
    MOVE G6A, 80, 155,  85, 150, 150, 100
    MOVE G6D, 80, 155,  85, 150, 150, 100
    MOVE G6B,185,  20, 70,  100, 100, 100
    MOVE G6C,185,  20, 70,  100, 100, 100
    WAIT

    MOVE G6A, 75, 162,  55, 162, 155, 100
    MOVE G6D, 75, 162,  59, 162, 155, 100
    MOVE G6B,188,  10, 100, 100, 100, 100
    MOVE G6C,188,  10, 100, 100, 100, 100
    WAIT

    SPEED 10
    MOVE G6A, 60, 162,  30, 162, 145, 100
    MOVE G6D, 60, 162,  30, 162, 145, 100
    MOVE G6B,170,  10, 100, 100, 100, 100
    MOVE G6C,170,  10, 100, 100, 100, 100
    WAIT

    GOSUB Leg_motor_mode3	
    MOVE G6A, 60, 150,  28, 155, 140, 100
    MOVE G6D, 60, 150,  28, 155, 140, 100
    MOVE G6B,150,  60,  90, 100, 100, 100
    MOVE G6C,150,  60,  90, 100, 100, 100
    WAIT

    MOVE G6A,100, 150,  28, 140, 100, 100
    MOVE G6D,100, 150,  28, 140, 100, 100
    MOVE G6B,130,  50,  85, 100, 100, 100
    MOVE G6C,130,  50,  85, 100, 100, 100
    WAIT
    DELAY 100


    WAIT
    SPEED 10
    GOSUB BASIC
    RETURN

    '****************************************************
Turn_Left_5:
    HIGHSPEED SETON
    GOSUB GYRO_OFF
    GOSUB Leg_motor_mode3

    SPEED 7
    MOVE G6A, 100, 100, 145, 100, 100
    MOVE G6D, 100,  50, 145, 100, 100
    WAIT

    SPEED 7
    MOVE G6A, 110, 100, 145, 101, 100
    MOVE G6D, 110,  50, 145, 101, 100
    WAIT

    SPEED 8
    GOSUB BASIC
    RETURN

Turn_Right_5:
    HIGHSPEED SETON
    GOSUB GYRO_OFF
    GOSUB Leg_motor_mode3

    SPEED 7
    MOVE G6D, 100, 100, 145, 100, 100
    MOVE G6A, 100,  50, 145, 100, 100
    WAIT

    SPEED 7
    MOVE G6D, 110, 100, 145,  99, 100
    MOVE G6A, 110,  50, 145,  99, 100
    WAIT

    SPEED 8
    GOSUB BASIC
    RETURN

Turn_Left_10: '앞으로 전진
    HIGHSPEED SETON
    GOSUB GYRO_OFF
    GOSUB Leg_motor_mode3

    SPEED 7
    MOVE G6A, 100, 100, 145, 100, 100
    MOVE G6D, 100,  50, 145, 100, 100
    WAIT

    SPEED 7
    MOVE G6A, 119, 100, 145, 101, 100
    MOVE G6D, 119,  50, 145, 101, 100
    WAIT

    SPEED 8
    GOSUB BASIC
    RETURN

Turn_Right_10:
    HIGHSPEED SETON
    GOSUB GYRO_OFF
    GOSUB Leg_motor_mode3

    SPEED 7
    MOVE G6D, 100, 100, 145, 98, 100
    MOVE G6A, 100,  50, 145, 98, 100
    WAIT

    SPEED 7
    MOVE G6D, 119, 100, 145,  96, 100
    MOVE G6A, 119,  50, 145,  96, 100
    WAIT

    SPEED 8
    GOSUB BASIC
    RETURN

Turn_Left_30: '앞으로 전진
    HIGHSPEED SETOFF
    GOSUB GYRO_OFF

    SPEED 10
    MOVE G6A,95, 100, 145,  68, 100, 100
    MOVE G6D,95,  42, 145,  125, 105, 98
    WAIT

    SPEED 12
    MOVE G6A,93,  100, 145,  68, 100, 100
    MOVE G6D,93,  40, 145,  125, 105, 100
    WAIT

    SPEED 8
    GOSUB BASIC
    RETURN

Turn_Right_30: '앞으로 전진
    HIGHSPEED SETOFF
    GOSUB GYRO_OFF

    SPEED 10
    MOVE G6D,95, 100, 145,  68, 100, 100
    MOVE G6A,95,  42, 145, 125, 105,  98
    WAIT

    SPEED 12
    MOVE G6D,93, 100, 145,  68, 100, 100
    MOVE G6A,93,  40, 145, 125, 105, 100
    WAIT

    SPEED 8
    GOSUB BASIC
    RETURN

Turn_Left_45:
    HIGHSPEED SETOFF
    GOSUB GYRO_OFF
    GOSUB Leg_motor_mode3
    HIGHSPEED SETON
    SPEED 7
    MOVE G6A,95, 110, 145,  60, 105, 100
    MOVE G6D,95,  30, 145, 140, 105, 100
    WAIT
    HIGHSPEED SETOFF
    SPEED 10
    MOVE G6A,90, 110, 145,  60, 105, 100
    MOVE G6D,90,  30, 145, 140, 105, 100
    WAIT

    SPEED 7
    GOSUB BASIC
    RETURN

SideMove_Left_10:

    RETURN

SideMove_Right_10:

    RETURN

SideMove_Left_50:
    HIGHSPEED SETOFF
    DELAY  10

    SPEED 12
    MOVE G6A, 90,  84, 120, 112, 110, 100
    MOVE G6D, 96,  70, 145, 100, 105, 100
    'MOVE G6C,100,  40
    'MOVE G6B,100,  40
    WAIT

    SPEED 15
    MOVE G6A, 98,  70, 145, 100, 100, 100
    MOVE G6D, 98,  70, 145, 100, 100, 100
    WAIT

    SPEED 12
    GOSUB BASIC
    WAIT

    RETURN

SideMove_Right_50:
    HIGHSPEED SETOFF
    DELAY  10

    SPEED 12
    MOVE G6D, 90,  84, 120, 112, 110, 100
    MOVE G6A, 96,  70, 145, 100, 105, 100
    'MOVE G6C,100,  40
    'MOVE G6B,100,  40
    WAIT

    SPEED 15
    MOVE G6D, 98,  70, 145, 100, 100, 100
    MOVE G6A, 98,  70, 145, 100, 100, 100
    WAIT

    SPEED 12
    GOSUB BASIC
    WAIT

    RETURN

SideMove_Left_40:
    HIGHSPEED SETOFF
    DELAY  10

    SPEED 12
    MOVE G6A, 90,  84, 120, 112, 110, 100
    MOVE G6D, 96,  70, 145, 100, 105, 100
    'MOVE G6C,100,  40
    'MOVE G6B,100,  40
    WAIT

    SPEED 15
    MOVE G6A,102,  70, 145, 100, 100, 100
    MOVE G6D, 90,  72, 140, 103, 113, 100
    WAIT

    SPEED 15
    MOVE G6A, 98,  70, 145, 100, 100, 100
    MOVE G6D, 98,  70, 145, 100, 100, 100
    WAIT

    SPEED 12
    GOSUB BASIC
    WAIT

    RETURN

SideMove_Right_40:
    HIGHSPEED SETOFF
    DELAY  10

    SPEED 12
    MOVE G6D, 90,  84, 120, 112, 110, 100
    MOVE G6A, 96,  70, 145, 100, 105, 100
    'MOVE G6C,100,  40
    'MOVE G6B,100,  40
    WAIT

    SPEED 15
    MOVE G6D,102,  70, 145, 100, 100, 100
    MOVE G6A, 90,  72, 140, 103, 113, 100
    WAIT

    SPEED 15
    MOVE G6D, 98,  70, 145, 100, 100, 100
    MOVE G6A, 98,  70, 145, 100, 100, 100
    WAIT

    SPEED 12
    GOSUB BASIC
    WAIT

    RETURN

SideMove_Left_70:
    HIGHSPEED SETOFF
    DELAY  10

    SPEED 10
    MOVE G6A, 90,  84, 120, 112, 110, 100
    MOVE G6D,100,  70, 145, 100, 107, 100
    'MOVE G6C,100,  40
    'MOVE G6B,100,  40
    WAIT

    SPEED 13
    MOVE G6A,102,  70, 145, 100, 100, 100
    MOVE G6D, 83,  72, 140, 103, 115, 100
    WAIT

    SPEED 13
    MOVE G6A, 98,  70, 145, 100, 100, 100
    MOVE G6D, 98,  70, 145, 100, 100, 100
    WAIT

    SPEED 10
    GOSUB BASIC
    WAIT

    RETURN

SideMove_Right_70:
    HIGHSPEED SETOFF
    DELAY  10

    SPEED 10
    MOVE G6D, 90,  84, 120, 112, 110, 100
    MOVE G6A,100,  70, 145, 100, 107, 100
    'MOVE G6C,100,  40
    'MOVE G6B,100,  40
    WAIT

    SPEED 13
    MOVE G6D,102,  70, 145, 100, 100, 100
    MOVE G6A, 83,  72, 140, 103, 115, 100
    WAIT

    SPEED 13
    MOVE G6D, 98,  70, 145, 100, 100, 100
    MOVE G6A, 98,  70, 145, 100, 100, 100
    WAIT

    SPEED 10
    GOSUB BASIC
    WAIT

    RETURN

Run_Ready_H:
    HIGHSPEED SETON
    GOSUB GYRO_OFF
    GOSUB All_motor_mode3

    SPEED 7
    MOVE G6A, 95,  70, 147, 100, 101
    MOVE G6D,100,  70, 147, 100,  98
    MOVE G6B,100
    MOVE G6C,100
    WAIT

    ' HIGHSPEED SETOFF

    RETURN

Running_H:
    ' HIGHSPEED SETON
    SPEED 7

    MOVE G6A, 95,  84, 125, 107, 104
    MOVE G6D,110,  71, 145, 100, 102
    MOVE G6B, 85
    MOVE G6C,115
    WAIT

    MOVE G6A,105,  67, 140, 110, 100
    MOVE G6D, 95,  79, 145,  92, 102
    WAIT

    MOVE G6D, 95,  84, 125, 107, 104
    MOVE G6A,110,  71, 145, 100, 102
    MOVE G6C, 85
    MOVE G6B,115
    WAIT

    MOVE G6D,105,  67, 140, 110, 100
    MOVE G6A, 95,  79, 145,  92, 102
    WAIT

    'HIGHSPEED SETOFF
    RETURN

Run_End_H:
    '    HIGHSPEED SETON
    '   GOSUB GYRO_OFF
    '    GOSUB Leg_motor_mode3

    SPEED 7
    MOVE G6A, 95,  84, 125, 102, 104
    MOVE G6D,104,  70, 145,  98, 102
    MOVE G6B, 100
    MOVE G6C,100
    WAIT

    HIGHSPEED SETOFF

    SPEED 15
    GOSUB 안정화자세

    SPEED 5
    GOSUB BASIC

    RETURN

RUN_55_H:
    GOSUB Run_Ready_H
    FOR i = 0 TO 10
        GOSUB Running_H
    NEXT i
    GOSUB Run_End_H
    RETURN
    
RUN_45_H:
    GOSUB Run_Ready_H
    FOR i = 0 TO 8
        GOSUB Running_H
    NEXT i
    GOSUB Run_End_H
    RETURN
    
RUN_20_H:
    GOSUB Run_Ready_H
    FOR i = 0 TO 3
        GOSUB Running_H
    NEXT i
    GOSUB Run_End_H
    RETURN
    
RUN_10_H:
    GOSUB Run_Ready_H
    FOR i = 0 TO 1
        GOSUB Running_H
    NEXT i
    GOSUB Run_End_H
    RETURN

FS_Ready_H:
    HIGHSPEED SETON
    GOSUB GYRO_OFF
    GOSUB All_motor_mode3

    HIGHSPEED SETOFF

    RETURN

FSing_H:
    HIGHSPEED SETON
    SPEED 7

    MOVE G6A, 95,  70, 147, 100, 101
    MOVE G6D,100,  70, 147, 100,  98
    MOVE G6B,100
    MOVE G6C,100
    WAIT

    MOVE G6A, 97,  77, 123, 120, 103
    MOVE G6D,102,  69, 134, 113,  98
    MOVE G6B, 80
    MOVE G6C,120
    WAIT

    MOVE G6A, 95,  76, 130, 110, 103
    MOVE G6D,104,  73, 143,  99, 100
    WAIT

    MOVE G6A,100,  79, 130, 110, 104
    MOVE G6D,100,  73, 143,  99,  97
    WAIT

    MOVE G6D, 95,  70, 147, 100, 101
    MOVE G6A,100,  70, 147, 100,  98
    MOVE G6C,100
    MOVE G6B,100
    WAIT

    MOVE G6D, 97,  77, 123, 120, 103
    MOVE G6A,102,  69, 134, 113,  98
    MOVE G6C, 80
    MOVE G6B,120
    WAIT

    MOVE G6D, 95,  76, 130, 110, 103
    MOVE G6A,104,  73, 143,  99, 100
    WAIT

    MOVE G6D,100,  79, 130, 110, 104
    MOVE G6A,100,  73, 143,  99,  97
    WAIT


    HIGHSPEED SETOFF
    RETURN

FS_End_H:
    HIGHSPEED SETON
    GOSUB GYRO_OFF
    GOSUB Leg_motor_mode3

    HIGHSPEED SETOFF

    SPEED 15
    GOSUB 안정화자세

    SPEED 5
    GOSUB BASIC

    RETURN

FS_15_H:
    GOSUB FS_Ready_H
    FOR i = 0 TO 10
        GOSUB FSing_H
    NEXT i
    GOSUB FS_End_H

Golf_Kick:
    GOSUB GYRO_ON
    GOSUB Leg_motor_mode3
    SPEED 4
    '무게중심오른쪽으로
    MOVE G6D,105,  77, 145,  93,  92, 100	
    MOVE G6A, 85,  71, 152,  91, 114, 100
    MOVE G6B,100,  40,  80, , , ,
    MOVE G6C,100,  40,  80, , , ,	
    WAIT

    SPEED 10
    '왼발뒤로들기
    MOVE G6D,112,  75, 145,  100,  95	
    MOVE G6A, 83,  85, 122,  105, 114
    WAIT

    GOSUB Leg_motor_mode2
    HIGHSPEED SETON

    SPEED 10
    '왼발스윙
    MOVE G6D,110,  73, 145,  85,  95	
    MOVE G6A, 83,  15, 172,  125, 114
    MOVE G6B,50
    MOVE G6C,150
    WAIT

    DELAY 400
    HIGHSPEED SETOFF

    SPEED 8
    '왼발내리기
    MOVE G6D,110,  72, 145,  94,  95
    MOVE G6A, 83,  58, 122,  130, 114
    MOVE G6B,100,  40,  80, , , ,
    MOVE G6C,100,  40,  80, , , ,	
    WAIT	

    SPEED 6
    '왼발들기
    MOVE G6D,110,  77, 145,  92,  95	
    MOVE G6A, 80,  80, 142,  95, 114
    MOVE G6B,100,  40,  80, , , ,
    MOVE G6C,100,  40,  80, , , ,
    WAIT	

    SPEED 6
    '무게중심오른쪽으로
    MOVE G6D,105,  77, 145,  93,  93, 100	
    MOVE G6A, 80,  71, 152,  91, 114, 100
    WAIT
    GOSUB GYRO_OFF
    SPEED 3
    GOSUB BASIC
    DELAY 400

    RETURN

MAIN: '라벨설정
    GOSUB 앞뒤기울기측정
    GOSUB MOTOR_ONOFF_LED

    ERX 4800, A, MAIN
    A_old = A

MAIN1:
    ON A GOTO MAIN, 리모컨_1, 리모컨_2, 리모컨_3, 리모컨_4, 리모컨_5, 리모컨_6, 리모컨_7, 리모컨_8, 리모컨_9, 리모컨_10
    GOTO MAIN

    'MAIN 라벨로 가기
    '*******************************************

리모컨_1: '1
    GOSUB BASIC
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

리모컨_2: '2
    GOSUB MINE_RUN
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

리모컨_3: '3
    GOSUB 계단올라가기4CM
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

리모컨_4: '4
    GOSUB Golf_Kick
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

리모컨_5: '5
    GOSUB Half_Walk_H
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

리모컨_6: '6
    GOSUB SideMove_Right_40
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

리모컨_7: '7 임현
    GOSUB SideMove_Right_50
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

리모컨_8: '8
    GOSUB SideMove_Right_70
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

리모컨_9: '9
    GOSUB FS_15_H
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

리모컨_10: '10
    GOSUB BASIC
    Send = "A"
    ETX 4800, Send
    GOTO MAIN
