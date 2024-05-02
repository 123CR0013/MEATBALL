#pragma once	// .hの先頭に記述。#includeでこのファイルを何度読み込みしても、1度しか読み込まない
#include "SceneBase.h"

class SceneGameTitle : public SceneBase {
public:
	SceneGameTitle();			// コンストラクタ
	virtual ~SceneGameTitle();	// デストラクタ

	void	Input() override;	// 入力
	void	Process() override;	// 計算
	void	Draw() override;	// 描画

#define TITLE_MAX       (50)





protected:
	int title_scene[TITLE_MAX];
	int title_logo_sheet[12];
	int yes_sheet[12];
	int no_sheet[12];
	int error_R[12];
	int error_L[12];
	int movie;


	

	int		_cgBg;		// 背景画像
	int    time = 0;
	

	int		_step;		// フェードインアウト用ステップ
	int     _stepcnt;

	
	int    use = 0;
	int    enduse=0;

	int fadetalk=0;

	int startA = 4;
	int flamecnt = 0;

	int trueend=0;
	int meatvoicecnt = 0;
	

	//easing
	int easingTitle;
	float start;
	float end;
	float cnt;
	float flame;
	int max;
	int moreeasing;
	float morecnt;
	float morestart;
	int returning;
	
	int returntitle=0;
	int returnframe = 0;
};

