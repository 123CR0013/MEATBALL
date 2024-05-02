#pragma once
#include "DxLib.h"
#include "gamemain.h"
#include "enemybase.h"
#include "enemy_data.h"


// ���[�V�����p�萔
#define SHIELD_MOTION_MAX 2
			
#define SHIELD_MOTION_ENCOUNT 0
#define SHIELD_MOTION_NORMAL 1



// �����v���C���[�̕����ɍ\���āA�~�[�g�{�[���ł̍U�����K�[�h����G
class EnemyShield : public EnemyBase
{
public:
	EnemyShield(int modelHandle, int size, int index, VECTOR startPos, int type);
	~EnemyShield();

	// �v�Z����
	void Process(VECTOR playerPos) override;

	// ���̒��S���W�Ɗp�x���v�Z����
	void UpdateShieldCenterPos();
	// ����3D���f������]����
	void SetShieldModelRotation();

	// �`��ʒu�̕␳����
	void AdjustDrawPos(MATRIX mView) override;
	// �`�揈��
	void Draw() override;

	// �O�ʃ��C���[�ɕ`��
	// ���̕`��
	void DrawFrontLayer(MATRIX mView) override;


	//
	// �e�N�X�`���֘A
	//
	// �� �ÓI�����o�֐��̓N���X�̌p���悲�Ƃɒ�`������
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




	// ���炩���ߏ��̃��f����ǂݍ���ł���
	// ���G�̃C���X�^���X�𐶐�����Ƃ��ɂ�����R�s�[���Ďg�p����
	static int _shieldModelBase;

protected:

	// �s���^�C�v
	// 0 : stay, 1 : move
	int _moveType;


	// ���̃��f���n���h��
	int _shieldModelHandle;
	// ����3D���f���̏����̉�]�l
	VECTOR _defaultShieldRot;

	// ���̒��S���W
	VECTOR _shieldCenterPos;
	// ���̊p�x
	VECTOR _shieldDirection;

	// �`��p���W
	// �X�N���[�����W�i����p�̍��W�ɃJ�����̕␳���|�������́j
	VECTOR _shieldScreenPos;
	// ���[���h���W�i_shieldScreenPos��3D���f���`��p�̍��W�ɕϊ��������́j
	VECTOR _shieldWorldPos;

	// �e�N�X�`�����
	static std::vector<TEXTURE>	_texture;
	// �}�e���A�����
	static std::vector<MATERIAL>	_material;



};



//
// �\���́uSHIELD�v�ɑ΂��鏈��
// 

// ����������������i�S�ă��Z�b�g����j
void InitShieldInfo();
// �������Z�b�g����
void SetShieldInfo(int index, float r, int h);
// �������X�V����
void UpdateShieldInfo(int index, int use, VECTOR pos = VGet(0, 0, 0), VECTOR dir = VGet(0, 0, 0));
// �������擾����
SHIELD GetShieldInfo(int index);