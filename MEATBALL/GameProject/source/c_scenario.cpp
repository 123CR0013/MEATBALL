#include "C_Scenario.h"
#include "DXLib.h"
#include "winmain.h"
#include "gamemain.h"
#include "resource.h"
#include "gamepad.h"

C_Scenario::C_Scenario() {

	//02start
	talkstart_02[0] = LoadGraph("res/UI/scenario_start02/c_meatball_talk_01.png");
	talkstart_02[1] = LoadGraph("res/UI/scenario_start02/c_meatball_talk_02.png");
	talkstart_02[2] = LoadGraph("res/UI/scenario_start02/c_meatball_talk_03.png");
	talkstart_02[3] = LoadGraph("res/UI/scenario_start02/c_meatball_talk_04.png");
	talkstart_02[4] = LoadGraph("res/UI/scenario_start02/c_meatball_talk_05.png");
	talkstart_02[5] = LoadGraph("res/UI/scenario_start02/c_meatball_talk_06.png");
	talkstart_02[6] = LoadGraph("res/UI/scenario_start02/c_meatball_talk_07.png");
	talkstart_02[7] = LoadGraph("res/UI/scenario_start02/c_meatball_talk_08.png");

}
C_Scenario::~C_Scenario() {
	for (int i = 0; i <= 7; i++) {
		DeleteGraph(talkstart_02[i]);
	}
	
}
void C_Scenario::Input() {
	startCnt++;
}
void C_Scenario::Process() {

	if (talkStartCnt02 <= 6) {
		if (gPad->_trg[PAD_BUTTON_A]) {
			talkStartCnt02++;
			PlaySoundMem(se[27], DX_PLAYTYPE_BACK);
		}
		if (gPad->_trg[PAD_BUTTON_B]) {
			talkStartCnt02 = 7;
			PlaySoundMem(se[27], DX_PLAYTYPE_BACK);
		}
	}
	else {
		_talkFlg = 1;
	}
	

}

void C_Scenario::Draw() {

	if (talkStartCnt02 <= 6) {
		if (gPad->_trg[PAD_BUTTON_A]) {
			j++;
		}
	}
	DrawGraph(0, 0, talkstart_02[j], TRUE);
	DrawGraph(instX, instY, talkinst_sheet[startCnt / 8 % 12], TRUE);
	

}

