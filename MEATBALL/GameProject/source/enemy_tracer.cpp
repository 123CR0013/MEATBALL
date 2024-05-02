#include "enemy_tracer.h"
#include "enemy_data.h"
#include "resource.h"

// テクスチャ情報
std::vector<TEXTURE>	EnemyTracer::_texture;
// マテリアル情報
std::vector<MATERIAL>	EnemyTracer::_material;

// プレイヤーを追いかけてくる敵
EnemyTracer::EnemyTracer(int modelHandle, int size, int index, VECTOR startPos, VECTOR direction) : EnemyBase(modelHandle, index, startPos, direction)
{
	_type = ENEMY_TYPE_TRACER;
	_index = index;

	// 敵の強さを初期設定する
	SetStregth(_type, size);


	_pos.x = _pos.x * CHIPSIZE_W + _difBodyRect.w / 2;
	_pos.y = _pos.y * CHIPSIZE_W + _difBodyRect.h / 2;



	/*_pos.x = _pos.x * CHIPSIZE_W + 32;
	_pos.y = _pos.y * CHIPSIZE_W + 64;*/


	// 初期状態のモーションの設定
	_motionNum = TRACER_MOTION_NORMAL;

	// モーションの初期化処理
	MotionInit(TRACER_MOTION_MAX);

	_speed = 2;

	_screenPos = _pos;
	// ワールド座標を更新する
	UpdateWorldPos();
	// 3Dモデルの座標を更新する
	MV1SetPosition(_modelHandle, _worldPos);

	_difCenterY = 32;

	// 体の中心位置を更新
	UpdateCenterPos();
	// 当たり判定矩形を更新
	SetCollision();

	_canPlaySe = 1;
}

EnemyTracer::~EnemyTracer()
{
	//CharaBase::~CharaBase();
}

// 計算処理
void EnemyTracer::Process(VECTOR playerPos) {
	// この敵が倒されていなかったら
	if (_use == 1) {
		int motionNum_old = _motionNum;

		// 共通処理
		// ヒットストップ処理
		EnemyBase::Process(playerPos);

		// ヒットストップ中で無ければ
		if (_isHitStop == 0) {
			// 画面内にいたら
			if (_isInScreen == 1) {
				// 非索敵状態なら
				if (_isSpotPlayer == 1) {

					// 機械音SEの再生
					// 索敵状態に1度だけ再生する
					// 画面外に出たら再び再生可能にする
					if (_canPlaySe == 1){
						PlaySoundMem(se[21], DX_PLAYTYPE_BACK);
						// SEを再生不可能にする
						_canPlaySe = 0;
					}

					// 移動方向の計算
					VECTOR vDir = VSub(playerPos, _centerPos);
					_direction = VNorm(vDir);
					VECTOR vMove = VScale(_direction, _speed);

					//
					// x軸の移動
					//
					_pos.x += vMove.x;

					// 体の中心位置を更新
					UpdateCenterPos();
					// 当たり判定矩形を更新
					SetCollision();

					// マップチップとの当たり判定
					HITCHIP hitChip = gMap.IsHitBoxAndMapChip(_bodyRect);
					// マップチップに当たっていたら
					if (hitChip.chip_no != -1) {
						float w;
						if (vMove.x < 0) {
							w = (hitChip.x * CHIPSIZE_W + CHIPSIZE_W) - _bodyRect.x;
						}
						else if (vMove.x > 0) {
							w = hitChip.x * CHIPSIZE_W - (_bodyRect.x + _bodyRect.w);
							w--;
						}
						// 押し戻し処理
						_pos.x += w;

						// 体の中心位置を更新
						UpdateCenterPos();
						// 当たり判定矩形を更新
						SetCollision();
					}

					//
					// y軸の移動
					//
					_pos.y += vMove.y;

					// 体の中心位置を更新
					UpdateCenterPos();
					// 当たり判定矩形を更新
					SetCollision();

					// マップチップとの当たり判定
					hitChip = gMap.IsHitBoxAndMapChip(_bodyRect);
					// マップチップに当たっていたら
					if (hitChip.chip_no != -1) {
						float h;
						// 下に移動しているとき
						if (vMove.y > 0) {
							h = hitChip.y * CHIPSIZE_H - (_bodyRect.y + _bodyRect.h);
							_pos.y--;
						}
						// 上に移動しているとき
						else {
							h = (hitChip.y * CHIPSIZE_H + CHIPSIZE_H) - _bodyRect.y;
						}
						// 押し戻し処理
						_pos.y += h;

						// 体の中心位置を更新
						UpdateCenterPos();
						// 当たり判定矩形を更新
						SetCollision();
					}
					_motionNum = TRACER_MOTION_ENCOUNT;
				}
				else {
					_motionNum = TRACER_MOTION_NORMAL;
				}
			}
			else {
				// SEを再生可能にする
				_canPlaySe = 1;
				// 非索敵状態にする
				SetIsSpotPlayer(0);
			}

		}

		// モーションの計算処理
		MotionProcess(motionNum_old);
	}
}


