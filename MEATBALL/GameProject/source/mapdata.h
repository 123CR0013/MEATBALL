#pragma once	// .hの先頭に記述。#includeでこのファイルを何度読み込みしても、1度しか読み込まない
#include "DxLib.h"		// VECTORやMATRIXを定義として使うため、.hに入れておく
#include "mymath.h"
#include <vector>

// チップのサイズ
#define	CHIPSIZE_W		(64)
#define	CHIPSIZE_H		(64)

// マップのサイズ
#define	MAPSIZE_W		(425)
#define	MAPSIZE_H		(80)

// ステージ数
#define	STAGE_MAX	(2)

#define LAYER_MAX       (3)


#define LAYER_BACKMAP  0
#define LAYER_MAP      1
#define LAYER_LIGHT    2
//#define LAYER_FRONT    3



// 斜めのマップチップ
#define LOWER_RIGHT 10
#define LOWER_LEFT 11
#define UPPER_RIGHT 12
#define UPPER_LEFT 13


// 壊せるブロック
#define BREAK_CHIP_1 18
#define BREAK_CHIP_2 19
#define BREAK_CHIP_3 20
#define BREAK_CHIP_4 21





struct HITCHIP {
	int chip_no; // 当たったマップチップの番号（当たっていない場合は-1）
	int x, y;		 // 当たったマップチップの座標
};

class MapData {
public:
	MapData();			// コンストラクタ
	virtual ~MapData();	// デストラクタ

	void MapInit(int stageNo);
	//void MapChiipInit(int stageNo);






	void Process();

	// マップチップ描画
	void	 Draw(MATRIX mView);
	// 背景レイヤー描画（背景・背景チップ）
	void	 DrawBG(MATRIX mView);
	// 前面レイヤー描画（光闇）
	void DrawFrontLayer(MATRIX mView); 

	// 当たり判定関連
	int		CheckHitMapChip(int x, int y);	// 指定位置のチップが当たり判定ありか？
	int		CheckHitChip(int chip_no);		// 指定のチップIDが当たり判定ありか？
	int IsHitMapchip(float x, float y);
	HITCHIP IsHitBoxAndMapChip(MYRECT rect);

	void BreakMapChip(HITCHIP hitChip);

	void SetGimmickChip();
	void VanishDoor(int gimmickID);






	// worldのサイズを取得(pixel単位)
	int	GetWorldSizeW() { return MAPSIZE_W * CHIPSIZE_W; }
	int	GetWorldSizeH() { return MAPSIZE_H * CHIPSIZE_H; }









	// ボス戦用
	void StartBossBattle();
	void SetBossBarrier(VECTOR barrierPos);
	void VanishBossBarrier();
	void BossBattleProcess();






	// デバッグ情報の表示
	void DrawDebugInfo(MATRIX mView);

protected:
	int _gMapData[LAYER_MAX][MAPSIZE_W * MAPSIZE_H] = { 0 };

	//int	_cgChip[64];		// チップ画像


	int _stageNo;

	int _gimmickID;
	int _vanishCnt;


	int _startBossCnt = 0;
	int _bossBattle;
	int _isSetBossBarrier;
	int _barrierCnt;
	std::vector<VECTOR> _barrierChip;
};

// マップデータ
extern MapData	gMap;