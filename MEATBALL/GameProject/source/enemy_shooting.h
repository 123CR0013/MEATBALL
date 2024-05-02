#pragma once
#include "DxLib.h"
#include "gamemain.h"
#include "enemybase.h"



// ���[�V�����p�萔
#define SHOOTING_MOTION_MAX 2

#define SHOOTING_MOTION_ENCOUNT 0
#define SHOOTING_MOTION_NORMAL 1

// �e�̍ő吔
#define BULLET_MAX 1

// �v���C���[�Ɍ������Ēe�����G
class EnemyShooting : public EnemyBase
{
public:
	EnemyShooting(int modelHandle, int size, int index, VECTOR startPos, VECTOR direction = VGet(0, 0, 0));
	~EnemyShooting();
	// �v�Z����
	void Process(VECTOR playerPos) override;
	// �`�揈��
	void Draw() override;
	
	

	// �`��ʒu�̕␳����
	void AdjustDrawPos(MATRIX mView) override;

	// �e��������������
	void ShotBullet(VECTOR playerPos);
	// �e�̌v�Z����
	void BulletProcess();

	// �e�j��G�t�F�N�g�̃Z�b�g
	void SetBulletHitEffect();

	// �e�̕`�揈��
	void DrawBullet(MATRIX mView);

	// �O�ʃ��C���[�ɕ`��
	// �e�̕`��
	void DrawFrontLayer(MATRIX mView) override;



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


protected:
	// �e�̑��x
	float _bulletSpeed;
	// �e�𔭎˂��Ă���A���̒e�𔭎˂���܂ł̃t���[����
	int _bulletCoolTime;

	// �e���
	struct BULLET {
		int use;
		VECTOR direction;
		VECTOR pos;

		MYRECT rect;

		int animCnt;
	};
	// �e
	BULLET bullet[BULLET_MAX];
	// �e�̃X�N���[�����W
	VECTOR _bulletScreenPos;


	// �ڂ̃p�[�c�����点��ۂ̖��邳
	float _eyeEmiColorFade;


	// �e�N�X�`�����
	static std::vector<TEXTURE>	_texture;
	// �}�e���A�����
	static std::vector<MATERIAL>	_material;
};