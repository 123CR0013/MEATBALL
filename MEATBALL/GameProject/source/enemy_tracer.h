#pragma once
#include "DxLib.h"
#include "gamemain.h"
#include "mymath.h"
#include "enemybase.h"


#define TRACER_MOTION_MAX 2

#define TRACER_MOTION_ENCOUNT 0
#define TRACER_MOTION_NORMAL 1

class EnemyTracer : public EnemyBase
{
public:
	EnemyTracer(int modelHandle, int size, int index, VECTOR startPos, VECTOR direction = VGet(0, 0, 0));
	~EnemyTracer();

	// �v�Z����
	void Process(VECTOR playerPos) override;
	//void Draw() override;

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
	//void DrawDebugInfo(MATRIX mView, int fillFrag = 1) override;



protected:

	// SE���Đ��\���ǂ���
	int _canPlaySe;

	// �e�N�X�`�����
	static std::vector<TEXTURE>	_texture;
	// �}�e���A�����
	static std::vector<MATERIAL>	_material;




};