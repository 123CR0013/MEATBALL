#pragma once
#include "DxLib.h"
#include "mymath.h"



class Respawn
{
public:

	Respawn(int stageNo, VECTOR playerPos);
	~Respawn();

	void Process(MYRECT playerRec, int meatBallSize);

	void Draw(MATRIX mView);

	// �f�o�b�O���̕\��
	void DrawDebugInfo(MATRIX mView, int fillFrag = 1);

	// ���X�|�[���n�_�̍X�V�����邩�𒲂ׂ�
	// �Ԃ�l 
	// ���X�|�[���n�_�̍X�V�@-1:���Ȃ�, -1�ȊO: ����i�Ԃ�l�͍X�V��̃��X�|�[���n�_�ԍ��ƂȂ�j 
	int CheckRespawnPoint(MYRECT playerRect);







	//
	// ���X�|�[�����ɔ��f��������擾����֐�
	//

	// �v���C���[�̍��W��Ԃ�
	VECTOR GetRespawnPlayerPos() { return _playerPos; };
	// �~�[�g�{�[���̃T�C�Y��Ԃ�
	int GetRespawnMeatBallSize() { return _meatBallSize; };
	// �J�����̃t�F�[�Y�ԍ���Ԃ�
	int GetRespawnCamPhase() { return _camPhase; };
	// �J�����̃t�F�[�Y�ԍ���Ԃ�
	VECTOR GetRespawnCamPos() { return _camPos; };



protected:
	// ���X�|�[���n�_�����擾����
	auto GetRespawnPointInfo(int stageNo, int respawnPointNo);

	// �X�e�[�W�ԍ�
	int _stageNo;
	// ���݂̃��X�|�[���n�_�ԍ�
	int _respawnPointNo;



	// ���X�|�[���n�_���
	struct RESPAWN_POINT
	{
		// ���X�|�[���n�_�ԍ�
		int respawnPointNo;
		// �}�b�v�`�b�v���W
		VECTOR pos;
	};



	// ���X�|�[���n�_���X�V�����ۂɕێ�������
	// ���X�|�[�����ɂ����̏��𔽉f������
	
	// �v���C���[���W
	VECTOR _playerPos;
	// �~�[�g�{�[���̃T�C�Y
	int _meatBallSize;
	// �J�����̃t�F�[�Y
	int _camPhase;
	// �J�������W
	VECTOR _camPos;










	// �摜�n���h��
	// �v���W�F�N�^�[�ƃz���O�����̉摜��g�ݍ��킹�āA���X�|�[���n�_��\������
	
	// �z���O����
	int _cgHologram;
	// �z���O�����A�j���[�V�����i���X�|�[���n�_�X�V���̉��o�Ɏg���j
	int _cgHologramAnimation[12];
	// �v���W�F�N�^�[
	int _cgProjector;


	// �z���O�����A�j���[�V�����p

	// �A�j���[�V���������邩�ǂ���
	int _isPlayingAnimation;
	// �A�j���[�V�����J�E���g
	int _animCnt;


	// ���X�|�[���n�_�X�V����SE
	int _sound;
};

