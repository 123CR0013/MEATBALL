// 独自計算ライブラリ
#include "mymath.h"

// degree→radian
float DegToRad(float deg) {
	return deg * PI / 180.0f;		// 角度をdegreeからradianに変換
}

// radian→degree
float RadToDeg(float rad) {
	return rad * 180.0f / PI;		// 角度をradianからdegreeに変換
}



// 当たり判定用。2つのboxが当たったかを判定
// 当たっていたら1, 当たっていなかったら0を返す
int IsHitBox(
	int x1, int y1, int w1, int h1,		// ひとつめのbox 左上(x,y), 大きさw,h
	int x2, int y2, int w2, int h2		// ふたつめのbox 左上(x,y), 大きさw,h
) {
	if (x1 < x2 + w2 && x2 < x1 + w1		// x方向の判定
		&& y1 < y2 + h2 && y2 < y1 + h1		// y方向の判定
		&& w1 != 0 && h1 != 0           // box1の大きさがある
		&& w2 != 0 && h2 != 0           // box2の大きさがある
		)
	{
		// 2つのboxは当たっている
		return 1;
	}

	// 2つのboxは当たっていない
	return 0;
}


// 当たり判定用。2つの円が当たったかを判定
// 当たっていたら1, 当たっていなかったら0を返す
int IsHitCircle(
	int x1, int y1, int r1,		// ひとつめのcircle 中心(x,y), 半径r
	int x2, int y2, int r2		// ふたつめのcircle 中心(x,y), 半径r
) {
	int w, h, r;
	w = x1 - x2;
	h = y1 - y2;
	r = r1 + r2;
	if (r * r > w * w + h * h) {
		return 1;
	}
	return 0;
}

// 当たり判定用。円と矩形が当たったかを判定
// 当たっていたら1, 当たっていなかったら0を返す
// 参考サイト: https://takap-tech.com/entry/2019/09/26/004257
int IsHitCircleAndBox(
	float cx, float cy, float r,	 // circle 中心(x,y), 半径r
	MYRECT box				  // box 左上(x, y), 大きさw, h
) {
	float left = box.x;
	float right = left + box.w;
	float top = box.y;
	float bottom = top + box.h;

	// どれかの条件に当てはまっていたら当たっている
	if (
		// 以下の2行で大まかな当たり判定を取る（矩形の頂点付近の当たり判定は取れない）
		((left < cx) && (cx < right) && (top < cy + r) && (cy - r < bottom)) ||
		((left < cx + r) && (cx - r < right) && (top < cy) && (cy < bottom)) ||

		// 矩形の頂点付近の当たり判定を取る
		((pow(left - cx, 2) + pow(top - cy, 2)) < pow(r, 2)) ||		// 左上
		((pow(right - cx, 2) + pow(top - cy, 2)) < pow(r, 2)) ||		// 右上
		((pow(left - cx, 2) + pow(bottom - cy, 2)) < pow(r, 2)) ||		// 左下
		((pow(right - cx, 2) + pow(bottom - cy, 2)) < pow(r, 2))			// 右下
		)
	{
		return 1;
	}
	return 0;
}


// 当たり判定用。カプセルと矩形が当たったかを判定
// 円の位置をずらしながら IsHitCircleAndBox繰り返し実行する
// 当たっていたら1, 当たっていなかったら0を返す
int IsHitCapsuleAndBox(
	float cx, float cy, float r,	 // カプセル 端の位置(x,y), 半径r
	VECTOR direction,					 // カプセルを伸ばす方向
	float capsuleH,					 // カプセルの縦の長さ
	MYRECT box							 //box 左上(x,y), 大きさw,h
) {
	float x = cx;
	float y = cy;
	float length = r * 2;

	// カプセルの開始位置の計算
	// 1つ目の円の位置をcx,cyから半円分ずらす
	VECTOR vDir = VNorm(direction);
	VECTOR vTmp = VScale(vDir, r);
	x += vTmp.x;
	y += vTmp.y;

	// ずらす幅を計算
	MATRIX m = MGetScale(VGet(r / 4, r / 4, 0)); // 割る数字は任意(3より大きくないとカプセルの形状にならない)
	vTmp = VTransform(vDir, m);

	// 円をvDir方向に少しずつずらしながら, 円と矩形の当たり判定を行う
	while (length < capsuleH) {
		if (IsHitCircleAndBox(x, y, r, box) == 1) {
			return 1;
		}
		x += vTmp.x;
		y += vTmp.y; 
		//y -= vTmp.y;	 
		length += VSize(vTmp);
	}

	// 上のループだと、当たり判定用のカプセルの長さがcapsuleHに満たないことがある
	// 足りない分の当たり判定
	length -= length - capsuleH;
	VECTOR v = VGet(x, y, 0);
	v = VSub(v, VScale(vDir, length - capsuleH));

	if (IsHitCircleAndBox(v.x, v.y, r, box) == 1) {
		return 1;
	}


	return 0;
}


