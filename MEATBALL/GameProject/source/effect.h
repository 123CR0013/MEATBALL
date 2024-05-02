#pragma once
#define EFFECT_MAX	50 //EFFECT�\���̔z��̍ő�v�f��

// ���j�G�t�F�N�g
#define EFFECT_TYPE_DEFEAT_MACHINE 0
#define EFFECT_TYPE_DEFEAT_CREATURE 1
// �ˌ��G �e�G�t�F�N�g
#define EFFECT_TYPE_BULLET_HIT 2
#define EFFECT_TYPE_BULLET_CHARGE 3
// �`�b�v�j��G�t�F�N�g
#define EFFECT_TYPE_CHIP_BREAK 4

// �{�X �e�G�t�F�N�g
#define EFFECT_TYPE_BOSS_BULLET_HIT 5
// �~�[�g�{�[���d���G�t�F�N�g
#define EFFECT_TYPE_MEATBALL_ELECTRO 6

// ���G�t�F�N�g
// �v���C���[: �W�����v
// �v���C���[�E�{�X: ���n
// �~�[�g�{�[��: ����
#define EFFECT_TYPE_SMOKE 7



// �~�[�g�{�[�����Ăі߂�����ɏo��G�t�F�N�g
#define EEFECT_TYPE_MEATBALL_RETURN 8


// �~�[�g�{�[���Ə������������Ƃ��̉��G�t�F�N�g
// EFFECT_TYPE_SMOKE�̉摜��F��ς��ĕ`�悷��
#define EFFECT_TYPE_SMOKE_COLOR 9


struct EFFECT
{
	int use;		// ���̔z��v�f���g�p���Ă��邩

	int type;	// �G�t�F�N�g�̎��

	int x, y;		// �G�t�F�N�g���Đ�������W
	int w, h; // �\���T�C�Y

	int cnt;		// �A�j���[�V�����J�E���g
	int sheets;	// �摜�̖���
	int speed;	// �G�t�F�N�g�̍Đ����x�i1��������̕\���t���[�����j
	
	int loop; // ���[�v�Đ����邩
};

// ����������
void EffectInit();

// �G�t�F�N�g���Z�b�g���A���̗v�f�ԍ���Ԃ�
// �G�t�F�N�g��C�ӂ̃^�C�~���O�ŏ��������ꍇ�ɔz��ԍ����K�v�ɂȂ�
//int SetEffect(int x, int y, int type, int loop, int w = 0, int h = 0);
int SetEffect(int type, int x, int y, int w, int h, int loop = 0);


// �v�Z����
void EffectProcess();

// �G�t�F�N�g���~����
// SetEffect�Ŏ擾�����v�f�ԍ����g�p����
void StopEffect(int index);



// �w�ʃ��C���[�ւ̕`��
void DrawBackEffect(MATRIX mView);



// �O�ʃ��C���[�ւ̕`��
void DrawFrontEffect(MATRIX mView);