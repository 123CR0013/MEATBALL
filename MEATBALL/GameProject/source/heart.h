#pragma once
#include "DxLib.h"
#include "gamemain.h"

// ハートがプレイヤーから飛び出してから消えるまでの時間
#define HEART_COUNT_MAX 10 * 60 // 10秒

class Heart
{
public:
	Heart();
	~Heart();

	// 計算処理
	void Process();
	// 描画位置の補正処理
	void AdjustDrawPos(MATRIX mView);
	// 描画処理
	void Draw();

	// ワールド座標を更新する
	void UpdateWorldPos();

	void SetUse(int n);
	int  GetUse() { return _use; };

	int GetLost() { return _isLost; };

	void SetPos(VECTOR playerPos);
	void SetDirection(int dirX) { _direction = dirX; };

	void UpdateBodyRect();
	MYRECT GetBodyRect() { return _bodyRect; };

	int GetRemainCnt() { return _remainCount; };

	// デバッグ情報の表示
	void DrawDebugInfo(MATRIX mView);

	// 前面レイヤーへの描画
	void DrawFrontLayer();

private:
	// モデルハンドル
	int _modelHandle;

	// 飛び出しているかどうか
	int _use;
	// 失うまでの残りカウント
	int _remainCount;
	// 失ったかどうか
	int _isLost;

	int danger[2];
	int dangerCnt;

	// 座標系は1つのキャラに対して、3つ用意する
	VECTOR _pos;		// 起点位置（当たり判定, 移動処理に使う） 
	VECTOR _screenPos;	// スクリーン座標（_posにカメラの補正をかけたもの）
	VECTOR _worldPos;	// ワールド座標（_screenPosを3Dモデル描画用の座標に変換したもの）

	// x軸の移動方向
	int _direction;

	// y軸方向の移動速度
	float _gravity;

	// 移動速度
	float _speed;					
	
	// 体の当たり判定矩形
	// 他プレイヤーや風との当たり判定に使う
	MYRECT _difBodyRect;				// キャラ当たり判定(配置座標からの差分 x,y / 大きさ w,h)
	MYRECT _bodyRect;									// 当たり判定矩形(左上座標 x,y / 大きさ w,h) 


};