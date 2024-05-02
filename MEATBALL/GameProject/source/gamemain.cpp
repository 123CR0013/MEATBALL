#include "DxLib.h"
#include "winmain.h"
#include "gamemain.h"
#include "mymath.h"


#include "scenegamemain.h"
#include "scenemovie.h"
#include "scenegameover.h"
#include "scenegametitle.h"
#include "SceneBadEnding.h"
#include "c_scenario.h"
#include "B_Scenario.h"
#include "talk_end.h"
#include "explanation.h"

#include "resource.h"
#include "scenebase.h"
#include "fps.h"

//
// �萔�E�\���̒�`
//

GamePad* gPad;

//�V�[���|�C���^
SceneBase* gScene;					// �|�C���^�Ȃ̂�new����܂Ŏ��͖̂���

ScenePause* gScenePause;            //�|�[�Y��ʗp�؂�ւ��|�C���^

 C_Scenario* gC_Scenario;
 B_Scenario* gB_Scenario;


 explanation* gExplanation;

//
// �O���[�o���ϐ��錾
//

// �Q�[�����

int Exit ;








// Scene�̐؂�ւ��p
SceneBase* gNewScene;				// NULL�ȊO���Z�b�g����Ă�����A�V�[����؂�ւ���

// Scene��ύX����
void ChangeScene(SceneBase* scene) {
	gNewScene = scene;		// ���̃V�[�����Z�b�g
}

void SetScenePause() {
	gScenePause = new ScenePause();
}

void gSetC_Scenario() {
	gC_Scenario = new C_Scenario();
}

void gSetB_Scenario() {
	gB_Scenario = new B_Scenario();
}

void Set_Explanation(int changeInst) {
	gExplanation = new explanation(changeInst);
}

// �J���[�}�X�N�p
int gColorMaskR = 0, gColorMaskG = 0, gColorMaskB = 0, gColorMaskA = 0;

// �w��̐F���J���[�}�X�N
void ColorMask(int colorR, int colorG, int colorB, int colorA) {
	gColorMaskR = colorR;
	gColorMaskG = colorG;
	gColorMaskB = colorB;
	gColorMaskA = colorA;
}

// �t�F�[�h�C���A�E�g�p
int gColorFadeStR, gColorFadeStG, gColorFadeStB, gColorFadeStA;
int gColorFadeEdR, gColorFadeEdG, gColorFadeEdB, gColorFadeEdA;
int gFadeFrames = 0, gFadeCnt = 0;

// �w��̐F�Ƀt�F�[�h�A�E�g�i�J���[�}�X�N�g�p, alpha=255�ɂ���j
void ColorFadeOut(int colorR, int colorG, int colorB, int frames) {
	// �t�F�[�h�J�n�l
	gColorFadeStR = gColorMaskR;
	gColorFadeStG = gColorMaskG;
	gColorFadeStB = gColorMaskB;
	gColorFadeStA = gColorMaskA;
	// �t�F�[�h���B�l
	gColorFadeEdR = colorR;
	gColorFadeEdG = colorG;
	gColorFadeEdB = colorB;
	gColorFadeEdA = 255;
	// �t�F�[�h�J�E���^
	gFadeFrames = frames;
	gFadeCnt = 0;
}

// ���݂̃J���[�}�X�N����t�F�[�h�C���i�J���[�}�X�N�g�p, alpha=0�ɂ���j
void ColorFadeIn(int frames) {
	// �t�F�[�h�J�n�l
	gColorFadeStR = gColorMaskR;
	gColorFadeStG = gColorMaskG;
	gColorFadeStB = gColorMaskB;
	gColorFadeStA = gColorMaskA;
	// �t�F�[�h���B�l
	gColorFadeEdR = gColorMaskR;
	gColorFadeEdG = gColorMaskG;
	gColorFadeEdB = gColorMaskB;
	gColorFadeEdA = 0;
	// �t�F�[�h�J�E���^
	gFadeFrames = frames;
	gFadeCnt = 0;
}

// �t�F�[�h�C���A�E�g�͏I���������H
// �߂�l: 1 = �t�F�[�h��, 0 = �t�F�[�h�I��(�������͊J�n�O)
int IsColorFade() {
	if(gFadeCnt < gFadeFrames) {
		return 1;
	}
	return 0;
}

