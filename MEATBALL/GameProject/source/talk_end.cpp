#include "talk_end.h"
#include "scenegamemain.h"
#include "DXLib.h"
#include "SceneBadEnding.h"
#include "title_talk.h"
#include "winmain.h"
#include "gamemain.h"
#include "resource.h"
#include "gamepad.h"

talk_end::talk_end() {
	
	_step = 0;
	// siroからフェードインする
	ColorMask(255, 255, 255, 255);

	talkBadEnd[0] = LoadGraph("res/UI/scenario_BadEnd/d_meatball_talk_01.png");
	talkBadEnd[1] = LoadGraph("res/UI/scenario_BadEnd/d_meatball_talk_02.png");
	talkBadEnd[2] = LoadGraph("res/UI/scenario_BadEnd/d_meatball_talk_03.png");
	talkBadEnd[3] = LoadGraph("res/UI/scenario_BadEnd/d_meatball_talk_04.png");
	talkBadEnd[4] = LoadGraph("res/UI/scenario_BadEnd/d_meatball_talk_05.png");
	talkBadEnd[5] = LoadGraph("res/UI/scenario_BadEnd/d_meatball_talk_06.png");
	talkBadEnd[6] = LoadGraph("res/UI/scenario_BadEnd/d_meatball_talk_07.png");
	talkBadEnd[7] = LoadGraph("res/UI/scenario_BadEnd/d_meatball_talk_08.png");
	talkBadEnd[8] = LoadGraph("res/UI/scenario_BadEnd/d_meatball_talk_09.png");
	talkBadEnd[9] = LoadGraph("res/UI/scenario_BadEnd/d_meatball_talk_10.png");
	talkBadEnd[10] = LoadGraph("res/UI/scenario_BadEnd/d_meatball_talk_11.png");
	talkBadEnd[11] = LoadGraph("res/UI/scenario_BadEnd/d_meatball_talk_12.png");
	talkBadEnd[12] = LoadGraph("res/UI/scenario_BadEnd/d_meatball_talk_13.png");
	talkBadEnd[13] = LoadGraph("res/UI/scenario_BadEnd/d_meatball_talk_14.png");
	talkBadEnd[14] = LoadGraph("res/UI/scenario_BadEnd/d_meatball_talk_15.png");
	talkBadEnd[15] = LoadGraph("res/UI/scenario_BadEnd/d_meatball_talk_16.png");
	talkBadEnd[16] = LoadGraph("res/UI/scenario_BadEnd/d_meatball_talk_17.png");
	talkBadEnd[17] = LoadGraph("res/UI/scenario_BadEnd/d_meatball_talk_18.png");
	talkBadEnd[18] = LoadGraph("res/UI/scenario_BadEnd/d_meatball_talk_19.png");
	talkBadEnd[19] = LoadGraph("res/UI/scenario_BadEnd/d_meatball_talk_20.png");
	talkBadEnd[20] = LoadGraph("res/UI/scenario_BadEnd/d_meatball_talk_21.png");
	talkBadEnd[21] = LoadGraph("res/UI/scenario_BadEnd/d_meatball_talk_22.png");
	talkBadEnd[22] = LoadGraph("res/UI/scenario_BadEnd/d_meatball_talk_23.png");
	talkBadEnd[23] = LoadGraph("res/UI/scenario_BadEnd/d_meatball_talk_24.png");
	talkBadEnd[24] = LoadGraph("res/UI/scenario_BadEnd/d_meatball_talk_25.png");

	talkBG = LoadGraph("res/UI/talk_bg.png");


	
     PlaySoundMem(bgm[4], DX_PLAYTYPE_LOOP);
}

talk_end::~talk_end() {

	for (int i = 0; i <= 24; i++) {
		DeleteGraph(talkBadEnd[i]);
	}
	DeleteGraph(talkBG);
}
void talk_end::Input(){
	endCnt++;
}
void talk_end::Process() {

	if (badcnt <= 24) {
		if (gPad->_trg[PAD_BUTTON_A]) {
			badcnt++;
			PlaySoundMem(se[27], DX_PLAYTYPE_BACK);
		}
		if (gPad->_trg[PAD_BUTTON_B]) {
			badcnt = 25;
			PlaySoundMem(se[27], DX_PLAYTYPE_BACK);
		}
	}

	
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
	
	case 2:
		// 通常処理
		if (badcnt >= 25) {
			
				// フェードアウト開始
				ColorFadeOut(0, 0, 0, 60);
				_step++;
				StopSoundMem(bgm[4]);
		}
		break;
	case 3:
		if (IsColorFade() == 0) {
			// フェードアウト終了
			SceneBase* scene = new SceneBadEnding();
			ChangeScene(scene);
		}
		break;
	}

	
}
void talk_end::Draw() {

	DrawGraph(0, 0, talkBG, TRUE);

	if (badcnt <= 24) {
		if (gPad->_trg[PAD_BUTTON_A]) {
			i++;
		}
	}

	DrawGraph(0, 0, talkBadEnd[i], TRUE);


	DrawGraph(instX, instY, talkinst_sheet[endCnt / 8 % 12], TRUE);

}