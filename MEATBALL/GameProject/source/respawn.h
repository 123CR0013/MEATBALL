#pragma once
#include "DxLib.h"
#include "mymath.h"



class Respawn
{
public:

	Respawn(int stageNo, VECTOR playerPos);
	~Respawn();

	void Process(MYRECT playerRec, int meatBallSize);

	void Draw(MATRIX mView);

	// デバッグ情報の表示
	void DrawDebugInfo(MATRIX mView, int fillFrag = 1);

	// リスポーン地点の更新があるかを調べる
	// 返り値 
	// リスポーン地点の更新　-1:しない, -1以外: する（返り値は更新後のリスポーン地点番号となる） 
	int CheckRespawnPoint(MYRECT playerRect);







	//
	// リスポーン時に反映する情報を取得する関数
	//

	// プレイヤーの座標を返す
	VECTOR GetRespawnPlayerPos() { return _playerPos; };
	// ミートボールのサイズを返す
	int GetRespawnMeatBallSize() { return _meatBallSize; };
	// カメラのフェーズ番号を返す
	int GetRespawnCamPhase() { return _camPhase; };
	// カメラのフェーズ番号を返す
	VECTOR GetRespawnCamPos() { return _camPos; };



protected:
	// リスポーン地点情報を取得する
	auto GetRespawnPointInfo(int stageNo, int respawnPointNo);

	// ステージ番号
	int _stageNo;
	// 現在のリスポーン地点番号
	int _respawnPointNo;



	// リスポーン地点情報
	struct RESPAWN_POINT
	{
		// リスポーン地点番号
		int respawnPointNo;
		// マップチップ座標
		VECTOR pos;
	};



	// リスポーン地点を更新した際に保持する情報
	// リスポーン時にこれらの情報を反映させる
	
	// プレイヤー座標
	VECTOR _playerPos;
	// ミートボールのサイズ
	int _meatBallSize;
	// カメラのフェーズ
	int _camPhase;
	// カメラ座標
	VECTOR _camPos;










	// 画像ハンドル
	// プロジェクターとホログラムの画像を組み合わせて、リスポーン地点を表現する
	
	// ホログラム
	int _cgHologram;
	// ホログラムアニメーション（リスポーン地点更新時の演出に使う）
	int _cgHologramAnimation[12];
	// プロジェクター
	int _cgProjector;


	// ホログラムアニメーション用

	// アニメーションをするかどうか
	int _isPlayingAnimation;
	// アニメーションカウント
	int _animCnt;


	// リスポーン地点更新時のSE
	int _sound;
};

