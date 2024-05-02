#include "enemy_shield.h"
#include "enemy_data.h"

// テクスチャ情報
std::vector<TEXTURE>	EnemyShield::_texture;
// マテリアル情報
std::vector<MATERIAL>	EnemyShield::_material;

// 盾モデルハンドル
int EnemyShield::_shieldModelBase;

// 盾をプレイヤーの方向に構えて、ミートボールでの攻撃をガードする敵
EnemyShield::EnemyShield(int modelHandle, int size, int index, VECTOR startPos, int type) : EnemyBase(modelHandle, index, startPos)
{
	_type = ENEMY_TYPE_SHIELD;
	_index = index;

	// 敵の強さを初期設定する
	SetStregth(_type, size);

	_moveType = type;

	_pos.x = _pos.x * CHIPSIZE_W + _difBodyRect.w / 2;
	_pos.y = _pos.y * CHIPSIZE_W + _difBodyRect.h / 2;

	/*_pos.x = _pos.x * CHIPSIZE_W + 64;
	_pos.y = _pos.y * CHIPSIZE_W + 128;*/

	_speed = 1;

	_screenPos = _pos;
	UpdateWorldPos();
	MV1SetPosition(_modelHandle, _worldPos);


	_difCenterY = 0;
	// 体の中心位置を更新
	UpdateCenterPos();
	// 当たり判定矩形を更新
	SetCollision();



	struct SHIELD_SIZE
	{
		float r;
		int h;
	};
	SHIELD_SIZE sSize[3] = {
		{32, 96},
		{32 * 2, 96 * 2},
		{32 * 3, 96 * 3},
	};
	_shieldCenterPos = VGet(0, 0, 0);
	_shieldDirection = VGet(0, 0, 0);
	_shieldScreenPos = VGet(0, 0, 0);
	_shieldWorldPos = VGet(0, 0, 0);
	SetShieldInfo(_index, sSize[size].r, sSize[size].h);
	// 盾の中心座標と角度を計算する
	UpdateShieldCenterPos();
	UpdateShieldInfo(_index, 1, _shieldCenterPos, _shieldDirection);




	// 初期状態のモーションの設定
	_motionNum = SHIELD_MOTION_NORMAL;

	// モーションの初期化処理
	MotionInit(SHIELD_MOTION_MAX);







	_shieldModelHandle = -1;
	_shieldModelHandle = MV1DuplicateModel(_shieldModelBase);

	float rad = DegToRad(100.f);
	_defaultShieldRot = VGet(rad, 0, 0);
	MV1SetRotationXYZ(_shieldModelHandle, _defaultShieldRot);
	MV1SetScale(_shieldModelHandle, VGet(2.8f, 2.8f, 2.8f));
}

EnemyShield::~EnemyShield()
{
	//CharaBase::~CharaBase();

	if (_shieldModelHandle != -1) {
		MV1DeleteModel(_shieldModelHandle);
		_shieldModelHandle = -1;
	}
}


