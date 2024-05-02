#include "DXLib.h"
#include "winmain.h"
#include "gamemain.h"
#include "pause.h"
#include "resource.h"
#include "mymath.h"

ScenePause::ScenePause() {
	LoadDivGraph("res/UI/title/ui_yes_sheet.png", 12, 12, 1, 512, 128, yessheet);
	LoadDivGraph("res/UI/title/ui_no_sheet.png", 12, 12, 1, 512, 128, nosheet);
	LoadDivGraph("res/UI/error_sheet.png", 12, 2, 6, 1920, 1080, error);
	LoadDivGraph("res/UI/error_sheet_02.png", 12, 2, 6, 1920, 1080, error_L);
	LoadDivGraph("res/UI/menu_back_sheet.png", 12, 12, 1, 512, 128, back);
	
	inst[0] = LoadGraph("res/UI/menu_htp_00.png");
	inst[1] = LoadGraph("res/UI/menu_htp_01.png");
	flame[0] = LoadGraph("res/UI/menu_frame_00.png");
	flame[1] = LoadGraph("res/UI/menu_frame_01.png");
	
	gradation = LoadGraph("res/UI/gradation.png");
	yes = LoadGraph("res/UI/title/ui_yes.png");
	no = LoadGraph("res/UI/title/ui_no.png");
	confirmation = LoadGraph("res/UI/title/ui_kakunin.png");
	info = LoadGraph("res/UI/ui_information.png");

	PlaySoundMem(se[25], DX_PLAYTYPE_BACK);


	easingMax = 100;
	easingEnd = 0;
	easingStart = -1920;
	easingFlame = 75;
	easingCnt = 0;

	RightMax = 100;
	RightEnd = 0;
	RightStart = 1920;
	RightFlame = 60;
	RightCnt = 0;

	endmax = 100;
	end = 0;
	endstart = 1920;
	endflame = 60;
	endcnt = 0;


	rerurnInst = 0;
	trueend = 0;

	insterror = 0;
}
ScenePause::~ScenePause() {
	for (int i = 0; i < 12; i++) {
		DeleteGraph(yessheet[i]);
	}
	for (int i = 0; i < 12; i++) {
		DeleteGraph(nosheet[i]);
	}
	for (int i = 0; i < 12; i++) {
		DeleteGraph(error[i]);
	}
	for (int i = 0; i < 12; i++) {
		DeleteGraph(error_L[i]);
	}
	for (int i = 0; i < 12; i++) {
		DeleteGraph(back[i]);
	}
	DeleteGraph(inst[0]);
	DeleteGraph(inst[1]);
	DeleteGraph(flame[0]);
	DeleteGraph(flame[1]);
	DeleteGraph(gradation);
	DeleteGraph(yes);
	DeleteGraph(no);
	DeleteGraph(confirmation);
	DeleteGraph(info);

}

void ScenePause::Input() {
	pauseCnt++;



}

void ScenePause::Process() {

	//選択肢上下移動
	if (easingCnt >= easingFlame - 12) {
	if (_isExplanation != 1) {
		if (selectEnd != 1) {
			if (dTrg & PAD_INPUT_UP) {
				use--;

				PlaySoundMem(se[1], DX_PLAYTYPE_BACK);
			}

			if (dTrg & PAD_INPUT_DOWN) {
				use++;

				PlaySoundMem(se[1], DX_PLAYTYPE_BACK);
			}
			if (use == 3) {
				use = 0;
			}
			if (use == -1) {
				use = 2;
			}

	if (gPad->_trg[PAD_BUTTON_A]) {


		PlaySoundMem(se[26], DX_PLAYTYPE_BACK);
		switch (use) {
		case 0: {
			_isPause = 0;
			break;
		}
		case 1: {
			_isExplanation = 1;
			break;
		}
		case 2: {
			selectEnd = 1;
			break;
		}
		}
	}


			}
		}
	}




	if (_isExplanation == 1) {



		if (rerurnInst != 1) {
			if (RightCnt <= 112) {
				RightCnt++;

				
			}
			else if (gPad->_trg[PAD_BUTTON_A]) {
				PlaySoundMem(se[26], DX_PLAYTYPE_BACK);
				rerurnInst = 1;

			}
		}
		else if (RightCnt >= 1) {
			RightCnt--;
		}

		if (RightCnt >= 80) {

		if (dTrg & PAD_INPUT_LEFT) {

			instuse--;
			insterror = 0;

			PlaySoundMem(se[24], DX_PLAYTYPE_BACK);
		}

		if (dTrg & PAD_INPUT_RIGHT) {

			instuse++;
			insterror = 0;

			PlaySoundMem(se[24], DX_PLAYTYPE_BACK);
		}
		}
		if (instuse == 2) {
			instuse = 0;
		}
		if (instuse == -1) {
			instuse = 1;
		}



		if (RightCnt == 0) {
			_isExplanation = 0;
			rerurnInst = 0;
		}

		if (insterror <= 1) {
			insterrorCnt++;
	}



	}

	if (selectEnd == 1) {
		endtime++;
		if (returnend != 1) {
			if (endcnt <= 112) {
				endcnt++;
			}
		}
		else if (endcnt >= 1) {
			endcnt--;
		}


		if (endcnt >= 80) {
		if (dTrg & PAD_INPUT_LEFT) {

			Enduse--;

			PlaySoundMem(se[1], DX_PLAYTYPE_BACK);
		}

		if (dTrg & PAD_INPUT_RIGHT) {

			Enduse++;

			PlaySoundMem(se[1], DX_PLAYTYPE_BACK);
		}
		}
		if (Enduse == 2) {
			Enduse = 0;
		}
		if (Enduse == -1) {
			Enduse = 1;
		}

		if (endcnt == 0 || endcnt == 113) {
			if (gPad->_trg[PAD_BUTTON_A]) {
				if (Enduse == 0) {
					PlaySoundMem(se[26], DX_PLAYTYPE_BACK);
					trueend++;
				}
				if (Enduse == 1) {

					PlaySoundMem(se[26], DX_PLAYTYPE_BACK);
					returnend = 1;

				}
			}
		}






		if (trueend == 1) {
			Exit = 1;

		}
		if (endcnt == 0) {
			selectEnd = 0;
			returnend = 0;
			Enduse = 0;

			trueend = 0;
		}


	}
	easingCnt++;
	easingOUT = EasingOutQuad(easingCnt, easingStart, easingEnd, easingFlame);
	easingRight = EasingOutQuad(RightCnt, RightStart, RightEnd, RightFlame);
	easingend = EasingOutQuad(endcnt, endstart, end, endflame);

}


