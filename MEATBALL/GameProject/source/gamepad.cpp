#include "DxLib.h"
#include "winmain.h"
#include "gamemain.h"
#include "GamePad.h"


int gKey;			// �L�[���͂̏��
int gTrg;			// �g���K���͂̏��
int gRel;			// �����[�X���͂̏��

int dKey;
int dTrg;
int dRel;

GamePad::GamePad() {
	for (int i = 0; i < PAD_BUTTON_MAX; i++) {
		_trg[i] = 0;
		_rel[i] = 0;
	}

	// ���͏�Ԃ��擾�i�������̑���j
	GetJoypadDirectInputState(DX_INPUT_PAD1, &_input);
}

GamePad::~GamePad() {
}

void GamePad::Input() {
	// �L�[���́A�g���K���́A�����[�X���͂𓾂�
	int gKeyold = gKey;
	gKey = GetJoypadInputState(DX_INPUT_KEY);		// �L�[���͂��擾
	gTrg = (gKey ^ gKeyold) & gKey;
	gRel = (gKey ^ gKeyold) & ~gKey;

	int dkeyold = dKey;
	dKey = GetJoypadInputState(DX_INPUT_PAD1);		// �L�[���͂��擾
	// �L�[�̃g���K��񐶐��i�������u�Ԃ����������Ȃ��L�[���j
	dTrg = (dKey ^ dkeyold) & dKey;

	// �L�[���́A�g���K���́A�����[�X���͂𓾂�
	int keyold[PAD_BUTTON_MAX];
	
	for (int i = 0; i < PAD_BUTTON_MAX; i++) {
		keyold[i] = _input.Buttons[i];
	}

	// ���͏�Ԃ��擾
	GetJoypadDirectInputState(DX_INPUT_PAD1, &_input);
	for (int i = 0; i < PAD_BUTTON_MAX; i++) {
		// �g���K���͂̎擾
		_trg[i] = (_input.Buttons[i] ^ keyold[i]) & _input.Buttons[i];
		// �����[�X���͂̎擾
		_rel[i] = (_input.Buttons[i] ^ keyold[i]) & ~_input.Buttons[i];
	}
}