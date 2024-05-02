#pragma once	// .hの先頭に記述。#includeでこのファイルを何度読み込みしても、1度しか読み込まない
#include "DxLib.h"		// VECTORやMATRIXを定義として使うため、.hに入れておく
#include "mymath.h"
#include "mapdata.h"



struct CamPhase
{
	int x = -1;
	int y;
	int phase;
};




class Camera {
public:
	Camera();
	~Camera();

	// ステージ開始時の処理
	void Init(int stageNo);

	void	 Process(VECTOR playerPos);
	MATRIX	GetViewMatrix() { return _mView; }


	void CheckPhase(VECTOR pos);
	void AdjustCam();

	// リスポーン時の処理
	void SetRespawn(int respawnPhase, VECTOR respawnPos);

	// デバッグ情報の表示
	void DrawDebugInfo();

public:
	int _camControl;	// カメラ操作するなら1
	MATRIX	_mView;		// View行列
	VECTOR	_pos;		// カメラの位置。world座標で
	VECTOR	_lookat;	// カメラの注視点。画面のここを中心に、拡大/回転をする
	float	_scale;		// カメラの拡大率
	float	_rotate;	// カメラの回転（degree)

	MYRECT	_rcLimit;	// world座標でリミット設定




	int _stageNo;

	int _phase;
	int _isFollowingPlayer;
	float _adjustValueY;


	// 非追従モードから追従モードに移行中かどうか
	int _isChangingFollowMode;



	// リスポーン処理を行うかどうか
	int _isRespawn;


	// BGMのボリューム
	int _volume;
};
extern CamPhase camPhaseChip[STAGE_MAX][175];