void ScenePause::Draw() {

	DrawGraph(0, 0, gradation, TRUE);
	if (easingCnt >= easingFlame-12) {
		DrawGraph(0, 0, flame[1], TRUE);
		DrawGraph(429, 793, info, TRUE);
	}
	else {
		DrawGraph(easingOUT, 0, flame[1], TRUE);

	}
	if (easingCnt >= easingFlame-12) {
		if (easingCnt <= easingFlame) {
			DrawGraph(-32, 0, error_L[pauseCnt % 12], TRUE);
		}
	}

	//左フレーム
	if (easingCnt >= easingFlame) {
		if (use == 0) {
			DrawGraph(135, 207, arrow, TRUE);

			DrawGraph(136, 182, jRetryAnim[pauseCnt / 8 % 12], TRUE);
			DrawGraph(136, 356, Operation, TRUE);

			DrawGraph(136, 550, close, TRUE);
		}
		if (use == 1) {
		
			DrawGraph(135, 389, arrow, TRUE);
			
			DrawGraph(136, 182, reStart, TRUE);
			DrawGraph(136, 356, instAnim[pauseCnt / 8 % 12], TRUE);
			DrawGraph(136, 550, close, TRUE);

		
		}

		if (use == 2) {
			DrawGraph(135, 590, arrow, TRUE);


			DrawGraph(136, 182, reStart, TRUE);
			DrawGraph(136, 356, Operation, TRUE);
			DrawGraph(151, 550, closeAnim[pauseCnt / 8 % 12], TRUE);

		}
	}
	//説明フレーム
	if (_isExplanation != 0) {
		if (RightCnt <= RightFlame) {

			DrawGraph(easingRight, 0, flame[0], TRUE);
		}
		else {
			DrawGraph(0, 0, flame[0], TRUE);
			

		}
		if (RightFlame+5 <= RightCnt) {

			insttime++;
			if (RightCnt <= RightFlame+16) {
				DrawGraph(0, 0, error[insttime % 12], TRUE);
			}

		}
		if (RightCnt >= RightFlame+17) {
		
			if (instuse == 0) {
				insterror++;
				DrawGraph(0, 0, inst[0], TRUE);
			}
			if (instuse == 1) {
				insterror++;
				DrawGraph(0, 0, inst[1], TRUE);
			
			}
			DrawGraph(1390, 900, back[insttime/8%12], TRUE);
		}

	}

	//ゲームオーバーフレーム
	if (selectEnd != 0) {

		if (endcnt <= endflame) {
			DrawGraph(easingend, 0, flame[0], TRUE);
		}

		else {
			DrawGraph(0, 0, flame[0], TRUE);
		}
		if (endflame+5 <= endcnt) {

			if (endcnt <= endflame+16) {
				DrawGraph(0, 0, error[endtime % 12], TRUE);
			}
		}
		if (endcnt >= endflame+17) {
			DrawGraph(1000, 328, confirmation, TRUE);
			if (Enduse == 0) {
				DrawGraph(1285, 564, no, TRUE);
				DrawGraph(878, 564, yessheet[(endtime / 8) % 12], TRUE);
				DrawGraph(907, 594, arrow, TRUE);
			}
			if (Enduse == 1) {
				DrawGraph(1286, 564, nosheet[(endtime / 8) % 12], TRUE);
				DrawGraph(878, 564, yes, TRUE);
				DrawGraph(1285, 594, arrow, TRUE);

			}
		}
	}
	
	
	
}