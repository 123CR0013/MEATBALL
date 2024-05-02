#include "charabase.h"
#include <math.h>
#include "mymath.h"

#include "gamepad.h"

// 真っ白のテクスチャ画像
int CharaBase::_cgTexWhite = { -1 };

// キャラクターの基底クラス
CharaBase::CharaBase(int modelHandle, VECTOR startPos) {
	// ３Ｄモデルの読み込み（複製する）
	// 3Dモデルの読み込み MV1LoadModel()は処理に時間がかかるので、3Dモデルの複製 MV1DuplicateModel()を使う
	// 複製元3Dモデルの読み込みはは scenegamemain.cpp の SceneGameMainResourceInit() 内で行う
	_modelHandle = MV1DuplicateModel(modelHandle);



	_use = 1;

	_speed = 0.0f;

	_difBodyRect = { 0,0,0,0 };
	_bodyRect = { -1,-1,-1,-1 };


	_motionNum = -1;
	_playTime = 0.0f;


	_useGravity = 0;
	_gravity = 0;
	_stand = 0;




	// 初期位置の設定	
	_pos = startPos;
	_screenPos = _pos;
	_worldPos = VGet(0, 0, 0);

	_centerPos = VGet(0, 0, 0);
	_difCenterY = 0.0f;
	_direction = VGet(1, 0, 0);

	// 3Dモデル表示用座標の更新
	UpdateWorldPos();

	// 初期位置に移動
	MV1SetPosition(_modelHandle, _worldPos);

	_isAttack = 0;

}

CharaBase::~CharaBase() {
	if (_modelHandle != -1) {
		MV1DeleteModel(_modelHandle);
		_modelHandle = -1;
	}
}

// 計算処理
void CharaBase::Process() {
	// 重力処理（y軸の移動処理）
	if (_useGravity == 1) {
		_gravity += 1;			// キャラの、重力による加速値を大きくする
		_pos.y += _gravity;	// 重力による加速値の分だけ移動する

		_stand = 0;			// 床に当たってないことを前提に、床フラグリセット

		MYRECT tmpRect = _bodyRect;
		tmpRect.y += _gravity;
		// マップチップとの当たり判定
		HITCHIP hitChip = gMap.IsHitBoxAndMapChip(tmpRect);
		// マップチップと当たっていたら
		if (hitChip.chip_no != -1) {

			float h = 0;

			// 斜めのチップに当たったときに必要な情報
			// 当たったかどうか
			int isDiagonalHit = 0;
			//斜面の始点と終点
			VECTOR startPos, endPos;

			// 当たったチップの種類ごとに処理を行う
			switch (hitChip.chip_no)
			{

			case -1:
				break;

			default:
				// 下に移動しているとき
				if (_gravity > 0) {
					h = hitChip.y * CHIPSIZE_H - (tmpRect.y + tmpRect.h);
					_stand = 1;
					_pos.y--;
				}
				// 上に移動しているとき
				else {
					h = (hitChip.y * CHIPSIZE_H + CHIPSIZE_H) - tmpRect.y;
				}
				_pos.y += h;
				_gravity = 0;
				break;

			
			case LOWER_RIGHT: // 右下 斜めチップ
				// 落下中のみ処理を行う（そうしないと斜めのマップチップに乗った状態でのジャンプができなくなる）
				if (0 < _gravity) {
					isDiagonalHit = 1;
					startPos = VGet(hitChip.x * CHIPSIZE_W, hitChip.y * CHIPSIZE_H + CHIPSIZE_H, 0);
					endPos = VGet(hitChip.x * CHIPSIZE_W + CHIPSIZE_W, hitChip.y * CHIPSIZE_H, 0);

					// 
					//if (_direction.x < 0) {
					_pos.y += CHIPSIZE_H / 2;
					//}
				}
				break;
			case LOWER_LEFT: // 左下 斜めチップ
				// 落下中のみ処理を行う（そうしないと斜めのマップチップに乗った状態でのジャンプができなくなる）
				if (0 < _gravity) {
					isDiagonalHit = 1;
					startPos = VGet(hitChip.x * CHIPSIZE_W, hitChip.y * CHIPSIZE_H, 0);
					endPos = VGet(hitChip.x * CHIPSIZE_W + CHIPSIZE_W, hitChip.y * CHIPSIZE_H + CHIPSIZE_H, 0);

					//if (_direction.x < 0) {
					_pos.y += CHIPSIZE_H / 2;
					//}
				}				
				break;
			}

			// 斜めのチップに当たった場合
			// 斜面をスムーズに移動する
			if (isDiagonalHit == 1) {
				int isHit = 1;
				while (isHit == 1) {
					_pos.y--;
					//体の中心位置を更新
					UpdateCenterPos();
					// 当たり判定矩形を更新
					SetCollision();

					// 
					// 斜面を線分とみなして、斜面と体の矩形の当たり判定をとる
					isHit = IsHitRectAndLine(_bodyRect, startPos, endPos);
				}


				_stand = 1;
				_gravity = 0;
			}
		}

		// 体の中心位置を更新
		UpdateCenterPos();
		// 当たり判定矩形を更新
		SetCollision();
	}
}



void CharaBase::UpdateCenterPos() {
	_centerPos = _pos;
	_centerPos.y -= _difCenterY;
}


void CharaBase::SetCollision() {
	_bodyRect = SetMyRect(_difBodyRect, _pos);
}