// �N���X�t�F�[�h�p
int gCgCrossFade[448] = { -1 };	// ��ʂ���荞�񂾉摜
int gCrossFadeA = 0;
int gCrossFadeFrames = 0, gCrossFadeCnt = 0;
// ���˂��˃p�����[�^
float	gCrossFade_speed;		// �h�炬�̑��x
float	gCrossFade_xspan;		// �������̂�炬�x����
float	gCrossFade_yspan;		// �c�����̂�炬�x����


// �N���X�t�F�[�h�p�摜�̍폜
void DeleteCrossFadeCg() {
	for(int y = 0; y < 448; y++) {
		if(gCgCrossFade[y] != -1) {
			// �摜���폜
			DeleteGraph(gCgCrossFade[y]);
			gCgCrossFade[y] = -1;
		}
	}
}

// ���݂̉�ʂ���N���X�t�F�[�h�A�E�g
void CrossFadeOut(int frames) {
	DeleteCrossFadeCg();

	// ��ʃT�C�Y�Ɠ����X�N���[�����A�c1pixel���ō쐬
	for(int y = 0; y < 448; y++) {
		gCgCrossFade[y] = MakeGraph(1280, 1);

		// ��ʂɕ\������Ă���摜���c1pixel����荞��
		GetDrawScreenGraph(320, 64, 1600, 513, gCgCrossFade[y]);
	}

	// �t�F�[�h�J�E���^
	gCrossFadeFrames = frames;
	gCrossFadeCnt = 0;

	// ���˂��˃p�����[�^
	gCrossFade_speed = 10.0;
	gCrossFade_yspan = 5.5;
	gCrossFade_xspan = 5.0;	// x��0����X�^�[�g
}

// �N���X�t�F�[�h�A�E�g�͏I���������H
// �߂�l: 1 = �t�F�[�h��, 0 = �t�F�[�h�I��(�������͊J�n�O)
int IsCrossFade() {
	if(gCrossFadeCnt < gCrossFadeFrames) {
		return 1;
	}
	return 0;
}

// �A�v���̏�����
// �N������1�񂾂����s�����
void AppInit() {
	ResourceInit();

	gScenePause = nullptr;
	
	gNewScene = NULL;		// ���̃V�[��������������

	gScene = new SceneGameTitle();
	//gScene = new SceneGameMain();
	//gScene = new SceneGameOver();
	//gScene = new talk_end();
	//gScene = new SceneMovie(MOVIE_PROLOGUE);
	//gScene = new SceneMovie(MOVIE_CLEAR);

	gPad = nullptr;

	gPad = new GamePad;
	Exit = 0;

}


// �A�v���̉��
// �I������1�񂾂����s�����
void AppRelease() {
	delete gPad;
	delete gScene;				// Scene���폜����

	ResourceRelease();
}





//
// �t���[�������B1�t���[����1�񂸂��s�����
//

// �t���[�������F����
void FrameInput() {
	gPad->Input();

	gScene->Input();
	if(gScenePause != NULL) {
		gScenePause->Input();
	}
	if (gC_Scenario != NULL) {
		gC_Scenario->Input();
	}
	if (gB_Scenario != NULL) {
		gB_Scenario->Input();
	}
	if (gExplanation != NULL) {
		gExplanation->Input();
	}



	
	
}




