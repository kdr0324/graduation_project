'******** ��Ż������ �⺻�� ���α׷� ********

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

DIM ���⼾���������� AS BYTE

'*** GYRO Setting  ***********************************
GOSUB GYRO_INIT
GOSUB GYRO_MID

'**** ���⼾����Ʈ ����

CONST �յڱ���AD��Ʈ = 0
CONST �¿����AD��Ʈ = 1
CONST ����Ȯ�νð� = 10  'ms

CONST min = 61			'�ڷγѾ�������
CONST max = 107			'�����γѾ�������
CONST COUNT_MAX = 20
CONST �������� = 103	'��6V����

PTP SETON 				'�����׷캰 ���������� ����
PTP ALLON				'��ü���� ������ ���� ����

DIR G6A, 1, 0, 0, 1, 0, 0		'����0~5��
DIR G6B, 1, 1, 1, 1, 1, 1		'����6~11��
DIR G6C, 0, 0, 0, 0, 0, 0		'����12~17��
DIR G6D, 0, 1, 1, 0, 1, 0		'����18~23��


'***** �ʱ⼱�� *********************************
MOTOR_ONOFF = 0
GRADIENT_CHECK_COUNT = 0
FALLDOWN_CHECK = 0
���⼾���������� = 1

'****�ʱ���ġ *****************************
OUT 52, 0
GOSUB MOTOR_ON
HIGHSPEED SETOFF
SPEED 5
GOSUB GYRO_INIT
GOSUB �����ʱ��ڼ�
'GOSUB ����ȭ�ڼ�
GOSUB BASIC

GOTO MAIN
'************************************************


'************ ���̷� ****************************

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
    '����Ʈ�������ͻ�뼳��
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

    '��ġ���ǵ��
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
    '*******�⺻�ڼ�����*****************************
    '************************************************
�����ʱ��ڼ�:
    'GOSUB GYRO_OFF
    GOSUB GYRO_ON
    MOVE G6A, 95, 70, 145, 100, 105, 100
    MOVE G6D, 95, 70, 145, 100, 105, 100
    MOVE G6B, 100, 45, 90, 100, 100, 100
    MOVE G6C, 100, 45, 90, 100, 135, 100
    WAIT

    RETURN
    '************************************************
����ȭ�ڼ�:

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
���̷α⺻�ڼ�:
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
    '******* �ǿ��� �Ҹ� ���� ***********************
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


�յڱ�������:
    FOR i = 0 TO COUNT_MAX
        A = AD(�յڱ���AD��Ʈ)	'���� �յ�
        IF A > 250 OR A < 5 THEN RETURN
        IF A > MIN AND A < MAX THEN RETURN
        DELAY ����Ȯ�νð�
    NEXT i

    IF A < MIN THEN GOSUB �����
    IF A > MAX THEN GOSUB �����

    RETURN
    '**************************************************
�����:
    GRADIENT_VAL = AD(�յڱ���AD��Ʈ)
    'IF A < MIN THEN GOSUB �������Ͼ��
    'IF GRADIENT_VAL < MIN THEN  GOSUB �ڷ��Ͼ��
    RETURN

�����:
    GRADIENT_VAL = AD(�յڱ���AD��Ʈ)
    'IF A > MAX THEN GOSUB �ڷ��Ͼ��
    'IF GRADIENT_VAL > MAX THEN GOSUB �������Ͼ��
    RETURN
    '**************************************************

�¿��������:
    FOR i = 0 TO COUNT_MAX
        GRADIENT_VAL = AD(�¿����AD��Ʈ)	'���� �¿�
        IF GRADIENT_VAL > 250 OR GRADIENT_VAL < 5 THEN RETURN
        IF GRADIENT_VAL > MIN AND GRADIENT_VAL < MAX THEN RETURN
        DELAY ����Ȯ�νð�
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

    '�ٸ� ������
    MOVE G6A, 90, 70, 145, 105, 110
    MOVE G6D, 90, 70, 145, 105, 110
    WAIT
    DELAY 100

    SPEED 15
    HIGHSPEED SETON

    '�ٸ� ����̱�
    MOVE G6A, 80, 70, 145, 105, 110
    MOVE G6D, 100, 70, 145, 105, 110
    WAIT

    '�޹ߵ��
    MOVE G6A, 80, 80, 125, 115, 110
    MOVE G6D, 100, 70, 145, 105, 110
    MOVE G6C, 80
    MOVE G6B, 120
    WAIT

    '�ٸ� ������
    MOVE G6A, 90, 40, 175, 105, 110
    MOVE G6D, 90, 70, 145, 105, 110
    WAIT

    RETURN
    '**************************************************
