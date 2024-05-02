#include "UIScore.h"


// UI�X�R�A�N���X
UIScore::UIScore() {
	_score = 0;
	_drawScore = _score;		// �`��p�X�R�A
	_easingStartScore = 0;		// easing�J�n���̃X�R�A
	_easingMax = 60;			// easing�ő�l
	_easingCnt = _easingMax;	// easing�J�E���^

	LoadDivGraph("res/T_digitWhite_sheet.png", 10, 10, 1, 70, 80, _cgNumber);

	SetBasePos(8, 8);
}
UIScore::~UIScore() {
	for (int i = 0; i < 10; i++) {
		DeleteGraph(_cgNumber[i]);
	}
}

void	UIScore::SetScore(int score) {
	if (_easingCnt == _easingMax) {
		// easing�����łɏI����Ă����̂ŁA���̃X�R�A���\���J�n�X�R�A
		_easingStartScore = _score;	// easing�J�n���̃X�R�A
	}
	else {
		// easing���r���̃^�C�~���O�ŃX�R�A���X�V���ꂽ�̂ŁA���\������Ă���X�R�A���J�n���X�R�A
		_easingStartScore = _drawScore;	// easing�J�n���̃X�R�A
	}
	_easingCnt = 0;				// easing�J�E���^
	_score = score;				// ���ۓ��B���ׂ��X�R�A
}

// ���t���[���Ă΂��O��
void	UIScore::Process() {
	UIBase::Process();
	if (_easingCnt < _easingMax) {
		// easing�������B�J�E���^��i�߂āA���l���߂Â���
		_easingCnt++;
		_drawScore = EasingLinear(_easingCnt, _easingStartScore, _score, _easingMax);		// easing�֐��͍D���ȃ��m���g����
		//_drawScore = EasingOutQuint(_easingCnt, _easingStartScore, _score, _easingMax);
	}
}

// �`��
void	UIScore::Draw() {
	UIBase::Draw();
	// _drawScore��1�����\������B���������킹�邽�߁A��������0�\���B
	int x = _base_x, y = _base_y;	// �\���ʒu
	int keta = 8;	// ����
	int w = 48;		// 1���̉��T�C�Y
	int n = _drawScore;	// �v�Z���ɒl���󂵂Ă��܂��̂ŁA_drawScore�����[�J���ϐ��ɓ����

	// 1���ځi��ԉE�j����Ђƌ����`�悷��
	x += keta * w;	// ��ԉE�̈ʒu��x���W���ړ�
	for (int i = 0; i < keta; i++) {
		x -= w;		// 1����x���W�����炷
		// n�́u��ԏ�����1�����v���摜�Ƃ��ĕ`��
		DrawGraph(x, y, _cgNumber[n % 10], TRUE);
		// n���Ђƌ������炷
		n = n / 10;		// int�Ȃ̂ŏ����_�ȉ��͐؂�̂Ă���
	}

	// �J���p
	DrawFormatString(x, y + 80 + 0 * 16, GetColor(255, 0, 0), "���X�R�A�F%d", _score);
	DrawFormatString(x, y + 80 + 1 * 16, GetColor(255, 0, 0), "easing, cnt = %d/%d, score:%d -> %d", _easingCnt, _easingMax, _easingStartScore, _score);
}

