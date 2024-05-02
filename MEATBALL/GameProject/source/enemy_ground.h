#pragma once
#include "DxLib.h"
#include "gamemain.h"
#include "mymath.h"
#include "enemybase.h"

// ����G�̍��E�̈ړ��͈�
#define ENEMY_GROUND_WALK_RANGE 512

// ����G���̈ړ����x�i�ʏ��ԁj
#define ENEMY_GROUND_SPEED_WALK 1
// �ǐՎ��̈ړ����x�i�ʏ��ԁj
#define ENEMY_GROUND_SPEED_RUN	3
// �U�����̈ړ����x�i�ʏ��ԁj
#define ENEMY_GROUND_SPEED_ATTACK 8

// ����G���̈ړ����x�i���^����ԁj
#define ENEMY_METAL_SPEED_WALK 1
// �ǐՎ��̈ړ����x�i���^����ԁj
#define ENEMY_METAL_SPEED_RUN	2
// �U�����̈ړ����x�i���^����ԁj
#define ENEMY_METAL_SPEED_ATTACK 5


// �U����Ԃ��������鎞��
#define ENEMY_GROUND_ATTACKTIME 3 * 60
// �U����Ɉړ����J�n����܂ł̎���
#define ENEMY_GROUND_COOLTIME 1 * 60

// ���[�V�����p�萔
#define GROUND_MOTION_MAX 3
				
#define GROUND_MOTION_RUN 0
#define GROUND_MOTION_TIRED 1
#define GROUND_MOTION_WALK 2



// �n���������A�v���C���[��������ƒǂ������Ă���G
// �ʏ��Ԃƃ��^����Ԃ�����
class EnemyGround : public EnemyBase
{
public:
	EnemyGround(int modelHandle, int size, int index, VECTOR startPos, int type);
	~EnemyGround();

	

	// �v�Z����
	void Process(VECTOR playerPos) override;
	// ���[�V�����̌v�Z����
	void MotionProcess(int motionNum_old) override;
	// �`�揈��
	void Draw() override;
	// �`��ʒu�̕␳����
	void AdjustDrawPos(MATRIX mView) override;


	void SetAttack(int n) override;


	// ���^����Ԃ��ǂ������擾����
	int GetIsMetal() override { return _isMetal; };
	// ���^����Ԃ���ʏ��Ԃɖ߂�
	void ChangeToNormal() override { _isChange = 1; };


	//
	// �e�N�X�`���֘A
	//

	// �e�N�X�`���摜�̓\��ւ�
	// �_���[�W�󂯂��Ƃ�, ���t���[�������^�����ȉ摜��\��
	// ���̌�, ���ɖ߂�
	// ����isDamaged �ɂ���ē\��e�N�X�`����ς���
	// 0 : �ʏ펞, 1 : ��_���[�W��
	void ChangeTexture(int isDamaged) override;

	// �\���́uMATERIAL�v�̏�����
	static void MaterialInit(int modelHandle);

	// �}�e���A���̃f�B�t���[�Y�J���[�̕ύX
	// ����isDamaged�ɂ���ăf�B�t���[�Y�J���[��ύX����
	void ChangeMaterialDifColor(int isDamaged) override;







	// �@�B�p�[�c�̕`��ʒu�̕␳����
	void MetalPartsAdjustDrawPos(MATRIX mView);



	// �f�o�b�O���̕\��
	//void DrawDebugInfo(MATRIX mView, int fillFrag = 1) override;

	// �e�N�X�`���摜�n���h��
	static int _cgTexEnGround[2];

protected:

	// ���^����Ԃ��ǂ���
	int _isMetal;
	// ���^����Ԃ���ʏ��ԂɈڍs���邩�ǂ���
	int _isChange;

	// �@�B�p�[�c�̃��f���n���h��
	int _metalModel;


	// ����G���ɂ��鍶�E�ړ��̋N�_�ʒu
	// ���̍��W�𒆐S��ENEMY_GROUND_WALK_RANGE�͈̔͂��ړ�����
	VECTOR _walkStartPos;

	// �̂Ɏh�����Ă���@�B�̃��f������������Ƃ��Ɏg��
	// �����̊J�n�ʒu
	VECTOR _fallStartPos;
	// �����̏I���ʒu
	VECTOR _fallEndPos;
	// �������̃J�E���g
	int _fallCnt;


	// SE���Đ��\���ǂ���
	// �U����ԂɂȂ����^�C�~���O��1�xSE��炷
	int _canPlaySe;

	// �}�e���A�����
	static std::vector<MATERIAL> _material;
};