void EnemyShield::Process(VECTOR playerPos) {
	// この敵が倒されていなかったら
	if (_use == 1) {
		int motionNum_old = _motionNum;

		// 共通処理
		// ヒットストップ処理
		EnemyBase::Process(playerPos);

		// ヒットストップ中でなければ
		if (_isHitStop == 0) {

			// 画面内に入っていたら
			if (_isInScreen == 1) {
				// 索敵状態なら
				if (_isSpotPlayer == 1) {
					// 移動方向の計算
					VECTOR vDir = VSub(playerPos, _centerPos);
					_direction = VNorm(vDir);


					if (_moveType == ENSHIELD_TYPE_MOVE) {
						VECTOR vMove = VScale(_direction, _speed);
						_pos.x += vMove.x;
						_pos.y += vMove.y;

						// 体の中心位置を更新
						UpdateCenterPos();
						// 当たり判定矩形を更新
						SetCollision();
					}


					_motionNum = SHIELD_MOTION_ENCOUNT;
				}
				else {
					MATRIX mRot = MGetRotZ(DegToRad(2.f));
					_direction = VTransform(_direction, mRot);
				}
			}
			else {
				SetIsSpotPlayer(0);
				MATRIX mRot = MGetRotZ(DegToRad(2.f));
				_direction = VTransform(_direction, mRot);
			}

			if (_isHitStop == 0) {
				// 盾を出す位置と盾の角度を計算する
				UpdateShieldCenterPos();
				// 盾の情報をセットする
				UpdateShieldInfo(_index, 1, _shieldCenterPos, _shieldDirection);
				// 盾モデルの回転値をセットする
				SetShieldModelRotation();
			}
			else {
				// 盾を無効にする
				UpdateShieldInfo(_index, 0);
			}


			//// 敵モデルの回転値をセットする
			//SetModelRotation();

		}


		MotionProcess(motionNum_old);
	}
	else {
		// 盾を無効にする
		UpdateShieldInfo(_index, 0);
	}
}

// 盾の中心座標と角度を計算する
void EnemyShield::UpdateShieldCenterPos()
{
	VECTOR vDir = VScale(_direction, 64 + 32 + 32);
	VECTOR vPos = VAdd(_centerPos, vDir);
	_shieldCenterPos = vPos;

	_shieldDirection.x = -_direction.y;
	_shieldDirection.y = _direction.x;

	_shieldDirection = VNorm(_shieldDirection);
}


// 盾の3Dモデルを回転する
void EnemyShield::SetShieldModelRotation()
{
	// 回転処理
	float rad;
	////VGet(0, -1, 0)を起点とした回転角を計算
	//VECTOR vOrigin = VGet(0, -1, 0);

	// VGet(0, 1, 0)を起点とした回転角を計算
	VECTOR vOrigin = VGet(0, 1, 0);
	float dot = VDot(vOrigin, _direction);
	float cos = dot / (VSize(vOrigin) * VSize(_direction));
	rad = acos(cos);

	//起点から左に回転する場合
	if (_direction.x < 0) {
		rad *= -1;
	}
	MV1SetRotationXYZ(_shieldModelHandle, VGet(_defaultShieldRot.x, 0, rad));
}

// 描画位置の補正処理
void EnemyShield::AdjustDrawPos(MATRIX mView)
{
	CharaBase::AdjustDrawPos(mView);

	_shieldScreenPos = _shieldCenterPos;
	_shieldScreenPos = VTransform(_shieldScreenPos, mView);
	// スクリーン座標をワールド座標に変換
	_shieldWorldPos = ConvScreenPosToWorldPos(_shieldScreenPos);
	_shieldWorldPos.z = 0; // z軸表示位置の調整
	// モデル表示座標の反映
	MV1SetPosition(_shieldModelHandle, _shieldWorldPos);

}

// 描画処理
void EnemyShield::Draw()
{
	if (_use == 1) {
		EnemyBase::Draw();
	}
}

// 前面レイヤーに描画
void EnemyShield::DrawFrontLayer(MATRIX mView)
{
	if (_use == 1) {
		// 盾の描画
		MV1DrawModel(_shieldModelHandle);
	}
}


// デバッグ用
// カプセルの描画
void MyDrawCapsule(
	float cx, float cy, float r, // カプセル 端の位置(x,y), カプセルの横幅（円の半径）r
	VECTOR direction,						 // カプセルを伸ばす方向
	float h,										 // カプセルの縦の長さ
	int color, 
	int fillFrag
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

	while (length < h) {
		DrawCircle(x, y, r, color, fillFrag);

		x += vTmp.x;
		y += vTmp.y;
		length += VSize(vTmp);
	}

	length -= length - h;
	VECTOR v = VGet(x, y, 0);
	v = VSub(v, VScale(vDir, length - h));
	DrawCircle(v.x, v.y, r, color, fillFrag);
}


