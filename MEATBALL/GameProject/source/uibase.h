#pragma once	// .h�̐擪�ɋL�q�B#include�ł��̃t�@�C�������x�ǂݍ��݂��Ă��A1�x�����ǂݍ��܂Ȃ�
#include "DxLib.h"
#include "gamemain.h"
#include "mymath.h"

class UIBase {
public:
	UIBase();
	virtual ~UIBase();

	virtual void	Process();
	virtual void	Draw();

	virtual void	SetBasePos(int base_x, int base_y);

protected:
	int		_base_x, _base_y;	// UI�x�[�X�ʒu�i����j
};

