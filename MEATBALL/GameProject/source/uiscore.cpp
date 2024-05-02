#include "UIScore.h"


// UIスコアクラス
UIScore::UIScore() {
	_score = 0;
	_drawScore = _score;		// 描画用スコア
	_easingStartScore = 0;		// easing開始時のスコア
	_easingMax = 60;			// easing最大値
	_easingCnt = _easingMax;	// easingカウンタ

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
		// easingがすでに終わっていたので、今のスコアが表示開始スコア
		_easingStartScore = _score;	// easing開始時のスコア
	}
	else {
		// easingが途中のタイミングでスコアが更新されたので、今表示されているスコアが開始時スコア
		_easingStartScore = _drawScore;	// easing開始時のスコア
	}
	_easingCnt = 0;				// easingカウンタ
	_score = score;				// 実際到達すべきスコア
}

// 毎フレーム呼ばれる前提
void	UIScore::Process() {
	UIBase::Process();
	if (_easingCnt < _easingMax) {
		// easing処理中。カウンタを進めて、数値を近づける
		_easingCnt++;
		_drawScore = EasingLinear(_easingCnt, _easingStartScore, _score, _easingMax);		// easing関数は好きなモノを使える
		//_drawScore = EasingOutQuint(_easingCnt, _easingStartScore, _score, _easingMax);
	}
}

// 描画
void	UIScore::Draw() {
	UIBase::Draw();
	// _drawScoreを1桁ずつ表示する。桁数を合わせるため、無い桁は0表示。
	int x = _base_x, y = _base_y;	// 表示位置
	int keta = 8;	// 桁数
	int w = 48;		// 1桁の横サイズ
	int n = _drawScore;	// 計算時に値を壊してしまうので、_drawScoreをローカル変数に入れる

	// 1桁目（一番右）からひと桁ずつ描画する
	x += keta * w;	// 一番右の位置にx座標を移動
	for (int i = 0; i < keta; i++) {
		x -= w;		// 1桁分x座標を減らす
		// nの「一番小さい1桁分」を画像として描画
		DrawGraph(x, y, _cgNumber[n % 10], TRUE);
		// nをひと桁分減らす
		n = n / 10;		// intなので小数点以下は切り捨てられる
	}

	// 開発用
	DrawFormatString(x, y + 80 + 0 * 16, GetColor(255, 0, 0), "実スコア：%d", _score);
	DrawFormatString(x, y + 80 + 1 * 16, GetColor(255, 0, 0), "easing, cnt = %d/%d, score:%d -> %d", _easingCnt, _easingMax, _easingStartScore, _score);
}

