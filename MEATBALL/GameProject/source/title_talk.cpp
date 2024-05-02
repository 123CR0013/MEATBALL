#include "title_talk.h"
#include "scenegamemain.h"
#include "scenegametitle.h"
#include "DXLib.h"
#include "winmain.h"
#include "gamemain.h"
#include "resource.h"
#include "gamepad.h"

Title_Talk::Title_Talk() {

	_step = 0;
	ColorMask(0, 0, 0, 255);
	//start
	talkstart[0] = LoadGraph("res/UI/scenario_start/a_meatball_talk01.png");
	talkstart[1] = LoadGraph("res/UI/scenario_start/a_meatball_talk02.png");
	talkstart[2] = LoadGraph("res/UI/scenario_start/a_meatball_talk03.png");
	talkstart[3] = LoadGraph("res/UI/scenario_start/a_meatball_talk04.png");
	talkstart[4] = LoadGraph("res/UI/scenario_start/a_meatball_talk05.png");
	talkstart[5] = LoadGraph("res/UI/scenario_start/a_meatball_talk06.png");
	talkstart[6] = LoadGraph("res/UI/scenario_start/a_meatball_talk07.png");
	talkstart[7] = LoadGraph("res/UI/scenario_start/a_meatball_talk08.png");
	talkstart[8] = LoadGraph("res/UI/scenario_start/a_meatball_talk09.png");
	talkstart[9] = LoadGraph("res/UI/scenario_start/a_meatball_talk10.png");
	talkstart[10] = LoadGraph("res/UI/scenario_start/a_meatball_talk11.png");
	talkstart[11] = LoadGraph("res/UI/scenario_start/a_meatball_talk12.png");
	talkstart[12] = LoadGraph("res/UI/scenario_start/a_meatball_talk13.png");
	talkstart[13] = LoadGraph("res/UI/scenario_start/a_meatball_talk14.png");
	talkstart[14] = LoadGraph("res/UI/scenario_start/a_meatball_talk15.png");
	talkstart[15] = LoadGraph("res/UI/scenario_start/a_meatball_talk16.png");
	talkstart[16] = LoadGraph("res/UI/scenario_start/a_meatball_talk17.png");
	talkstart[17] = LoadGraph("res/UI/scenario_start/a_meatball_talk18.png");
	talkstart[18] = LoadGraph("res/UI/scenario_start/a_meatball_talk19.png");
	talkstart[19] = LoadGraph("res/UI/scenario_start/a_meatball_talk20.png");
	talkstart[20] = LoadGraph("res/UI/scenario_start/a_meatball_talk21.png");
	talkstart[21] = LoadGraph("res/UI/scenario_start/a_meatball_talk22.png");
	


	talkBG = LoadGraph("res/UI/talk_bg.png");

	PlaySoundMem(bgm[3], DX_PLAYTYPE_LOOP);
}

Title_Talk::~Title_Talk() {

	for (int i = 0; i <= 21; i++) {
		DeleteGraph(talkstart[i]);
	}

	DeleteGraph(talkBG);

}
void Title_Talk::Input() {
	titleCnt++;
}
void Title_Talk::Process() {

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
			_stepcnt = 0;
		}
		break;

	case 2:
		// 通常処理
		
		if (talkStartCnt >= 21) {
			// クロースフェードアウト開始
			CrossFadeOut(60);
			// クロスフェードは開始したら終了を待たずシーン切り替えしちゃって良い
			// フェードアウト終了
			_stepcnt++;
			_step++;
			
			
		}
		break;
	case 3:
		if (IsColorFade() == 0) {
			// フェードアウト終了
			StopSoundMem(bgm[3]);

			SceneBase* scene = new SceneGameMain();
			ChangeScene(scene);
		}
		break;
	}



	if (talkStartCnt <= 21) {
		if (gPad->_trg[PAD_BUTTON_A]) {
			talkStartCnt++;
			PlaySoundMem(se[27], DX_PLAYTYPE_BACK);

		}


		if (gPad->_trg[PAD_BUTTON_B]) {
			talkStartCnt = 21;
			PlaySoundMem(se[27], DX_PLAYTYPE_BACK);
		}
	}

	
}
	



void Title_Talk::Draw() {
	
	DrawGraph(0, 0, talkBG, TRUE);

	if (talkStartCnt <= 21) {
		if (gPad->_trg[PAD_BUTTON_A]) {
			i++;
		}
	}
	 DrawGraph(0, 0, talkstart[i], TRUE);
	 DrawGraph(instX, instY, talkinst_sheet[titleCnt / 8 % 12], TRUE);

}