MINE_RUNNING:

    '�ٸ�����̱� (������)
    MOVE G6A, 80, 40, 175, 105, 110
    MOVE G6D, 100, 70, 145, 105, 110
    WAIT

    '�����ߵ��
    MOVE G6D, 80, 80, 125, 115, 110
    MOVE G6A, 100, 70, 145, 105, 110
    MOVE G6B, 80
    MOVE G6C, 120
    WAIT

    '�ٸ� ������
    MOVE G6D, 90, 40, 175, 105, 110
    MOVE G6A, 90, 70, 145, 105, 110
    WAIT

    '�ٸ�����̱� (����)
    MOVE G6D, 80, 40, 175, 105, 110
    MOVE G6A, 100, 70, 145, 105, 110
    WAIT

    '�޹ߵ��
    MOVE G6A, 80, 80, 125, 115, 110
    MOVE G6D, 100, 70, 145, 105, 110
    MOVE G6C, 80
    MOVE G6B, 120
    WAIT

    '�ٸ� ������
    MOVE G6A, 90, 70, 145, 105, 110
    MOVE G6D, 90, 70, 145, 105, 110
    WAIT

    RETURN
    '**************************************************
MINE_RUN_END:
    '�ٸ�����̱� (������)
    MOVE G6A, 80, 40, 175, 105, 110
    MOVE G6D, 100, 70, 145, 105, 110
    WAIT

    '�����ߵ��
    MOVE G6D, 80, 80, 125, 115, 110
    MOVE G6A, 100, 70, 145, 105, 110
    MOVE G6B, 80
    MOVE G6C, 120
    WAIT

    '�ٸ� ������
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

    '�ٸ� ������
    MOVE G6A, 98, 70, 145, 105, 102
    MOVE G6D, 98, 70, 145, 105, 102
    WAIT
    DELAY 100

    SPEED 15
    HIGHSPEED SETON

    '�ٸ� ����̱�
    MOVE G6A, 88, 70, 145, 105, 102
    MOVE G6D, 108, 70, 145, 105, 102
    WAIT

    '�޹ߵ��
    MOVE G6A, 88, 80, 125, 115, 102
    MOVE G6D, 108, 70, 145, 105, 102
    MOVE G6C, 80
    MOVE G6B, 120
    WAIT

    '�ٸ� ������
    MOVE G6A, 98, 40, 175, 105, 102
    MOVE G6D, 98, 70, 145, 105, 102
    WAIT

    RETURN
    '**************************************************
RUNNING_D:

    '�ٸ�����̱� (������)
    MOVE G6A, 88, 40, 175, 105, 102
    MOVE G6D, 108, 70, 145, 105, 102
    WAIT

    '�����ߵ��
    MOVE G6D, 88, 80, 125, 115, 102
    MOVE G6A, 108, 70, 145, 105, 102
    MOVE G6B, 80
    MOVE G6C, 120
    WAIT

    '�ٸ� ������
    MOVE G6D, 98, 40, 175, 105, 102
    MOVE G6A, 98, 70, 145, 105, 102
    WAIT

    '�ٸ�����̱� (����)
    MOVE G6D, 88, 40, 175, 105, 102
    MOVE G6A, 108, 70, 145, 105, 102
    WAIT

    '�޹ߵ��
    MOVE G6A, 88, 80, 125, 115, 102
    MOVE G6D, 108, 70, 145, 105, 102
    MOVE G6C, 80
    MOVE G6B, 120
    WAIT

    '�ٸ� ������
    MOVE G6A, 98, 70, 145, 105, 102
    MOVE G6D, 98, 70, 145, 105, 102
    WAIT

    RETURN
    '**************************************************
RUN_END_D:
    '�ٸ�����̱� (������)
    MOVE G6A, 88, 40, 175, 105, 102
    MOVE G6D, 108, 70, 145, 105, 102
    WAIT

    '�����ߵ��
    MOVE G6D, 88, 80, 125, 115, 102
    MOVE G6A, 108, 70, 145, 105, 102
    MOVE G6B, 80
    MOVE G6C, 120
    WAIT

    '�ٸ� ������
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

    '    SPEED 10
    '    MOVE G6A, 86,  56, 145, 115, 110
    '    MOVE G6D,108,  76, 147,  93,  96
    '    WAIT
    '
    '    SPEED 4
    '    GOSUB Leg_motor_mode3
    '    MOVE G6A,110,  76, 147, 93,  96
    '    MOVE G6D,86, 100, 145,  69, 110
    '    WAIT

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
    GOSUB BASIC

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
    GOSUB BASIC

    RETURN

Walk:
    GOSUB Walk_Ready
    GOSUB Walking
    GOSUB Walk_Left_End

    RETURN

Half_Walk:
    GOSUB Walk_Ready
    GOSUB Walk_Right_End

    RETURN
    '**********************************************
Q_Walk_Ready:
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

Q_Walk_End:
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
    GOSUB BASIC

    RETURN

Q_Walk:
    '    GOSUB HEAD_DOWN_80
    GOSUB Q_Walk_Ready
    GOSUB Q_Walk_End

    RETURN

    '**********************************************
