#pragma once	// .h�̐擪�ɋL�q�B#include�ł��̃t�@�C�������x�ǂݍ��݂��Ă��A1�x�����ǂݍ��܂Ȃ�
#include "SceneBase.h"
#include "mymath.h"

#define MOVIE_PROLOGUE 0
#define MOVIE_CLEAR 1

class SceneMovie : public SceneBase {
public:
	SceneMovie(int movieNum);			// �R���X�g���N�^
	virtual ~SceneMovie();	// �f�X�g���N�^

	void	Input() override;	// ����
	void	Process() override;	// �v�Z
	void	Draw() override;	// �`��

	void AttachMeatBallToPlayer();

	int GetMovieNo() { return _movieNo; };
	int GetEndMovie() { return _isEndMovie; };

protected:
	// ���[�r�[���
	int _movieNo;
	// ���[�r�[���Ō�܂ŗ��ꂽ���ǂ���
	int _isEndMovie;

	// �w�i�摜�n���h��
	int _cgBgMovie;

	// �t�F�[�h�C���E�t�F�[�h�A�E�g�֘A
	int _step;
	int _stepcnt;

	int movievoicecnt = 0;
	int presscnt = 0;


	int press = 0;
	

	// 3D���f�����
	struct MODEL {
		int modelHandle;
		VECTOR pos;

		int animIndex;
		float totalTime;
		float playTime;
	};
	// 3D���f��
	MODEL _player; // �v���C���[�̃��f��
	MODEL _meatBall; // �~�[�g�{�[���̃��f��

	// �~�[�g�{�[���̉�]�p
	float _rot;
	// �~�[�g�{�[����z�u����t���[���̔ԍ�
	int _frameNo;


	// �v���C���[��3D���f���ɁA�J�����̈ʒu�ƒ����_�ƂȂ�t���[�����܂܂�Ă���
	// �J������z�u����t���[���̔ԍ�
	int _camFrameNo;
	// �J�����̒����_�ƂȂ�t���[���̔ԍ�
	int _gazeFrameNo;


	//�J�����̍��W
	VECTOR _camPos;

	// �����_
	VECTOR _gazePoint;



	// �~�[�g�{�[���̋����֘A

	// �A�E�g���C����`�悷�邩�ǂ���
	int _isDrawAura;

	// ��]��
	int _rotType;
	// ��]�l
	VECTOR _rotation;

	// �X�P�[�����O���Ă��邩�ǂ���
	int _isScaling;
	// �X�P�[�����O�̃J�E���g
	int _scalingCnt;

	
};