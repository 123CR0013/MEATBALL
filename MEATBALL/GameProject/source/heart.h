#pragma once
#include "DxLib.h"
#include "gamemain.h"

// �n�[�g���v���C���[�����яo���Ă��������܂ł̎���
#define HEART_COUNT_MAX 10 * 60 // 10�b

class Heart
{
public:
	Heart();
	~Heart();

	// �v�Z����
	void Process();
	// �`��ʒu�̕␳����
	void AdjustDrawPos(MATRIX mView);
	// �`�揈��
	void Draw();

	// ���[���h���W���X�V����
	void UpdateWorldPos();

	void SetUse(int n);
	int  GetUse() { return _use; };

	int GetLost() { return _isLost; };

	void SetPos(VECTOR playerPos);
	void SetDirection(int dirX) { _direction = dirX; };

	void UpdateBodyRect();
	MYRECT GetBodyRect() { return _bodyRect; };

	int GetRemainCnt() { return _remainCount; };

	// �f�o�b�O���̕\��
	void DrawDebugInfo(MATRIX mView);

	// �O�ʃ��C���[�ւ̕`��
	void DrawFrontLayer();

private:
	// ���f���n���h��
	int _modelHandle;

	// ��яo���Ă��邩�ǂ���
	int _use;
	// �����܂ł̎c��J�E���g
	int _remainCount;
	// ���������ǂ���
	int _isLost;

	int danger[2];
	int dangerCnt;

	// ���W�n��1�̃L�����ɑ΂��āA3�p�ӂ���
	VECTOR _pos;		// �N�_�ʒu�i�����蔻��, �ړ������Ɏg���j 
	VECTOR _screenPos;	// �X�N���[�����W�i_pos�ɃJ�����̕␳�����������́j
	VECTOR _worldPos;	// ���[���h���W�i_screenPos��3D���f���`��p�̍��W�ɕϊ��������́j

	// x���̈ړ�����
	int _direction;

	// y�������̈ړ����x
	float _gravity;

	// �ړ����x
	float _speed;					
	
	// �̂̓����蔻���`
	// ���v���C���[�╗�Ƃ̓����蔻��Ɏg��
	MYRECT _difBodyRect;				// �L���������蔻��(�z�u���W����̍��� x,y / �傫�� w,h)
	MYRECT _bodyRect;									// �����蔻���`(������W x,y / �傫�� w,h) 


};