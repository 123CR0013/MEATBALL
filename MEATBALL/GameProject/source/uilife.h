#pragma once	// .h�̐擪�ɋL�q�B#include�ł��̃t�@�C�������x�ǂݍ��݂��Ă��A1�x�����ǂݍ��܂Ȃ�
#include "DxLib.h"
#include "gamemain.h"
#include "mymath.h"
#include "UIBase.h"

class UILife : public UIBase {
public:
	UILife();
	virtual ~UILife();

	void	Process();
	void	Draw();
	void	Set(int life);

private:
	int		_cgHeart;
	int		_life;
};

