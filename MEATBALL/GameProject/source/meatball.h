#pragma once
#include "DxLib.h"
#include "gamemain.h"
#include "mymath.h"
#include "charabase.h"

// ���ˁi�Ĕ��ˁj���̃X�s�[�h
#define SPEED_MAX 25
// �X�s�[�h�̍Œ�l
#define SPEED_MIN 5
// �����p�l���ɏ�������̃X�s�[�h
#define SPEED_DASH 40

// 1�t���[��������̌����l
#define SPEED_DECREMENT_VALUE 0.1f
// ���ˁi�����j���Ă��猸�����n�߂�܂ł̃t���[����
#define LAUCH_COUNT_MAX 80

// �����p���[
#define POWER_DEFAULT 50
// size��1�����邲�Ƃɑ�������p���[
#define POWER_INCREMENT_VALUE 5

// �����̔��a�T�C�Y
#define RADIUS_DEFAULT 16
// size��1�����邲�Ƃɑ������锼�a�T�C�Y
#define RADIUS_INCREMENT_VALUE 4

// �����̊g�嗦
#define MAGNIFICATION_DEFAULT 1.0f
// size��1�����邲�Ƃɑ�������g�嗦
#define MAGNIFICATION_INCREMENT_VALUE (4.0f / 16.0f)    // (RADIUS_INCREMENT_VALUE / RADIUS_DEFAULT)

// �čU�����ł���܂ł̎���
#define MEATBALL_ATTACK_COOLTIME 2 * 30

#define MEATBALL_ROTAION_COOLTIME 3 * 60


// �Ăі߂���������Ă��鎞�ԁA�܂��́A�~�[�g�{�[������ʊO�ɂ��鎞�Ԃ����t���[�����o�߂���ƃ~�[�g�{�[�����v���C���[�̌��֋A���Ă���
#define MEATBALL_RETURN_CNT_MAX 30

// �c���̍ő吔
#define MEATBALL_AFTERIMAGE_MAX 10


// �~�[�g�{�[���ɃA�^�b�`����@�B�p�[�c�̍ő吔
#define MEATBALL_METAL_PARTS_MAX 8


class MeatBall : public CharaBase {
public:
	MeatBall(int modelHandle, VECTOR playerPos);
	~MeatBall();

	// �~�[�g�{�[�����v���C���[�̃^���N���ɔz�u���邽�߂̏���������
	// ����:
	// �v���C���[�̃��f���n���h��
	// �^���N�̃t���[���ԍ�
	void SetAttachToPlayer(int pHandle, int n);

	// �v�Z����
	void Process(VECTOR playerPos);
	// �`�揈��
	void Draw(MATRIX mview);
	// �`��ʒu�̕␳����
	void AdjustDrawPos(MATRIX mView) override;

	// �O�ʃ��C���[�ɕ`��
	// �������̕`��
	void DrawFrontLayer(MATRIX mView);

	// �������̕`��
	void DrawArrow(MATRIX mView);

	// 3D���f���\���p���W�̍X�V
	void UpdateWorldPos() override;

	// �U����Ԃ�ύX����
	// ���� n
	// 0: ��U����Ԃ�, 1: �U����Ԃ�
	void SetAttack(VECTOR playerCenterPos = VGet(0, 0, 0), int n = 1);

	// �v���C���[�̌��֖߂�
	void ReturnToPlayer(VECTOR playerPos);

	// �G�E�{�X�Ɠ��������Ƃ��̔��ˏ���
	void Reflect(MYRECT objRect);

	int GetAttackCollTime() { return _attackCoolTime; };


	float GetPower() { return _power; };
	void SpeedUp() { _speed = SPEED_DASH; _launchCnt = LAUCH_COUNT_MAX; };
	float GetSpeed() { return _speed; };



	VECTOR GetPos() { return _pos; };
	void	 SetPos(VECTOR tmpPos) { _pos = tmpPos; };



	VECTOR GetDirection() { return _direction; };
	void SetDirection(VECTOR dirTmp) { _direction = dirTmp; };
	float GetRadius() { return _r; };

	int GetIsAttack() { return _isAttack; };
	int GetCanReattack() { return _canReattack; };

	// �}�b�v�`�b�v�Ƃ̔��˗p��`�̍X�V
	void UpdateReflectRect();

	MYRECT GetReflectRect() { return _reflectRect; };


	// 3D���f���p���W�̍X�V
	//void UpdateWorldPos();



	// �T�C�Y�̍X�V����
	void UpdateSize(int n);
	int  GetSize() { return _size; };
	void SetSize(int n) { _size = n; };

	void SetLevel(int n) { _level = n; };
	int  GetLevel() { return _level; };



	// �~�[�g�{�[���̉�]��5�̃p�^�[�����烉���_���Ō��肷��
	void SetRotType();

	// �~�[�g�{�[������ʓ��ɂ��邩�𔻒肷��
	// �Ԃ�l 0: ��ʓ��ɂ��Ȃ�, 1: ��ʓ��ɂ���
	int CheckIsInScreen();

	// �~�[�g�{�[������ʊO�ɏo�Ă��鎞�Ԃ��擾����
	// 60�ɂȂ�ƃv���C���[�̌��ɖ߂�
	int GetOffScreenTime() { return _offScreenTime; };


	void SetHitStopCnt() { _hitStopCnt = 10; _beforHitStopPos = _pos; };

	int GetReturn() { return _isReturn; };

	
	void SetIsDrawFullArrow(int n) { _isDrawFullArrow = n; };


	//
	// �X�P�[�����O
	//

