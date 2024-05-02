#include "DXLib.h"
#include "effect.h"
#include "resource.h"
#include "mydraw.h"


EFFECT effect[EFFECT_MAX];

// 初期化処理
void EffectInit() {
	for (int i = 0; i < EFFECT_MAX; i++) {
		effect[i].use = 0;
		effect[i].type = -1;

		effect[i].x = 0;
		effect[i].y = 0;
		effect[i].w = 0;
		effect[i].h = 0;

		effect[i].cnt = 0;
		effect[i].sheets = 0;
		effect[i].speed = 0;

		effect[i].loop = 0;
	}
}

// エフェクトをセットし、その要素番号を返す
// エフェクトを任意のタイミングで消したい場合に配列番号が必要になる
int SetEffect(int type, int x, int y, int w, int h, int loop) {
	int i;

	for (i = 0; i < EFFECT_MAX; i++) {
		// 使用していない配列要素にエフェクトをセットする
		if (effect[i].use == 0) {
			effect[i].use = 1;
			effect[i].type = type;

			effect[i].x = x;
			effect[i].y = y;
			effect[i].w = w;
			effect[i].h = h;

			effect[i].cnt = 0;


			effect[i].loop = loop;

			switch (effect[i].type)
			{
			case EFFECT_TYPE_DEFEAT_MACHINE:
				effect[i].sheets = 16;
				effect[i].speed = 4;
				break;
			case EFFECT_TYPE_DEFEAT_CREATURE:
				effect[i].sheets = 25;
				effect[i].speed = 2;
				break;
			case EFFECT_TYPE_BULLET_HIT:
				effect[i].sheets = 17;
				effect[i].speed = 4;
				break;

			case EFFECT_TYPE_BULLET_CHARGE:
				effect[i].sheets = 13;
				effect[i].speed = 3;
				break;

			case EFFECT_TYPE_CHIP_BREAK:
				effect[i].sheets = 25;
				effect[i].speed = 4;
				break;

			case EFFECT_TYPE_BOSS_BULLET_HIT:
				effect[i].sheets = 16;
				effect[i].speed = 4;
				break;

			case EFFECT_TYPE_SMOKE:
				effect[i].sheets = 20;
				effect[i].speed = 1;
				break;
			case EFFECT_TYPE_SMOKE_COLOR:
				effect[i].sheets = 20;
				effect[i].speed = 1;
				break;

			case EEFECT_TYPE_MEATBALL_RETURN:
				effect[i].sheets = 13;
				effect[i].speed = 1;
				break;
			}

			break;
		}
	}

	return i;
}


// 計算処理
void EffectProcess() {
	for (int i = 0; i < EFFECT_MAX; i++) {
		if (effect[i].use == 1) {
			// カウントを増やす
			effect[i].cnt++;
			
			// エフェクトの描画が終了したら use = 0 にする
			if (effect[i].cnt > effect[i].speed * effect[i].sheets - 1) {
				effect[i].cnt = 0;

				// ループ再生をしない場合のみ use = 0 にする
				if (effect[i].loop == 0) {
					effect[i].use = 0;
				}
			}
		}
	}
}

// エフェクトを停止する
// SetEffectで取得した要素番号を使用する
void StopEffect(int index)
{
	effect[index].use = 0;
}


// 背面レイヤーへの描画
void DrawBackEffect(MATRIX mView)
{
	for (int i = 0; i < EFFECT_MAX; i++) {
		if (effect[i].use == 1) {
			VECTOR effectPos = VGet(effect[i].x, effect[i].y, 0);
			effectPos = VTransform(effectPos, mView);
			switch (effect[i].type)
			{
			case EFFECT_TYPE_BULLET_CHARGE:
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 216.0f);
				MyDrawModiGraph(mView, VGet(effect[i].x + effect[i].w / 2, effect[i].y + effect[i].h / 2, 0), 1.0f, 0.0f, effect[i].w, effect[i].h, cgEffect[effect[i].type][(effect[i].cnt / effect[i].speed) % effect[i].sheets]);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				break;
			}
		}
	}
}


// 前面レイヤーへの描画
void DrawFrontEffect(MATRIX mView)
{
	for (int i = 0; i < EFFECT_MAX; i++) {
		if (effect[i].use == 1) {
			VECTOR effectPos = VGet(effect[i].x, effect[i].y, 0);
			effectPos = VTransform(effectPos, mView);
			switch (effect[i].type)
			{
				// 射撃敵のチャージエフェクトは敵の裏に描画する
			case EFFECT_TYPE_BULLET_CHARGE:
				break;
			case EFFECT_TYPE_SMOKE_COLOR:
				//SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
				SetDrawBright(rand() % 256, rand() % 256, rand() % 256);
				MyDrawModiGraph(mView, VGet(effect[i].x + effect[i].w / 2, effect[i].y + effect[i].h / 2, 0), 1.0f, 0.0f, effect[i].w, effect[i].h, cgEffect[EFFECT_TYPE_SMOKE][(effect[i].cnt / effect[i].speed) % effect[i].sheets]);
				SetDrawBright(255, 255, 255);
				//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				break;


			case EEFECT_TYPE_MEATBALL_RETURN:
				MyDrawModiGraph(mView, VGet(effect[i].x, effect[i].y, 0), 1.0f, 0.0f, effect[i].w, effect[i].h, cgEffect[effect[i].type][(effect[i].cnt / effect[i].speed) % effect[i].sheets]);
				break;


			default:
				MyDrawModiGraph(mView, VGet(effect[i].x + effect[i].w / 2, effect[i].y + effect[i].h / 2, 0), 1.0f, 0.0f, effect[i].w, effect[i].h, cgEffect[effect[i].type][(effect[i].cnt / effect[i].speed) % effect[i].sheets]);
				break;
			}
		}
	}
}
