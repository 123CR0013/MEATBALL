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

	

	// “–‚½‚è”»’è‹éŒ`
	MYRECT _rect;				// “–‚½‚è”»’è‹éŒ`(¶ãÀ•W x,y / ‘å‚«‚³ w,h) 

	int _hitCount;
	int _animCnt;
};