// 当たり判定矩形を生成する
// 引数
// difRect: キャラ当たり判定(配置座標からの差分 x,y / 大きさ w,h)
// pos : 起点座標
MYRECT CharaBase::SetMyRect(MYRECT difRect, VECTOR pos) {
	MYRECT rc = { 0, 0, 0, 0 };
	MYRECT h = { 0, 0, 0, 0 };
	VECTOR tmpPos = { 0, 0, 0 };


	h = difRect;
	tmpPos = pos;

	if (_use == 1) {
		rc.x = tmpPos.x + h.x;
		rc.y = tmpPos.y + h.y;
		rc.w = h.w;
		rc.h = h.h;
	}
	return rc;
}

// 当たり判定矩形を取得する
MYRECT CharaBase::GetMyRect(int collisionType) {
	MYRECT tmpRect = { 0, 0, 0, 0 };
	switch (collisionType)
	{
	case COLLISION_TYPE_BODY:
		tmpRect = _bodyRect;
	}

	return tmpRect;
}

// 3Dモデル表示用座標の更新
void CharaBase::UpdateWorldPos()
{
	// スクリーン座標をワールド座標に変換
	_worldPos = ConvScreenPosToWorldPos(_screenPos);
	_worldPos.z = -100; // z軸表示位置の調整
}

// 描画処理
void CharaBase::Draw() {
	//// ３Ｄモデルの描画
	MV1DrawModel(_modelHandle);
}

// モーション情報の初期化処理
void CharaBase::MotionInit(int motionMax)
{
	// 複数のアニメーションをアタッチしている状態だと3Dモデルの状態がブレンドされたものになる
	// ブレンドさせたくない場合は
	// ①1つのモーションだけをアタッチする（他のモーションを外すにはデタッチをする）　※DxLibではこちらを想定している
	// ②再生したくないモーションのブレンドレートを0にする (使用メモリや処理負荷が多少上昇する) 
	// MV1SetAttachAnimBlendRate
	
	// ここでは①の方法をとる
	// 以下のループは totalTime を取るために行う
	for (int i = 0; i < motionMax; i++) {
		//アニメーションのアタッチ
		_anim[i].index = MV1AttachAnim(_modelHandle, i, -1, FALSE);
		// アタッチしたアニメーションの総再生時間を取得する
		_anim[i].totalTime = MV1GetAttachAnimTotalTime(_modelHandle, _anim[i].index);
		if (i != _motionNum) {
			// アニメーションのデタッチ
			MV1DetachAnim(_modelHandle, _anim[i].index);
		}
	}
}

// モーションの計算処理
void CharaBase::MotionProcess(int motionNum_old)
{
	_playTime += 1;
	if (_playTime >= _anim[_motionNum].totalTime) {
		_playTime = 0;
	}

	// モーションが変更された場合に、変更前のモーションをデタッチして変更後のモーションをアタッチする
	if (motionNum_old != _motionNum) {
		// 変更前のモーションのデタッチ
		MV1DetachAnim(_modelHandle, _anim[motionNum_old].index);
		// 変更後のモーションのアタッチ
		_anim[_motionNum].index = MV1AttachAnim(_modelHandle, _motionNum, -1, FALSE);

		_playTime = 0;
	}


	// 再生時間をセットする(アニメーションの再生)
	MV1SetAttachAnimTime(_modelHandle, _anim[_motionNum].index, _playTime);



}

// 描画位置の補正処理
void CharaBase::AdjustDrawPos(MATRIX mView)
{
	_screenPos = _pos;
	_screenPos = VTransform(_screenPos, mView);
	UpdateWorldPos();
	// モデル表示座標の反映
	MV1SetPosition(_modelHandle, _worldPos);
}

// 3Dモデルの回転処理
void CharaBase::UpdateModelRotation()
{
	// 回転処理
	float rad;
	//= old_rad;
	if (VSize(_direction) != 0) {
		////VGet(0, -1, 0)を起点とした回転角を計算
		//VECTOR vOrigin = VGet(0, -1, 0);

		// VGet(0, 1, 0)を起点とした回転角を計算
		VECTOR vOrigin = VGet(0, 1, 0);
		float dot = VDot(vOrigin, VGet(1, 0, 0));
		float cos = dot / (VSize(vOrigin) * VSize(_direction));
		rad = acos(cos);

		//起点から左に回転する場合
		if (_direction.x > 0) {
			rad *= -1;
		}
	}
	//回転処理の反映
	MV1SetRotationXYZ(_modelHandle, VGet(0.0f, rad, 0.0f));
}



// テクスチャ画像の貼り替え
// ※ 関数内で継承先のクラスの静的メンバ変数にアクセスするので, 継承先ごとに関数定義をする
void CharaBase::ChangeTexture(int isDamaged)
{
}

// _modelHandleのテクスチャ番号indexのテクスチャ画像をgraphHandleの画像に差し替える
void CharaBase::SetTexture(int index, int graphHandle) {
	MV1SetTextureGraphHandle(_modelHandle, index, graphHandle, FALSE);
}

// マテリアルのディフューズカラー（拡散光色）の変更
// ※ 関数内で継承先のクラスの静的メンバ変数にアクセスするので, 継承先ごとに関数定義をする
void CharaBase::ChangeMaterialDifColor(int isDamaged)
{
}

// _modelHandleのマテリアル番号indexのディフューズカラー（拡散光色）を変更する
void CharaBase::SetMaterialDifColor(int index, COLOR_F difColor)
{
	MV1SetMaterialDifColor(_modelHandle, index, difColor);
}



// 前面レイヤーに描画
void CharaBase::DrawFrontLayer(MATRIX mView)
{
}

// デバッグ情報の表示
void CharaBase::DrawDebugInfo(MATRIX mView, int fillFrag)
{
}
