#include "DxLib.h"
#include "winmain.h"
#include "gamemain.h"
#include "GamePad.h"


int gKey;			// キー入力の情報
int gTrg;			// トリガ入力の情報
int gRel;			// リリース入力の情報

int dKey;
int dTrg;
int dRel;

GamePad::GamePad() {
	for (int i = 0; i < PAD_BUTTON_MAX; i++) {
		_trg[i] = 0;
		_rel[i] = 0;
	}

	// 入力状態を取得（初期化の代わり）
	GetJoypadDirectInputState(DX_INPUT_PAD1, &_input);
}

GamePad::~GamePad() {
}

void GamePad::Input() {
	// キー入力、トリガ入力、リリース入力を得る
	int gKeyold = gKey;
	gKey = GetJoypadInputState(DX_INPUT_KEY);		// キー入力を取得
	gTrg = (gKey ^ gKeyold) & gKey;
	gRel = (gKey ^ gKeyold) & ~gKey;

	int dkeyold = dKey;
	dKey = GetJoypadInputState(DX_INPUT_PAD1);		// キー入力を取得
	// キーのトリガ情報生成（押した瞬間しか反応しないキー情報）
	dTrg = (dKey ^ dkeyold) & dKey;

	// キー入力、トリガ入力、リリース入力を得る
	int keyold[PAD_BUTTON_MAX];
	
	for (int i = 0; i < PAD_BUTTON_MAX; i++) {
		keyold[i] = _input.Buttons[i];
	}

	// 入力状態を取得
	GetJoypadDirectInputState(DX_INPUT_PAD1, &_input);
	for (int i = 0; i < PAD_BUTTON_MAX; i++) {
		// トリガ入力の取得
		_trg[i] = (_input.Buttons[i] ^ keyold[i]) & _input.Buttons[i];
		// リリース入力の取得
		_rel[i] = (_input.Buttons[i] ^ keyold[i]) & ~_input.Buttons[i];
	}
}