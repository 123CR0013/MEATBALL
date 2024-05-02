#pragma once	// .hの先頭に記述。#includeでこのファイルを何度読み込みしても、1度しか読み込まない
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
	int		_score;			// 実際のスコア
	int		_drawScore;		// 描画用スコア
	int		_easingStartScore;	// easing開始時のスコア
	int		_easingCnt;			// easingカウンタ
	int		_easingMax;			// easing最大値
	int		_cgNumber[10];		// 0～9の画像
};

