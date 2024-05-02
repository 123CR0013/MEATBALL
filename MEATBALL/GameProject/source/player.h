#pragma once
#include "DxLib.h"
#include "gamemain.h"
#include "mymath.h"

#include "charabase.h"
#include "heart.h"


#define PLAYER_MOTION_MAX 18

#define PLAYER_AFTERIMAGE_MAX 10





class Player : public CharaBase {
public:
	Player(int modelHandle, VECTOR startPos);
	~Player();

	// �v�Z����
	void Process();
	// �`�揈��
	void Draw() override;
	// �`��ʒu�̕␳����
	void AdjustDrawPos(MATRIX mView) override;

	// 3D���f���\���p���W�̍X�V
	void UpdateWorldPos() override;


	// �L�����N�^�[�̌����Ă�������ɂ���ă��[�V������ύX����
	// 
	// Process�������Ń��[�V�������w�肷��Ƃ��͑S�ĉE�����̃��[�V�������w�肷��
	// �L�����N�^�[�����������Ă���ꍇ�́A�������̃��[�V�����ɕύX����
	void SetMotionReverse();
	void MotionProcess(int motionNum_old) override;


	void SetPos(VECTOR pos) { _pos = pos; };
	void SetGravity(int n) { _gravity = n; };

	void SetIsAttacked(int n) { _isAttacked = n; };

	int GetIsValidHit() { return _isValidHit; };


	void SetStand() { _stand = 1; _gravity = 0; };

	// �_�b�V����r���ŃL�����Z������
	void StopDash();


	// �~�[�g�{�[�����Ăі߂��܂ł̃J�E���g���擾����
	// max 60�ɂȂ�ƌĂі߂����
	int GetReturnCnt() { return _returnCnt; };

	// ��_���[�W����
	void SetDamage();

	int GetGameOver() { return _isGameOver; };
	int GetGameOverMotion() { return _isPlayingGameOverMotion; };




	// �A�E�g���C���̐ݒ�
	void SetOutLine();


	// �c������
	void AfterImageInit(int modelHandle);
	// �c���̌v�Z����
	void AfterImageProcess();
	// �c���̕`�揈��
	void SetAfterImage();


	// SE�֘A

	// �Đ�����SE���~����
	// �T�C�R�L�l�V�XSE�ƌĂі߂�SE���~����
	void StopSE();



	// ���G�t�F�N�g�̃Z�b�g
	void SetSmokeEffect();




	//
	// �e�N�X�`���֘A
	//
	// �� �֐����Ōp����̃N���X�̐ÓI�����o�ϐ��ɃA�N�Z�X����̂�, �p���悲�ƂɊ֐���`������


	// �\���́uTEXTURE�v�̏�����
	static void TextureInit(int modelHandle);

	// �e�N�X�`���摜�̓\��ւ�
	// �_���[�W�󂯂��Ƃ�, ���t���[�������^�����ȉ摜��\��
	// ���̌�, ���ɖ߂�
	// ����isDamaged �ɂ���ē\��e�N�X�`����ύX����
	// 0 : �ʏ펞, 1 : ��_���[�W��
	void ChangeTexture(int isDamaged) override;

	// �\���́uMATERIAL�v�̏�����
	static void MaterialInit(int modelHandle);

	// �}�e���A���̃f�B�t���[�Y�J���[�̕ύX
	// ����isDamaged�ɂ���ăf�B�t���[�Y�J���[��ύX����
	void ChangeMaterialDifColor(int isDamaged) override;





	// �f�o�b�O���̕\��
	void DrawDebugInfo(MATRIX mView, int fillFrag = 1) override;





	// �n�[�g�̃C���X�^���X
	Heart _heart;






protected:



	// �G����̍U�����󂯂���
	int _isAttacked;
	// �G��G�̍U���Ƃ̓����蔻�肪�L�����ǂ���
	int _isValidHit;
	// �c��̖��G����
	int _invincibleTime;
	// ��_���[�W���[�V�������Đ��\���ǂ����i1��̃_���[�W�ŕ����񃂁[�V�������Đ�����Ȃ��悤�ɂ��邽�߂Ɏg���j
	int _canPlayHitMotion;


	// �_�b�V���\���ǂ���
	int _canDash;
	// �c��_�b�V������
	int _remainDashTime;
	// �_�b�V���̃N�[���^�C��
	int _dashCoolTime;


	// �~�[�g�{�[�����Ăі߂��܂ł̃J�E���g
	// �J�E���g�����ȏ�ɂȂ�����A�~�[�g�{�[�����Ăі߂��i���[�v������j
	int _returnCnt;

	// �A�j���[�V�������
	CHARA_ANIM _anim[PLAYER_MOTION_MAX];






	// �Q�[���I�[�o�[�������ǂ���
	int _isGameOver;
	// �Q�[���I�[�o�[���[�V�����Đ������ǂ���
	int _isPlayingGameOverMotion;


	// �I�[����`�悷�邩�ǂ���
	// 0: �`�悵�Ȃ�, 1: �~�[�g�{�[�����˃z�[���h��, 2: �~�[�g�{�[���Ăі߂���
	int _isDrawAura;
	int _auraCnt;





	AFTERIMAGE _afterImage[PLAYER_AFTERIMAGE_MAX];

	// �e�N�X�`�����
	static std::vector<TEXTURE>	_texture;
	// �}�e���A�����
	static std::vector<MATERIAL>	_material;

};