// 矩形同士の当たり判定をとった後に、押し出し方向が縦か横かを判定する
// 戻り値 ： 0 横方向, 1 縦方向
// ただし、当たり判定の重なった部分の横幅と縦幅が同じになった場合はうまく判定することができない
int CheckExtrusionDirection(MYRECT rect1, MYRECT rect2)
{
	float sx, sy, ex, ey;
	sx = rect1.x; if (sx < rect2.x) { sx = rect2.x; }
	sy = rect1.y; if (sy < rect2.y) { sy = rect2.y; }
	ex = rect1.x + rect1.w; if (ex > rect2.x + rect2.w) { ex = rect2.x + rect2.w; }
	ey = rect1.y + rect1.h; if (ey > rect2.y + rect2.h) { ey = rect2.y + rect2.h; }
	float w, h;
	w = ex - sx;
	h = ey - sy;


	

	if (w > h) {
		return 1;
	}

	if (w < h) {
		return 0;
	}

	if (w == h) {
		return 2;
	}






	return -1;
}

#define TwoPI   (PI * 2.0f)
#define PIOver2 (PI / 2.0f)

// イージング
float EasingLinear(float cnt, float start, float end, float frames) {
	return (end - start) * cnt / frames + start;
}
float EasingInQuad(float cnt, float start, float end, float frames) {
	cnt /= frames;
	return (end - start) * cnt * cnt + start;
}
float EasingOutQuad(float cnt, float start, float end, float frames) {
	cnt /= frames;
	return -(end - start) * cnt * (cnt - 2) + start;
}
float EasingInOutQuad(float cnt, float start, float end, float frames) {
	cnt /= frames / 2.0;
	if (cnt < 1) {
		return (end - start) / 2.0 * cnt * cnt + start;
	}
	cnt--;
	return -(end - start) / 2.0 * (cnt * (cnt - 2) - 1) + start;
}
float EasingInCubic(float cnt, float start, float end, float frames) {
	cnt /= frames;
	return (end - start) * cnt * cnt * cnt + start;
}
float EasingOutCubic(float cnt, float start, float end, float frames) {
	cnt /= frames;
	cnt--;
	return (end - start) * (cnt * cnt * cnt + 1) + start;
}
float EasingInOutCubic(float cnt, float start, float end, float frames) {
	cnt /= frames / 2.0;
	if (cnt < 1) {
		return (end - start) / 2.0 * cnt * cnt * cnt + start;
	}
	cnt -= 2;
	return (end - start) / 2.0 * (cnt * cnt * cnt + 2) + start;
}
float EasingInQuart(float cnt, float start, float end, float frames) {
	cnt /= frames;
	return (end - start) * cnt * cnt * cnt * cnt + start;
}
float EasingOutQuart(float cnt, float start, float end, float frames) {
	cnt /= frames;
	cnt--;
	return -(end - start) * (cnt * cnt * cnt * cnt - 1) + start;
}
float EasingInOutQuart(float cnt, float start, float end, float frames) {
	cnt /= frames / 2.0;
	if (cnt < 1) {
		return (end - start) / 2.0 * cnt * cnt * cnt * cnt + start;
	}
	cnt -= 2;
	return -(end - start) / 2.0 * (cnt * cnt * cnt * cnt - 2) + start;
}
float EasingInQuint(float cnt, float start, float end, float frames) {
	cnt /= frames;
	return (end - start) * cnt * cnt * cnt * cnt * cnt + start;
}
float EasingOutQuint(float cnt, float start, float end, float frames) {
	cnt /= frames;
	cnt--;
	return (end - start) * (cnt * cnt * cnt * cnt * cnt + 1) + start;
}
float EasingInOutQuint(float cnt, float start, float end, float frames) {
	cnt /= frames / 2.0;
	if (cnt < 1) {
		return (end - start) / 2.0 * cnt * cnt * cnt * cnt * cnt + start;
	}
	cnt -= 2;
	return (end - start) / 2.0 * (cnt * cnt * cnt * cnt * cnt + 2) + start;
}
float EasingInSine(float cnt, float start, float end, float frames) {
	return -(end - start) * cos(cnt / frames * (PIOver2)) + end + start;
}
float EasingOutSine(float cnt, float start, float end, float frames) {
	return (end - start) * sin(cnt / frames * PIOver2) + start;
}
float EasingInOutSine(float cnt, float start, float end, float frames) {
	return -(end - start) / 2.0 * (cos(PI * cnt / frames) - 1) + start;
}
float EasingInExpo(float cnt, float start, float end, float frames) {
	return (end - start) * pow(2.0, 10 * (cnt / frames - 1)) + start;
}
float EasingOutExpo(float cnt, float start, float end, float frames) {
	return (end - start) * (-pow(2.0, -10 * cnt / frames) + 1) + start;
}
float EasingInOutExpo(float cnt, float start, float end, float frames) {
	cnt /= frames / 2.0;
	if (cnt < 1) {
		return (end - start) / 2.0 * pow(2.0, 10 * (cnt - 1)) + start;
	}
	cnt--;
	return (end - start) / 2.0 * (-pow(2.0, -10 * cnt) + 2) + start;
}
float EasingInCirc(float cnt, float start, float end, float frames) {
	cnt /= frames;
	return -(end - start) * (sqrt(1 - cnt * cnt) - 1) + start;
}
float EasingOutCirc(float cnt, float start, float end, float frames) {
	cnt /= frames;
	cnt--;
	return (end - start) * sqrt(1 - cnt * cnt) + start;
}
float EasingInOutCirc(float cnt, float start, float end, float frames) {
	cnt /= frames / 2.0;
	if (cnt < 1) {
		return -(end - start) / 2.0 * (sqrt(1 - cnt * cnt) - 1) + start;
	}
	cnt -= 2;
	return (end - start) / 2.0 * (sqrt(1 - cnt * cnt) + 1) + start;
}

