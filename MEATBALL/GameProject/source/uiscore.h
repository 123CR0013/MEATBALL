#pragma once	// .h�̐擪�ɋL�q�B#include�ł��̃t�@�C�������x�ǂݍ��݂��Ă��A1�x�����ǂݍ��܂Ȃ�
#include "DxLib.h"
#include "gamemain.h"
#include "mymath.h"
#include "UIBase.h"

class UIScore : public UIBase {
public:
	UIScore();
	virtual ~UIScore();

	void	Process();
	void	Draw();

	void	SetScore(int score);
private:
	int		_score;			// ���ۂ̃X�R�A
	int		_drawScore;		// �`��p�X�R�A
	int		_easingStartScore;	// easing�J�n���̃X�R�A
	int		_easingCnt;			// easing�J�E���^
	int		_easingMax;			// easing�ő�l
	int		_cgNumber[10];		// 0�`9�̉摜
};

