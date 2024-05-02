#pragma once
#include "DxLib.h"
#include "enemybase.h"


#define STAGE_MAX 2


#define ENEMY_TYPE_MAX 6
#define ENEMY_MAX 250

#define ENEMY_FLOAT_MAX			50
#define ENEMY_SHOOTING_MAX		50
#define ENEMY_GROUND_MAX		50
#define ENEMY_SHIELD_MAX		50
#define ENEMY_TRACER_MAX		50

#define ROUTE_MAX 10

#define ENGROUND_TYPE_NORMAL 0
#define ENGROUND_TYPE_METAL  1

#define ENFLOAT_TYPE_PATROL 0
#define ENFLOAT_TYPE_TRACE  1


#define ENSHIELD_TYPE_STAY  0
#define ENSHIELD_TYPE_MOVE 1

// ���V�G���
struct FLOAT_DATA
{
	int size;
	VECTOR pos;
	VECTOR direction;
};

// �ړ����[�g���
// �w�肵���}�b�v�`�b�v���W�ɐG�ꂽ�Ƃ��ɁA�ړ�������ύX����
struct ENEMY_ROUTE
{
	// �ړ�������ύX����}�b�v�`�b�v���W
	VECTOR pos = VGet(-1, -1, -1);
	// �ύX��̈ړ�����
	VECTOR direction = VGet(0, 0, 0);
};

// �G���
struct ENEMY_DATA {
	int size = -1;
	VECTOR pos = VGet(-1, -1, -1);
	int type = 0;
};

// �c���
struct SHIELD {
	int use = 0;
	float r;
	int h;
	VECTOR centerPos;
	VECTOR direction;
};


FLOAT_DATA GetFloatData(int stageNo, int index);
ENEMY_ROUTE GetFloatRoute(int stageNo, int enemyIndex, int routeIndex);
ENEMY_DATA GetShootingData(int stageNo, int index);
ENEMY_DATA GetGroundData(int stageNo, int index);
ENEMY_DATA GetShieldData(int stageNo, int index);;
ENEMY_DATA GetTracerData(int stageNo, int index);


extern SHIELD shieldInfo[ENEMY_SHIELD_MAX];




#define SIZE_MAX 3

#define SMALL 0
#define MEDIUM 1
#define LARGE 2



// �^�C�v�E�T�C�Y���Ƃ�, hp, modelSize, rect���w�肷��
// 2�d�z�� [type][size]
// type  0:float, 1:shooting, 2:ground, 3:shield
// size  0:small, 1:medium, 2:large
struct ENEMY_INFO {	
	// �̗�
	int hp;
	// 3D���f���̃T�C�Y
	VECTOR modelSize;

	// �����蔻��̃T�C�Y (�z�u���W����̍���x,y �����蔻��̑傫��w,h)
	// �{��
	MYRECT difBodyRect;
	// ���G�͈�
	MYRECT difSearchRect;
	// �U���͈�
	MYRECT difAttackRect;
};

ENEMY_INFO GetEnemySizeInfo(int type, int size);