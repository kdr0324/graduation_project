/**
 @title : Mine.h
 @author : ���� (010 - 9019 - 3799)
 @since : 2017 - 07 - 05
 @brief : ���� �̼� ��� ����
*/

#ifndef _MINE_H_
// ������ Ȯ�� ������ ����
#define _MINE_H_

// Config.h : ��ǻ���� ȯ���� ����
#include "../Amazon/Config.h"

// ���ڸ� ã�Ҵµ� ����
#define MINE_DETECT_IS_NOT_MINE		0x00000000

#define MINE_DETECT_LEFT_MOTION		0x00000001

#define MINE_DETECT_MINE_RUN		0x00000002
#define MINE_DETECT_QUARTER_WALK	0x00000004
// ���ڰ� ������ ����
#define MINE_DETECT_HALF_WALK		0x00000008
// ���ڸ� ã������ �ݰ���
#define MINE_DETECT_WALK			0x00000010
// �۰�
#define MINE_DETECT_SIDE_MOVE_10		0x00000020
#define MINE_DETECT_SIDE_MOVE_30		0x00000040
// ũ��
#define MINE_DETECT_SIDE_MOVE_70		0x00000080

int MineDetect(FILE *fp, const char *name);

#endif // _MINE_H_