float EasingOutElastic(float cnt, float start, float end, float frames)
{
	if (cnt == 0) {
		return start;
	}
	else if (cnt == frames) {
		return end;
	}
	else {


		float time = cnt / frames;
		float rad = (2.0f * PI) / 3.0f;


		float rate = pow(2, -10 * time) * sin((time * 10 - 0.75) * rad) + 1;

		return  start + (end - start) * rate;
	} 
}


// 範囲内を返す
float Clamp(float min_n, float max_n, float n) {
	if (n <= min_n) { return min_n; }
	if (n >= max_n) { return max_n; }
	return n;
}



// 線分と矩形の当たり判定
// 0:当たっていない, 1:当たった
int IsHitRectAndLine(MYRECT rect, VECTOR lineStartPos, VECTOR lineEndPos)
{

	// 線分の始点と終点を基に作成した矩形内にいるかを判定する
	/*この部分をコメントアウトすると直線と矩形の当たり判定をとれる*/
	float left, right, top, bottom;
	if (lineEndPos.x - lineStartPos.x > 0) {
		left = lineStartPos.x;
		right = lineEndPos.x;
	}
	else {
		left = lineEndPos.x;
		right = lineStartPos.x;
	}
	if (lineEndPos.y - lineStartPos.y > 0) {
		top = lineStartPos.y;
		bottom = lineEndPos.y;
	}
	else {
		top = lineEndPos.y;
		bottom = lineStartPos.y;
	}
	if (rect.x + rect.w < left
		|| right < rect.x
		|| rect.y + rect.h < top
		|| bottom < rect.y
		) {
		return 0;
	}
	/*ここまで*/


	VECTOR line = VSub(lineEndPos, lineStartPos);
	MATRIX m = MGetRotZ(90.f * DX_PI_F / 180.f);
	line = VTransform(line, m);

	VECTOR rectPos[4] = {
		VGet(rect.x, rect.y, 0),
		VGet(rect.x + rect.w, rect.y, 0),
		VGet(rect.x + rect.w, rect.y + rect.h, 0),
		VGet(rect.x, rect.y + rect.h, 0)
	};


	VECTOR c[4];
	for (int i = 0; i < 4; i++) {
		c[i] = VSub(lineStartPos, rectPos[i]);
	}

	float dot[4];
	for (int i = 0; i < 4; i++) {
		dot[i] = VDot(line, c[i]);
	}

	if (dot[0] * dot[1] <= 0
		|| dot[1] * dot[2] <= 0
		|| dot[2] * dot[3] <= 0
		) {
		return 1;
	}



	return 0;
}
//カメラ回転用
static const float ROTATE_SPEED = DX_PI_F / 90;//回転スピード

// (x,y)の点を(mx,my)を中心にang角回転する
void rotate(float* x, float* y, const float ang, const float mx, const float my) {
	const float ox = *x - mx, oy = *y - my;
	*x = ox * cos(ang) + oy * sin(ang);
	*y = -ox * sin(ang) + oy * cos(ang);
	*x += mx;
	*y += my;
}
