#pragma once
#define EFFECT_MAX	50 //EFFECT構造体配列の最大要素数

// 撃破エフェクト
#define EFFECT_TYPE_DEFEAT_MACHINE 0
#define EFFECT_TYPE_DEFEAT_CREATURE 1
// 射撃敵 弾エフェクト
#define EFFECT_TYPE_BULLET_HIT 2
#define EFFECT_TYPE_BULLET_CHARGE 3
// チップ破壊エフェクト
#define EFFECT_TYPE_CHIP_BREAK 4

// ボス 弾エフェクト
#define EFFECT_TYPE_BOSS_BULLET_HIT 5
// ミートボール電撃エフェクト
#define EFFECT_TYPE_MEATBALL_ELECTRO 6

// 煙エフェクト
// プレイヤー: ジャンプ
// プレイヤー・ボス: 着地
// ミートボール: 反射
#define EFFECT_TYPE_SMOKE 7



// ミートボールを呼び戻した後に出るエフェクト
#define EEFECT_TYPE_MEATBALL_RETURN 8


// ミートボールと盾が当たったときの煙エフェクト
// EFFECT_TYPE_SMOKEの画像を色を変えて描画する
#define EFFECT_TYPE_SMOKE_COLOR 9


struct EFFECT
{
	int use;		// この配列要素を使用しているか

	int type;	// エフェクトの種類

	int x, y;		// エフェクトを再生する座標
	int w, h; // 表示サイズ

	int cnt;		// アニメーションカウント
	int sheets;	// 画像の枚数
	int speed;	// エフェクトの再生速度（1枚あたりの表示フレーム数）
	
	int loop; // ループ再生するか
};

// 初期化処理
void EffectInit();

// エフェクトをセットし、その要素番号を返す
// エフェクトを任意のタイミングで消したい場合に配列番号が必要になる
//int SetEffect(int x, int y, int type, int loop, int w = 0, int h = 0);
int SetEffect(int type, int x, int y, int w, int h, int loop = 0);


// 計算処理
void EffectProcess();

// エフェクトを停止する
// SetEffectで取得した要素番号を使用する
void StopEffect(int index);



// 背面レイヤーへの描画
void DrawBackEffect(MATRIX mView);



// 前面レイヤーへの描画
void DrawFrontEffect(MATRIX mView);