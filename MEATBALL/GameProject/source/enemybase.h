#pragma once
#include "DxLib.h"
#include "gamemain.h"
#include "mymath.h"

#include "charabase.h"
#include "enemy_data.h"

#include <vector>


// �G�̃^�C�v�����ʂ��邽�߂̒萔
#define ENEMY_TYPE_FLOAT 0
#define ENEMY_TYPE_SHOOTING 1
#define ENEMY_TYPE_GROUND 2
#define ENEMY_TYPE_SHIELD 3
#define ENEMY_TYPE_TRACER 4
#define ENEMY_TYPE_METAL 5

// �G�L�����N�^�[�̊��N���X
class EnemyBase : public CharaBase
{
public:
	EnemyBase(int modelHandle, int index, VECTOR startPos, VECTOR direction = VGet(0, 0, 0));
	~EnemyBase();

	// �v�Z����
	virtual void Process(VECTOR playerPos);
	// �`�揈��
	virtual void Draw();

	// �G�̋����������ݒ肷��
	// Small, Medium, Large
	void SetStregth(int type, int size);


	// �����蔻���`���擾����
	MYRECT GetMyRect(int collisionType);

	int GetType() { return _type; };
	int GetIndex() { return _index; };


	void UpdateHp(int n) { _hp += n; };
	int GetHP() { return _hp; };



	void SetCollision();


	// ��ʓ��ɂ��邩�𔻒肷��
	void CheckIsInScreen();
	int GetIsInScreen() { return _isInScreen; };

	// ���G��ԁi�v���C���[�������Ă����ԁj��ݒ肷��
	// ���� n
	// 0: ����G��Ԃɂ���, 1: ���G��Ԃɂ���
	void SetIsSpotPlayer(int n) { _isSpotPlayer = n; };

	virtual void SetAttack(int n) { _isAttack = n; };
	int GetAttack() { return _isAttack; };



	int GetCoolTime() { return _attackCoolTime; };


	void SetIsHit() { _isHitCnt = 3; };

	int  GetHitStop() { return _isHitStop; };
	void SetHitStop(VECTOR meatBallDir) { _isHitStop = 1; _hitStopCnt = 10; _hitStopDir = meatBallDir; };
	// �q�b�g�X�g�b�v����
	void HitStopProcess();



	// 3D���f���\���p���W�̍X�V
	void UpdateWorldPos() override;



	// �f�o�b�O���̕\��
	void DrawDebugInfo(MATRIX mView, int fillFrag = 1) override;




	// �n��G�p�ienemy_ground.cpp�Ŋ֐��̒��g�������j
	// ���^����Ԃ��ǂ������擾����
	virtual int GetIsMetal() { return 0; };
	// ���^����Ԃ���ʏ��Ԃɖ߂�
	virtual void ChangeToNormal() {};




protected:
	// �^�C�v
	int _type;
	// �C���f�b�N�X�ԍ��i�^�C�v���Ƃɔԍ�0���珇�Ԃɐݒ肷��j
	int _index;

	// �̗�
	int _hp;

	// ��ʓ��ɂ��邩�ǂ���
	int _isInScreen;
	// ���G��Ԃ��ǂ����i�v���C���[�𔭌����Ă��邩�j
	int _isSpotPlayer;

	// ���G�͈�
	// ���͈͓̔��Ƀv���C���[����������A���G��Ԃɂ���
	
	MYRECT _difSearchRect;	// ���G�͈͂̋N�_�ʒu����̍���(�z�u���W����̍��� x,y / �傫�� w,h)	
	MYRECT _searchRect;		// ���G�͈͋�`(������W x,y / �傫�� w,h) 


	// �U���͈�
	// �e�^�C�v�̓G�̒e�Ȃǂ̍U������Ɏg��
	// �i�v���C���[�ƓG�̑̂Ƃ̓����蔻���_bodyRect�ōs���j
	MYRECT _difAttackRect;	// �U���͈͂̋N�_�ʒu����̍���(�z�u���W����̍��� x,y / �傫�� w,h)
	MYRECT _attackRect;		// �U���͈͋�`(������W x,y / �傫�� w,h) 


	// �c��U������
	int _attackRemainTime;
	// �U�����I����Ă��玟�̍U���܂ł̃N�[���^�C��
	int _attackCoolTime;


	
	// �U������Ă���̃J�E���g��
	// �U������Ă��琔�t���[���̊Ԃ́A�G�̃e�N�X�`���ƃ}�e���A���f�B�t���[�Y�����ύX����
	int _isHitCnt;

	// �q�b�g�X�g�b�v�����ǂ���
	int _isHitStop;
	// �q�b�g�X�g�b�v�̃J�E���g
	int _hitStopCnt;
	// �q�b�g�X�g�b�v���ɓG�̃��f����h�炷����
	VECTOR _hitStopDir;
};