Rolling_Hurdle:
    GOSUB GYRO_OFF
    GOSUB Leg_motor_mode3


    SPEED 15			'�� ���̸� �ȹ�����

    MOVE G6A,  98,  73, 145,  96, 100, 100
    MOVE G6D, 100,  75, 144,  94,  98, 100
    MOVE G6B,  100,  100,  81, 100, 100, 100
    MOVE G6C,  100,  100,  81, 100, 185,
    WAIT

    SPEED 15			'�ȹ�����
    MOVE G6A,  98,  73, 145,  96, 100, 100
    MOVE G6D, 100,  75, 144,  94,  98, 100
    MOVE G6B,  188,  100,  81, 100, 100, 100
    MOVE G6C,  190,  100,  81, 100, 185, 100
    WAIT

    SPEED 15			'�Ⱦ����� ������
    MOVE G6A,  98,  73, 145,  96, 100, 100
    MOVE G6D, 100,  75, 144,  94,  98, 100
    MOVE G6B,  183,  40,  70, 100, 100, 100
    MOVE G6C,  185,  40,  70, 100, 185, 100
    WAIT

    'SPEED 6			'������¦������
    'MOVE G6A, 100,  80, 127, 110, 100, 100
    'MOVE G6D, 102,  80, 124, 111,  98, 100
    'WAIT

    SPEED 3 		'�㸮���̱�
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

    SPEED 15 '�ٸ� ���ư���
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

    '�ɱ�
    SPEED 10
    MOVE G6A,100, 143,  28, 142, 100, 100
    MOVE G6D,100, 143,  28, 142, 100, 100
    MOVE G6B,100,  30,  80
    MOVE G6C,100,  30,  80
    WAIT

    '�Ȼ���
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

    '�ٸ��ø���
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

    '    SPEED 15 '�ٸ� ���ư���
    '    MOVE G6A,100, 110, 70,  65, 100, 100
    '    MOVE G6D,100, 110, 70,  65, 100, 100

    '    MOVE G6A,100, 170,  70,  15, 100, 100��

    '    GOSUB �⺻�ڼ�
    GOSUB GYRO_OFF
    RETURN


���ڸ�����:
    GOSUB All_motor_mode3
    HIGHSPEED SETOFF
    SPEED 15
    GOSUB BASIC
    DELAY 50
    SPEED 12

    '�ٸ� ������
    MOVE G6A, 98, 70, 145, 100, 102
    MOVE G6D, 98, 70, 145, 100, 102
    WAIT
    DELAY 100

    HIGHSPEED SETON
    FOR i = 0 TO 4
        '�ٸ� ����̱�
        MOVE G6A, 88, 70, 145, 100, 102
        MOVE G6D, 108, 70, 145, 100, 102
        WAIT

        '�޹ߵ��
        MOVE G6A, 88, 80, 125, 110, 102
        MOVE G6D, 108, 70, 145, 100, 102
        MOVE G6C, 80
        MOVE G6B, 120
        WAIT

        '�ٸ� ������
        MOVE G6A, 98, 70, 145, 100, 102
        MOVE G6D, 98, 70, 145, 100, 102
        WAIT

        '�ٸ� ����̱�
        MOVE G6D, 88, 70, 145, 100, 102
        MOVE G6A, 108, 70, 145, 100, 102
        WAIT

        '�����ߵ��
        MOVE G6D, 88, 80, 125, 110, 102
        MOVE G6A, 108, 70, 145, 100, 102
        MOVE G6B, 80
        MOVE G6C, 120
        WAIT

        '�ٸ� ������
        MOVE G6A, 98, 70, 145, 100, 102
        MOVE G6D, 98, 70, 145, 100, 102
        WAIT
    NEXT i

    '�ٸ������ڼ�
    MOVE G6A, 95, 70, 145, 100, 102
    MOVE G6D, 95, 70, 145, 100, 102
    WAIT

    GOSUB BASIC

    RETURN

    '**************************************************

��ܿö󰡱�4CM:
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
    MOVE G6D, 90,  50, 145, 101, 100
    WAIT

    SPEED 8
    GOSUB BASIC
    HIGHSPEED SETOFF
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
    MOVE G6A, 90,  50, 145,  99, 100
    WAIT

    SPEED 8
    GOSUB BASIC
    HIGHSPEED SETOFF
    RETURN

Turn_Left_15:
    HIGHSPEED SETON

    SPEED 5
    MOVE G6D,97,  60, 145,  110, 103, 100
    MOVE G6A,97,  80, 145,  90, 103, 100
    WAIT

    SPEED 10
    MOVE G6D,94,  60, 145,  110, 101, 100
    MOVE G6A,94,  80, 145,  90, 101, 100
    WAIT
    HIGHSPEED SETOFF
    SPEED 5
    MOVE G6A,101,  70, 146,  100, 98, 100
    MOVE G6D,101,  70, 146,  100, 98, 100
    WAIT

    GOSUB BASIC
    GOSUB All_motor_reset
    RETURN

Turn_Right_15:
    HIGHSPEED SETON

    SPEED 5
    MOVE G6A,97,  60, 145,  110, 103, 100
    MOVE G6D,97,  80, 145,  90, 103, 100
    WAIT

    SPEED 10
    MOVE G6A,94,  60, 145,  110, 101, 100
    MOVE G6D,94,  80, 145,  90, 101, 100
    WAIT
    HIGHSPEED SETOFF
    SPEED 5
    MOVE G6A,101,  70, 146,  100, 98, 100
    MOVE G6D,101,  70, 146,  100, 98, 100
    WAIT

    GOSUB BASIC
    GOSUB All_motor_reset
    RETURN

Turn_Left_30: '������ ����
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

Turn_Right_30: '������ ����
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

