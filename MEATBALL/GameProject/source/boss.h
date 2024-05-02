#pragma once
#include "charabase.h"
#include "mymath.h"

#define BOSS_MOTION_MAX 16


#define BOSS_ATTACK_MAX 5

#define BOSS_BLOCK_MAX 20
#define BOSS_BULLET_MAX 30



#define BOSS_AFTERIMAGE_MAX 10


// �e���
struct BULLET
{
	int use = -1; // ���̔z��v�f���g�p�����ǂ���
	int shotFrame; // �U���J�n���炱�̒e��ł��n�߂�܂ł̌o�߃t���[����
	int speed; // �e�̑���
	VECTOR pos = VGet(-1, -1, -1); // �e�̏������W
	VECTOR direction = VGet(-1, -1, -1); // �e�̎ˏo����
};

// �U�����
struct BOSS_ATTACK {
	int id = -1;

	// ���̍U���ֈڍs����܂ł̃t���[����
	int frame = 0;
	// �{�X�̈ړ���̍��W
	VECTOR pos = VGet(-1, -1, -1);
	// �o���A�u���b�N���o�����W
	VECTOR blockPos[BOSS_BLOCK_MAX] = {VGet(-1, -1, -1)};


	// �e�̋����^�C�v
	// 0:���˒e, 1: �ݒu�e
	int bulletType = -1;

	// �e���
	BULLET bullet[BOSS_BULLET_MAX];

};

class Boss : public CharaBase
{
public:
	Boss(int modelHandle, VECTOR startPos);
	~Boss();

	// �v�Z����
	void Process(VECTOR playerPos);

	// �e�̏���������
	void BulletInit();
	// �e�̌v�Z����
	void BulletProcess();

	// �`��ʒu�̕␳����
	void AdjustDrawPos(MATRIX mView) override;
	// �`�揈��
	void Draw() override;
	// �e�̕`�揈��
	void DrawBullet(MATRIX mView);


	// �O�ʃ��C���[�ɕ`��
	// �e�̕`��
	void DrawFrontLayer(MATRIX mView) override;



	MYRECT GetBulletRect(int index);
	

	int GetBossBattle() { return _isBossBattle; };
	// �{�X�̍U�����J�n����
	void SetBossBattle();



	int GetHp() { return _hp; };
	void UpdateHp(int n) { _hp += n; };


	// �_���[�W���󂯂��Ƃ��������Z�b�g����
	void SetIsHit(float damage);


	// ���̍U�������Z�b�g����
	void SetAttackInfo(VECTOR playerPos);
	// �o�����Ă���e��S�ď���
	void DeleteAllBullet();
	// �U���p�^�[���������_���Ɍ��߂�
	void RandAttackPattern(VECTOR playerPos);
	// �U�������Q�Ƃ��ăo���A�`�b�v���o��������
	void SetBarrier();


	// �L�����N�^�[�̌����Ă�������ɂ���ă��[�V������ύX����
	// 
	// Process�������Ń��[�V�������w�肷��Ƃ��͑S�ĉE�����̃��[�V�������w�肷��
	// �L�����N�^�[�����������Ă���ꍇ�́A�������̃��[�V�����ɕύX����
	void SetMotionReverse();
	void MotionProcess(int motionNum_old) override;



	//
	// �c������
	//

	// ����������
	void AfterImageInit(int modelHandle);
	// �v�Z����
	void AfterImageProcess();
	// �c�����Z�b�g����
	void SetAfterImage();





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





	// �{�X��pBGM�̓ǂݍ���
	static void LoadBossBGM();
	// �{�X��pBGM�̔j��
	static void DeleteBossBGM();

	// �{�X��pBGM�̍Đ��E��~
	static void PlayBossBGM(int n);



	// �f�o�b�O���̕\��
	void DrawDebugInfo(MATRIX mView, int fillFrag = 1) override;

private:
	// �{�X�킪�J�n���Ă��邩�ǂ���
	int _isBossBattle;

	// �̗�
	int _hp;

	// ���G���ԃt���[����
	int _invincibleCnt;

	// �A�j���[�V�������
	CHARA_ANIM _anim[BOSS_MOTION_MAX];

	// ���̍U���Ɉڍs���邩�ǂ���
	int _changeAttack;

	// ���݂̍U�����
	BOSS_ATTACK _attackInfo;
	// ���݂̍U�����J�n����Ă���̌o�߃t���[����
	int _attackCnt;

	// ���j���ꂽ���ǂ���
	int _isEliminated;

	// �U�������ǂ���
	int _isAttack;
	// �U�����I�����Ă��玟�̍U���Ɉڂ�܂ł̃t���[����
	int _attackCoolTime;

	// ���̍U���n�_�܂ňړ�����ۂ̈ړ��O�̍��W
	// ���̒n�_�܂ł̈ړ����C�[�W���O��p���čs�����߁A�ړ��O�̍��W��ێ�����K�v������
	VECTOR _startPos;

	// �U�����󂯂����ǂ���
	int _isHit;

	// �I�[���`��p�̃A�j���[�V�����J�E���g
	int _auraAnimCnt;




	// �c�����
	AFTERIMAGE _afterImage[BOSS_AFTERIMAGE_MAX];



	// �e�N�X�`�����
	static std::vector<TEXTURE>	_texture;
	// �}�e���A�����
	static std::vector<MATERIAL> _material;


	// �{�X��pBGM
	static int	_bgmBossBattle;
};


extern Boss* boss;





