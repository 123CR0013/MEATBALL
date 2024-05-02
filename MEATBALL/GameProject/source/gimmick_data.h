#pragma once

#define STAGE_MAX 2

#define DOOR_MAX 100
#define JUMPPAD_MAX 50
#define LEVER_MAX 50
#define SPIKE_MAX 80

#define BOMB_MAX 50
#define DASH_PANEL_MAX 50


//#define GIMMICK_TYPE_MAX 2

#define GIMMICK_TYPE_DOOR 0
#define GIMMICK_TYPE_JUMPPAD 1

#define GIMMICK_TYPE_LEVER 2
#define GIMMICK_TYPE_BOMB 3
#define GIMMICK_TYPE_DASHPANEL 4



// �M�~�b�N�̉�]
// �M�~�b�N���������Ă���ʂ��w��
#define GIMMICK_ROTATION_FLOOR 0
#define GIMMICK_ROTATION_RIGHT_WALL 1
#define GIMMICK_ROTATION_CEILING 2
#define GIMMICK_ROTATION_LEFT_WALL 3



struct GIMMICK_DATA 
{
	// �M�~�b�N��ID
	int id = -1;
	// �}�b�v�`�b�v���W
	int x, y;
};


struct JUMPPAD_DATA
{
	// �M�~�b�N��ID
	int id = -1;
	// �}�b�v�`�b�v���W
	// �����蔻���`�̍���̃}�b�v�`�b�v���W
	int x, y;


	// ��ɗL�����ǂ���
	// 0 : ���o�[�쓮���̂ݗL��, 1: ��ɗL��
	int isAlwaysActive;
};


struct LEVER_DATA
{
	// �}�b�v�`�b�v���W
	// �����蔻���`�̍���̃}�b�v�`�b�v���W
	int x = -1;
	int y;

	// 
	// �쓮���ɃA�N�V����������M�~�b�N
	// (DOOR_DATA, JUMPPAD_DATA�ƑΉ��t����)
	//
	
	// �M�~�b�N�̃^�C�v
	// GIMMICK_TYPE_DOOR, GIMMICK_TYPE_JUMPPAD
	int gimmickType;
	// �M�~�b�N��id 
	int gimmickId;


	// ��쓮��Ԃɖ߂邩�ǂ���
	// 0 : �߂�Ȃ� , 1 : �߂� 
	int returnInactive;


	// ��]���w��
	// �萔(GIMMICK_ROTATION_�Z�Z)���g���Ďw��
	int rotation = GIMMICK_ROTATION_FLOOR;
};


struct SPIKE_DATA
{
	// �}�b�v�`�b�v���W
	int x = -1;
	int y;

	// �M�~�b�N�̉�]
	// �M�~�b�N���������Ă���ʂ��w��
	int rotation = GIMMICK_ROTATION_FLOOR;
};


GIMMICK_DATA GetDoorData(int stageNo, int index);
JUMPPAD_DATA GetJumpPadData(int stageNo, int index);
LEVER_DATA GetLeverData(int stageNo, int index);
SPIKE_DATA GetSpikeData(int stageNo, int index);

GIMMICK_DATA GetBombData(int stageNo, int index);
GIMMICK_DATA GetDashPanelData(int stageNo, int index);