SideMove_Left_10: '
    SPEED 12
    MOVE G6A, 95,  73, 130, 110, 103, 100
    MOVE G6D,101,  70, 145,  100, 103, 100
    WAIT

    DELAY 50

    SPEED 13
    MOVE G6A,98,  76, 145,  93, 100, 100
    MOVE G6D,98,  76, 145,  93, 100, 100
    WAIT

    GOSUB BASIC
    GOSUB All_motor_reset
    RETURN

SideMove_Right_10: '
    SPEED 12
    MOVE G6D, 95,  73, 130, 110, 103, 100
    MOVE G6A,101,  70, 145,  100, 103, 100
    WAIT

    DELAY 50

    SPEED 13
    MOVE G6D,98,  76, 145,  93, 100, 100
    MOVE G6A,98,  76, 145,  93, 100, 100
    WAIT

    GOSUB BASIC
    GOSUB All_motor_reset
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

    SPEED 10
    MOVE G6A, 97, 70, 145, 100, 102, 100
    MOVE G6D, 97, 70, 145, 100, 102, 100

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

    SPEED 10
    MOVE G6A, 97, 70, 145, 100, 102, 100
    MOVE G6D, 97, 70, 145, 100, 102, 100

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
    WAIT

    SPEED 15
    MOVE G6A,102,  70, 145, 100, 100, 100
    MOVE G6D, 90,  72, 140, 103, 113, 100
    WAIT

    SPEED 15
    MOVE G6A, 98,  70, 145, 100, 100, 100
    MOVE G6D, 98,  70, 145, 100, 100, 100
    WAIT

    SPEED 10
    MOVE G6A, 97, 70, 145, 100, 102, 100
    MOVE G6D, 97, 70, 145, 100, 102, 100

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
    WAIT

    SPEED 15
    MOVE G6D,102,  70, 145, 100, 100, 100
    MOVE G6A, 90,  72, 140, 103, 113, 100
    WAIT

    SPEED 15
    MOVE G6D, 98,  70, 145, 100, 100, 100
    MOVE G6A, 98,  70, 145, 100, 100, 100
    WAIT

    SPEED 10
    MOVE G6A, 97, 70, 145, 100, 102, 100
    MOVE G6D, 97, 70, 145, 100, 102, 100

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
    MOVE G6A, 97, 70, 145, 100, 102, 100
    MOVE G6D, 97, 70, 145, 100, 102, 100

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
    MOVE G6A, 97, 70, 145, 100, 102, 100
    MOVE G6D, 97, 70, 145, 100, 102, 100

    SPEED 10
    GOSUB BASIC
    WAIT

    RETURN

Run_Ready:
    HIGHSPEED SETON
    GOSUB GYRO_OFF
    GOSUB All_motor_mode3

    SPEED 7
    MOVE G6A, 95,  70, 147, 100, 101
    MOVE G6D,100,  70, 147, 100,  98
    MOVE G6B,100
    MOVE G6C,100
    WAIT

    RETURN

Running:
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

    RETURN

Run_End:

    SPEED 7
    MOVE G6A, 95,  84, 125, 102, 104
    MOVE G6D,104,  70, 145,  98, 102
    MOVE G6B, 100
    MOVE G6C,100
    WAIT

    HIGHSPEED SETOFF

    SPEED 15
    GOSUB ����ȭ�ڼ�

    SPEED 5
    GOSUB BASIC

    RETURN

RUN_15:
    GOSUB Run_Ready
    FOR i = 0 TO 2
        GOSUB Running
    NEXT i
    GOSUB Run_End
    RETURN

RUN_20:
    GOSUB Run_Ready
    FOR i = 0 TO 3
        GOSUB Running
    NEXT i
    GOSUB Run_End
    RETURN

RUN_25:
    GOSUB Run_Ready
    FOR i = 0 TO 4
        GOSUB Running
    NEXT i
    GOSUB Run_End
    RETURN

RUN_30:
    GOSUB Run_Ready
    FOR i = 0 TO 5
        GOSUB Running
    NEXT i
    GOSUB Run_End
    RETURN

RUN_35:
    GOSUB Run_Ready
    FOR i = 0 TO 6
        GOSUB Running
    NEXT i
    GOSUB Run_End
    RETURN

RUN_40:
    GOSUB Run_Ready
    FOR i = 0 TO 7
        GOSUB Running
    NEXT i
    GOSUB Run_End
    RETURN

RUN_45:
    GOSUB Run_Ready
    FOR i = 0 TO 8
        GOSUB Running
    NEXT i
    GOSUB Run_End
    RETURN

RUN_50:
    GOSUB Run_Ready
    FOR i = 0 TO 9
        GOSUB Running
    NEXT i
    GOSUB Run_End
    RETURN
    '*****************************************************
    '**************Floor_Sweep****************************
    '*****************************************************
FS_Ready:
    HIGHSPEED SETON
    GOSUB GYRO_OFF
    GOSUB All_motor_mode3
    SPEED 7
    RETURN

