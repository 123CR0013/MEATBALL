#pragma once	// .hの先頭に記述。#includeでこのファイルを何度読み込みしても、1度しか読み込まない
#include <math.h>
#include "DxLib.h"

// 円周率
#define	PI				(3.141592653589793)

// degree→radian
float DegToRad(float deg);

// radian→degree
float RadToDeg(float rad);


// RECT用構造体
struct MYRECT {
	float		x, y, w, h;
};

// 当たり判定用。2つのboxが当たったかを判定
// 当たっていたら1, 当たっていなかったら0を返す
int IsHitBox(
	int x1, int y1, int w1, int h1,		// ひとつめのbox 左上(x,y), 大きさw,h
	int x2, int y2, int w2, int h2		// ふたつめのbox 左上(x,y), 大きさw,h
);


// 当たり判定用。2つの円が当たったかを判定
// 当たっていたら1, 当たっていなかったら0を返す
int IsHitCircle(
	int x1, int y1, int r1,		// ひとつめのcircle 中心(x,y), 半径r
	int x2, int y2, int r2		// ふたつめのcircle 中心(x,y), 半径r
);

// 当たり判定用。円と矩形が当たったかを判定
// 当たっていたら1, 当たっていなかったら0を返す
int IsHitCircleAndBox(
	float cx, float cy, float r,	 // circle 中心(x,y), 半径r
	MYRECT box				 // box 左上(x, y), 大きさw, h
);


// 当たり判定用。カプセルと矩形が当たったかを判定
// 円の位置をずらしながら IsHitCircleAndBox繰り返し実行する
// 当たっていたら1, 当たっていなかったら0を返す
int IsHitCapsuleAndBox(
	float cx, float cy, float r,	 // circle 中心(x,y), 半径r
	VECTOR direction,					 // カプセルを伸ばす方向
	float capsuleH,					 // カプセルの縦の長さ
	MYRECT box							  // box 左上(x, y), 大きさw, h
);





// 矩形同士の当たり判定をとった後に、押し出し方向が縦か横かを判定する
// 戻り値 ： 0 横方向, 1 縦方向
int CheckExtrusionDirection(MYRECT rect1, MYRECT rect2);

// 様々なイージング
// https://game-ui.net/?p=835 を参考に
float EasingLinear(float cnt, float start, float end, float frames);
float EasingInQuad(float cnt, float start, float end, float frames);
float EasingOutQuad(float cnt, float start, float end, float frames);
float EasingInOutQuad(float cnt, float start, float end, float frames);
float EasingInCubic(float cnt, float start, float end, float frames);
float EasingOutCubic(float cnt, float start, float end, float frames);
float EasingInOutCubic(float cnt, float start, float end, float frames);
float EasingInQuart(float cnt, float start, float end, float frames);
float EasingOutQuart(float cnt, float start, float end, float frames);
float EasingInOutQuart(float cnt, float start, float end, float frames);
float EasingInQuint(float cnt, float start, float end, float frames);
float EasingOutQuint(float cnt, float start, float end, float frames);
float EasingInOutQuint(float cnt, float start, float end, float frames);
float EasingInSine(float cnt, float start, float end, float frames);
float EasingOutSine(float cnt, float start, float end, float frames);
float EasingInOutSine(float cnt, float start, float end, float frames);
float EasingInExpo(float cnt, float start, float end, float frames);
float EasingOutExpo(float cnt, float start, float end, float frames);
float EasingInOutExpo(float cnt, float start, float end, float frames);
float EasingInCirc(float cnt, float start, float end, float frames);
float EasingOutCirc(float cnt, float start, float end, float frames);
float EasingInOutCirc(float cnt, float start, float end, float frames);


float  EasingOutElastic(float cnt, float start, float end, float frames);





// 範囲内を返す
float Clamp(float min_n, float max_n, float n);



// 線分と矩形の当たり判定
// 0:当たっていない, 1:当たった
int IsHitRectAndLine(MYRECT rect, VECTOR lineStartPos, VECTOR lineEndPos);