#include "DxLib.h"
#include "resource.h"
#include "effect.h"


//ミートボール画像データ
int _cgChip[200];  //背景チップ（配列）
int _cgBG[2]; // 背景画像


int cgDashPanel[4];
int cgBomb[6];



int retry;
int NOW_LOADING[12];
int	gameover;
int close;
int retryAnim[12]; 
int closeAnim[12];
int instAnim[12];
int jRetryAnim[12];
int startAnim[12];

int arrow;
int flame[2];
int   reStart;
int   Operation;

int title_BG;

int _cgUI_LevelBar;
int _cgUI_BossBar;
int _cgUI_Bottom;
int UI_Number[3];
int level;
int talkinst_sheet[12];
int talkBG;
int titleBottunSheet[12];

int cgEffect[EFFECT_TYPE_MAX][EFFECT_SHEETS_MAX];

// プレイヤー サイコキネシス
int cgAttackAura[20];
int cgReturnAura[13];

// ミートボール 呼び戻しエフェクト
int cgMeatBallReturn[13];


int meat_face[2];

int instlower_right[6];


int cgBullet[30];
int cgBossBullet[30];



// 3Dモデル
// 機械パーツ

int metalPartsModel[3];

//音声データ
int se[SE];
int bgm[BGM];
int voice[VOICE];
int sound_explanation;





