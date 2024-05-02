#include "DxLib.h"
#include "winmain.h"
#include "gamemain.h"
#include "scenegametitle.h"
#include "scenegamemain.h"
#include "scenegameover.h"
#include "title_talk.h"
#include "gamepad.h"
#include "resource.h"
#include "mymath.h"





SceneGameTitle::SceneGameTitle() {
 
    _step = 0;
	

	/*LoadDivGraph("res/UI/happyend_roll/movie_bg_smoke_00000_sheet.png", 24, 5, 5, 1920, 1080, Smoke);*/
	LoadDivGraph("res/UI/title/title_logo_sheet.png", 12, 2, 6, 1920, 1080, title_logo_sheet);
	LoadDivGraph("res/UI/title/ui_yes_sheet.png", 12, 12, 1, 512, 128, yes_sheet);
	LoadDivGraph("res/UI/title/ui_no_sheet.png", 12, 12, 1, 512, 128, no_sheet);
	LoadDivGraph("res/UI/error_sheet.png", 12, 2, 6, 1920, 1080, error_R);
	LoadDivGraph("res/UI/error_sheet_02.png", 12, 2, 6, 1920, 1080,error_L);
	
	title_scene[0] = LoadGraph("res/UI/menu_start.png");
	title_scene[1] = LoadGraph("res/UI/menu_retry.png");
	title_scene[2] = LoadGraph("res/UI/menu_start.png");
	title_scene[3] = LoadGraph("res/UI/menu_taitoru.png");
	title_scene[4] = LoadGraph("res/UI/title/title_bg.png");
	title_scene[5] = LoadGraph("res/UI/title/title_logo.png");
	title_scene[6] = LoadGraph("res/UI/title/title_inst.png");
	title_scene[7] = LoadGraph("res/UI/menu_frame_01.png");
	title_scene[8] = LoadGraph("res/UI/title/title_obj.png");
	title_scene[9] = LoadGraph("res/UI/exit.png");
	title_scene[10] = LoadGraph("res/UI/menu_frame_00.png");
	title_scene[11] = LoadGraph("res/UI/title/title_inst.png");
	title_scene[12] = LoadGraph("res/UI/title/ui_kakunin.png");
	title_scene[13] = LoadGraph("res/UI/title/ui_no.png");
	title_scene[14] = LoadGraph("res/UI/title/ui_yes.png");
	title_scene[15] = LoadGraph("res/UI/ui_information.png");
	title_scene[16] = LoadGraph("res/UI/memu_arrow.png");
	
	movie = LoadGraph("res/UI/title/meatball_PV.mov");
	
	max = 75;
	end = 0;
	start = 1980;
	flame = 75;
	cnt = 0;
	morecnt = 0;
	morestart = -1980;

	meatvoicecnt = 0;
	


}

SceneGameTitle::~SceneGameTitle() {
	
	DeleteGraph(_cgBg);
	for (int i = 0; i < 17; i++) {
		DeleteGraph(title_scene[i]);
	}
	
	for (int i = 0; i < 12; i++) {
		DeleteGraph(title_logo_sheet[i]);
	}
	for (int i = 0; i < 12; i++) {
		DeleteGraph(yes_sheet[i]);
	}
	for (int i = 0; i < 12; i++) {
		DeleteGraph(no_sheet[i]);
	}
	for (int i = 0; i < 12; i++) {
		DeleteGraph(error_R[i]);
	}
	for (int i = 0; i < 12; i++) {
		DeleteGraph(error_L[i]);
	}
	DeleteGraph(movie);
}

void SceneGameTitle::Input() {

	time++;


}

