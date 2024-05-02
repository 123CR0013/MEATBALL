#include "enemy_shooting.h"
#include "enemy_data.h"
#include "resource.h"
#include "mydraw.h"
#include "effect.h"

// 弾を撃ってから次の弾を撃つまでの間隔
#define BULLET_COOLTIME_MAX 2 * 60

// テクスチャ情報
std::vector<TEXTURE>	EnemyShooting::_texture;
// マテリアル情報
std::vector<MATERIAL>	EnemyShooting::_material;

// プレイヤーに向かって弾を撃つ敵
EnemyShooting::EnemyShooting(int modelHandle, int size, int index, VECTOR startPos, VECTOR direction) : EnemyBase(modelHandle, index, startPos, direction)
{
	_type = ENEMY_TYPE_SHOOTING;
	_index = index;


	// 敵の強さを初期設定する
	SetStregth(_type, size);

	_pos.x = _pos.x * CHIPSIZE_W + _difBodyRect.w / 2;
	_pos.y = _pos.y * CHIPSIZE_W + _difBodyRect.h / 2;


	/*_pos.x = _pos.x * CHIPSIZE_W + 64;
	_pos.y = _pos.y * CHIPSIZE_W + 128;*/
	

	_speed = 0;

	_screenPos = _pos;
	UpdateWorldPos();
	MV1SetPosition(_modelHandle, _worldPos);
	

	_difCenterY = 0;
	// 体の中心位置を更新
	UpdateCenterPos();
	// 当たり判定矩形を更新
	SetCollision();



	for (int i = 0; i < BULLET_MAX; i++) {
		bullet[i].use = 0;
	}
	_bulletSpeed = 7;
	_bulletCoolTime = 27;

	_bulletScreenPos = VGet(-1, -1, -1);



	// 初期状態のモーションの設定
	_motionNum = SHOOTING_MOTION_NORMAL;

	// モーションの初期化処理
	MotionInit(SHOOTING_MOTION_MAX);



	_eyeEmiColorFade = 0.0f;

}

EnemyShooting::~EnemyShooting()
{
	//CharaBase::~CharaBase();
}


// 計算処理
void EnemyShooting::Process(VECTOR playerPos) {
	// この敵が倒されていなかったら
	if (_use == 1) {
		int motionNum_old = _motionNum;

		// 共通処理
		// ヒットストップ処理
		EnemyBase::Process(playerPos);

		// ヒットストップ中でなければ
		if (_isHitStop == 0) {
			// 画面内にいたら
			if (_isInScreen == 1) {
				// 索敵状態なら
				if (_isSpotPlayer == 1) {
					// 弾を撃った後のクールタイムが終わっていたら
					if (_bulletCoolTime <= 0) {
						// 弾を撃ちだす処理
						ShotBullet(playerPos);
					}
					else if(_bulletCoolTime == 27){
						_eyeEmiColorFade = 1.0f;


						// チャージエフェクトのセット
						int effectX = _bodyRect.x - _bodyRect.w * 1.5;
						int effectY = _bodyRect.y - _bodyRect.h * 1.5;
						int effectSize = _bodyRect.w * 4;
						SetEffect(EFFECT_TYPE_BULLET_CHARGE, effectX, effectY, effectSize, effectSize);
					}
				}
			}
			else {
				// 画面買いに出たら、非索敵状態にする
				SetIsSpotPlayer(0);
			}

			// モーションの設定
			if (_isSpotPlayer == 1) {
				_motionNum = SHOOTING_MOTION_ENCOUNT;
			}
			else {
				_motionNum = SHOOTING_MOTION_NORMAL;
			}
		}

		// 目のパーツを光らせる
		MV1SetMaterialEmiColor(_modelHandle, 8, GetColorF(_eyeEmiColorFade, _eyeEmiColorFade, _eyeEmiColorFade, 1.0f));
		_eyeEmiColorFade -= 0.1f;
		if (_eyeEmiColorFade < 0) {
			_eyeEmiColorFade = 0.0f;
		}

		// モーションの計算処理
		MotionProcess(motionNum_old);
	}


	// キャラクターの回転処理
	VECTOR vDir = VSub(playerPos, _centerPos);
	_direction = VNorm(vDir);
	float rad;
	if (VSize(_direction) != 0) {
		////VGet(0, -1, 0)を起点とした回転角を計算
		//VECTOR vOrigin = VGet(0, -1, 0);

		// VGet(0, 1, 0)を起点とした回転角を計算
		VECTOR vOrigin = VGet(0, 1, 0);
		float dot = VDot(vOrigin, _direction);
		float cos = dot / (VSize(vOrigin) * VSize(_direction));
		rad = acos(cos);

		//起点から左に回転する場合
		if (_direction.x > 0) {
			rad *= -1;
		}
	}
	//回転処理の反映
	MV1SetRotationXYZ(_modelHandle, VGet(0.0f, rad, 0.0f));

	// 弾の計算処理
	BulletProcess();


	// クールタイム中の処理
	if (_bulletCoolTime > 0) {
		if (_isSpotPlayer == 1 || _isAttack == 1) {
			_bulletCoolTime--;
		}
	}
}