FSing:
    HIGHSPEED SETON
    GOSUB GYRO_OFF
    GOSUB All_motor_mode3

    MOVE G6A,95,  76, 145, 93, 102
    MOVE G6D,101,  77, 145, 93, 99
    MOVE G6B,100, 40
    MOVE G6C,100, 40
    WAIT

    MOVE G6A,97,  83, 123, 113, 103
    MOVE G6D,102, 75, 134,  106,  98
    MOVE G6B, 80
    MOVE G6C,120
    WAIT

    MOVE G6A,95,  82, 130, 103, 103
    MOVE G6D,104,  79, 143,  92,  100
    WAIT

    MOVE G6A,100,   85, 130, 103, 104
    MOVE G6D, 100,  79, 143,  92, 97
    WAIT

    MOVE G6D,95,  76, 145,  93, 102
    MOVE G6A,101,  77, 145,  93, 99
    MOVE G6B,100, 40
    MOVE G6C,100,  40
    WAIT

    MOVE G6D,97,  83, 123, 113, 103
    MOVE G6A,102, 75, 134,  106,  98
    MOVE G6C, 80
    MOVE G6B,120
    WAIT

    MOVE G6D,95,  82, 130, 103, 103
    MOVE G6A,104,  79, 143,  92,  100
    WAIT

    MOVE G6D,100,   85, 130, 103, 104
    MOVE G6A, 100,  79, 143,  92, 97
    WAIT


    HIGHSPEED SETOFF
    RETURN

FS_End:
    HIGHSPEED SETON
    GOSUB GYRO_OFF
    GOSUB Leg_motor_mode3

    HIGHSPEED SETOFF

    SPEED 15
    GOSUB ����ȭ�ڼ�

    SPEED 5
    GOSUB BASIC

    RETURN

FS_15:
    GOSUB FS_Ready
    FOR i = 0 TO 5
        GOSUB FSing
    NEXT i
    GOSUB FS_End
    RETURN

FS_6:
    GOSUB FS_Ready
    FOR i = 0 TO 1
        GOSUB FSing
    NEXT i
    GOSUB FS_End
    RETURN

Back_FS_Ready:
    GOSUB All_motor_mode3
    HIGHSPEED SETON
    SPEED 7

    RETURN

Back_FSing:
    MOVE G6D,108,  79, 142,  91,  105
    MOVE G6A,95,  84, 130, 100, 104
    MOVE G6B,115
    MOVE G6C,85
    WAIT

    MOVE G6A,95,  95, 125, 90, 104
    MOVE G6D,104,  77, 146,  91,  102
    WAIT

    MOVE G6A, 103,  79, 146,  89, 100
    MOVE G6D,95,   73, 146, 95,  102
    WAIT

    MOVE G6A,104,  77, 146,  91,  100
    MOVE G6D,95,  95, 120, 100, 106
    MOVE G6C,115
    MOVE G6B,85
    WAIT

    MOVE G6A,102,  77, 146,  91,  100
    MOVE G6D,95,  90, 135, 90, 104
    WAIT

    MOVE G6D, 103,  79, 146,  89, 100
    MOVE G6A,95,   73, 146, 95,  104
    WAIT

    RETURN

Back_FS_End:

    HIGHSPEED SETOFF
    SPEED 5
    GOSUB BASIC

    RETURN	

Back_FS_15:
    GOSUB Back_FS_Ready
    FOR i = 0 TO 5
        GOSUB Back_FSing
    NEXT i
    GOSUB Back_FS_End
    RETURN	

    '*****************************************************
    '**************Golf_Kick******************************
    '*****************************************************
Golf_Kick:
    GOSUB GYRO_ON
    GOSUB Leg_motor_mode3
    SPEED 4
    '�����߽ɿ���������
    MOVE G6D,105,  77, 145,  93,  92, 100	
    MOVE G6A, 85,  71, 152,  91, 114, 100
    MOVE G6B,100,  40,  80, , , ,
    MOVE G6C,100,  40,  80, , , ,	
    WAIT

    SPEED 10
    '�޹ߵڷε��
    MOVE G6D,112,  75, 145,  100,  95	
    MOVE G6A, 83,  85, 122,  105, 114
    WAIT

    GOSUB Leg_motor_mode2
    HIGHSPEED SETON

    SPEED 10
    '�޹߽���
    MOVE G6D,110,  73, 145,  85,  95	
    MOVE G6A, 83,  15, 172,  125, 114
    MOVE G6B,50
    MOVE G6C,150
    WAIT

    DELAY 400
    HIGHSPEED SETOFF

    SPEED 8
    '�޹߳�����
    MOVE G6D,110,  72, 145,  94,  95
    MOVE G6A, 83,  58, 122,  130, 114
    MOVE G6B,100,  40,  80, , , ,
    MOVE G6C,100,  40,  80, , , ,	
    WAIT	

    SPEED 6
    '�޹ߵ��
    MOVE G6D,110,  77, 145,  92,  95	
    MOVE G6A, 80,  80, 142,  95, 114
    MOVE G6B,100,  40,  80, , , ,
    MOVE G6C,100,  40,  80, , , ,
    WAIT	

    SPEED 6
    '�����߽ɿ���������
    MOVE G6D,105,  77, 145,  93,  93, 100	
    MOVE G6A, 80,  71, 152,  91, 114, 100
    WAIT
    GOSUB GYRO_OFF
    SPEED 3
    GOSUB BASIC
    DELAY 400

    RETURN
    '*****************************************************
    '**************Stair4cm*******************************
    '*****************************************************
