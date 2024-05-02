#pragma once	// .hの先頭に記述。#includeでこのファイルを何度読み込みしても、1度しか読み込まない
#include "SceneBase.h"
#include "mymath.h"

#define MOVIE_PROLOGUE 0
#define MOVIE_CLEAR 1

class SceneMovie : public SceneBase {
public:
	SceneMovie(int movieNum);			// コンストラクタ
	virtual ~SceneMovie();	// デストラクタ

	void	Input() override;	// 入力
	void	Process() override;	// 計算
	void	Draw() override;	// 描画

	void AttachMeatBallToPlayer();

	int GetMovieNo() { return _movieNo; };
	int GetEndMovie() { return _isEndMovie; };

protected:
	// ムービー種類
	int _movieNo;
	// ムービーが最後まで流れたかどうか
	int _isEndMovie;

	// 背景画像ハンドル
	int _cgBgMovie;

	// フェードイン・フェードアウト関連
	int _step;
	int _stepcnt;

	int movievoicecnt = 0;
	int presscnt = 0;


	int press = 0;
	

	// 3Dモデル情報
	struct MODEL {
		int modelHandle;
		VECTOR pos;

		int animIndex;
		float totalTime;
		float playTime;
	};
	// 3Dモデル
	MODEL _player; // プレイヤーのモデル
	MODEL _meatBall; // ミートボールのモデル

	// ミートボールの回転角
	float _rot;
	// ミートボールを配置するフレームの番号
	int _frameNo;


	// プレイヤーの3Dモデルに、カメラの位置と注視点となるフレームが含まれている
	// カメラを配置するフレームの番号
	int _camFrameNo;
	// カメラの注視点となるフレームの番号
	int _gazeFrameNo;


	//カメラの座標
	VECTOR _camPos;

	// 注視点
	VECTOR _gazePoint;



	// ミートボールの挙動関連

	// アウトラインを描画するかどうか
	int _isDrawAura;

	// 回転軸
	int _rotType;
	// 回転値
	VECTOR _rotation;

	// スケーリングしているかどうか
	int _isScaling;
	// スケーリングのカウント
	int _scalingCnt;

	
};