void ResourceInit() {

	//タイトル画像
	_cgBG[0] = LoadGraph("res/bg/BG_01.jpg");
	_cgBG[1] = LoadGraph("res/UI/title/title_bg.png");

	title_BG = LoadGraph("res/bg/title_bg.png");
	

	//bgm
	for (auto& cg : bgm) {
		cg = -1;
	}
	bgm[0] = LoadSoundMem("res/sound/BGM/bgm_stage_01.mp3");
	bgm[1] = LoadSoundMem("res/sound/BGM/bgm_stage_02.mp3");
	bgm[2] = LoadSoundMem("res/sound/BGM/bgm_title.mp3");
	bgm[3] = LoadSoundMem("res/sound/BGM/bgm_beforestage01.mp3");
	bgm[4] = LoadSoundMem("res/sound/BGM/bgm_afterstage02.mp3");
	bgm[5] = LoadSoundMem("res/sound/BGM/bgm_gameover.mp3");
	bgm[6] = LoadSoundMem("res/sound/BGM/bgm_result.mp3");

	//sound_explanation用
	sound_explanation = LoadSoundMem("res/sound/SE/se_ui_explain.mp3");


	//
	// サウンド
	// 
	for (auto& cg : se) {
		cg = -1;
	}
	se[0] = LoadSoundMem("res/sound/SE/se_enemy_rush.mp3");
	se[1] = LoadSoundMem("res/sound/SE/se_ui_noise.mp3");
	se[2] = LoadSoundMem("res/sound/SE/se_enemy_shoot.mp3");
	se[3] = LoadSoundMem("res/sound/SE/se_gimmick_jump.mp3");
	se[4] = LoadSoundMem("res/sound/SE/se_gimmick_lever.mp3");
	se[5] = LoadSoundMem("res/sound/SE/se_meat_reflection.mp3");
	se[6] = LoadSoundMem("res/sound/SE/se_rbenemy_break.mp3");
	se[7] = LoadSoundMem("res/sound/SE/se_sbenemy_break.mp3");
	se[10] = LoadSoundMem("res/sound/SE/se_player_jump.mp3");
	se[11] = LoadSoundMem("res/sound/SE/se_player_psycho.mp3");
	se[12] = LoadSoundMem("res/sound/SE/se_player_landing.mp3");
	se[13] = LoadSoundMem("res/sound/SE/se_player_dash.mp3");
	se[14] = LoadSoundMem("res/sound/SE/se_player_move.mp3");
	se[15] = LoadSoundMem("res/sound/SE/se_player_heartbeat.mp3");
	se[16] = LoadSoundMem("res/sound/SE/se_gimmick_breakblock.mp3");
	se[17] = LoadSoundMem("res/sound/SE/se_gimmick_bomb.mp3");
	se[18] = LoadSoundMem("res/sound/SE/se_gimmick_axel.mp3");
	se[19] = LoadSoundMem("res/sound/SE/se_gimmick_door.mp3");
	se[20] = LoadSoundMem("res/sound/SE/se_player_call.mp3");
	se[21] = LoadSoundMem("res/sound/SE/se_enemy_boot.mp3");
	se[22] = LoadSoundMem("res/sound/SE/se_ui_gameoverselect.mp3");
	se[24] = LoadSoundMem("res/sound/SE/se_ui_page.mp3");
	se[25] = LoadSoundMem("res/sound/SE/se_ui_menustart.mp3");
	se[26] = LoadSoundMem("res/sound/SE/se_select.mp3");
	se[27] = LoadSoundMem("res/sound/SE/se_ui_scenarioskip.mp3");
	se[28] = LoadSoundMem("res/sound/SE/se_meat_hit.mp3");


	//ボイス
	for (auto& cg : voice) {
		cg = -1;
	}
	//ミートボール攻撃ボイス（リリース)
	voice[0] = LoadSoundMem("res/sound/VOICE/meatball/vc_meat_attack_01.mp3");
	voice[1] = LoadSoundMem("res/sound/VOICE/meatball/vc_meat_attack_02.mp3");
	voice[2] = LoadSoundMem("res/sound/VOICE/meatball/vc_meat_attack_03.mp3");
	voice[3] = LoadSoundMem("res/sound/VOICE/meatball/vc_meat_attack_04.mp3");
	voice[4] = LoadSoundMem("res/sound/VOICE/meatball/vc_meat_attack_05.mp3");
	//少女アクションボイス
	voice[5] = LoadSoundMem("res/sound/VOICE/girl28/vc_player_action_00.mp3");
	voice[6] = LoadSoundMem("res/sound/VOICE/girl28/vc_player_action_01.mp3");
	voice[7] = LoadSoundMem("res/sound/VOICE/girl28/vc_player_action_02.mp3");
	voice[8] = LoadSoundMem("res/sound/VOICE/girl28/vc_player_action_03.mp3");
	//ミートボール攻撃ボイス（ヒット）
	voice[9] = LoadSoundMem("res/sound/VOICE/meatball/vc_meat_hit_00.mp3");
	voice[10] = LoadSoundMem("res/sound/VOICE/meatball/vc_meat_hit_01.mp3");
	//ミートボールクリアボイス
	voice[11] = LoadSoundMem("res/sound/VOICE/meatball/vc_meat_clear.mp3");
	//少女クリアボイス
	voice[12] = LoadSoundMem("res/sound/VOICE/girl28/vc_player_clear.mp3");
	//少女スタートボイス
	voice[13] = LoadSoundMem("res/sound/VOICE/girl28/vc_player_start.mp3");
	//少女死亡ボイス
	voice[14] = LoadSoundMem("res/sound/VOICE/girl28/vc_player_death.mp3");
	//少女被ダメージボイス
	voice[15] = LoadSoundMem("res/sound/VOICE/girl28/vc_player_damage_00.mp3");
	voice[16] = LoadSoundMem("res/sound/VOICE/girl28/vc_player_damage_01.mp3");

	UI_Number[0] = LoadGraph("res/UI/level_1.png");
	UI_Number[1] = LoadGraph("res/UI/level_2.png");
	UI_Number[2] = LoadGraph("res/UI/level_3.png");
	
	level = LoadGraph("res/UI/level.png");

	
	//汎用アニメーションシート読み込み
	 LoadDivGraph("res/UI/menu_retry_sheet.png", 12, 12, 1, 512, 128,retryAnim);
	 LoadDivGraph("res/UI/menu_close_sheet.png", 12, 12, 1, 512, 128, closeAnim);
	 LoadDivGraph("res/UI/menu_inst_sheet.png", 12, 12, 1, 512, 128, instAnim);
	 LoadDivGraph("res/UI/menu_saikai_sheet.png", 12, 12, 1, 512, 128, jRetryAnim);

	 LoadDivGraph("res/UI/menu_start_sheet.png", 12, 12, 1, 512, 128, startAnim);

	 LoadDivGraph("res/UI/talkinst_sheet.png", 12, 12, 1, 512, 128, talkinst_sheet);
	 LoadDivGraph("res/UI/title/ui_pressa_sheet.png", 12, 12, 1, 512, 128, titleBottunSheet);


	//ポーズ画面画像
	reStart= LoadGraph("res/UI/menu_saikai.png");
	Operation= LoadGraph("res/UI/menu_sousa.png");
	
	
	

	//ゲームオーバー画面画像
	gameover = LoadGraph("res/UI/gameover/gameover.png");
	close = LoadGraph("res/UI/gameover/menu_close.png");
	retry = LoadGraph("res/UI/gameover/menu_retry.png");

	arrow = LoadGraph("res/UI/gameover/memu_yajurusi.png");
}

