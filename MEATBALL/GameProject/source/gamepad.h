#pragma once	// .h�̐擪�ɋL�q�B#include�ł��̃t�@�C�������x�ǂݍ��݂��Ă��A1�x�����ǂݍ��܂Ȃ�

// elecom�R���g���[���[�̃{�^��
// �ő吔
#define PAD_BUTTON_MAX		12
// �{�^���ԍ�
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
	GamePad();			// �R���X�g���N�^
	virtual ~GamePad();	// �f�X�g���N�^

	virtual void Input();		// ���͍X�V

public:
	DINPUT_JOYSTATE _input;		// �L�[����
	int _trg[PAD_BUTTON_MAX];			// �g���K���͂̏��
	int _rel[PAD_BUTTON_MAX];			// �����[�X���͂̏��

};

// �R���g���[���[
extern GamePad* gPad;



extern int gKey;			// �L�[���͂̏��
extern int gTrg;			// �g���K���͂̏��
extern int gRel;			// �����[�X���͂̏��

extern int dKey;
extern int dTrg;
extern int dRel;