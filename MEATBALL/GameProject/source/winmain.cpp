#include "DxLib.h"
#include "winmain.h"
#include "gamemain.h"
#include <time.h>


// プログラムはWinMain から始まります
int WINAPI WinMain(
	HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// 初期化 -------------------------------------------------------------------------
	// ＤＸライブラリ初期化処理

	SetOutApplicationLogValidFlag(false);


	SetGraphMode(SCREEN_W, SCREEN_H, SCREEN_DEPTH);	// 画面の解像度を横1280×縦720、1ドットあたり32ビットに指定する
	//ChangeWindowMode(false);							// フルスクリーンモードに指定する
	ChangeWindowMode(true);							// ウィンドウンモードに指定する
	//SetMouseDispFlag(FALSE);
	if (DxLib_Init() == -1)
	{	// エラーが起きたら直ちに終了
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);		// 描画先画面を裏画面にセット
	srand((unsigned)time(NULL));		// 乱数の初期化

	// ゲームメイン
	GameMain();


	// 解放 ---------------------------------------------------------------------------
	DxLib_End();		// ＤＸライブラリ使用の終了処理

	// 終了
 	return 0;
}
