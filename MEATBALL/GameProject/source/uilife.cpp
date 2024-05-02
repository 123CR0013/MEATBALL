#include "UILife.h"


// UILife�N���X
UILife::UILife() {
	_cgHeart = LoadGraph("res/uiHeart.png");
	Set(10);
	SetBasePos(50, 300);
}
UILife::~UILife() {
	DeleteGraph(_cgHeart);
}

void	UILife::Set(int life) {
	_life = life;
}

// ���t���[���Ă΂��O��
void	UILife::Process() {
	UIBase::Process();
}

// �`��
void	UILife::Draw() {
	UIBase::Draw();
	int base_x = _base_x, base_y = _base_y;

	// �ЂƂ̃n�[�g�̏��
	int heart_w = 84, heart_h = 70;
	int heart_one = 4;		// �ЂƂ̃n�[�g��life�������\�����邩

	// life���n�[�g�ŕ`��
	int x, y, w;
	x = 0;
	y = 0;
	w = 72;			// �n�[�g�ƃn�[�g�̃X�L�}
	int life = _life;
	while (life > 0) {
		// �n�[�g�𕪊��`�悩�A�t���`�悩
		if (life < heart_one) {
			// �����`��
			DrawRectGraph(base_x + x, base_y + y,
				0, 0, heart_w * life / heart_one, heart_h,
				_cgHeart, TRUE);
		}
		else {
			// �t���`��
			DrawGraph(base_x + x, base_y + y, _cgHeart, TRUE);
		}
		life -= heart_one;	// �n�[�g�ЂƂ����C�t�����炷
		x += w;	// ���̃n�[�g�ʒu��
	}

	// �J���p
	DrawFormatString(base_x, base_y + 80 + 0 * 16, GetColor(255, 0, 0), "�����C�t�F%d", _life);



}

