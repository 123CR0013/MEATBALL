#pragma once
#include "DxLib.h"
#include "gamemain.h"
#include "scenegamemain.h"

// �����蔻���`�̎��

// �{�̂̋�`
#define COLLISION_TYPE_BODY	  1
// ���G�͈�
#define COLLISION_TYPE_SEARCH	  2
// �U���͈�
#define COLLISION_TYPE_ATTACK	  3


struct CHARA_ANIM
{
	int index = -1;
	int totalTime = -1;
};

// �c�����
struct AFTERIMAGE
{
	// �g�p����
	int use = 0;

	// �c�����o��������^�C�~���O
	int frame = 0;
	// �c��������������t���[����
	int remainCnt = 0;

	// �c����3D���f��
	int modelHandle = -1;
	// �c���̍��W
	VECTOR pos = VGet(0, 0, 0);

	// �c���̃A�j���[�V����
	// �c����������playTime�ŌŒ�
	CHARA_ANIM anim;

	// �c���̓����x
	float alpha = 0;
};

// �e�N�X�`�����
struct TEXTURE
{
	// �e�N�X�`���ԍ�
	int index = -1;
	// �e�N�X�`���摜�n���h��
	int graphHandle = -1;
};

// �}�e���A�����
struct MATERIAL 
{
	// �}�e���A���ԍ�
	int index = -1;
	// �f�B�t���[�Y�J���[�i�g�U���F�j���
	COLOR_F difColor = GetColorF(0.0f, 0.0f, 0.0f, 0.0f);
};

// �L�����N�^�[�̊��N���X
class CharaBase {
public:
	CharaBase(int modelHandle, VECTOR startPos);
	virtual ~CharaBase();

	// �v�Z����
	virtual void Process();
	// �`�揈��
	virtual void Draw();

	// ���[�V�������̏���������
	void MotionInit(int motionMax);
	// ���[�V�����̌v�Z����
	virtual void MotionProcess(int motionNum_old);

	// �`��ʒu�̕␳����
	virtual void AdjustDrawPos(MATRIX mView);


	int GetModelHandle() { return _modelHandle; };

	// 3D���f���̉�]����
	void UpdateModelRotation();


	VECTOR GetPos() { return _pos; };

	// 3D���f���\���p���W�̍X�V
	virtual void UpdateWorldPos();



	VECTOR GetCenterPos() { return _centerPos; };



	void UpdateCenterPos();


	void SetDirection(VECTOR vDir) { _direction = vDir; };

	void SetCollision();
	MYRECT SetMyRect(MYRECT difRect, VECTOR pos);
	MYRECT GetMyRect(int collisionType);




	//void Attack(int i);
	void SetUse(int n) { _use = n; };
	int GetUse() { return _use; };


	// �e�N�X�`���摜�̓\��ւ�
	// �_���[�W�󂯂��Ƃ�, ���t���[�������^�����ȉ摜��\��
	// ���̌�, ���ɖ߂�
	// ����isDamaged �ɂ���ē\��e�N�X�`����ς���
	// 0 : �ʏ펞, 1 : ��_���[�W��
	// �� �֐����Ōp����̃N���X�̐ÓI�����o�ϐ��ɃA�N�Z�X����̂�, �p���悲�ƂɊ֐���`������
	virtual void ChangeTexture(int isDamaged);

	// ChangeTexture�̒��ŌĂ�
	// _modelHandle�̃e�N�X�`���ԍ�index�̃e�N�X�`���摜��graphHandle�̉摜�ɍ����ւ���
	void SetTexture(int index, int graphHandle);

	// �}�e���A���̃f�B�t���[�Y�J���[�i�g�U���F�j�̕ύX
	// ����isDamaged�ɂ���ăf�B�t���[�Y�J���[��ύX����
	// �� �֐����Ōp����̃N���X�̐ÓI�����o�ϐ��ɃA�N�Z�X����̂�, �p���悲�ƂɊ֐���`������
	virtual void ChangeMaterialDifColor(int isDamaged);

	// ChangeTexture�̒��ŌĂ�
	// _modelHandle�̃}�e���A���ԍ�index�̃f�B�t���[�Y�J���[�i�g�U���F�j��ύX����
	void SetMaterialDifColor(int index, COLOR_F difColor);




	// �O�ʃ��C���[�ɕ`��
	// �p����̊e�N���X�Œ�`
	virtual void DrawFrontLayer(MATRIX mView);


	// �f�o�b�O���̕\��
	virtual void DrawDebugInfo(MATRIX mView, int fillFrag = 1);



	// �^�����̃e�N�X�`���摜
	// �_���[�W�󂯂��Ƃ���3D���f���𔒂��_�ł�����i�e�N�X�`���𔒂��\��ւ���j
	static int _cgTexWhite;


protected:

	// ���̃C���X�^���X���g�p�����ǂ���
	int _use;

	// �U�������ǂ���
	int _isAttack;

	// �d�͏������s�����ǂ���
	int _useGravity;
	// �d�͒l �iy���̈ړ������j
	// �W�����v���͏d�͒l�Ƀ}�C�i�X�̐���������
	float _gravity;
	// ���ɗ����Ă��邩�ǂ���
	int _stand;

	// �ړ����x
	float _speed;


	//
	// ���W�֘A
	//

	VECTOR _pos;			// �N�_���W�i�����蔻��, �ړ������Ɏg���j 
	VECTOR _screenPos;		// �X�N���[�����̕\���ʒu�̕␳�Ɏg��
	VECTOR _worldPos;		// 3D���f���̕\���ʒu

	VECTOR _centerPos;			// ���S���W
	float _difCenterY;			// �N�_���W�ƒ��S���W�̍���
	VECTOR _direction;			// �����Ă������



	// �̂̓����蔻���`
	// ���v���C���[�╗�Ƃ̓����蔻��Ɏg��
	MYRECT _difBodyRect;				// �L���������蔻��(�z�u���W����̍��� x,y / �傫�� w,h)
	MYRECT _bodyRect;					// �����蔻���`(������W x,y / �傫�� w,h) 


	//
	// 3D���f���֘A
	// 

	// ���f���n���h��
	int _modelHandle;
	// ���݂̃��[�V�����ԍ�
	int _motionNum;
	// ���[�V�����̍Đ�����
	float _playTime;

	// �A�j���[�V�������
	CHARA_ANIM _anim[5];
};