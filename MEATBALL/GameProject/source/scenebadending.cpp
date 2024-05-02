#include "scenebadending.h"
#include "scenegamemain.h"
#include "DXLib.h"
#include "winmain.h"
#include "gamemain.h"
#include "resource.h"
#include "gamepad.h"

SceneBadEnding::SceneBadEnding() {

	BadBGM = LoadSoundMem("res/sound/BGM/bgm_endroll.mp3");
	_step = 0;
	EndRoll = LoadGraph("res/UI/endroll.png");
	title_bodyL = LoadGraph("res/UI/end_roll/title_bodyL_00.png");
	LoadDivGraph("res/UI/end_roll/title_base_07sheet.png", 7, 2, 4, 1920, 1080, title_base);
	LoadDivGraph("res/UI/end_roll/title_bg_07_sheet.png", 7, 2, 4, 1920, 1080, title_bg);
	LoadDivGraph("res/UI/end_roll/title_faceL_04_sheet.png", 4, 2, 2, 1920, 1080, title_faceL);
	LoadDivGraph("res/UI/end_roll/title_faceS_04_sheet.png", 4, 2, 2, 1920, 1080, title_faceS);
	LoadDivGraph("res/UI/end_roll/title_hair_07_sheet.png", 7, 2, 4, 1920, 1080, title_hair);
	LoadDivGraph("res/UI/end_roll/bubble_sheet.png", 55, 6, 10,1920,1080, title_bubble);
	PlaySoundMem(BadBGM, DX_PLAYTYPE_LOOP);
}
SceneBadEnding::~SceneBadEnding() {
	DeleteGraph(EndRoll);
	DeleteSoundMem(BadBGM);
	DeleteGraph(title_bodyL);
	for (int i = 0; i < 7; i++) {
		DeleteGraph(title_base[i]);
		DeleteGraph(title_bg[i]);
		DeleteGraph(title_hair[i]);
	}
	for (int i = 0; i < 4; i++) {
		DeleteGraph(title_faceL[i]);
		DeleteGraph(title_faceS[i]);
	}
	for (int i = 0; i < 55; i++) {
		DeleteGraph(title_bubble[i]);
	}
}

void SceneBadEnding::Input() {

}
void SceneBadEnding::Process() {

	badCnt++;

	switch (_step) {
	case 0:
		// フェードイン開始
		ColorFadeIn(60);
		_step++;
		break;
	case 1:
		if (IsColorFade() == 0) {
			// フェードイン終了
			_step++;
		}
		break;
	}

	if (y >= -13830) {
		y += BadRollSpeed;
		
	}
	else
	{
		SceneBase* scene = new SceneGameTitle();
		ChangeScene(scene);
		
	}

	if (gPad->_trg[PAD_BUTTON_A]) {
		Abadspeed = 15;
		
	}
	if (y >= -13500) {
		Abadspeed = 1;
	}
	if (y <= -13490) {
		time = 1;
	}
	
}
void SceneBadEnding::Draw() {
	

	DrawGraph(0, 0, title_bg[badCnt / 22 % 7], TRUE);

	DrawGraph(0, 0, title_base[badCnt / 22 % 7], TRUE);

  
	
		if (y <= -13400) {
			if (time != 1) {
				DrawGraph(0, 0, title_faceS[badCnt / 22 % 4], TRUE);

				DrawGraph(0, 0, title_faceL[badCnt  / 22 % 4], TRUE);
			}
		}
		if (time != 1) {
			if (y >= -13399) {
				DrawGraph(0, 0, title_faceS[0], TRUE);

				DrawGraph(0, 0, title_faceL[0], TRUE);

			}
		}


		if (time == 1) {
			DrawGraph(0, 0, title_faceS[3], TRUE);

			DrawGraph(0, 0, title_faceL[3], TRUE);
		}
  


	DrawGraph(0, 0, title_bodyL, TRUE);

	DrawGraph(0, 0, title_hair[badCnt / 22 % 7], TRUE);

	
    DrawGraph(0, 0, title_bubble[badCnt /10 % 55], TRUE);
	

	DrawGraph(0, y, EndRoll, TRUE);


	//DrawFormatString(0, 20, COLOR_WHITE, "Y座標%d",y);
	//DrawFormatString(0, 40, COLOR_WHITE, "eyetime%d", time);

}