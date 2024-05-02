#include "B_Scenario.h"
#include "DXLib.h"
#include "winmain.h"
#include "gamemain.h"
#include "resource.h"
#include "gamepad.h"

B_Scenario::B_Scenario() {

	//02start
	talkclear[0] = LoadGraph("res/UI/scenario_clear01/b_meatball_talk_01.png");
	talkclear[1] = LoadGraph("res/UI/scenario_clear01/b_meatball_talk_02.png");
	talkclear[2] = LoadGraph("res/UI/scenario_clear01/b_meatball_talk_03.png");
	talkclear[3] = LoadGraph("res/UI/scenario_clear01/b_meatball_talk_04.png");
	talkclear[4] = LoadGraph("res/UI/scenario_clear01/b_meatball_talk_05.png");
	talkclear[5] = LoadGraph("res/UI/scenario_clear01/b_meatball_talk_06.png");
	talkclear[6] = LoadGraph("res/UI/scenario_clear01/b_meatball_talk_07.png");
	talkclear[7] = LoadGraph("res/UI/scenario_clear01/b_meatball_talk_08.png");
	talkclear[8] = LoadGraph("res/UI/scenario_clear01/b_meatball_talk_09.png");
	talkclear[9] = LoadGraph("res/UI/scenario_clear01/b_meatball_talk_10.png");
	talkclear[10] = LoadGraph("res/UI/scenario_clear01/b_meatball_talk_11.png");

	_step = 0;
}
B_Scenario::~B_Scenario() {
	for (int i = 0; i <= 10; i++) {
		DeleteGraph(talkclear[i]);
	}
}
void B_Scenario::Input() {
	clearCnt++;
}
void B_Scenario::Process() {

	if (talkClearCnt <= 11) {
		if (gPad->_trg[PAD_BUTTON_A]) {
			talkClearCnt++;
			PlaySoundMem(se[27], DX_PLAYTYPE_BACK);
		}
		if (gPad->_trg[PAD_BUTTON_B]) {
			talkClearCnt=11;
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
			_stepcnt = 0;
		}
		break;

	case 2:
		// 通常処理

		if (talkClearCnt >= 11) {
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
				_BtalkFlg = 1;
		}
		break;
	}


}

void B_Scenario::Draw() {

	if (talkClearCnt <= 11) {
		if (gPad->_trg[PAD_BUTTON_A]) {
			j++;
		}
	}
	DrawGraph(0, 0, talkclear[j], TRUE);

	DrawGraph(instX, instY, talkinst_sheet[clearCnt / 8 % 12], TRUE);



	/*DrawFormatString(0, 500, COLOR_WHITE, "talk=%d", talkClearCnt);*/
}

