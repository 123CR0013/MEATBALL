#pragma once	// .hの先頭に記述。#includeでこのファイルを何度読み込みしても、1度しか読み込まない

// elecomコントローラーのボタン
// 最大数
#define PAD_BUTTON_MAX		12
// ボタン番号
#define PAD_BUTTON_X			 0
#define PAD_BUTTON_Y			 1
#define PAD_BUTTON_A			 2
#define PAD_BUTTON_B			 3
#define PAD_BUTTON_LB			 4
#define PAD_BUTTON_RB			 5
#define PAD_BUTTON_LT			 6
#define PAD_BUTTON_RT			 7
#define PAD_BUTTON_LSTICK  8
#define PAD_BUTTON_RSTICK  9
#define PAD_BUTTON_BACK   10
#define PAD_BUTTON_START  11

class GamePad {
public:
	GamePad();			// コンストラクタ
	virtual ~GamePad();	// デストラクタ

	virtual void Input();		// 入力更新

public:
	DINPUT_JOYSTATE _input;		// キー入力
	int _trg[PAD_BUTTON_MAX];			// トリガ入力の情報
	int _rel[PAD_BUTTON_MAX];			// リリース入力の情報

};

// コントローラー
extern GamePad* gPad;



extern int gKey;			// キー入力の情報
extern int gTrg;			// トリガ入力の情報
extern int gRel;			// リリース入力の情報

extern int dKey;
extern int dTrg;
extern int dRel;