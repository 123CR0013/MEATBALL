#pragma once	// .hの先頭に記述。#includeでこのファイルを何度読み込みしても、1度しか読み込まない
#include "SceneBase.h"

class SceneGameOver : public SceneBase {
public:
	SceneGameOver();			// コンストラクタ
	virtual ~SceneGameOver();	// デストラクタ

	void	Input() override;	// 入力
	void	Process() override;	// 計算
	void	Draw() override;	// 描画

	

	void MeatBallProcess();


	void MenuProcess();
	
	void FadeProcess();



	int GetIsRetry() { return _isRetry; };


protected:
	// リトライを選択したかどうか
	// 選択された場合、scenegamemain.cpp内でこのインスタンスを削除しゲームメインシーンに移る
	int _isRetry;


	//3Dモデル関連
	int _playerModel;

	VECTOR _playerPos;



	float  _viewRoute;
	VECTOR _CameraPos;




	int _floorModel;


	int _meatBallModel;
	VECTOR _meatBallPos;
	VECTOR _meatBallDefaultScale;

	int _faceFrameNo;



	// 動きの種類
	// 0 ~ 2 横揺れ
	// 3 ~ 6 ジャンプ
	int _moveCnt;


	// 横揺れ
	float _zTwist;
	int _twistOrder;
	int _twistCnt;

	// ジャンプ
	float _meatBallY;
	int _isJump;
	int _jumpCnt;


	//選択関連
	int use = 0;
	int setCnt = 0;	
	int noperation = 0;
	
	/*int sound_volume = 0;
	int volMusic = 150;*/

	int		_step;		// フェードインアウト用ステップ
	int		_stepcnt;	// ステップカウンタ



};

