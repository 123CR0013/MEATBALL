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
// 定数・構造体定義
//

GamePad* gPad;

//シーンポインタ
SceneBase* gScene;					// ポインタなのでnewするまで実体は無い

ScenePause* gScenePause;            //ポーズ画面用切り替えポインタ

 C_Scenario* gC_Scenario;
 B_Scenario* gB_Scenario;


 explanation* gExplanation;

//
// グローバル変数宣言
//

// ゲーム情報

int Exit ;








// Sceneの切り替え用
SceneBase* gNewScene;				// NULL以外がセットされていたら、シーンを切り替える

// Sceneを変更する
void ChangeScene(SceneBase* scene) {
	gNewScene = scene;		// 次のシーンをセット
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

// カラーマスク用
int gColorMaskR = 0, gColorMaskG = 0, gColorMaskB = 0, gColorMaskA = 0;

// 指定の色をカラーマスク
void ColorMask(int colorR, int colorG, int colorB, int colorA) {
	gColorMaskR = colorR;
	gColorMaskG = colorG;
	gColorMaskB = colorB;
	gColorMaskA = colorA;
}

// フェードインアウト用
int gColorFadeStR, gColorFadeStG, gColorFadeStB, gColorFadeStA;
int gColorFadeEdR, gColorFadeEdG, gColorFadeEdB, gColorFadeEdA;
int gFadeFrames = 0, gFadeCnt = 0;

// 指定の色にフェードアウト（カラーマスク使用, alpha=255にする）
void ColorFadeOut(int colorR, int colorG, int colorB, int frames) {
	// フェード開始値
	gColorFadeStR = gColorMaskR;
	gColorFadeStG = gColorMaskG;
	gColorFadeStB = gColorMaskB;
	gColorFadeStA = gColorMaskA;
	// フェード到達値
	gColorFadeEdR = colorR;
	gColorFadeEdG = colorG;
	gColorFadeEdB = colorB;
	gColorFadeEdA = 255;
	// フェードカウンタ
	gFadeFrames = frames;
	gFadeCnt = 0;
}

// 現在のカラーマスクからフェードイン（カラーマスク使用, alpha=0にする）
void ColorFadeIn(int frames) {
	// フェード開始値
	gColorFadeStR = gColorMaskR;
	gColorFadeStG = gColorMaskG;
	gColorFadeStB = gColorMaskB;
	gColorFadeStA = gColorMaskA;
	// フェード到達値
	gColorFadeEdR = gColorMaskR;
	gColorFadeEdG = gColorMaskG;
	gColorFadeEdB = gColorMaskB;
	gColorFadeEdA = 0;
	// フェードカウンタ
	gFadeFrames = frames;
	gFadeCnt = 0;
}

// フェードインアウトは終了したか？
// 戻り値: 1 = フェード中, 0 = フェード終了(もしくは開始前)
int IsColorFade() {
	if(gFadeCnt < gFadeFrames) {
		return 1;
	}
	return 0;
}

// クロスフェード用
int gCgCrossFade[448] = { -1 };	// 画面を取り込んだ画像
int gCrossFadeA = 0;
int gCrossFadeFrames = 0, gCrossFadeCnt = 0;
// うねうねパラメータ
float	gCrossFade_speed;		// 揺らぎの速度
float	gCrossFade_xspan;		// 横方向のゆらぎ度合い
float	gCrossFade_yspan;		// 縦方向のゆらぎ度合い


// クロスフェード用画像の削除
void DeleteCrossFadeCg() {
	for(int y = 0; y < 448; y++) {
		if(gCgCrossFade[y] != -1) {
			// 画像を削除
			DeleteGraph(gCgCrossFade[y]);
			gCgCrossFade[y] = -1;
		}
	}
}

// 現在の画面からクロスフェードアウト
void CrossFadeOut(int frames) {
	DeleteCrossFadeCg();

	// 画面サイズと同じスクリーンを、縦1pixelずつで作成
	for(int y = 0; y < 448; y++) {
		gCgCrossFade[y] = MakeGraph(1280, 1);

		// 画面に表示されている画像を縦1pixelずつ取り込み
		GetDrawScreenGraph(320, 64, 1600, 513, gCgCrossFade[y]);
	}

	// フェードカウンタ
	gCrossFadeFrames = frames;
	gCrossFadeCnt = 0;

	// うねうねパラメータ
	gCrossFade_speed = 10.0;
	gCrossFade_yspan = 5.5;
	gCrossFade_xspan = 5.0;	// xは0からスタート
}

// クロスフェードアウトは終了したか？
// 戻り値: 1 = フェード中, 0 = フェード終了(もしくは開始前)
int IsCrossFade() {
	if(gCrossFadeCnt < gCrossFadeFrames) {
		return 1;
	}
	return 0;
}

// アプリの初期化
// 起動時に1回だけ実行される
void AppInit() {
	ResourceInit();

	gScenePause = nullptr;
	
	gNewScene = NULL;		// 次のシーンを初期化する

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


// アプリの解放
// 終了時に1回だけ実行される
void AppRelease() {
	delete gPad;
	delete gScene;				// Sceneを削除する

	ResourceRelease();
}





//
// フレーム処理。1フレームに1回ずつ実行される
//

// フレーム処理：入力
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




// フレーム処理：計算
void FrameProcess() {
	// Sceneの計算呼び出し
	
	if (gScenePause != NULL) {
		gScenePause->Process();

		int pauseFlag = 0;
		//if(gScenePause->_isExplanation == 1) {
		//	SceneBase* scene = new SceneExplanation();
		//	ChangeScene(scene);//なんかの画像
		//	pauseFlag = 1;
		//}
		//if(gScenePause->_isVoiceChange == 1) {
		//	SceneBase* scene = new SceneVoiceChange();
		//	ChangeScene(scene);//Pauseの中でやる（仮置きでクラス）
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

	


	// カラーマスクのフェードインアウト
	if(IsColorFade() != 0) {
		gFadeCnt++;
		gColorMaskR = EasingLinear(gFadeCnt, gColorFadeStR, gColorFadeEdR, gFadeFrames);
		gColorMaskG = EasingLinear(gFadeCnt, gColorFadeStG, gColorFadeEdG, gFadeFrames);
		gColorMaskB = EasingLinear(gFadeCnt, gColorFadeStB, gColorFadeEdB, gFadeFrames);
		gColorMaskA = EasingLinear(gFadeCnt, gColorFadeStA, gColorFadeEdA, gFadeFrames);
	}
	// クロスフェード画像のフェードアウト
	if(IsCrossFade() != 0) {
		gCrossFadeCnt++;
		gCrossFadeA = EasingLinear(gCrossFadeCnt, 255, 0, gCrossFadeFrames);

		gCrossFade_xspan += 2.5;	// 横のうねうね加速度
	}
	
}

// フレーム処理：描画
void FrameDraw() {

	ClearDrawScreen();						// 画面を初期化する	

	// Sceneの描画呼び出し
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

	// クロスフェード画像の上乗せ
	if(gCrossFadeA > 0) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, gCrossFadeA);	// 半透明モード

		// sinで横をうねうねさせる
		int x, y;
		for(y = 0; y < 448; y++) {
			float rad = (((float)(gCrossFadeFrames - gCrossFadeCnt) * gCrossFade_speed) + (float)y * gCrossFade_yspan) * PI / 180.0;
			x = (int)(sin(rad) * gCrossFade_xspan);
			DrawGraph(320, 64, gCgCrossFade[y], FALSE);
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	// カラーマスク
	if(gColorMaskA > 0) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, gColorMaskA);	// 半透明モード
		DrawBox(0, 0, SCREEN_W, SCREEN_H, GetColor(gColorMaskR, gColorMaskG, gColorMaskB), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, gColorMaskA);
	}

	fps_wait();

#ifdef _DEBUG
	//int loadNum = GetASyncLoadNum();
	//DrawFormatString(1800, 0, GetColor(255, 255, 255), "%3d", loadNum);
	//draw_fps(0, 0);
#endif

	ScreenFlip();							// 裏画面の内容を表画面に反映させる
}


void GameMain()
{
	AppInit();	// 初期化

	// 1フレームループを組む ----------------------------------------------------------
	while (ProcessMessage() == 0)		// プログラムが終了するまでループ
	{
		// シーンを切り替えるか？
		if (gNewScene != NULL) {
			delete gScene;		// 今のシーンを削除する
			gScene = gNewScene;	// 受け取ったシーンをポインタセット
			gNewScene = NULL;	// 次のシーンを初期化する
		}

		FrameInput();	// 入力
		FrameProcess();	// 計算
		FrameDraw();	// 描画
		if(Exit != 0) {
			break;
		}
		if (CheckHitKey(KEY_INPUT_ESCAPE) != 0)	// ESCAPEキーが押されているかを調べる
			break;			// 押されていたらメインループを抜ける
	}

	AppRelease();	// 解放
}











