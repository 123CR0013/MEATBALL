#include "UIPosBar.h"
#include "scenegamemain.h"
#include "gamemain.h"


// UIbarクラス
UIPosBar::UIPosBar() {
	/*_cgBarBase = LoadGraph("res/UI/ui_boss.png");*/
	_cgBarMain = LoadGraph("res/UI/ui_player.png");
	_cgBarR = LoadGraph("res/UI/bar_psycho.png");
	
	

	// テスト用初期値
	_rangeMin = 0;
	_rangeMax = 100;
	_data = 50;

	_drawData = _data;
	_easingStartData = 0;
	_easingMax = 30;			// easing最大値
	_easingCnt = _easingMax;	// easingカウンタ

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
		// easingがすでに終わっていたので、今のdataが表示開始data
		_easingStartData = _data;
	}
	else {
		// easingが途中のタイミングでdataが更新されたので、今表示されているdataが開始時data
		_easingStartData = _drawData;
	}
	_easingCnt = 0;				// easingカウンタ
	_data = data;
}



// 毎フレーム呼ばれる前提
void	UIPosBar::Process() {
	UIBase::Process();
	if (_easingCnt < _easingMax) {
		// easing処理中。カウンタを進めて、数値を近づける
		_easingCnt++;
	
		_drawData = EasingOutQuad(_easingCnt, _easingStartData, _data, _easingMax);		
	}
}


void	UIPosBar::DrawBar(int x, int y, int bar_w, int bar_h, int colorR) {
	// 一度バーの位置を黒で塗りつぶす
	DrawFillBox(x, y, x + bar_w, y + bar_h, GetColor(0, 0, 0));
	// 色を、RGB3色を割合計算して重ねて描画
	SetDrawBlendMode(DX_BLENDMODE_ADD, colorR);
	DrawExtendGraph(x, y, x + bar_w, y + bar_h, _cgBarR, TRUE);
	

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
  
// 描画
void	UIPosBar::Draw() {
	
	int base_x = _base_x, base_y = _base_y;



	// 画像内のバー部分情報
	int bar_x = 205, bar_y = 143, bar_max_w = 512, bar_h = 64;

	// バーを拡大を使って描画
	int x, y;
	x = base_x + bar_x;
	y = base_y + bar_y;
	// バーの動きが減って行くときと、増えていく時で動作を変える
	if (_easingStartData > _data) {
		// バーが減って行く。元の長さを白で表示して動かし、実際の長さを実体色で表示

		// バーの長さを計算
		// rangeMin～rangeMaxの中で、表示する値dataは、バーの画像幅bar_wの中でどの位置になるか
		float n = Clamp(_rangeMin, _rangeMax, _drawData);	// 範囲幅を超えないようにClamp
		int bar_w = EasingLinear(n - _rangeMin, 0, bar_max_w, _rangeMax - _rangeMin);		// linearなら等間隔表現になる



		// 実体の長さのバー
		n = Clamp(_rangeMin, _rangeMax, _data);	// 範囲幅を超えないようにClamp
		bar_w = EasingLinear(n - _rangeMin, 0, bar_max_w, _rangeMax - _rangeMin);		// linearなら等間隔表現になる

	}
	else {
		// バーが増えていく。実際の長さを白で表示し、増えていくバーを実体色で動かす

		// バーの長さを計算
		// rangeMin～rangeMaxの中で、表示する値dataは、バーの画像幅bar_wの中でどの位置になるか
		float n = Clamp(_rangeMin, _rangeMax, _data);	// 範囲幅を超えないようにClamp
		int bar_w = EasingLinear(n - _rangeMin, 0, bar_max_w, _rangeMax - _rangeMin);		// linearなら等間隔表現になる
	

		// 増えていくバー
		n = Clamp(_rangeMin, _rangeMax, _drawData);	// 範囲幅を超えないようにClamp
		bar_w = EasingLinear(n - _rangeMin, 0, bar_max_w, _rangeMax - _rangeMin);		// linearなら等間隔表現になる

		DrawGraph(-32, -32, _cgBarMain, TRUE);

	// 開発用
	/*	DrawFormatString(0,1000, GetColor(255, 0, 0), "colorRGB：%d", _colorR);
		DrawFormatString(0,1020, GetColor(255, 0, 0), "easing, cnt = %d/%d, data:%f -> %f", _easingCnt, _easingMax, _easingStartData, _data);
		DrawFormatString(0,1040, GetColor(255, 0, 0), "data：%f, min/max = %f, %f", _data, _rangeMin, _rangeMax);*/


	}
}

