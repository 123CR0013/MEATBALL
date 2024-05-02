#pragma once
#include "Dxlib.h"
#include "mymath.h"


// （爆弾がある座標を除いた）爆発範囲の半分
// 9×9マス
// （BOMB_RANGE_HARF_W + 1 + BOMB_RANGE_HARF_W）×（BOMB_RANGE_HARF_H + 1 + BOMB_RANGE_HARF_H）
#define BOMB_RANGE_HARF_W 4
#define BOMB_RANGE_HARF_H 4


#define BOMB_COLLISION_TYPE_BODY 0
#define BOMB_COLLISION_TYPE_EXPLOSION 1


#define BOMB_COUNT_MAX 30


class BOMB
{
public:
	BOMB(int index, int x, int y);
	~BOMB();

	void Process();
	void Draw(MATRIX mView);


	void InitCollision();

	void Delete();
	int GetUse() { return _use; };
	void SetExplosion() { _isExplosion = 1; };
	int GetExplosion() { return _isExplosion; };

	MYRECT GetRect(int type);


	int GetPosX() { return _x; };
	int GetPosY() {		return _y;};


	void ExplodeChip();


	void SetCountDown() { _count = BOMB_COUNT_MAX; };



	void DrawDebugInfo(MATRIX mView);


protected:
	int _index;
	int _use;

	int _x, _y;

	int _isExplosion;

	int _count;

	// 当たり判定矩形
	MYRECT _rect;				// 当たり判定矩形(左上座標 x,y / 大きさ w,h) 


	MYRECT _explosionRect;				// 当たり判定矩形(左上座標 x,y / 大きさ w,h) 


	int _animCnt;
	int _animOrder;

};

