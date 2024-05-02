#pragma once

#define STAGE_MAX 2

#define DOOR_MAX 100
#define JUMPPAD_MAX 50
#define LEVER_MAX 50
#define SPIKE_MAX 80

#define BOMB_MAX 50
#define DASH_PANEL_MAX 50


//#define GIMMICK_TYPE_MAX 2

#define GIMMICK_TYPE_DOOR 0
#define GIMMICK_TYPE_JUMPPAD 1

#define GIMMICK_TYPE_LEVER 2
#define GIMMICK_TYPE_BOMB 3
#define GIMMICK_TYPE_DASHPANEL 4



// ギミックの回転
// ギミックがくっついている面を指定
#define GIMMICK_ROTATION_FLOOR 0
#define GIMMICK_ROTATION_RIGHT_WALL 1
#define GIMMICK_ROTATION_CEILING 2
#define GIMMICK_ROTATION_LEFT_WALL 3



struct GIMMICK_DATA 
{
	// ギミックのID
	int id = -1;
	// マップチップ座標
	int x, y;
};


struct JUMPPAD_DATA
{
	// ギミックのID
	int id = -1;
	// マップチップ座標
	// 当たり判定矩形の左上のマップチップ座標
	int x, y;


	// 常に有効かどうか
	// 0 : レバー作動時のみ有効, 1: 常に有効
	int isAlwaysActive;
};


struct LEVER_DATA
{
	// マップチップ座標
	// 当たり判定矩形の左上のマップチップ座標
	int x = -1;
	int y;

	// 
	// 作動時にアクションさせるギミック
	// (DOOR_DATA, JUMPPAD_DATAと対応付ける)
	//
	
	// ギミックのタイプ
	// GIMMICK_TYPE_DOOR, GIMMICK_TYPE_JUMPPAD
	int gimmickType;
	// ギミックのid 
	int gimmickId;


	// 非作動状態に戻るかどうか
	// 0 : 戻らない , 1 : 戻る 
	int returnInactive;


	// 回転を指定
	// 定数(GIMMICK_ROTATION_〇〇)を使って指定
	int rotation = GIMMICK_ROTATION_FLOOR;
};


struct SPIKE_DATA
{
	// マップチップ座標
	int x = -1;
	int y;

	// ギミックの回転
	// ギミックがくっついている面を指定
	int rotation = GIMMICK_ROTATION_FLOOR;
};


GIMMICK_DATA GetDoorData(int stageNo, int index);
JUMPPAD_DATA GetJumpPadData(int stageNo, int index);
LEVER_DATA GetLeverData(int stageNo, int index);
SPIKE_DATA GetSpikeData(int stageNo, int index);

GIMMICK_DATA GetBombData(int stageNo, int index);
GIMMICK_DATA GetDashPanelData(int stageNo, int index);
