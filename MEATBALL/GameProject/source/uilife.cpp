#include "UILife.h"


// UILifeクラス
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

// 毎フレーム呼ばれる前提
void	UILife::Process() {
	UIBase::Process();
}

// 描画
void	UILife::Draw() {
	UIBase::Draw();
	int base_x = _base_x, base_y = _base_y;

	// ひとつのハートの情報
	int heart_w = 84, heart_h = 70;
	int heart_one = 4;		// ひとつのハートでlifeをいくつ表現するか

	// lifeをハートで描画
	int x, y, w;
	x = 0;
	y = 0;
	w = 72;			// ハートとハートのスキマ
	int life = _life;
	while (life > 0) {
		// ハートを分割描画か、フル描画か
		if (life < heart_one) {
			// 分割描画
			DrawRectGraph(base_x + x, base_y + y,
				0, 0, heart_w * life / heart_one, heart_h,
				_cgHeart, TRUE);
		}
		else {
			// フル描画
			DrawGraph(base_x + x, base_y + y, _cgHeart, TRUE);
		}
		life -= heart_one;	// ハートひとつ分ライフを減らす
		x += w;	// 次のハート位置へ
	}

	// 開発用
	DrawFormatString(base_x, base_y + 80 + 0 * 16, GetColor(255, 0, 0), "実ライフ：%d", _life);



}

