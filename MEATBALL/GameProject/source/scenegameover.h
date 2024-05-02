#pragma once	// .h�̐擪�ɋL�q�B#include�ł��̃t�@�C�������x�ǂݍ��݂��Ă��A1�x�����ǂݍ��܂Ȃ�
#include "SceneBase.h"

class SceneGameOver : public SceneBase {
public:
	SceneGameOver();			// �R���X�g���N�^
	virtual ~SceneGameOver();	// �f�X�g���N�^

	void	Input() override;	// ����
	void	Process() override;	// �v�Z
	void	Draw() override;	// �`��

	

	void MeatBallProcess();


	void MenuProcess();
	
	void FadeProcess();



	int GetIsRetry() { return _isRetry; };


protected:
	// ���g���C��I���������ǂ���
	// �I�����ꂽ�ꍇ�Ascenegamemain.cpp���ł��̃C���X�^���X���폜���Q�[�����C���V�[���Ɉڂ�
	int _isRetry;


	//3D���f���֘A
	int _playerModel;

	VECTOR _playerPos;



	float  _viewRoute;
	VECTOR _CameraPos;




	int _floorModel;


	int _meatBallModel;
	VECTOR _meatBallPos;
	VECTOR _meatBallDefaultScale;

	int _faceFrameNo;



	// �����̎��
	// 0 ~ 2 ���h��
	// 3 ~ 6 �W�����v
	int _moveCnt;


	// ���h��
	float _zTwist;
	int _twistOrder;
	int _twistCnt;

	// �W�����v
	float _meatBallY;
	int _isJump;
	int _jumpCnt;


	//�I���֘A
	int use = 0;
	int setCnt = 0;	
	int noperation = 0;
	
	/*int sound_volume = 0;
	int volMusic = 150;*/

	int		_step;		// �t�F�[�h�C���A�E�g�p�X�e�b�v
	int		_stepcnt;	// �X�e�b�v�J�E���^



};

