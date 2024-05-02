#include "UIPosBar.h"
#include "scenegamemain.h"
#include "gamemain.h"


// UIbar�N���X
UIPosBar::UIPosBar() {
	/*_cgBarBase = LoadGraph("res/UI/ui_boss.png");*/
	_cgBarMain = LoadGraph("res/UI/ui_player.png");
	_cgBarR = LoadGraph("res/UI/bar_psycho.png");
	
	

	// �e�X�g�p�����l
	_rangeMin = 0;
	_rangeMax = 100;
	_data = 50;

	_drawData = _data;
	_easingStartData = 0;
	_easingMax = 30;			// easing�ő�l
	_easingCnt = _easingMax;	// easing�J�E���^

	SetBasePos(100, 150);
}
UIPosBar::~UIPosBar() {
	DeleteGraph(_cgBarBase);
	DeleteGraph(_cgBarR);

}

void	UIPosBar::SetRange(float rangeMin, float rangeMax) {
	_rangeMin = rangeMin;
	_rangeMax = rangeMax;
}

void	UIPosBar::Set(float data) {
	if (_easingCnt == _easingMax) {
		// easing�����łɏI����Ă����̂ŁA����data���\���J�ndata
		_easingStartData = _data;
	}
	else {
		// easing���r���̃^�C�~���O��data���X�V���ꂽ�̂ŁA���\������Ă���data���J�n��data
		_easingStartData = _drawData;
	}
	_easingCnt = 0;				// easing�J�E���^
	_data = data;
}



// ���t���[���Ă΂��O��
void	UIPosBar::Process() {
	UIBase::Process();
	if (_easingCnt < _easingMax) {
		// easing�������B�J�E���^��i�߂āA���l���߂Â���
		_easingCnt++;
	
		_drawData = EasingOutQuad(_easingCnt, _easingStartData, _data, _easingMax);		
	}
}


void	UIPosBar::DrawBar(int x, int y, int bar_w, int bar_h, int colorR) {
	// ��x�o�[�̈ʒu�����œh��Ԃ�
	DrawFillBox(x, y, x + bar_w, y + bar_h, GetColor(0, 0, 0));
	// �F���ARGB3�F�������v�Z���ďd�˂ĕ`��
	SetDrawBlendMode(DX_BLENDMODE_ADD, colorR);
	DrawExtendGraph(x, y, x + bar_w, y + bar_h, _cgBarR, TRUE);
	

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
  
// �`��
void	UIPosBar::Draw() {
	
	int base_x = _base_x, base_y = _base_y;



	// �摜���̃o�[�������
	int bar_x = 205, bar_y = 143, bar_max_w = 512, bar_h = 64;

	// �o�[���g����g���ĕ`��
	int x, y;
	x = base_x + bar_x;
	y = base_y + bar_y;
	// �o�[�̓����������čs���Ƃ��ƁA�����Ă������œ����ς���
	if (_easingStartData > _data) {
		// �o�[�������čs���B���̒����𔒂ŕ\�����ē������A���ۂ̒��������̐F�ŕ\��

		// �o�[�̒������v�Z
		// rangeMin�`rangeMax�̒��ŁA�\������ldata�́A�o�[�̉摜��bar_w�̒��łǂ̈ʒu�ɂȂ邩
		float n = Clamp(_rangeMin, _rangeMax, _drawData);	// �͈͕��𒴂��Ȃ��悤��Clamp
		int bar_w = EasingLinear(n - _rangeMin, 0, bar_max_w, _rangeMax - _rangeMin);		// linear�Ȃ瓙�Ԋu�\���ɂȂ�



		// ���̂̒����̃o�[
		n = Clamp(_rangeMin, _rangeMax, _data);	// �͈͕��𒴂��Ȃ��悤��Clamp
		bar_w = EasingLinear(n - _rangeMin, 0, bar_max_w, _rangeMax - _rangeMin);		// linear�Ȃ瓙�Ԋu�\���ɂȂ�

	}
	else {
		// �o�[�������Ă����B���ۂ̒����𔒂ŕ\�����A�����Ă����o�[�����̐F�œ�����

		// �o�[�̒������v�Z
		// rangeMin�`rangeMax�̒��ŁA�\������ldata�́A�o�[�̉摜��bar_w�̒��łǂ̈ʒu�ɂȂ邩
		float n = Clamp(_rangeMin, _rangeMax, _data);	// �͈͕��𒴂��Ȃ��悤��Clamp
		int bar_w = EasingLinear(n - _rangeMin, 0, bar_max_w, _rangeMax - _rangeMin);		// linear�Ȃ瓙�Ԋu�\���ɂȂ�
	

		// �����Ă����o�[
		n = Clamp(_rangeMin, _rangeMax, _drawData);	// �͈͕��𒴂��Ȃ��悤��Clamp
		bar_w = EasingLinear(n - _rangeMin, 0, bar_max_w, _rangeMax - _rangeMin);		// linear�Ȃ瓙�Ԋu�\���ɂȂ�

		DrawGraph(-32, -32, _cgBarMain, TRUE);

	// �J���p
	/*	DrawFormatString(0,1000, GetColor(255, 0, 0), "colorRGB�F%d", _colorR);
		DrawFormatString(0,1020, GetColor(255, 0, 0), "easing, cnt = %d/%d, data:%f -> %f", _easingCnt, _easingMax, _easingStartData, _data);
		DrawFormatString(0,1040, GetColor(255, 0, 0), "data�F%f, min/max = %f, %f", _data, _rangeMin, _rangeMax);*/


	}
}

