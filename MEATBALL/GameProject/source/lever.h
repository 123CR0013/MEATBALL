#pragma once
#include "gimmick.h"
#include "gimmick_data.h"

#include "jumppad.h"


#define LEVER_MODEL_RED 0
#define LEVER_MODEL_BLUE 1

class Lever : public Gimmick
{
public:
	Lever(int stageNo, int index);
	~Lever();
	// �`��ʒu�̕␳����
	void AdjustDrawPos(MATRIX mView) override;


	void ModelInit(int modelHandle, VECTOR size, int motionMax, int stageNo) override;


	void Process();

	void SetActive();
	int GetActive() { return _isActive; };


	// �A�E�g���C���̐ݒ�
	// ���� n  
	// 0: ������, 1: �L����
	void SetOutLine(bool n);


	void DrawDebugInfo(MATRIX mView) override;



	static int _leverModel[2];

protected:
	// ���o�[���쓮�������ǂ���
	int _isActive;

	// ��L����Ԃɖ߂邩�ǂ���
	int _returnInactive;

	// ���o�[�̎c��쓮����
	// 0�ɂȂ�܂Ŏ��̗L�����͍s��Ȃ�
	// ���[�V�����E�G�t�F�N�g�ƗL�����̃^�C�~���O�����킹��
	int _remainActiveTime;

	// ���[�V�����̍Đ�����
	// ���o�[�̃��[�V�����͓|��Ă��猳�ɖ߂�
	// 1 : �ʏ�, -1 : �t
	int _isPlayMotion;


	//
	// �쓮���ɃA�N�V����������M�~�b�N
	// �igimmickdata�ƑΉ��t����j
	//

	// �A�N�V����������M�~�b�N�̃^�C�v
	int _gimmickType;
	// �A�N�V����������M�~�b�N��id 
	int _gimmickId;
};


extern Lever* lever[];