void ResourceRelease()
{

	// サウンド
	for (auto& cg : se) {
		if (cg != -1) {
			DeleteSoundMem(cg);
			cg = -1;
		}
	}
	if(sound_explanation != -1) {
		DeleteSoundMem(sound_explanation);
		sound_explanation = -1;
	}


	//bgm
	for (auto& cg : bgm) {
		if (cg != -1) {
			DeleteSoundMem(cg);
			cg = -1;
		}
	}

	//voice
	for (auto& cg : voice) {
		if (cg != -1) {
			DeleteSoundMem(cg);
			cg = -1;
		}
	}

	//photo
	for (int i = 0; i < 2; i++) {
		if (_cgBG[i] != -1) {
			DeleteGraph(_cgBG[i]);
			_cgBG[i] = -1;
		}
	}
	
	for(int i = 0; i < 3; i++) {
		if(UI_Number[i] != -1) {
			DeleteGraph(UI_Number[i]);
			UI_Number[i] = -1;
		}
	}
	if(level != -1) {
		DeleteGraph(level);
		level = -1;
	}
	if (reStart != -1) {
		DeleteGraph(reStart);
		reStart = -1;
	}
	if (title_BG != -1) {
		DeleteGraph(title_BG);
		title_BG = -1;
	}
	if (Operation != -1) {
		DeleteGraph(Operation);
		Operation = -1;
	}
	if (gameover != -1) {
		DeleteGraph(gameover);
		gameover = -1;
	}
	if (close != -1) {
		DeleteGraph(close);
		close = -1;
	}
	if (retry != -1) {
		DeleteGraph(retry);
		retry = -1;
	}
	if (arrow != -1) {
		DeleteGraph(arrow);
		arrow = -1;
	}

	//sheet
	for (auto& cg : retryAnim) {
		if (cg != -1) {
			DeleteGraph(cg);
			cg = -1;
		}
	}
	for (auto& cg : closeAnim) {
		if (cg != -1) {
			DeleteGraph(cg);
			cg = -1;
		}
	}
	for (auto& cg : instAnim) {
		if (cg != -1) {
			DeleteGraph(cg);
			cg = -1;
		}
	}
	for (auto& cg : jRetryAnim) {
		if (cg != -1) {
			DeleteGraph(cg);
			cg = -1;
		}
	}
	for (auto& cg : startAnim) {
		if (cg != -1) {
			DeleteGraph(cg);
			cg = -1;
		}
	}
	for (auto& cg : talkinst_sheet) {
		if (cg != -1) {
			DeleteGraph(cg);
			cg = -1;
		}
	}
	for (auto& cg : titleBottunSheet) {
		if (cg != -1) {
			DeleteGraph(cg);
			cg = -1;
		}
	}

}
;