// 構造体「TEXTURE」の初期化
void EnemyTracer::TextureInit(int modelHandle)
{
	// 初期化
	_texture.clear();

	// テクスチャの合計数を取得する
	int n = MV1GetTextureNum(modelHandle);

	for (int i = 0; i < n; i++) {
		// テクスチャ画像のハンドルを構造体TEXTUREに保持する
		TEXTURE t = { i, MV1GetTextureGraphHandle(modelHandle, i) };
		_texture.push_back(t);
	}
}


// テクスチャ画像の貼り替え
// ダメージ受けたとき, 数フレームだけ真っ白な画像を貼る
// その後, 元に戻す
// 引数isDamaged によって貼るテクスチャを変更する
// 0 : 通常時, 1 : 被ダメージ時
void EnemyTracer::ChangeTexture(int isDamaged)
{
	for (auto tex : _texture) {
		int cgTex;

		// 通常時
		if (isDamaged == 0) {
			cgTex = tex.graphHandle;
		}
		// 被ダメージ時
		else {
			cgTex = _cgTexWhite;
		}

		SetTexture(tex.index, cgTex);
	}
}



// 構造体「MATERIAL」の初期化
void EnemyTracer::MaterialInit(int modelHandle)
{
	// 初期化
	_material.clear();

	// マテリアルの合計数を取得する
	int n = MV1GetMaterialNum(modelHandle);

	for (int i = 0; i < n; i++) {
		// マテリアルのディフューズカラーを構造体MATERIALに保持する
		MATERIAL m = { i, MV1GetMaterialDifColor(modelHandle, i) };
		_material.push_back(m);
	}
}


// マテリアルのディフューズカラーの変更
// 引数isDamagedによってディフューズカラーを変更する
void EnemyTracer::ChangeMaterialDifColor(int isDamaged)
{
	for (auto m : _material) {
		COLOR_F difColor;

		// 通常時
		if (isDamaged == 0) {
			difColor = m.difColor;
		}
		// 被ダメージ時
		else {
			difColor = GetColorF(1.0f, 1.0f, 1.0f, 1.0f);

			// 透明度は維持する
			difColor.a = m.difColor.a;
		}

		SetMaterialDifColor(m.index, difColor);
	}
}

//void EnemyTracer::DrawDebugInfo(MATRIX mView)
//{
//	if (_use == 1) {
//		VECTOR rectPos;
//
//		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 140.f);
//
//		// 索敵範囲
//		rectPos = VGet(_searchRect.x, _searchRect.y, 0);
//		rectPos = VTransform(rectPos, mView);
//		DrawBox(rectPos.x, rectPos.y, rectPos.x + _searchRect.w, rectPos.y + _searchRect.h, COLOR_GREEN, TRUE);
//		
//
//		// 敵の当たり判定
//		rectPos = VGet(_bodyRect.x, _bodyRect.y, 0);
//		rectPos = VTransform(rectPos, mView);
//		DrawBox(rectPos.x, rectPos.y, rectPos.x + _bodyRect.w, rectPos.y + _bodyRect.h, COLOR_BLUE, TRUE);
//
//		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
//
//
//
//		DrawFormatString(rectPos.x, rectPos.y, COLOR_WHITE, "dir: x %3.2f, y %3.2f", _direction.x, _direction.y);
//	}
//}