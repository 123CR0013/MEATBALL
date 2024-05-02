#pragma once
#include "Dxlib.h"
#include "mymath.h"
#include "mapdata.h"
#include "gamemain.h"

#include "charabase.h"




class Gimmick
{
public:
	Gimmick();
	virtual ~Gimmick();
	// 描画位置の補正処理
	virtual void AdjustDrawPos(MATRIX mView);
	virtual void Draw();

	int GetUse() { return _use; };


	// 3Dモデル表示用座標の更新
	void UpdateWorldPos();





	virtual void ModelInit(int modelHandle, VECTOR size, int motionMax, int stageNo = -1);
	void MotionProcess(int motionNum_old);


	virtual void SetCollision();

	MYRECT GetRect() { return _rect; };
	VECTOR GetPos() { return _pos; };


	virtual void DrawDebugInfo(MATRIX mView);

protected:
	int _index;

	int _use;



	int _modelHandle;
	int _motionNum;
	int _playTime;

	CHARA_ANIM _anim[5];


	int _rotation;


	VECTOR _pos;
	VECTOR _screenPos;
	VECTOR _worldPos;


	// 当たり判定矩形
	// ミートボールとの当たり判定に使う
	MYRECT _difRect;				// 配置座標からの差分 (x,y / 大きさ w,h)
	MYRECT _rect;				// 当たり判定矩形(左上座標 x,y / 大きさ w,h) 
};

