#pragma once
#include "Dxlib.h"
#include "mymath.h"


// �i���e��������W���������j�����͈͂̔���
// 9�~9�}�X
// �iBOMB_RANGE_HARF_W + 1 + BOMB_RANGE_HARF_W�j�~�iBOMB_RANGE_HARF_H + 1 + BOMB_RANGE_HARF_H�j
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

	// �����蔻���`
	MYRECT _rect;				// �����蔻���`(������W x,y / �傫�� w,h) 


	MYRECT _explosionRect;				// �����蔻���`(������W x,y / �傫�� w,h) 


	int _animCnt;
	int _animOrder;

};