	// �X�P�[�����O�̊J�n�ݒ�
	void SetScaling();
	// �X�P�[�����O����
	void ScalingProcess(VECTOR vScale);


	//
	// �@�B�p�[�c����
	//

	// �@�B�p�[�c�̏���������
	void MetalPartsInit(int metalPartsModel[]);
	// �@�B�p�[�c�̌v�Z����
	// �~�[�g�{�[���̓���̃��O�ɋ@�B�p�[�c����������
	void MetalPartsProcess();
	// �@�B�p�[�c�̕`�揈��
	void MetalPartsDraw();






	//
	// �G�t�F�N�g
	//

	// ���G�t�F�N�g�̃Z�b�g
	void SetSmokeEffect(HITCHIP hitChip);
	// �`�b�v�j��G�t�F�N�g�̃Z�b�g
	void SetChipBreakEffect(HITCHIP hitChip);
	// �I�[���`��̐ݒ�
	void SetIsDrawAura(int n);
	// �Ăі߂��G�t�F�N�g�̃Z�b�g
	void SetReturnEffect();



	//
	// �c���֘A�̏���
	//

	// �c���̏���������
	void AfterImageInit(int modelHandle);
	// �c���̌v�Z����
	void AfterImageProcess();
	// �c�����Z�b�g����
	void SetAfterImage();
	// �c���̕`�揈��
	void DrawAfterImage();



	// �f�o�b�O���̕\��
	void DrawDebugInfo(MATRIX mView, int fillFrag = 1) override;



	// �������̉摜�n���h��
	static int _cgAim;


private:
	// �U����
	float _power;
	// �~�[�g�{�[�����΂�����
	// ���X�e�B�b�N����
	VECTOR _arrow; 

	// �~�[�g�{�[���̔��a
	float _r; 
	// ���݂̈ړ����x
	float _speed;	

	// ���˂��Ă���čU���i�����]���j�ł���悤�ɂȂ�܂ł̃t���[����
	int _attackCoolTime;
	// �čU���i�����]���j�ł��邩�ǂ���
	int _canReattack;

	// �~�[�g�{�[�����v���C���[�̎茳�ɂ��邩�ǂ���
	int _isReturn;	

	// ���˂��Ă��猸������܂ł̃J�E���g
	int _launchCnt;

	// ��ʓ��ɃC���邩�ǂ���
	int _isInScreen;
	// ��ʊO�ɏo�Ă���J�E���g��
	// ��莞�ԉ�ʊO�ɏo�Ă���ƁA�����I�Ƀv���C���[�̂��Ƃɖ߂�
	int _offScreenTime;

	// �}�b�v�`�b�v�Ƃ̓����蔻��p
	// ���˂��邩�̔���Ɏg��
	// �~�ɓ��ڂ��鐳���`
	MYRECT _reflectRect;		// �n�`�����蔻��(������W x,y / �傫�� w,h)

	// ���݂̃~�[�g�{�[���T�C�Y
	int _size;
	// ���݂̃~�[�g�{�[�����x��
	int _level;
	

	//
	// 3D���f���֘A
	//

	// 3D���f���̏����T�C�Y
	VECTOR _defaultSize;
	// 3D���f���̉�]�l
	float _rad;
	// ��]�̃^�C�v�i5�̃p�^�[�����烉���_���Ɍ��߂�j
	int _rotType;
	// ��]�^�C�v��ύX���Ă���A���̉�]�^�C�v�ɕύX����܂ł̃t���[����
	int _rotCoolTime;


	// �v���C���[��3D���f���n���h��
	// �{�[�����v���C���[�̔|�{�|�b�h���ɔz�u����ۂɎg��
	int _playerHandle;
	// �{�[�����A�^�b�`����t���[���ԍ��i�v���C���[3D���f���̃��O�̔ԍ��j
	int _attachNum;






	// �q�b�g�X�g�b�v�J�n�O�̍��W
	VECTOR _beforHitStopPos;
	// �q�b�g�X�g�b�v�̃J�E���g
	int _hitStopCnt;



	// ���������ő�̒����ŕ`�悷�邩�ǂ���
	// 0�̂Ƃ���1/4�̒����ŕ`�悷��
	int _isDrawFullArrow;

	// �I�[����`�悷�邩�ǂ���
	int _isDrawAura;
	// �G�t�F�N�g�摜�̃A�j���[�V�����p�J�E���g
	int _effectCnt;


	// �c����3D���f�����
	AFTERIMAGE _afterImage[MEATBALL_AFTERIMAGE_MAX];


	// �@�B�p�[�c
	struct METAL
	{
		int modelHandle = -1;				// ���f���n���h��
		int level = -1;						// ���݂̃~�[�g�{�[���̃��x�������̐��l�𒴂��Ă�����A���̃p�[�c��`�悷��
		int attachNum = -1;					// ���̃p�[�c��z�u���郊�O�̖��O
		VECTOR scale = VGet(0, 0, 0);		// 3D���f���̊g��l
		VECTOR rotation = VGet(0, 0, 0);	// 3D���f���̉�]�l
	};

	METAL _metal[MEATBALL_METAL_PARTS_MAX];




	// �X�P�[�����O
	int _isScaling;
	// �X�P�[�����O�̃J�E���g
	int _scalingCnt;


	// �����o�����������肷�邽�߂̏��
	struct EXTRUSION
	{
		// �����o����
		// 0 : x��, 1: y�� 
		int axis;

		// �����o������
		// -1 : �}�C�i�X����, 1 : �v���X����
		int direction;

		// �����蔻��������W
		VECTOR point;
	};
};



extern MeatBall* meatBall;