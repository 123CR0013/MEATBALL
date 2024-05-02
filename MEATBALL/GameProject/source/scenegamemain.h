#pragma once	// .hの先頭に記述。#includeでこのファイルを何度読み込みしても、1度しか読み込まない
#include "SceneBase.h"
#include "MapData.h"
#include "Camera.h"
#include "scenemovie.h"
#include "SceneGameOver.h"


#include "respawn.h"





class SceneGameMain : public SceneBase {
public:
	SceneGameMain();			// コンストラクタ
	virtual ~SceneGameMain();	// デストラクタ

	void SceneGameMainResourceInit();
	void SceneGameMainResourceRelease();

	void StageInit(int stageNo);
	void StageRelease();



	void	Input() override;	// 入力
	void	Process() override;	// 計算
	void	Draw() override;	// 描画


	int _Flag_talk = 0;
	int _BFlag_talk = 0;
	int _Box_Flag = 0;
	int _C_BoxFlg = 0;


	
	// ハートの残りカウント描画
	// 左上のUIへの描画
	void DrawHeartCount_UI();
	// ハート本体に重ねて描画
	void DrawHeartCount_Body(MATRIX mView);



	// リスポーン処理
	void SetRespawn();

protected:
	// フェードイン・フェードアウト関連
	int _fadeInStep;
	int _instep;
	int _stepcnt;
	int _isEliminatedBoss = 0;
	int step;
	int start_UI = 0;
	int throw_UI=0;
	int throw_UI_two = 0;
	int return_UI = 0;
	int Dash_UI = 0;
	int METALENEMYGUIDE = 0;
	int hardwallguide = 0;

	//イージング適用
	int Guide_Easing;
	int Start_Easing;
	int Return_Easing;
	int Dash_Easing;
	int Throw_Easing;
	float guide_start;
	float guide_end;
	float guide_cnt;
	float return_cnt;
	float start_cnt;
	float throw_cnt;
	float dash_cnt;
	float guide_frame;






	int _cgReturnFrame;
	int _cgReturnBar;

	//チュートリアル導入
	int speed = 0;
	int damage = 0;


	// ハートUI画像
	int _cgUiHeart;
	// 数字UI画像（ハートの残りカウント数の表示用）
	// 左上のUI部分と本体に重ねて表示する部分の2箇所で使用する
	int _cgUiNumber[10];



	Respawn* _respawn;

	//
	// シーン
	//

	// ムービーシーン
	SceneMovie* _sceneMovie;
	// 画面遷移中かどうか
	int _isTransition;
	// 画面遷移中のカウント
	int _transitionCnt;

	//ゲームオーバーシーン
	SceneGameOver* _sceneGameOver;



	//
	// 開発用
	//

	// デバッグ表示をするか
	int _debug = 0;
	// デバッグ表示時に、敵の当たり判定矩形を塗りつぶすか
	int _enemyRectFill = 0;
};

// カメラ
extern Camera	gCam;




