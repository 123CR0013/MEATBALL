#include "heart.h"
#include "scenegamemain.h"
#include "resource.h"
#include "effect.h"


Heart::Heart()
{
	_modelHandle = MV1LoadModel("res/model/heart/heart.mv1");
	danger[0] = LoadGraph("res/UI/noheart_00.png");
	danger[1] = LoadGraph("res/UI/noheart_01.png");
	dangerCnt = 0;

	MV1SetScale(_modelHandle, VGet(2.3f, 2.3f, 2.3f));

	_use = 0;

	_remainCount = HEART_COUNT_MAX;
	_isLost = 0;

	_pos = VGet(0, 0, 0);
	_screenPos = VGet(0, 0, 0);
	_worldPos = VGet(0, 0, 0);

	_direction = 1;

	_gravity = 0;
	_speed = 8;

	_difBodyRect = { -32, -64, 64, 64 };
	_bodyRect = { -1, -1, -1, -1 };
}

Heart::~Heart()
{
	if (_modelHandle != -1) {
		MV1DeleteModel(_modelHandle);
		_modelHandle = -1;
	}

	for (int i = 0; i < 2; i++) {
		if (danger[i] != -1) {
			DeleteGraph(danger[i]);
			danger[i] = -1;
		}
	}
}

// 計算処理
void Heart::Process()
{
	// ハートが飛び出している状態
	if (_use == 1) {
		dangerCnt++;
		// 画面外に出ないように
		// x軸の反射
		if (_pos.x - _difBodyRect.w / 2 < gCam._pos.x - SCREEN_W / 2) {
			_direction = 1;
		}
		else if (gCam._pos.x + SCREEN_W / 2 < _pos.x + _difBodyRect.w / 2) {
			_direction = -1;
		}


		// y軸方向の移動処理
		_gravity += 1;			// キャラの、重力による加速値を大きくする
		_pos.y += _gravity;	// 重力による加速値の分だけ移動する



		// マップチップとの当たり判定
		// y軸方向の当たり判定
		MYRECT tmpRect = _bodyRect;
		tmpRect.y += _gravity;
		HITCHIP hitChip = gMap.IsHitBoxAndMapChip(tmpRect);
		// 当たっている場合、移動方向を変更する
		if (hitChip.chip_no != -1) {
			float h;
			// 床に当たっていた場合
			if (_gravity > 0) {
				h = hitChip.y * CHIPSIZE_H - (tmpRect.y + tmpRect.h);
				_pos.y--;
				// 次のフレームから上方向に移動させる
				_gravity = -16;
			}
			// 天井に当たっていた場合
			else {
				h = tmpRect.y - hitChip.y * CHIPSIZE_H;
				_gravity = 0;
			}
			_pos.y += h;			
		}

		// 当たり判定矩形の更新
		UpdateBodyRect();


		// x軸方向の移動処理
		float moveX = 5 * _direction;
		_pos.x += moveX;
		// マップチップとの当たり判定
		// x軸方向の当たり判定
		tmpRect = _bodyRect;
		tmpRect.x += moveX;
		hitChip = gMap.IsHitBoxAndMapChip(tmpRect);
		if (hitChip.chip_no != -1) {
			// 移動処理を取り消す
			_pos.x -= moveX;

			// 移動方向を反転させる
			_direction *= -1;
		}
		// 当たり判定矩形の更新
		UpdateBodyRect();





		_remainCount--;
		// 失うまでの残りカウントが0以下になったら
		if (_remainCount <= 0) {
			SetUse(0);
			_isLost = 1;

			// ハート破裂エフェクトのセット（生物敵撃破エフェクトと同じものを使用する）
			VECTOR effectPos = VGet(_bodyRect.x + _bodyRect.w / 2, _bodyRect.y + _bodyRect.h / 2, 0);
			float effectSize = _bodyRect.w * 3;
			SetEffect(EFFECT_TYPE_DEFEAT_CREATURE, effectPos.x - effectSize / 2, effectPos.y - effectSize / 2, effectSize, effectSize);

			// ハート破裂SEの再生（生物敵撃破SE同じものを使用する）
			PlaySoundMem(se[7], DX_PLAYTYPE_BACK);
		}
	}	
}

void Heart::SetPos(VECTOR playerPos)
{
	_pos = playerPos;

	UpdateBodyRect();
}

void Heart::UpdateBodyRect() {
	MYRECT rc = { 0, 0, 0, 0 };
	MYRECT h = _difBodyRect;
	VECTOR tmpPos = _pos;

	if (_use == 1) {
		rc.x = tmpPos.x + h.x;
		rc.y = tmpPos.y + h.y;
		rc.w = h.w;
		rc.h = h.h;
	}
	_bodyRect = rc;
}


// 描画位置の補正処理
void Heart::AdjustDrawPos(MATRIX mView)
{
	if (_use == 1) {

		_screenPos = _pos;
		_screenPos = VTransform(_screenPos, mView);

		UpdateWorldPos();
		// 移動処理の反映
		MV1SetPosition(_modelHandle, _worldPos);
	}
}

// ワールド座標を更新する
void Heart::UpdateWorldPos()
{
	// スクリーン座標をワールド座標に変換
	_worldPos = ConvScreenPosToWorldPos(_screenPos);
	_worldPos.z = 0; // z軸表示位置の調整
}

// 描画処理
void Heart::Draw()
{
	// ハートが飛び出しているとき
	if (_use == 1) {
		// ハート本体の描画
		MV1DrawModel(_modelHandle);
	}
}

// デバッグ情報の表示
void Heart::DrawDebugInfo(MATRIX mView)
{
	if (_use == 1) {
		VECTOR rectPos = VGet(_bodyRect.x, _bodyRect.y, 0);
		rectPos = VTransform(rectPos, mView);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 140.f);
		//DrawBox(_bodyRect.x, _bodyRect.y, _bodyRect.x + _bodyRect.w, _bodyRect.y + _bodyRect.h, COLOR_BLUE, TRUE);
		DrawBox(rectPos.x, rectPos.y, rectPos.x + _bodyRect.w, rectPos.y + _bodyRect.h, COLOR_RED, TRUE);



		//float cnt = float(_count) / 60.f;
		//cnt = ceilf(cnt);

		//DrawFormatStringToHandle(rectPos.x, rectPos.y, COLOR_WHITE, fontHandle, "%2.0f", cnt);



		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

}

// 前面レイヤーへの描画
void Heart::DrawFrontLayer()
{
	if (_use == 1) {
		DrawGraph(0, 0, danger[0], TRUE);
		if (dangerCnt % 60 == 0) {

			DrawGraph(0, 0, danger[1], TRUE);
		}

	}
	else if (_isLost == 1) {
		DrawGraph(0, 0, danger[0], TRUE);
	}
}

void Heart::SetUse(int n) {
	_use = n;
	// ハートが呼び出した場合の処理
	if (_use == 1) {
		// 鼓動SEの再生
		PlaySoundMem(se[15], DX_PLAYTYPE_LOOP);
	}
	// ハートを取ったときの処理
	else {
		// 秒数として扱い、小数点以下は切り上げ
		int sec = _remainCount / 60;
		_remainCount = (sec + 1) * 60;

		// 鼓動SEの停止
		StopSoundMem(se[15]);
	}
}