// 弾を撃ちだす処理
void EnemyShooting::ShotBullet(VECTOR playerPos)
{
	for (int i = 0; i < BULLET_MAX; i++) {
		if (bullet[i].use == 0) {
			_isAttack = 1;
			PlaySoundMem(se[2], DX_PLAYTYPE_BACK);
			bullet[i].use = 1;

			bullet[i].pos = _pos;

			// プレイヤーがいる方向に弾を撃ちだす（移動方向は途中で変更しない）
			VECTOR vTmp = VSub(playerPos, bullet[i].pos);
			vTmp = VNorm(vTmp);
			bullet[i].direction = vTmp;

			_bulletCoolTime = BULLET_COOLTIME_MAX;

			bullet[i].animCnt = 0;
			break;
		}
	}
}

// 弾の計算処理
void EnemyShooting::BulletProcess()
{
	for (int i = 0; i < BULLET_MAX; i++) {
		if (_isAttack == 0) {
			bullet[i].use = 0;
			_attackRect.x = -1;
		}
		if (bullet[i].use == 1) {
			// 移動処理
			VECTOR vDir = VNorm(bullet[i].direction);
			VECTOR move = VScale(vDir, _bulletSpeed);
			bullet[i].pos.x += move.x;
			bullet[i].pos.y += move.y;

			// 弾の当たり判定
			MYRECT rc = { 0, 0, 0, 0 };
			rc.x = bullet[i].pos.x + _difAttackRect.x;
			rc.y = bullet[i].pos.y + _difAttackRect.y;
			rc.w = _difAttackRect.w;
			rc.h = _difAttackRect.h;
			bullet[i].rect = rc;

			_attackRect = bullet[i].rect;

			// マップチップとの当たり判定
			HITCHIP hitChip = gMap.IsHitBoxAndMapChip(bullet[i].rect);
			if (hitChip.chip_no != -1) {
				bullet[i].use = 0;

				// 弾破裂エフェクトのセット
				SetBulletHitEffect();


				_attackRect.x = -1;
			}



			// 画面外にでたら弾を消す
			if (
				(bullet[i].rect.x + bullet[i].rect.w < gCam._pos.x - SCREEN_W / 2) ||
				(bullet[i].rect.x > gCam._pos.x + SCREEN_W / 2) ||
				(bullet[i].rect.y + bullet[i].rect.h < gCam._pos.y - SCREEN_H / 2) ||
				(bullet[i].rect.y > gCam._pos.y + SCREEN_H / 2)
				) {
				bullet[i].use = 0;

				// 弾破裂エフェクトのセット
				SetBulletHitEffect();

				_attackRect.x = -1;
			}

			bullet[i].animCnt++;
		}
	}
}

// 弾破裂エフェクトのセット
void EnemyShooting::SetBulletHitEffect()
{
	VECTOR centerPos = VGet(_attackRect.x, _attackRect.y, 0);
	centerPos.x += _attackRect.w / 2;
	centerPos.y += _attackRect.h / 2;
	float effectSize = _attackRect.w * 3 * 1.5f;
	SetEffect(EFFECT_TYPE_BULLET_HIT, centerPos.x - effectSize / 2, centerPos.y - effectSize / 2, effectSize, effectSize);
}


// 描画位置の補正処理
void EnemyShooting::AdjustDrawPos(MATRIX mView)
{
	CharaBase::AdjustDrawPos(mView);

	_bulletScreenPos = VGet(_attackRect.x, _attackRect.y, 0);
	//_bulletScreenPos = VTransform(_bulletScreenPos, mView);
}

// 描画処理
void EnemyShooting::Draw()
{
	EnemyBase::Draw();
}

