#pragma once
#include "DxLib.h"
#include "enemybase.h"


#define STAGE_MAX 2


#define ENEMY_TYPE_MAX 6
#define ENEMY_MAX 250

#define ENEMY_FLOAT_MAX			50
#define ENEMY_SHOOTING_MAX		50
#define ENEMY_GROUND_MAX		50
#define ENEMY_SHIELD_MAX		50
#define ENEMY_TRACER_MAX		50

#define ROUTE_MAX 10

#define ENGROUND_TYPE_NORMAL 0
#define ENGROUND_TYPE_METAL  1

#define ENFLOAT_TYPE_PATROL 0
#define ENFLOAT_TYPE_TRACE  1


#define ENSHIELD_TYPE_STAY  0
#define ENSHIELD_TYPE_MOVE 1

// 浮遊敵情報
struct FLOAT_DATA
{
	int size;
	VECTOR pos;
	VECTOR direction;
};

// 移動ルート情報
// 指定したマップチップ座標に触れたときに、移動方向を変更する
struct ENEMY_ROUTE
{
	// 移動方向を変更するマップチップ座標
	VECTOR pos = VGet(-1, -1, -1);
	// 変更後の移動方向
	VECTOR direction = VGet(0, 0, 0);
};

// 敵情報
struct ENEMY_DATA {
	int size = -1;
	VECTOR pos = VGet(-1, -1, -1);
	int type = 0;
};

// 縦情報
struct SHIELD {
	int use = 0;
	float r;
	int h;
	VECTOR centerPos;
	VECTOR direction;
};


FLOAT_DATA GetFloatData(int stageNo, int index);
ENEMY_ROUTE GetFloatRoute(int stageNo, int enemyIndex, int routeIndex);
ENEMY_DATA GetShootingData(int stageNo, int index);
ENEMY_DATA GetGroundData(int stageNo, int index);
ENEMY_DATA GetShieldData(int stageNo, int index);;
ENEMY_DATA GetTracerData(int stageNo, int index);


extern SHIELD shieldInfo[ENEMY_SHIELD_MAX];




#define SIZE_MAX 3

#define SMALL 0
#define MEDIUM 1
#define LARGE 2



// タイプ・サイズごとに, hp, modelSize, rectを指定する
// 2重配列 [type][size]
// type  0:float, 1:shooting, 2:ground, 3:shield
// size  0:small, 1:medium, 2:large
struct ENEMY_INFO {	
	// 体力
	int hp;
	// 3Dモデルのサイズ
	VECTOR modelSize;

	// 当たり判定のサイズ (配置座標からの差分x,y 当たり判定の大きさw,h)
	// 本体
	MYRECT difBodyRect;
	// 索敵範囲
	MYRECT difSearchRect;
	// 攻撃範囲
	MYRECT difAttackRect;
};

ENEMY_INFO GetEnemySizeInfo(int type, int size);