Stair_Down4cm:
    GOSUB Crawl
    GOSUB Crawl_LOOP
    GOSUB Wake_Up
    RETURN


Crawl:

    HIGHSPEED SETOFF
    GOSUB GYRO_OFF
    GOSUB Leg_motor_mode3

    SPEED 10
    MOVE G6A,100, 146,  38, 135, 102
    MOVE G6D,98, 145,  36, 137, 101
    WAIT

    SPEED 10
    MOVE G6B,186,  30, 80
    MOVE G6C,190,  25, 80
    WAIT

    DELAY 100
    SPEED 2	
    MOVE G6A, 100, 148,  60, 156, 102
    MOVE G6D,100, 148,  60, 158, 100
    MOVE G6B,186,  28, 80
    MOVE G6C,189,  23, 82
    WAIT	

    GOSUB All_motor_mode2

    DELAY 500
    PTP SETOFF
    PTP ALLOFF
    RETURN

Crawl_Loop:
    HIGHSPEED SETON
    SPEED 9
    MOVE G6A, 100, 147,  75, 160, 100,
    MOVE G6D, 102, 163,  55, 160, 100,
    MOVE G6B, 189,  40,  55,  ,  ,
    MOVE G6C, 175,  25,  72,  ,  ,
    WAIT


    MOVE G6A, 101, 143, 120, 121, 100, 100
    MOVE G6D, 102, 150,  72, 160,  98, 100
    MOVE G6B, 190,  25,  70, 100, 100, 100
    MOVE G6C, 160,  26,  70, 100, 100, 100
    WAIT

    MOVE G6D, 100, 147,  75, 160, 100,
    MOVE G6A, 102, 163,  55, 160, 100,
    MOVE G6C, 189,  40,  55,  ,  ,
    MOVE G6B, 175,  25,  72,  ,  ,
    WAIT


    MOVE G6D, 101, 143, 120, 121, 100, 100
    MOVE G6A, 102, 150,  72, 160,  98, 100
    MOVE G6C, 190,  25,  70, 100, 100, 100
    MOVE G6B, 160,  26,  70, 100, 100, 100
    WAIT
    DELAY 20
    HIGHSPEED SETOFF
    RETURN

Wake_Up:
    GOSUB GYRO_OFF

    PTP SETON 				
    PTP ALLON				

    GOSUB Arm_motor_mode1
    GOSUB Leg_motor_mode2

    DELAY 500

    SPEED 12	
    MOVE G6A,  100, 165,  25, 162, 101
    MOVE G6D,  100, 165,  25, 162, 100
    MOVE G6B,  155, 15, 90
    MOVE G6C,  155, 15, 90
    WAIT

    DELAY 50

    SPEED 10	
    MOVE G6A, 77, 165,  28, 164, 129, 100
    MOVE G6B,155,  13,  83,  98, 100, 100
    MOVE G6C,152,  12,  85, 100, 101, 100
    MOVE G6D, 75, 163,  29, 164, 135, 100
    WAIT

    MOVE G6A,  74, 162,  26, 156, 132,
    MOVE G6D,  74, 162,  26, 156, 132,
    MOVE G6B,151,  35,  92, 100, 100, 100
    MOVE G6C,148,  35,  86, 100, 101, 100
    WAIT

    SPEED 8
    MOVE G6A,  100, 138,  25, 155, 100
    MOVE G6D,  98, 138,  25, 155, 98
    MOVE G6B, 113,  30, 80
    MOVE G6C, 113,  30, 80
    WAIT

    DELAY 100

    SPEED 10
    GOSUB BASIC
    GOSUB Leg_motor_mode1
    PTP SETOFF
    PTP ALLOFF
    RETURN
    '*****************************************************
Stair_Down2cm:
    HIGHSPEED SETOFF
    GOSUB GYRO_OFF
    GOSUB All_motor_mode3

    SPEED 7
    MOVE G6D, 95,  70, 147, 100, 101
    MOVE G6A,103,  70, 147, 100,  98
    MOVE G6B,100
    MOVE G6C,100
    WAIT

    SPEED 4
    MOVE G6D, 88,  65, 152,  98, 110
    MOVE G6A,108,  70, 145, 100,  100
    MOVE G6B,100,40
    MOVE G6C,100,40
    WAIT
    DELAY 100
    GOSUB Leg_motor_mode1

    SPEED 5
    MOVE G6D, 88,  15, 152,  150, 110
    MOVE G6A,110,  68, 145, 95, 100
    MOVE G6B,100,100
    WAIT

    DELAY 100


