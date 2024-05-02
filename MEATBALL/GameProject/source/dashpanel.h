#pragma once
#pragma once
#include "Dxlib.h"
#include "mymath.h"

class DASH_PANEL
{
public:
	DASH_PANEL(int index, int x, int y);
	~DASH_PANEL();

	void Process();
	void Draw(MATRIX mView);

	void InitCollision();

	int GetUse() { return _use; };

	MYRECT GetRect() { return _rect; };


	int GetPosX() { return _x; };
	int GetPosY() { return _y; };


	void SetHit() { _hitCount = 120; }


	void DrawDebugInfo(MATRIX mView);


protected:
	int _index;
	int _use;

	int _x, _y;

	

	// �����蔻���`
	MYRECT _rect;				// �����蔻���`(������W x,y / �傫�� w,h) 

	int _hitCount;
	int _animCnt;
};