void SceneGameTitle::Process() {
	


	if (startA == 0) {
		if (gPad->_trg[PAD_BUTTON_A]) {
			startA = 1;
			PlaySoundMem(se[26], DX_PLAYTYPE_BACK);
		}
		if (time <= 1) {
			PlaySoundMem(bgm[2], DX_PLAYTYPE_LOOP);
		}
		if (time >= 1800) {
			StopSoundMem(bgm[2]);
			startA = 4;
			
		}
	}

	if (startA == 1) {
		flamecnt++;
		if (flamecnt >= 12) {
			morecnt = 0;
			startA = 2;
			
		}
	}

	if (startA == 2) {
		flamecnt = 0;

	}
		




	

	

	if (startA == 2) {
		
		
		morecnt++;
		if (morecnt >= flame) {
			if (gPad->_trg[PAD_BUTTON_A]) {


				if (use == 0) {
					if (meatvoicecnt != 1) {
						PlaySoundMem(voice[rand() % 4], DX_PLAYTYPE_BACK);
						PlaySoundMem(se[26], DX_PLAYTYPE_BACK);
						meatvoicecnt++;
					}
					fadetalk = 1;
	              
				}
				if (use == 1) {
					startA = 3;
					PlaySoundMem(se[26], DX_PLAYTYPE_BACK);


				}


			}
		}

					

	}

	if (fadetalk == 1) {
		StopSoundMem(bgm[2]);
		switch (_step) {
		case 0:

			ColorFadeOut(0, 0, 0, 255);
			_step++;

			break;
		case 1:
			if (IsColorFade() == 0) {
				// フェードアウト終了
				SceneBase* scene = new Title_Talk();
				ChangeScene(scene);
			}
			break;

		}

	}
	else {
		if (morecnt >= flame) {
		if (startA == 2) {
			if (dTrg & PAD_INPUT_UP) {
				use--;

				PlaySoundMem(se[1], DX_PLAYTYPE_BACK);
			}

			if (dTrg & PAD_INPUT_DOWN) {
				use++;

				PlaySoundMem(se[1], DX_PLAYTYPE_BACK);
			}
		}
		}

	}
if (use == 2) {
			use = 0;
		}
		if (use == -1) {
			use = 1;
		}
	
	if (startA == 3) {
		if (returnframe != 1) {
			if (cnt <= flame+12) {
				cnt++;
			}
		}
		else if (cnt >= 1) {
			cnt--;
		}
		if (cnt >= flame) {

			if (dTrg & PAD_INPUT_RIGHT) {

				enduse++;

				PlaySoundMem(se[1], DX_PLAYTYPE_BACK);
			}

			if (dTrg & PAD_INPUT_LEFT) {

				enduse--;

				PlaySoundMem(se[1], DX_PLAYTYPE_BACK);
			}
		}

        if (enduse == 2) {
			enduse = 0;
		}
		if (enduse == -1) {
			enduse = 1;
		}
		if (cnt== flame+13) {
			if (gPad->_trg[PAD_BUTTON_A]) {
				PlaySoundMem(se[26], DX_PLAYTYPE_BACK);
				if (enduse == 0) {
					trueend++;
				}

				if (enduse == 1) {



					
					enduse = 0;
					trueend = 0;
					returnframe = 1;

				}

			}
		}
		if (cnt == 0) {
			startA = 2;
			returnframe = 0;
		}


	
		


	
	}

	if (trueend == 1) {
		Exit = 1;
	}
	if (startA == 4) {
		PlayMovieToGraph(movie);
		if (TellMovieToGraph(movie) == 70583) {
			SeekMovieToGraph(movie, 0);
			PauseMovieToGraph(movie);
			time = 0;
			startA = 0;
		}
		if (gPad->_trg[PAD_BUTTON_A]) {
			SeekMovieToGraph(movie, 0);
			PauseMovieToGraph(movie);
			time = 0;
			startA = 0;
		}
	}

	easingTitle = EasingOutQuad(cnt, start, end, flame);
	moreeasing= EasingOutQuad(morecnt, morestart, end, flame);
}

void SceneGameTitle::Draw() {
    
	
	
	DrawGraph(0, 0, title_scene[4], TRUE);
	DrawGraph(0, 0, title_scene[8], TRUE);
	
	 if (startA == 0) {
	 DrawGraph(1200, 400, titleBottunSheet[time/8%12], TRUE);
     DrawGraph(0, 0, title_scene[5], TRUE);
 }
	
	if (startA == 1) {
		DrawGraph(0, 0, title_logo_sheet[time % 12], TRUE);
	}



	if (startA == 2 || startA == 3) {


		if (morecnt >= flame) {
			DrawGraph(0, 0, title_scene[7], TRUE);
			DrawGraph(429, 793, title_scene[15], TRUE);
			if (morecnt <= flame+12) {
				DrawGraph(-64, -128, error_L[time % 12], TRUE);
			}
		}

		if (morecnt <= flame) {

			DrawGraph(moreeasing, 0, title_scene[7], TRUE);
		}
		if (morecnt >= flame+12) {
			if (use == 0) {
				DrawGraph(168, 182, startAnim[time / 8 % 12], TRUE);
				DrawGraph(146, 216, arrow, TRUE);
				DrawGraph(168, 356, close, TRUE);
			}
			if (use == 1) {
				DrawGraph(168, 356, closeAnim[time / 8 % 12], TRUE);
				DrawGraph(146, 386, arrow, TRUE);
				DrawGraph(168, 182, title_scene[2], TRUE);
			}
		}

	}
	
	if (startA == 3) {
		
		
		if (cnt <= flame) {
			DrawGraph(easingTitle, 0, title_scene[10], TRUE);
		}
		if (cnt >= flame)  {
            DrawGraph(0, 0, title_scene[10], TRUE);
			
		
			if (cnt <= flame+12) {
				DrawGraph(0, 0, error_R[time % 12], TRUE);
			}
		}
		if (cnt == flame+13) {
				DrawGraph(1000, 328, title_scene[12], TRUE);
			if(enduse==0){
				DrawGraph(1285, 564, title_scene[13], TRUE);
				DrawGraph(878, 564, yes_sheet[(time / 8) % 12], TRUE);
				DrawGraph(907, 594, title_scene[16], TRUE);
			}
			if (enduse == 1) {
				DrawGraph(1286, 564, no_sheet[(time / 8) % 12], TRUE);
				DrawGraph(878, 564, title_scene[14], TRUE);
				DrawGraph(1285, 594, title_scene[16], TRUE);
			}
		}
	}
	if (startA==4) {
		DrawGraph(0, 0, movie, FALSE);
	}

		
		

	
	
	
}