'
'    SPEED 5
'    MOVE G6D, 88,  15, 155,  150, 110
'    MOVE G6A,110,  137, 70, 90, 100
'    MOVE G6B,100,100
'    MOVE G6C,20,20
'    WAIT


    '
    '    SPEED 4
    '    MOVE G6D, 88,  15, 155,  150, 110
    '    MOVE G6A,95,  135, 70, 90, 100
    '    MOVE G6B,100,100
    '    MOVE G6C,20,20
    '    WAIT



    ''''''''


    SPEED 8
    MOVE G6A, 102, 163,  77,  90,  92,
    MOVE G6D,  100,  17, 143, 164, 107,
    MOVE G6B, 114,  100,  80,  ,  ,
    MOVE G6C, 20,  20,  81,  ,  ,
    WAIT

    SPEED 8
    MOVE G6A, 103, 163,  77,  81,  92,
    MOVE G6D,  96,  17, 143, 164, 107,
    MOVE G6B, 114,  42,  80,  ,  ,
    MOVE G6C, 108,  49,  81,  ,  ,
    WAIT

    DELAY 300
    
    RETURN

    SPEED 8
    MOVE G6A, 103, 163,  77,  81,  92,
    MOVE G6D,  96,  17, 143, 164, 107,
    MOVE G6B, 114,  42,  80,  ,  ,
    MOVE G6C, 108,  49,  81,  ,  ,
    WAIT
    '    SPEED 8
    '    MOVE G6A, 103, 163,  50,  81,  92,
    '    MOVE G6D,  100,  17, 143, 164, 107,
    '    MOVE G6B, 114,  42,  80,  ,  ,
    '    MOVE G6C, 108,  49,  81,  ,  ,
    '    WAIT

    RETURN
    MOVE G6A, 90, 121,  36, 105, 115,  100
    MOVE G6D,  110,  73, 145, 100, 100,
    MOVE G6B,114,  42,  80
    MOVE G6C,108,  49,  81
    WAIT

    SPEED 7
    MOVE G6A, 103, 163,  77,  81,  97,
    MOVE G6D,  110,  80, 143, 95, 107,
    MOVE G6B, 114,  42,  80,  ,  ,
    MOVE G6C, 108,  49,  81,  ,  ,
    WAIT

    '    SPEED 10
    '    MOVE G6A, 103, 163,  77,  81,  97,
    '    MOVE G6D,  105,  85, 145, 85, 107,
    '    MOVE G6B, 114,  42,  80,  ,  ,
    '    MOVE G6C, 108,  49,  81,  ,  ,
    '    WAIT
    '

    GOSUB Leg_motor_mode2

    '    SPEED 5
    '    MOVE G6D, 90,  14, 150, 167, 110
    '    MOVE G6A,110, 149,  35, 137, 104
    '    MOVE G6C,100,50
    '    MOVE G6B,140,40
    '    WAIT

    MOVE G6A, 85,  98, 105,  115, 115, 100
    MOVE G6D,115,  74, 145,  98,  93, 100
    WAIT

    SPEED 7
    MOVE G6D,110, 74, 130, 115, 96
    MOVE G6A, 85, 59, 140, 117,  114
    MOVE G6C,130,50
    MOVE G6B,100,40
    WAIT

    SPEED 6
    MOVE G6D, 98, 74, 130, 112,99,
    MOVE G6A, 98, 74, 130,  112, 99
    MOVE G6B,110,40
    MOVE G6C,110,40
    WAIT

    GOSUB GYRO_OFF
    SPEED 4
    GOSUB BASIC
    GOSUB All_motor_Reset
    RETURN
    '
    '    '****************************
    '
    '    SPEED 8
    '    MOVE G6D,96,  24, 145, 157, 104
    '    MOVE G6A,98, 144, 75,  107,102
    '    MOVE G6C,140,50
    '    MOVE G6B,100,40
    '    WAIT
    '
    '    DELAY 100
    '
    '    SPEED 10
    '    MOVE G6D,110,  64, 130, 137, 98
    '    MOVE G6A, 90, 119, 140,  75,114
    '    MOVE G6C,170,50
    '    MOVE G6B,100,40
    '    WAIT
    '
    '    SPEED 9
    '    MOVE G6D,112,  64, 130, 135,  96
    '    MOVE G6A, 80, 109, 70,  147, 114
    '    WAIT
    '
    '    SPEED 8
    '    MOVE G6D,112, 69, 130, 122, 92
    '    MOVE G6A, 90, 56, 115, 162,114
    '    WAIT
    '
    '    SPEED 7
    '    MOVE G6D,110, 74, 130, 115, 96
    '    MOVE G6A, 85, 59, 140, 117,  114
    '    MOVE G6C,130,50
    '    MOVE G6B,100,40
    '    WAIT
    '
    '    SPEED 6
    '    MOVE G6D, 98, 74, 130, 112,99,
    '    MOVE G6A, 98, 74, 130,  112, 99
    '    MOVE G6B,110,40
    '    MOVE G6C,110,40
    '    WAIT
    '
    '    GOSUB GYRO_OFF
    '    SPEED 4
    '    GOSUB BASIC
    '    GOSUB All_motor_Reset
    '    RETURN
    '
Stair_Up2cm:
    RETURN

    '*****************************************************

MAIN: '�󺧼���
    GOSUB �յڱ�������
    GOSUB MOTOR_ONOFF_LED

    ERX 4800, A, MAIN
    A_old = A

