#pragma once	// .h�̐擪�ɋL�q�B#include�ł��̃t�@�C�������x�ǂݍ��݂��Ă��A1�x�����ǂݍ��܂Ȃ�
#include "DxLib.h"		// VECTOR��MATRIX���`�Ƃ��Ďg�����߁A.h�ɓ���Ă���
#include "mymath.h"
#include <vector>

// �`�b�v�̃T�C�Y
#define	CHIPSIZE_W		(64)
#define	CHIPSIZE_H		(64)

// �}�b�v�̃T�C�Y
#define	MAPSIZE_W		(425)
#define	MAPSIZE_H		(80)

// �X�e�[�W��
#define	STAGE_MAX	(2)

#define LAYER_MAX       (3)


#define LAYER_BACKMAP  0
#define LAYER_MAP      1
#define LAYER_LIGHT    2
//#define LAYER_FRONT    3



// �΂߂̃}�b�v�`�b�v
#define LOWER_RIGHT 10
#define LOWER_LEFT 11
#define UPPER_RIGHT 12
#define UPPER_LEFT 13


// �󂹂�u���b�N
#define BREAK_CHIP_1 18
#define BREAK_CHIP_2 19
#define BREAK_CHIP_3 20
#define BREAK_CHIP_4 21





struct HITCHIP {
	int chip_no; // ���������}�b�v�`�b�v�̔ԍ��i�������Ă��Ȃ��ꍇ��-1�j
	int x, y;		 // ���������}�b�v�`�b�v�̍��W
};

class MapData {
public:
	MapData();			// �R���X�g���N�^
	virtual ~MapData();	// �f�X�g���N�^

	void MapInit(int stageNo);
	//void MapChiipInit(int stageNo);






	void Process();

	// �}�b�v�`�b�v�`��
	void	 Draw(MATRIX mView);
	// �w�i���C���[�`��i�w�i�E�w�i�`�b�v�j
	void	 DrawBG(MATRIX mView);
	// �O�ʃ��C���[�`��i���Łj
	void DrawFrontLayer(MATRIX mView); 

	// �����蔻��֘A
	int		CheckHitMapChip(int x, int y);	// �w��ʒu�̃`�b�v�������蔻�肠�肩�H
	int		CheckHitChip(int chip_no);		// �w��̃`�b�vID�������蔻�肠�肩�H
	int IsHitMapchip(float x, float y);
	HITCHIP IsHitBoxAndMapChip(MYRECT rect);

	void BreakMapChip(HITCHIP hitChip);

	void SetGimmickChip();
	void VanishDoor(int gimmickID);






	// world�̃T�C�Y���擾(pixel�P��)
	int	GetWorldSizeW() { return MAPSIZE_W * CHIPSIZE_W; }
	int	GetWorldSizeH() { return MAPSIZE_H * CHIPSIZE_H; }









	// �{�X��p
	void StartBossBattle();
	void SetBossBarrier(VECTOR barrierPos);
	void VanishBossBarrier();
	void BossBattleProcess();






	// �f�o�b�O���̕\��
	void DrawDebugInfo(MATRIX mView);

protected:
	int _gMapData[LAYER_MAX][MAPSIZE_W * MAPSIZE_H] = { 0 };

	//int	_cgChip[64];		// �`�b�v�摜


	int _stageNo;

	int _gimmickID;
	int _vanishCnt;


	int _startBossCnt = 0;
	int _bossBattle;
	int _isSetBossBarrier;
	int _barrierCnt;
	std::vector<VECTOR> _barrierChip;
};

// �}�b�v�f�[�^
extern MapData	gMap;