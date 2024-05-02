// 独自描画ライブラリ
#include "mydraw.h"

// 画像の中心で回転して描画する
void _MyDrawModiGraph(MATRIX mView, VECTOR position, float zoom, float angle, int width, int height, int cgHandle, int turn) {
	// 回転×拡縮×並行移動
	MATRIX mRot = MGetRotZ(angle);						// 2Dでx,yを回転させるには、3DならZ軸回転になる
	MATRIX mScale = MGetScale(VGet(zoom, zoom, 1));		// Z方向の倍率は1のまま
	MATRIX mTrans = MGetTranslate(position);
	MATRIX m;
	m = MMult(mRot, mScale);
	m = MMult(m, mTrans);

	m = MMult(m, mView);

	// 描画する画像の4つの頂点座標
	VECTOR pos[4] = {
		// 通常用（左上から右回り）
		VGet(-width / 2.0, -height / 2.0, 0),	// 左上
		VGet(width / 2.0, -height / 2.0 ,0),	// 右上
		VGet(width / 2.0,  height / 2.0 ,0),	// 右下
		VGet(-width / 2.0,  height / 2.0 ,0),	// 左下
	};
	if (turn != 0) {
		// 反転用
		VECTOR posTurn[4] = {
			// 反転用（右上から左回り）
			VGet(width / 2.0, -height / 2.0 ,0),	// 右上
			VGet(-width / 2.0, -height / 2.0, 0),	// 左上
			VGet(-width / 2.0,  height / 2.0 ,0),	// 左下
			VGet(width / 2.0,  height / 2.0 ,0),	// 右下
		};
		for (int i = 0; i < 4; i++) {
			pos[i] = posTurn[i];
		}
	}

	// 4つの頂点座標全てに行列を掛けて変換する
	for (int i = 0; i < 4; i++) {
		pos[i] = VTransform(pos[i], m);
	}
	// 変換した座標で描画する
	DrawModiGraph(
		pos[0].x, pos[0].y,
		pos[1].x, pos[1].y,
		pos[2].x, pos[2].y,
		pos[3].x, pos[3].y,
		cgHandle, true);
}


void MyDrawModiGraph(MATRIX mView, VECTOR position, float zoom, float angle, int width, int height, int cgHandle) {
	// 左右反転しない
	_MyDrawModiGraph(mView, position, zoom, angle, width, height, cgHandle, 0);
}

void MyDrawTurnModiGraph(MATRIX mView, VECTOR position, float zoom, float angle, int width, int height, int cgHandle) {
	// 左右反転する
	_MyDrawModiGraph(mView, position, zoom, angle, width, height, cgHandle, 1);
}





// 4点を指定して四角を描く
void MyDraw4PointBox(MATRIX mView, VECTOR pos[4], unsigned int Color, int FillFlag) {
	MATRIX m = mView;

	// 4つの頂点座標全てに行列を掛けて変換する
	for (int i = 0; i < 4; i++) {
		pos[i] = VTransform(pos[i], m);
	}

	// 塗りつぶしか？
	if (FillFlag == 0) {
		// 変換した座標で、線で四角を描画する
		DrawLine(
			pos[0].x, pos[0].y,	// 左上
			pos[1].x, pos[1].y,	// 右上
			Color);
		DrawLine(
			pos[1].x, pos[1].y,	// 右上
			pos[3].x, pos[3].y,	// 右下
			Color);
		DrawLine(
			pos[3].x, pos[3].y,	// 右下
			pos[2].x, pos[2].y,	// 左下
			Color);
		DrawLine(
			pos[2].x, pos[2].y,	// 左下
			pos[0].x, pos[0].y,	// 左上
			Color);
	}
	else {
		// 変換した座標で、三角形２つを描画する
		DrawTriangle(
			pos[0].x, pos[0].y,	// 左上
			pos[1].x, pos[1].y,	// 右上
			pos[2].x, pos[2].y,	// 左下
			Color, FillFlag);
		DrawTriangle(
			pos[1].x, pos[1].y,	// 右上
			pos[2].x, pos[2].y,	// 左下
			pos[3].x, pos[3].y,	// 右下
			Color, FillFlag);
	}
}

// 矩形を描く
void MyDrawBox(MATRIX mView, int x1, int y1, int x2, int y2, unsigned int Color, int FillFlag) {
	VECTOR pos[4] = {
		VGet(x1,		y1,		0),	// 左上
		VGet(x2 - 1,	y1,		0),	// 右上
		VGet(x1,		y2 - 1,	0),	// 左下
		VGet(x2 - 1,	y2 - 1,	0),	// 右下
	};
	MyDraw4PointBox(mView, pos, Color, FillFlag);
}

// 位置を揃えて文字列を描く
// posBaseX = -1:左揃え, 0:中央, 1:右揃え
void DrawPosString(int x, int y, int posBaseX, unsigned int color, const char* str) {
	int w = GetDrawStringWidth(str, strlen(str));	// 描画する文字の横サイズを得る
	if (posBaseX == 0) {
		// 中央揃えなのでx位置を左に半分ずらす
		x -= w / 2;
	}
	else if (posBaseX > 0) {
		// 右揃えなのでx位置を左にずらす
		x -= w;
	}
	DrawString(x, y, str, color);
}

