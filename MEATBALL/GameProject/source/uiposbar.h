#pragma once	// .hの先頭に記述。#includeでこのファイルを何度読み込みしても、1度しか読み込まない
#include "DxLib.h"
#include "gamemain.h"
#include "mymath.h"
#include "UIBase.h"

class UIPosBar : public UIBase {
public:
	UIPosBar();
	virtual ~UIPosBar();

	void	Process();
	void	Draw();

	void	SetRange(float rangeMin, float rangeMax);
	void	Set(float data);


	

    void	DrawBar(int x, int y, int bar_w, int bar_h, int colorR);
	


    int		_colorR;	// バーの色

private:
	

	int		_cgBarBase;
	int     _cgBarMain;
	int		_cgBarR;


	float	_rangeMin, _rangeMax;	// バーの値範囲。最小min～最大max
	float	_data;					// バーで表現する値
	int		_drawData;			// 描画用データ
	int		_easingStartData;	// easing開始時のデータ
	int		_easingCnt;			// easingカウンタ
	int		_easingMax;			// easing最大値
	
};