MAIN1:
    ON A GOTO MAIN, ������_1, ������_2, ������_3, ������_4, ������_5, ������_6, ������_7, ������_8, ������_9, ������_10, �⺻�ڼ�_, ������_, ���Ʒ�20_, ���Ʒ�80_, ��������90�Ʒ�35_, ����_1_, ����_2_, ����_3_, �̼ǿ�_Ư������_1, �ٸ������޸���_, ��ܿö󰡱�2cm_, ��ܳ�������2cm_, ��ܿö󰡱�4cm_, ��ܳ�������4cm_, �޹�_������_, ���Ѿ��_, ���ġ��_, ���ġ��ڷ�_, ����_4_, ����_5_, �����޸���15_, �����޸���20_, �����޸���25_, �����޸���30_, �����޸���35_, �����޸���40_, �����޸���45_, �����޸���50_, ����_6_, ����_7_, ����������ǹݺ�_, ��������ݺ�_, ���������Ѻ�_, ����_8_, ����_9_, ������5_, ��������5_, ������15_, ��������15_, ������30_, ��������30_, ������45_, ��������45_, ����_10_, ����_11_, �����̵�10_, �������̵�10_, �����̵�30_, �������̵�30_, �����̵�50_, �������̵�50_, �����̵�70_, �������̵�70_
    GOTO MAIN

    'MAIN �󺧷� ����
    '*******************************************

������_1: '1
    GOSUB BASIC
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

������_2: '2
    GOSUB MINE_RUN
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

������_3: '3
    GOSUB Stair_Down2cm
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

������_4: '4
    GOSUB SideMove_Left_50
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

������_5: '5
    GOSUB SideMove_Left_70
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

������_6: '6
    GOSUB SideMove_Right_40
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

������_7: '7 ����
    GOSUB SideMove_Right_50
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

������_8: '8
    GOSUB SideMove_Right_70
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

������_9: '9
    GOSUB FS_15
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

������_10: '10
    GOSUB BASIC
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

�⺻�ڼ�_: '11
    GOSUB BASIC
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

������_: '12
    GOSUB HEAD_DOWN_0
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

���Ʒ�20_: '13
    GOSUB HEAD_DOWN_20
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

���Ʒ�80_: '14
    GOSUB HEAD_DOWN_80
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

��������90�Ʒ�35_: '15
    GOSUB HEAD_DOWN_35_SIDE_90
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

����_1_: '16
    GOSUB BASIC
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

����_2_: '17
    GOSUB BASIC
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

����_3_: '18
    GOSUB BASIC
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

�̼ǿ�_Ư������_1: '19
    GOSUB BASIC
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

�ٸ������޸���_: '20
    GOSUB BASIC
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

��ܿö󰡱�2cm_: '21
    GOSUB BASIC
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

��ܳ�������2cm_: '22
    GOSUB BASIC
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

��ܿö󰡱�4cm_: '23
    GOSUB ��ܿö󰡱�4CM
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

��ܳ�������4cm_: '24
    GOSUB BASIC
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

�޹�_������_: '25
    GOSUB BASIC
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

���Ѿ��_: '26
    GOSUB BASIC
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

���ġ��_: '27
    GOSUB FS_15
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

���ġ��ڷ�_: '28
    GOSUB BASIC
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

����_4_: '29
    GOSUB BASIC
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

����_5_: '30
    GOSUB BASIC
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

�����޸���15_: '31
    GOSUB RUN_15
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

�����޸���20_: '32
    GOSUB RUN_20
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

�����޸���25_: '33
    GOSUB RUN_25
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

�����޸���30_: '34
    GOSUB RUN_30
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

�����޸���35_: '35
    GOSUB RUN_35
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

�����޸���40_: '36
    GOSUB RUN_40
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

�����޸���45_: '37
    GOSUB RUN_45
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

�����޸���50_: '38
    GOSUB RUN_50
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

����_6_: '39
    GOSUB BASIC
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

����_7_: '40
    GOSUB BASIC
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

����������ǹݺ�_: '41
    GOSUB BASIC
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

��������ݺ�_: '42
    GOSUB BASIC
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

���������Ѻ�_: '43
    GOSUB BASIC
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

����_8_: '44
    GOSUB BASIC
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

����_9_: '45
    GOSUB BASIC
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

������5_: '46
    GOSUB BASIC
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

��������5_: '47
    GOSUB BASIC
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

������15_: '48
    GOSUB BASIC
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

��������15_: '49
    GOSUB BASIC
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

������30_: '50
    GOSUB BASIC
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

��������30_: '51
    GOSUB BASIC
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

������45_: '52
    GOSUB BASIC
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

��������45_: '53
    GOSUB BASIC
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

����_10_: '54
    GOSUB BASIC
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

����_11_: '55
    GOSUB BASIC
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

�����̵�10_: '56
    GOSUB SideMove_Left_10
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

�������̵�10_: '57
    GOSUB SideMove_Right_10
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

�����̵�30_: '58
    GOSUB SideMove_Left_40
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

�������̵�30_: '59
    GOSUB SideMove_Right_40
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

�����̵�50_: '60
    GOSUB SideMove_Left_50
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

�������̵�50_: '61
    GOSUB SideMove_Right_50
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

�����̵�70_: '62
    GOSUB SideMove_Left_70
    Send = "A"
    ETX 4800, Send
    GOTO MAIN

�������̵�70_: '63
    GOSUB SideMove_Right_70
    Send = "A"
    ETX 4800, Send
    GOTO MAIN