// 弾の描画処理
void EnemyShooting::DrawBullet(MATRIX mView)
{
	for (int i = 0; i < BULLET_MAX; i++) {
		if (bullet[i].use == 1) {
			//DrawGraph(_bulletScreenPos.x, _bulletScreenPos.y, cgBullet, TRUE);
			//DrawExtendGraph(_bulletScreenPos.x, _bulletScreenPos.y, _bulletScreenPos.x + 64, _bulletScreenPos.y + 64, cgBullet, TRUE);

			VECTOR bPos = _bulletScreenPos;
			bPos.x += 64 / 2;
			bPos.y += 64 / 2;
			MyDrawModiGraph(mView, bPos, 1.0f, 0.0f, 64 *1.5f, 64 * 1.5f, cgBullet[(bullet[i].animCnt / 1) % 30]);
		}
	}}


// 前面レイヤーに描画
// 弾の描画
void EnemyShooting::DrawFrontLayer(MATRIX mView)
{
	DrawBullet(mView);
}







// 構造体「TEXTURE」の初期化
void EnemyShooting::TextureInit(int modelHandle)
{
	// 初期化
	_texture.clear();

	// テクスチャの合計数を取得する
	int n = MV1GetTextureNum(modelHandle);

	for (int i = 0; i < n; i++) {
		// テクスチャ画像のハンドルを構造体TEXTUREに保持する
		TEXTURE t = { i, MV1GetTextureGraphHandle(modelHandle, i)};
		_texture.push_back(t);
	}
}

// テクスチャ画像の貼り替え
// ダメージ受けたとき, 数フレームだけ真っ白な画像を貼る
// その後, 元に戻す
// 引数isDamaged によって貼るテクスチャを変更する
// 0 : 通常時, 1 : 被ダメージ時
void EnemyShooting::ChangeTexture(int isDamaged)
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
void EnemyShooting::MaterialInit(int modelHandle)
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
void EnemyShooting::ChangeMaterialDifColor(int isDamaged)
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

// デバッグ情報の表示
void EnemyShooting::DrawDebugInfo(MATRIX mView, int fillFrag)
{
	if (_use == 1) {
		EnemyBase::DrawDebugInfo(mView, fillFrag);
		VECTOR rectPos;

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 140.f);
		/*DrawFormatStringToHandle(rectPos.x, rectPos.y, COLOR_WHITE, fontHandle, "%d", 2 - _type);
		DrawFormatString(rectPos.x, rectPos.y, COLOR_WHITE, "dir: x %3.2f, y %3.2f", _direction.x, _direction.y);*/

		//// 索敵範囲
		//rectPos = VGet(_searchRect.x, _searchRect.y, 0);
		//rectPos = VTransform(rectPos, mView);
		//DrawBox(rectPos.x, rectPos.y, rectPos.x + _searchRect.w, rectPos.y + _searchRect.h, COLOR_GREEN, FALSE);

		

		////// 敵の当たり判定
		//rectPos = VGet(_bodyRect.x, _bodyRect.y, 0);
		//rectPos = VTransform(rectPos, mView);
		//DrawBox(rectPos.x, rectPos.y, rectPos.x + _bodyRect.w, rectPos.y + _bodyRect.h, COLOR_BLUE, FALSE);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		//DrawFormatString(rectPos.x, rectPos.y, COLOR_WHITE, "%d", _bulletCoolTime);
	}
	// 弾の当たり判定
	for (int i = 0; i < BULLET_MAX; i++) {
		if (bullet[i].use == 1) {
			VECTOR rectPos;
			rectPos = VGet(bullet[i].rect.x, bullet[i].rect.y, 0);
			rectPos = VTransform(rectPos, mView);
			DrawBox(rectPos.x, rectPos.y, rectPos.x + bullet[i].rect.w, rectPos.y + bullet[i].rect.h, COLOR_RED, fillFrag);
		}
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//DrawFormatString(SCREEN_H / 2, 0, COLOR_WHITE, "bullet use: x %d", bullet[0].use);
	//DrawFormatString(SCREEN_H / 2, 16, COLOR_WHITE, "bullet pos: x %3.2f, y: %3.2f", bullet[0].pos.x, bullet[0].pos.y);
	//DrawFormatString(SCREEN_H / 2, 32, COLOR_WHITE, "cam pos: x %3.2f, y: %3.2f", gCam._pos.x, gCam._pos.y + SCREEN_H / 2);

}