// デバッグ情報の表示
void EnemyShield::DrawDebugInfo(MATRIX mView, int fillFrag)
{
	if (_use == 1) {
		EnemyBase::DrawDebugInfo(mView, fillFrag);

		VECTOR rectPos;

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 140.f);

		// 盾の当たり判定
		VECTOR vShieldPos = _shieldCenterPos;
		VECTOR sDir = _shieldDirection;
		vShieldPos = VTransform(vShieldPos, mView);		

		DrawCircle(vShieldPos.x, vShieldPos.y, shieldInfo[_index].r, COLOR_RED, fillFrag);
		MyDrawCapsule(vShieldPos.x, vShieldPos.y, shieldInfo[_index].r, sDir, shieldInfo[_index].h, COLOR_RED, fillFrag);
		sDir = VScale(sDir, -1);
		MyDrawCapsule(vShieldPos.x, vShieldPos.y, shieldInfo[_index].r, sDir, shieldInfo[_index].h, COLOR_RED, fillFrag);


		//VECTOR vShieldPos = _shieldCenterPos;
		//vShieldPos = VTransform(vShieldPos, mView);
		//DrawCircle(vShieldPos.x, vShieldPos.y, 32, COLOR_RED, TRUE);



		//VECTOR vShieldPos2 = VAdd(vShieldPos, VScale(_shieldDirection, 96 - 32));
		//DrawCircle(vShieldPos2.x, vShieldPos2.y, 32, COLOR_RED, TRUE);

		//VECTOR vShieldPos3 = VAdd(vShieldPos, VScale(_shieldDirection, -(96 -32)));
		//DrawCircle(vShieldPos3.x, vShieldPos3.y, 32, COLOR_RED, TRUE);

		/*VECTOR v = VSub(vShieldPos2, vShieldPos3);
		
		float s = VSize(v);
		s = s + 32 + 32;*/

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		/*VECTOR shieldRot = MV1GetRotationXYZ(_shieldModelHandle);
		DrawFormatString(SCREEN_W / 2, SCREEN_H / 2, COLOR_RED, "shieldRot : x %3.2f, y %3.2f, z %3.2f", shieldRot.x, shieldRot.y, shieldRot.z);
		DrawFormatString(SCREEN_W / 2, SCREEN_H / 2 + 16, COLOR_RED, "shieldDir : x %3.2f, y %3.2f, z %3.2f", _shieldDirection.x, _shieldDirection.y, _shieldDirection.z);
	*/
	}

}



//
// 構造体「SHIELD」に対する処理
// 

// 盾情報を初期化する（全てリセットする）
void InitShieldInfo()
{
	for (int i = 0; i < ENEMY_SHIELD_MAX; i++) {
		shieldInfo[i].use = 0;

		shieldInfo[i].r = 0;
		shieldInfo[i].h = 0;

		shieldInfo[i].centerPos = VGet(0, 0, 0);
		shieldInfo[i].direction = VGet(0, 0, 0);
	}
}

// 盾情報をセットする
void SetShieldInfo(int index, float r, int h) {
	shieldInfo[index].r = r;
	shieldInfo[index].h = h;
}

// 盾情報を更新する
void UpdateShieldInfo(int index, int use, VECTOR pos, VECTOR dir) {
	shieldInfo[index].use = use;
	if (use == 1) {
		shieldInfo[index].centerPos = pos;
		shieldInfo[index].direction = dir;
	}
}

// 盾情報を取得する
SHIELD GetShieldInfo(int index) {
	return shieldInfo[index];
}


// 構造体「TEXTURE」の初期化
void EnemyShield::TextureInit(int modelHandle)
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
void EnemyShield::ChangeTexture(int isDamaged)
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
void EnemyShield::MaterialInit(int modelHandle)
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
void EnemyShield::ChangeMaterialDifColor(int isDamaged)
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



