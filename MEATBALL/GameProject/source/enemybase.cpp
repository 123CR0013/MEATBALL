#include "enemybase.h"
#include <cmath>
#include "winmain.h"

// 敵キャラクターの基底クラス
EnemyBase::EnemyBase(int modelHandle, int index, VECTOR startPos, VECTOR direction) : CharaBase(modelHandle, startPos)
{
	SetUse(1);

	_type = -1;
	_index = -1;
	_hp = 0;
	_isInScreen = 0;
	_isSpotPlayer = 0;

	_speed = 8;
	_direction = VGet(-1, 0, 0);

	_difBodyRect = { 0, 0, 0, 0};
	_difSearchRect = { 0, 0, 0, 0};
	_difAttackRect = { 0, 0, 0, 0};

	_bodyRect = { -1, -1, -1, -1 };
	_searchRect = { -1, -1, -1, -1 };
	_attackRect = { -1, -1, -1, -1};

	_isAttack = 0;

	// 画面内にいるかを判定
	CheckIsInScreen();
	// 非索敵状態にする
	SetIsSpotPlayer(0);


	_attackRemainTime = 0;
	_attackCoolTime = 0;


	_isHitCnt = 0;
	_isHitStop = 0;
	_hitStopCnt = 0;
	_hitStopDir = VGet(0, 0, 0);
}

// 敵の強さを初期設定する
// Small, Medium, Large
void EnemyBase::SetStregth(int type, int size)
{
	auto eSizeInfo = GetEnemySizeInfo(type, size);
	_hp =eSizeInfo.hp;
	MV1SetScale(_modelHandle,eSizeInfo.modelSize);
	_difBodyRect =eSizeInfo.difBodyRect;
	_difSearchRect =eSizeInfo.difSearchRect;
	_difAttackRect =eSizeInfo.difAttackRect;
}




EnemyBase::~EnemyBase()
{
	//CharaBase::~CharaBase();
}

// 計算処理
void EnemyBase::Process(VECTOR playerPos) {
	// この敵が倒されていなかったら
	if (_use == 1) {
		if (_isHitStop == 0) {
			// 共通処理
			// 重力処理
			CharaBase::Process();

			// 画面内にいるかを判定
			CheckIsInScreen();
		}
		else {
			// ヒットストップ処理
			HitStopProcess();
		}
	}
}

// 当たり判定矩形を取得する
MYRECT EnemyBase::GetMyRect(int collisionType)
{
	MYRECT tmpRect = { 0, 0, 0, 0 };
	switch (collisionType)
	{
	case COLLISION_TYPE_SEARCH:
		tmpRect = _searchRect;
		break;
	case COLLISION_TYPE_ATTACK:
		tmpRect = _attackRect;
		break;
	default:
		tmpRect = CharaBase::GetMyRect(collisionType);
		break;
	}
	return tmpRect;
}

// 当たり判定矩形を更新する
void EnemyBase::SetCollision()
{
	CharaBase::SetCollision();

	VECTOR tmpPos = { 0, 0, 0 };

	switch (_type)
	{
	case ENEMY_TYPE_SHOOTING:
		tmpPos = _centerPos;
		break;
	case ENEMY_TYPE_GROUND:
		tmpPos = _pos;
		_attackRect = SetMyRect(_difAttackRect, tmpPos);
		break;
	case ENEMY_TYPE_SHIELD:
		tmpPos = _pos;
		_attackRect = SetMyRect(_difAttackRect, tmpPos);
		tmpPos = _centerPos;
	case ENEMY_TYPE_TRACER:
		tmpPos = _centerPos;
		break;
	};

	_searchRect = SetMyRect(_difSearchRect, tmpPos);
}


// 画面内にいるかを判定する
void EnemyBase::CheckIsInScreen()
{
	if (0 < _screenPos.x && _screenPos.x < SCREEN_W && 0 < _screenPos.y && _screenPos.y < SCREEN_H) {
		_isInScreen = 1;
	}
	else {
		_isInScreen = 0;
	}
}

// 描画処理
void EnemyBase::Draw() {
	// この敵が倒されていなかったら
	if (_use == 1) {
		if (_isHitCnt > 0)  {
			ChangeTexture(1);
			ChangeMaterialDifColor(1);
			_isHitCnt--;
			if (_isHitCnt <= 0) {
				ChangeTexture(0);
				ChangeMaterialDifColor(0);
				_isHitCnt = 0;
			}
		}

		CharaBase::Draw();

		
	}
}

// ヒットストップ処理
void EnemyBase::HitStopProcess()
{
	// _hitStopDirの方向（またはその反対方向）に揺らす
	if (_hitStopCnt % 2 == 0) {
		VECTOR tmpDir = _hitStopDir;
		if (_hitStopCnt % 4 == 2) {
			tmpDir = VScale(tmpDir, -1);
		}

		// 移動処理
		tmpDir = VNorm(tmpDir);
		VECTOR vMove = VScale(tmpDir, 30);
		_pos.x += vMove.x;
		_pos.y += vMove.y;
	}

	_hitStopCnt--;
	if (_hitStopCnt <= 0) {
		SetUse(0);
	}
}


// ワールド座標を更新する
void EnemyBase::UpdateWorldPos()
{
	// スクリーン座標をワールド座標に変換
	_worldPos = ConvScreenPosToWorldPos(_screenPos);
	_worldPos.z = -100; // z軸表示位置の調整
}



// デバッグ情報の表示
void EnemyBase::DrawDebugInfo(MATRIX mView, int fillFrag)
{
	if (_use == 1) {
		VECTOR rectPos;

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 140.f);

		
		// 索敵範囲の描画
		if (_searchRect.x != -1) {
			rectPos = VGet(_searchRect.x, _searchRect.y, 0);
			rectPos = VTransform(rectPos, mView);
			DrawBox(rectPos.x, rectPos.y, rectPos.x + _searchRect.w, rectPos.y + _searchRect.h, COLOR_GREEN, fillFrag);
		}
		// 攻撃範囲の描画
		if (_attackRect.x != -1) {
			rectPos = VGet(_attackRect.x, _attackRect.y, 0);
			rectPos = VTransform(rectPos, mView);
			DrawBox(rectPos.x, rectPos.y, rectPos.x + _attackRect.w, rectPos.y + _attackRect.h, COLOR_RED, fillFrag);
		}
		// 本体の当たり判定矩形の描画
		if (_bodyRect.x != -1) {
			rectPos = VGet(_bodyRect.x, _bodyRect.y, 0);
			rectPos = VTransform(rectPos, mView);
			DrawBox(rectPos.x, rectPos.y, rectPos.x + _bodyRect.w, rectPos.y + _bodyRect.h, COLOR_BLUE, fillFrag);
		}


		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);



		//DrawFormatString(rectPos.x, rectPos.y, COLOR_WHITE, "dir: x %3.2f, y %3.2f", _direction.x, _direction.y);
		DrawFormatStringToHandle(rectPos.x, rectPos.y, COLOR_WHITE, fontHandle, "%d", _hp);
	}
}