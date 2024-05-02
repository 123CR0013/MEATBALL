#include "DXLib.h"
#include "winmain.h"
#include "gamemain.h"
#include "explanation.h"
#include "resource.h"
#include "mymath.h"


explanation::explanation(int changeInst){

	speed_explamation[0] = LoadGraph("res/UI/tutorial/tr_attack_00.png");
	speed_explamation[1] = LoadGraph("res/UI/tutorial/tr_attack_01.png");
	speed_explamation[2] = LoadGraph("res/UI/tutorial/tr_attack_02.png");

	attack_explamation[0] = LoadGraph("res/UI/tutorial/tr_damage_00.png");
	attack_explamation[1] = LoadGraph("res/UI/tutorial/tr_damage_01.png");
	attack_explamation[2] = LoadGraph("res/UI/tutorial/tr_damage_02.png");
	push_a = LoadGraph("res/UI/tutorial/button_a.png");

	explanationNo = 0;

	explanationEnd = 0;

	explanationChange = changeInst;
	page = 0;
    explanationStart =1080;
	EexplanationEnd = 0;
	explanationCnt = 0;
	explanationFlame = 60;
	
}
explanation::~explanation() {

	for (int i = 0; i < 3; i++) {
		DeleteGraph(speed_explamation[i]);
	}
	for (int i = 0; i < 3; i++) {
		DeleteGraph(attack_explamation[i]);
	}
	DeleteGraph(push_a);
}
void explanation::Input() {

	

}
void explanation::Process() {
if (explanationCnt < explanationFlame) {
		explanationCnt++;
		explanationEasing = EasingOutQuad(explanationCnt, explanationStart, EexplanationEnd, explanationFlame);
	}
else {
	if (dTrg & PAD_INPUT_LEFT) {
		page--;
		PlaySoundMem(se[24], DX_PLAYTYPE_BACK);
	}
	if (dTrg & PAD_INPUT_RIGHT) {
		page++;
		PlaySoundMem(se[24], DX_PLAYTYPE_BACK);
	}

	
	if (page == 2) {
	if (gPad->_trg[PAD_BUTTON_A]) {
	
			explanationEnd = 1;
		
	}
	}
	if (page == 3) {
		page = 0;
	}
	if (page == -1) {
		page = 2;
	}
	}
}
void explanation::Draw() {
	if (explanationChange == 0) {
		if (page == 0) {
			DrawGraph(0, explanationEasing, speed_explamation[0], TRUE);
		}
		else {
			DrawGraph(0, 0, speed_explamation[page], TRUE);
		}

	}
	if (explanationChange == 1) {
		if (page == 0) {
			DrawGraph(0, explanationEasing, attack_explamation[0], TRUE);
		}
		else {
			DrawGraph(0, 0, attack_explamation[page], TRUE);
		}
	
	}
	
	if (page == 2) {
		DrawGraph(1750, 980, push_a, TRUE);
	}
}