// �t���[�������F�v�Z
void FrameProcess() {
	// Scene�̌v�Z�Ăяo��
	
	if (gScenePause != NULL) {
		gScenePause->Process();

		int pauseFlag = 0;
		//if(gScenePause->_isExplanation == 1) {
		//	SceneBase* scene = new SceneExplanation();
		//	ChangeScene(scene);//�Ȃ񂩂̉摜
		//	pauseFlag = 1;
		//}
		//if(gScenePause->_isVoiceChange == 1) {
		//	SceneBase* scene = new SceneVoiceChange();
		//	ChangeScene(scene);//Pause�̒��ł��i���u���ŃN���X�j
		//	pauseFlag = 1;
		//}
		if (gScenePause->_isGoTitle == 1) {
			Exit = 1;
		}
		if (gScenePause->_isPause == 0 || pauseFlag == 1) {
			delete gScenePause;
			gScenePause = nullptr;
		}
	}
	else if (gB_Scenario != NULL) {
		gB_Scenario->Process();

		if (gB_Scenario->_BtalkFlg == 1) {
			delete gB_Scenario;
			gB_Scenario = nullptr;
		}
	}
	else if (gC_Scenario != NULL) {
		gC_Scenario->Process();
		if (gC_Scenario->_talkFlg == 1) {
			delete gC_Scenario;
			gC_Scenario = nullptr;
		}
	}
	else if (gExplanation != NULL) {
		gExplanation->Process();
		if (gExplanation->explanationEnd == 1) {
			delete gExplanation;
			gExplanation = nullptr;

		}
	}
	else {
		gScene->Process();
	}

	


	// �J���[�}�X�N�̃t�F�[�h�C���A�E�g
	if(IsColorFade() != 0) {
		gFadeCnt++;
		gColorMaskR = EasingLinear(gFadeCnt, gColorFadeStR, gColorFadeEdR, gFadeFrames);
		gColorMaskG = EasingLinear(gFadeCnt, gColorFadeStG, gColorFadeEdG, gFadeFrames);
		gColorMaskB = EasingLinear(gFadeCnt, gColorFadeStB, gColorFadeEdB, gFadeFrames);
		gColorMaskA = EasingLinear(gFadeCnt, gColorFadeStA, gColorFadeEdA, gFadeFrames);
	}
	// �N���X�t�F�[�h�摜�̃t�F�[�h�A�E�g
	if(IsCrossFade() != 0) {
		gCrossFadeCnt++;
		gCrossFadeA = EasingLinear(gCrossFadeCnt, 255, 0, gCrossFadeFrames);

		gCrossFade_xspan += 2.5;	// ���̂��˂��ˉ����x
	}
	
}

// �t���[�������F�`��
void FrameDraw() {

	ClearDrawScreen();						// ��ʂ�����������	

	// Scene�̕`��Ăяo��
	gScene->Draw();
	if(gScenePause != NULL) {
		gScenePause->Draw();
	}
	if (gC_Scenario != NULL) {
		gC_Scenario->Draw();
	}
	if (gB_Scenario != NULL) {
		gB_Scenario->Draw();
	}
	if (gExplanation != NULL) {
		gExplanation->Draw();
	}

	// �N���X�t�F�[�h�摜�̏�悹
	if(gCrossFadeA > 0) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, gCrossFadeA);	// ���������[�h

		// sin�ŉ������˂��˂�����
		int x, y;
		for(y = 0; y < 448; y++) {
			float rad = (((float)(gCrossFadeFrames - gCrossFadeCnt) * gCrossFade_speed) + (float)y * gCrossFade_yspan) * PI / 180.0;
			x = (int)(sin(rad) * gCrossFade_xspan);
			DrawGraph(320, 64, gCgCrossFade[y], FALSE);
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	// �J���[�}�X�N
	if(gColorMaskA > 0) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, gColorMaskA);	// ���������[�h
		DrawBox(0, 0, SCREEN_W, SCREEN_H, GetColor(gColorMaskR, gColorMaskG, gColorMaskB), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, gColorMaskA);
	}

	fps_wait();

#ifdef _DEBUG
	//int loadNum = GetASyncLoadNum();
	//DrawFormatString(1800, 0, GetColor(255, 255, 255), "%3d", loadNum);
	//draw_fps(0, 0);
#endif

	ScreenFlip();							// ����ʂ̓��e��\��ʂɔ��f������
}


void GameMain()
{
	AppInit();	// ������

	// 1�t���[�����[�v��g�� ----------------------------------------------------------
	while (ProcessMessage() == 0)		// �v���O�������I������܂Ń��[�v
	{
		// �V�[����؂�ւ��邩�H
		if (gNewScene != NULL) {
			delete gScene;		// ���̃V�[�����폜����
			gScene = gNewScene;	// �󂯎�����V�[�����|�C���^�Z�b�g
			gNewScene = NULL;	// ���̃V�[��������������
		}

		FrameInput();	// ����
		FrameProcess();	// �v�Z
		FrameDraw();	// �`��
		if(Exit != 0) {
			break;
		}
		if (CheckHitKey(KEY_INPUT_ESCAPE) != 0)	// ESCAPE�L�[��������Ă��邩�𒲂ׂ�
			break;			// ������Ă����烁�C�����[�v�𔲂���
	}

	AppRelease();	// ���
}











