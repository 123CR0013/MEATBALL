#pragma once	// .h�̐擪�ɋL�q�B#include�ł��̃t�@�C�������x�ǂݍ��݂��Ă��A1�x�����ǂݍ��܂Ȃ�
#include "winmain.h"
#include "gamepad.h"
#include "camera.h"
#include "mapdata.h"
#include "mymath.h"
#include "Pause.h"
#include "C_Scenario.h"
#include "B_Scenario.h"
#include "explanation.h"

#include "scenebase.h"



#define PLAYER_MAX 1
#define ENEMY_MAX 150

#define	MAPSIZE_LAYER	1


// �Q�[�����

void GameMain();	// gamemain.cpp �Ɏ��̂��L�q
void ChangeScene(SceneBase* scene);	// Scene��ύX����

void SetScenePause();
void gSetC_Scenario();
void gSetB_Scenario();
void Set_Explanation(int changeInst);

extern int Exit  ;

extern ScenePause* gScenePause;

extern C_Scenario* gC_Scenario;
extern B_Scenario* gB_Scenario;

extern explanation* gExplanation;




extern int fontHandle;

void ColorMask(int colorR, int colorG, int colorB, int colorA);		// �J���[�}�X�N�̎w��

// �w��̐F�Ƀt�F�[�h�A�E�g�i�J���[�}�X�N�g�p, alpha=255�ɂ���j
void ColorFadeOut(int colorR, int colorG, int colorB, int frames);

// ���݂̃J���[�}�X�N����t�F�[�h�C���i�J���[�}�X�N�g�p, alpha=0�ɂ���j
void ColorFadeIn(int frames);

// �t�F�[�h�C���A�E�g�͏I���������H
// �߂�l: 1 = �t�F�[�h��, 0 = �t�F�[�h�I��(�������͊J�n�O)
int IsColorFade();

// ���݂̉�ʂ���N���X�t�F�[�h�A�E�g
void CrossFadeOut(int frames);

// �N���X�t�F�[�h�A�E�g�͏I���������H
// �߂�l: 1 = �t�F�[�h��, 0 = �t�F�[�h�I��(�������͊J�n�O)
int IsCrossFade();




