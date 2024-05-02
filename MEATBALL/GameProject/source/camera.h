#pragma once	// .h�̐擪�ɋL�q�B#include�ł��̃t�@�C�������x�ǂݍ��݂��Ă��A1�x�����ǂݍ��܂Ȃ�
#include "DxLib.h"		// VECTOR��MATRIX���`�Ƃ��Ďg�����߁A.h�ɓ���Ă���
#include "mymath.h"
#include "mapdata.h"



struct CamPhase
{
	int x = -1;
	int y;
	int phase;
};




class Camera {
public:
	Camera();
	~Camera();

	// �X�e�[�W�J�n���̏���
	void Init(int stageNo);

	void	 Process(VECTOR playerPos);
	MATRIX	GetViewMatrix() { return _mView; }


	void CheckPhase(VECTOR pos);
	void AdjustCam();

	// ���X�|�[�����̏���
	void SetRespawn(int respawnPhase, VECTOR respawnPos);

	// �f�o�b�O���̕\��
	void DrawDebugInfo();

public:
	int _camControl;	// �J�������삷��Ȃ�1
	MATRIX	_mView;		// View�s��
	VECTOR	_pos;		// �J�����̈ʒu�Bworld���W��
	VECTOR	_lookat;	// �J�����̒����_�B��ʂ̂����𒆐S�ɁA�g��/��]������
	float	_scale;		// �J�����̊g�嗦
	float	_rotate;	// �J�����̉�]�idegree)

	MYRECT	_rcLimit;	// world���W�Ń��~�b�g�ݒ�




	int _stageNo;

	int _phase;
	int _isFollowingPlayer;
	float _adjustValueY;


	// ��Ǐ]���[�h����Ǐ]���[�h�Ɉڍs�����ǂ���
	int _isChangingFollowMode;



	// ���X�|�[���������s�����ǂ���
	int _isRespawn;


	// BGM�̃{�����[��
	int _volume;
};
extern CamPhase camPhaseChip[STAGE_MAX][175];