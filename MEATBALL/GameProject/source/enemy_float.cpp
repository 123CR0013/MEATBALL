#include "enemy_float.h"
#include "mydraw.h"

// テクスチャ情報
std::vector<TEXTURE>	EnemyFloat::_texture;
// マテリアル情報
std::vector<MATERIAL>	EnemyFloat::_material;

// 決まったルートを巡回する敵
EnemyFloat::EnemyFloat(int modelHandle, int stageNo, int size, int index, VECTOR startPos, VECTOR direction) : EnemyBase(modelHandle, index, startPos, direction)
{
	_type = ENEMY_TYPE_FLOAT;
	_index = index;


	// 敵の強さを初期設定する
	SetStregth(_type, size);
	
	_pos.x = _pos.x * CHIPSIZE_W + _difBodyRect.w / 2;
	_pos.y = _pos.y * CHIPSIZE_W + _difBodyRect.h / 2;


	/*_pos.x = _pos.x * CHIPSIZE_W + 32;
	_pos.y = _pos.y * CHIPSIZE_W + 64;*/


	_direction = direction;


	// 移動ルート情報を取得して保持する
	for (int i = 0; i < 10; i++) {
		route[i] = GetFloatRoute(stageNo, _index, i);
	}

	


	// 初期状態のモーションの設定
	_motionNum = FLOAT_MOTION_NORMAL;

	// モーションの初期化処理
	MotionInit(FLOAT_MOTION_MAX);

	_speed = 3;

	_screenPos = _pos;
	UpdateWorldPos();
	MV1SetPosition(_modelHandle, _worldPos);

	_difCenterY = 32;

	// 体の中心位置を更新
	UpdateCenterPos();
	// 当たり判定矩形を更新
	SetCollision();

}

EnemyFloat::~EnemyFloat()
{
	//CharaBase::~CharaBase();
}

// 計算処理
void EnemyFloat::Process(VECTOR playerPos) {
	// この敵が倒されていなかったら
	if (_use == 1) {
		int motionNum_old = _motionNum;

		// 共通処理
		// ヒットストップ処理
		EnemyBase::Process(playerPos);

		// ヒットストップ中でなければ
		if (_isHitStop == 0) {

			// 移動方向の更新があるかを調べる
			int i = 0;
			while (route[i].pos.x != -1)
			{
				int x = _centerPos.x / CHIPSIZE_W;
				int y = _centerPos.y / CHIPSIZE_H;

				// 現在のマップチップ座標と移動ルート情報のマップチップ座標が一致していたら、移動方向を更新する
				if (x == route[i].pos.x && y == route[i].pos.y) {
					_direction = route[i].direction;
					break;
				}
				i++;
			}

			// 移動処理
			VECTOR vDir = _direction;
			VECTOR move = VScale(vDir, _speed);
			_pos.x += move.x;
			_pos.y += move.y;

			// 体の中心位置を更新
			UpdateCenterPos();
			// 当たり判定矩形を更新
			SetCollision();
		}

		// モーションの計算処理
		MotionProcess(motionNum_old);
	}
}

// 描画処理
void EnemyFloat::Draw()
{
}

// 前面レイヤーへの描画処理
void EnemyFloat::DrawFrontLayer(MATRIX mView)
{
	// 敵本体の描画
	if (_use == 1) {
		MV1DrawModel(_modelHandle);
	}
}


// 構造体「TEXTURE」の初期化
void EnemyFloat::TextureInit(int modelHandle)
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
void EnemyFloat::ChangeTexture(int isDamaged)
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
void EnemyFloat::MaterialInit(int modelHandle)
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
// 0 : 通常時, 1 : 被ダメージ時
void EnemyFloat::ChangeMaterialDifColor(int isDamaged)
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
void EnemyFloat::DrawDebugInfo(MATRIX mView, int fillFrag)
{
	// 
	// 巡回ルート表示
	// 
	for (int i = 0; i < ROUTE_MAX; i++) {
		if (route[i].pos.x == -1) break;
		VECTOR world = VGet(route[i].pos.x * CHIPSIZE_W, route[i].pos.y * CHIPSIZE_H, 0);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 64);		// 半透明描画指定
		MyDrawBox(mView, world.x, world.y, world.x + CHIPSIZE_W, world.y + CHIPSIZE_H, COLOR_RED, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		// 不透明描画指定

		world = VTransform(world, mView);


		if (route[i].direction.x == 1) {
			DrawFormatString(world.x, world.y, COLOR_WHITE, "→");
		}
		else if (route[i].direction.x == -1) {
			DrawFormatString(world.x, world.y, COLOR_WHITE, "←");

		}
		if (route[i].direction.y == 1) {
			DrawFormatString(world.x, world.y, COLOR_WHITE, "↓");
		}
		else if (route[i].direction.y == -1) {
			DrawFormatString(world.x, world.y, COLOR_WHITE, "↑");
		}
	}



	if (_use == 1) {
		EnemyBase::DrawDebugInfo(mView, fillFrag);
	}
}



