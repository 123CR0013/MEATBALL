#include "dashpanel.h"
#include "mapdata.h"
#include "resource.h"
#include "mydraw.h"

#define DASH_PANEL_ANIM_SPEED 8
#define DASH_PANEL_SHEETS_MAX 4

DASH_PANEL::DASH_PANEL(int index, int x, int y)
{
	_index = index;
	if (_index != -1) {
		_use = 1;
	}
	else {
		_use = 0;
	}

	_x = x;
	_y = y;



	_rect = { -1,-1,-1,-1 };
	InitCollision();

	_hitCount = 0;
	_animCnt = 0;

}

void DASH_PANEL::InitCollision()
{
	MYRECT difRect = { 0, 0, 64, 64 };
	int tmpX = _x * CHIPSIZE_W;
	int tmpY = _y * CHIPSIZE_H;

	_rect.x = tmpX + difRect.x;
	_rect.y = tmpY + difRect.y;
	_rect.w = difRect.w;
	_rect.h = difRect.h;
}


DASH_PANEL::~DASH_PANEL()
{
}


void DASH_PANEL::Process()
{
}



void DASH_PANEL::Draw(MATRIX mView)
{
	VECTOR world = VGet(_x * CHIPSIZE_W, _y * CHIPSIZE_H, 0);
	// 画像の中心をworld座標とするので、サイズ/2だけずらす
	world.x += CHIPSIZE_W / 2;
	world.y += CHIPSIZE_H / 2;
	MyDrawModiGraph(mView, world, 1.f, 0.f, CHIPSIZE_W, CHIPSIZE_H, cgDashPanel[(_animCnt / DASH_PANEL_ANIM_SPEED) % DASH_PANEL_SHEETS_MAX]);

	

	_animCnt++;
	if (_hitCount > 0) {
		_hitCount--;
		_animCnt += 3;
	}
	if (_animCnt > 160) {
		_animCnt = 0;
	}
}


