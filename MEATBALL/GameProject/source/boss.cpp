#include "boss.h"
#include "mapdata.h"
#include "winmain.h"
#include "mydraw.h"
#include "effect.h"
#include "resource.h"


// モーション用定数
#define BOSS_MOTION_RUN 0

#define BOSS_MOTION_WAIT_LEFT 1
#define BOSS_MOTION_WAIT_RIGHT 2

#define BOSS_MOTION_JUMP_UP 3
#define BOSS_MOTION_JUMP_DOWN 4

#define BOSS_MOTION_SHOOT_WAIT_LEFT 5
#define BOSS_MOTION_SHOOT_WAIT_RIGHT 6

#define BOSS_MOTION_SHOOT_LEFT 7
#define BOSS_MOTION_SHOOT_RIGHT 8

#define BOSS_MOTION_ABILITY_LEFT 9
#define BOSS_MOTION_ABILITY_RIGHT 10

#define BOSS_MOTION_DASH 11

#define BOSS_MOTION_HIT_LEFT 12
#define BOSS_MOTION_HIT_RIGHT 13

#define BOSS_MOTION_ELIMINATED_LEFT 14
#define BOSS_MOTION_ELIMINATED_RIGHT 15


// 攻撃が終了してから次の攻撃に移るまでのフレーム数
#define ATTACK_COOLTIME_MAX 90

// 無敵時間
#define BOSS_INVINCIBLE_CNT_MAX 15


// テクスチャ情報
std::vector<TEXTURE>	Boss:: _texture;
// マテリアル情報
std::vector<MATERIAL>	Boss:: _material;

// ボス戦用BGM
int	 Boss::_bgmBossBattle;

Boss::Boss(int modelHandle, VECTOR startPos) : CharaBase(modelHandle, startPos)
{
	// 大きさの調整
	MV1SetScale(_modelHandle, VGet(1.8f * 1.1f, 1.8f * 1.1f, 1.8f * 1.1f));
	_direction = VGet(-1, 0, 0);

	// 初期状態のモーションの設定
	_motionNum = BOSS_MOTION_WAIT_RIGHT;

	// 複数のアニメーションをアタッチしている状態だと3Dモデルの状態がブレンドされたものになる
	// ブレンドさせたくない場合は
	// ①1つのモーションだけをアタッチする（他のモーションを外すにはデタッチをする）　※DxLibではこちらを想定している
	// ②再生したくないモーションのブレンドレートを0にする (使用メモリや処理負荷が多少上昇する) 
	// MV1SetAttachAnimBlendRate
	
	// ここでは①の方法をとる
	// 以下のループは totalTime を取るために行う
	for (int i = 0; i < BOSS_MOTION_MAX; i++) {
		//アニメーションのアタッチ
		_anim[i].index = MV1AttachAnim(_modelHandle, i, -1, FALSE);
		// アタッチしたアニメーションの総再生時間を取得する
		_anim[i].totalTime = MV1GetAttachAnimTotalTime(_modelHandle, _anim[i].index);
		if (i != _motionNum) {
			// アニメーションのデタッチ
			MV1DetachAnim(_modelHandle, _anim[i].index);
		}
	}

	// アウトラインの初期設定
	for (int i = 0; i < 6; i++) {
		MV1SetMaterialOutLineWidth(_modelHandle, i, 1);
		MV1SetMaterialOutLineColor(_modelHandle, i, GetColorF(0.8, 0.5, 1, 1));

	}

	_difBodyRect = { -32, -128, 64, 120 };
	_useGravity = 1;

	_difCenterY = 60;

	// 体の中心位置を更新
	UpdateCenterPos();
	// 当たり判定矩形を更新
	SetCollision();


	_startPos = VGet(0, 0, 0);

	_use = 1;

	_isBossBattle = 0;
	_hp = 10000;
	_invincibleCnt = 0;





	_isAttack = 0;
	_changeAttack = 0;
	_attackCnt = 0;
	_attackInfo.id = -1;
	_attackCoolTime = 46;
	// 攻撃パターンをランダムに決める
	//SetAttackInfo();



	_useGravity = 1;


	_isEliminated = 0;
	_isHit = 0;


	_auraAnimCnt = 0;
};


// 残像の初期化処理
void Boss::AfterImageInit(int modelHandle)
{
	VECTOR modelScale = MV1GetScale(_modelHandle);

	for (int i = 0; i < BOSS_AFTERIMAGE_MAX; i++) {
		_afterImage[i].use = 0;
		_afterImage[i].frame = 0;
		_afterImage[i].remainCnt = 0;

		_afterImage[i].modelHandle = MV1DuplicateModel(modelHandle);
		MV1SetScale(_afterImage[i].modelHandle, modelScale);

		_afterImage[i].pos = VGet(0, 0, 0);

		//アニメーションのアタッチ
		// プレイヤーのモデルをもとにしているので、アニメーションはボスのアニメーション用定数を使わずに直接入力している
		// アニメーション番号13は 「PLAYER_MOTION_DASH」
		_afterImage[i].anim.index = MV1AttachAnim(_afterImage[i].modelHandle, 13, -1, FALSE);
		// アタッチしたアニメーションの総再生時間を取得する
		_afterImage[i].anim.totalTime = MV1GetAttachAnimTotalTime(_afterImage[i].modelHandle, _afterImage[i].anim.index);


		// アウトラインの初期設定
		for (int j = 0; j < 6; j++) {
			MV1SetMaterialOutLineWidth(_afterImage[i].modelHandle, j, 1);
			MV1SetMaterialOutLineColor(_afterImage[i].modelHandle, j, GetColorF(0.8, 0.7, 1, 0.1f));

		}


		_afterImage[i].alpha = 1;
	}



}

// 残像の計算処理
void Boss::AfterImageProcess()
{
	for (int i = 0; i < BOSS_AFTERIMAGE_MAX; i++) {
		if (_afterImage[i].use == 1) {

			// 毎フレーム透明度をあげる
			_afterImage[i].alpha -= 0.1f;
			for (int j = 0; j < 6; j++) {
				MV1SetMaterialDifColor(_afterImage[i].modelHandle, j, GetColorF(1, 1, 1, _afterImage[i].alpha));
			}

			// 残りカウントを減らす
			_afterImage[i].remainCnt--;
			// 残像の残りカウントが0を下回ったら残像を削除する
			if (_afterImage[i].remainCnt < 0) {
				_afterImage[i].use = 0;
			}
		}
	}

}

// 残像をセットする
void Boss::SetAfterImage()
{
	for (int i = 0; i < BOSS_AFTERIMAGE_MAX; i++) {
		if (_afterImage[i].use == 1) continue;
		_afterImage[i].use = 1;
		_afterImage[i].remainCnt = 10;

		_afterImage[i].pos = _pos;
		VECTOR modelRot = MV1GetRotationXYZ(_modelHandle);
		MV1SetRotationXYZ(_afterImage[i].modelHandle, modelRot);
		// 再生時間をセットする(アニメーションの再生)
		float afterImagePlayTime = _playTime;
		MV1SetAttachAnimTime(_afterImage[i].modelHandle, _afterImage[i].anim.index, afterImagePlayTime);

		_afterImage[i].alpha = 1;
		break;
	}
}

Boss::~Boss()
{
	//CharaBase::~CharaBase();
}

// 計算処理
void Boss::Process(VECTOR playerPos)
{
	if (_use == 1) {
		int motionNum_old = _motionNum;
		int gravity_old = _gravity;




		// 重力処理
		CharaBase::Process();


		// ボス戦中なら
		if (_isBossBattle == 1) {

			if (_isEliminated != 1) {
				// 撃破されたときの設定
				if (_hp <= 0) {
					_motionNum = BOSS_MOTION_ELIMINATED_RIGHT;
					_isEliminated = 1;
					_useGravity = 1;

					// 出現中の弾を全て消す
					DeleteAllBullet();

					// 出現中のバリアチップを消す
					gMap.VanishBossBarrier();


					// テクスチャを元に戻す
					ChangeTexture(0);
					// マテリアルのディフューズカラーを元に戻す
					ChangeMaterialDifColor(0);

					// アウトラインを消す
					for (int i = 0; i < 6; i++) {
						MV1SetMaterialOutLineWidth(_modelHandle, i, 0);
					}

					// ボス戦用BGMを停止する
					PlayBossBGM(0);
				}
			}
			else {
				_isHit = 0;
			}


			if (_attackCoolTime == 0 || _attackCoolTime > 45) {
				// 着地処理
				if (gravity_old >= 3 && _stand == 1) {
					// 煙エフェクトのセット
					VECTOR effectPos = _pos;
					effectPos.y -= 20;
					float effectSize = 160;
					int effectX = effectPos.x - effectSize / 2;
					int effectY = effectPos.y - effectSize / 2;
					SetEffect(EFFECT_TYPE_SMOKE, effectX, effectY, effectSize, effectSize);

					// 着地SEの再生
					PlaySoundMem(se[12], DX_PLAYTYPE_BACK);
				}
			}

			// 撃破されていなかったら
			if (_isEliminated != 1) {
				if (_stand == 0) {
					_motionNum = BOSS_MOTION_JUMP_DOWN;
					_isHit = 0;
				}
				else {
					_motionNum = BOSS_MOTION_WAIT_RIGHT;
					if (_isHit == 1) {
						_motionNum = BOSS_MOTION_HIT_RIGHT;
					}
				}

				// 弾の計算処理
				BulletProcess();

				// 攻撃中の処理
				if (_isAttack == 1) {
					_attackCnt++;

					// 攻撃を終了させる処理
					if (_attackCnt == _attackInfo.frame) {
						_changeAttack = 1;
						_isAttack = 0;

						// 出現中の弾を全て消す
						DeleteAllBullet();
						// 出現中のバリアチップを消す
						gMap.VanishBossBarrier();

						// バリアチップ出現SEの再生
						PlaySoundMem(se[19], DX_PLAYTYPE_BACK);

						_gravity = 0;
					}

				}
				// 非攻撃中の処理
				else {

					// 攻撃後、次の攻撃に移るまでの処理
					if (_attackCoolTime > 0) {
						_attackCoolTime--;

						// 次の攻撃までのフレーム数が45フレームになったら、次の攻撃の情報をセットし移動を開始する
						if (_attackCoolTime == 45) {
							// 次の攻撃の情報をセットする
							SetAttackInfo(playerPos);

							// 次の攻撃地点までの移動に必要な情報を保持する
							_startPos = _pos;
							float endX = _attackInfo.pos.x * CHIPSIZE_W + _bodyRect.w / 2;
							if (_startPos.x <= endX) {
								_direction = VGet(1, 0, 0);
							}
							else {
								_direction = VGet(-1, 0, 0);
							}
						}

						// 次の攻撃地点まで45フレームかけて移動する
						if (_attackCoolTime <= 45) {
							_useGravity = 0;
							_motionNum = BOSS_MOTION_DASH;

							// 残像をセットする
							SetAfterImage();

							// 次の地点までの移動処理
							int cnt = 45 - _attackCoolTime;
							VECTOR endPos = {
								_attackInfo.pos.x * CHIPSIZE_W + _bodyRect.w / 2,
								_attackInfo.pos.y * CHIPSIZE_H + _bodyRect.h,
								0
							};
							_pos.x = EasingInOutSine(cnt, _startPos.x, endPos.x, 45);
							_pos.y = EasingInOutSine(cnt, _startPos.y, endPos.y, 45);


							// 体の中心位置を更新
							UpdateCenterPos();
							// 当たり判定矩形を更新
							SetCollision();
						}


						// 攻撃地点に到着したら、パターンごとに設定をする
						if (_attackCoolTime == 0) {
							_isAttack = 1;
							_useGravity = 0;
							_stand = 0;

							// 弾の初期化処理
							BulletInit();

							switch (_attackInfo.id)
							{
							case 0:
								_useGravity = 1;
								break;
								// 中央バリア
							case 1:
							case 2:
								_useGravity = 1;
								break;
								// 左側
							case 3:
								_direction = VGet(1, 0, 0);
								break;
								// 右側
							case 4:
								_direction = VGet(-1, 0, 0);
								break;
							}
						}




					}
				}

				// 攻撃が終了したら、次の攻撃までの準備をする
				if (_changeAttack == 1) {
					_changeAttack = 0;
					_attackCnt = 0;
					_attackCoolTime = ATTACK_COOLTIME_MAX;
					_useGravity = 1;
				}

			}

		}


		// 無敵中の処理
		if (_invincibleCnt > 0) {
			_invincibleCnt--;
			// 無敵時間が終わったら
			if (_invincibleCnt == 0) {
				// テクスチャを元に戻す
				ChangeTexture(0);
				// マテリアルのディフューズカラーを元に戻す
				ChangeMaterialDifColor(0);
			}
		}


		// 3Dモデルの回転処理
		UpdateModelRotation();

		// キャラクターの向いている向きによってモーションを変更する
		SetMotionReverse();
		// モーションの計算処理
		MotionProcess(motionNum_old);

		// 残像の計算処理
		AfterImageProcess();
	}
}

// 弾の計算処理
void Boss::BulletProcess()
{
	// 弾の発射処理
	for (int i = 0; i < BOSS_BULLET_MAX; i++) {
		if (_attackInfo.bullet[i].use == -1) break; // use == -1は未使用配列

		// 反射弾
		// shotFrame == _attackCnt の時に弾を出現させる
		if (_attackInfo.bulletType == 0) {
			if (_isAttack == 1) {
				if (_attackInfo.bullet[i].shotFrame == _attackCnt) {
					_attackInfo.bullet[i].use = 1;
				}
			}
		}

		// この配列要素が仕様中なら
		if (_attackInfo.bullet[i].use == 1) {
			if (_attackInfo.bullet[i].shotFrame <= _attackCnt) {
				VECTOR vDir = VNorm(_attackInfo.bullet[i].direction);
				VECTOR vMove = VScale(vDir, _attackInfo.bullet[i].speed);
				_attackInfo.bullet[i].pos.x += vMove.x;
				_attackInfo.bullet[i].pos.y += vMove.y;

				// 弾がマップチップに当たったかを調べる
				MYRECT bulletRect = { _attackInfo.bullet[i].pos.x, _attackInfo.bullet[i].pos.y, 60, 60 };
				HITCHIP hitChip = gMap.IsHitBoxAndMapChip(bulletRect);

				// 当たっていた場合
				if (hitChip.chip_no != -1) {

					// 反射弾の場合、反射後の移動方向を計算する
					if (_attackInfo.bulletType == 0) {

						// 弾の当たり判定の上下左右の4点がマップチップに当たったかを調べる
						// 上下左右の4点がマップチップに当たっていなかった場合、弾の当たり判定矩形の角がマップチップに当たったとみなす

						VECTOR bulletPos[4] = {
						{_attackInfo.bullet[i].pos.x + 30, _attackInfo.bullet[i].pos.y		  , 0},
						{_attackInfo.bullet[i].pos.x + 30, _attackInfo.bullet[i].pos.y + 60, 0},
						{_attackInfo.bullet[i].pos.x		   , _attackInfo.bullet[i].pos.y + 30, 0},
						{_attackInfo.bullet[i].pos.x + 60, _attackInfo.bullet[i].pos.y + 30, 0},
						};

						// 押し戻してから反射方向の設定をする
						// 押し戻した際に当たったチップから抜け出せなくなるのを防ぐために * 2 をする
						_attackInfo.bullet[i].pos.x -= vMove.x;
						_attackInfo.bullet[i].pos.y -= vMove.y;

						int isHitCorner = 1;

						// 弾の上下の点がマップチップに当たったかを調べる
						for (int j = 0; j < 2; j++) {
							int isHit = gMap.IsHitMapchip(bulletPos[j].x, bulletPos[j].y);
							if (isHit != -1) {
								_attackInfo.bullet[i].direction.y = -_attackInfo.bullet[i].direction.y;
								isHitCorner = 0;
							}
						}

						// 弾の左右の点がマップチップに当たったかを調べる
						for (int j = 2; j < 4; j++) {
							int isHit = gMap.IsHitMapchip(bulletPos[j].x, bulletPos[j].y);
							if (isHit != -1) {
								_attackInfo.bullet[i].direction.x = -_attackInfo.bullet[i].direction.x;
								isHitCorner = 0;
							}
						}

						// 弾の当たり判定矩形の角がマップチップに当たった場合
						if (isHitCorner == 1) {
							_attackInfo.bullet[i].direction.x = -_attackInfo.bullet[i].direction.x;
							_attackInfo.bullet[i].direction.y = -_attackInfo.bullet[i].direction.y;
						}

					}
					// 設置弾の場合、その弾を消す
					else {
						_attackInfo.bullet[i].use = 0;
					
						// 弾破裂エフェクトのセット
						int effectX = bulletRect.x - bulletRect.w;
						int effectY = bulletRect.y - bulletRect.h;
						int effectSize = bulletRect.w * 3;
						SetEffect(EFFECT_TYPE_BOSS_BULLET_HIT, effectX, effectY, effectSize, effectSize);
					}
				}
			}
		}
	}
}

// 弾の初期化処理
void Boss::BulletInit()
{
	for (int i = 0; i < BOSS_BULLET_MAX; i++) {
		if (_attackInfo.bullet[i].use == -1) break;
		if (_attackInfo.bulletType != 0) {
			_attackInfo.bullet[i].use = 1;
		}
		_attackInfo.bullet[i].pos.x *= CHIPSIZE_W;
		_attackInfo.bullet[i].pos.y *= CHIPSIZE_H;
	}
}






// 攻撃情報を参照してバリアチップを出現させる
void Boss::SetBarrier()
{
	// バリアチップ出現SEの再生
	PlaySoundMem(se[19], DX_PLAYTYPE_BACK);

	// バリアチップを出現させる
	for (int i = 0; i < BOSS_BLOCK_MAX; i++) {
		if (_attackInfo.blockPos[i].x == -1) break;
		gMap.SetBossBarrier(_attackInfo.blockPos[i]);
	}
}


// キャラクターの向いている向きによってモーションを変更する
// 
// Process処理内でモーションを指定するときは全て右向きのモーションを指定する
// キャラクターが左を向いている場合は、左向きのモーションに変更する
void Boss::SetMotionReverse()
{
	switch (_motionNum)
	{
	default:
		break;

	case BOSS_MOTION_WAIT_RIGHT:
	case BOSS_MOTION_SHOOT_WAIT_RIGHT:
	case BOSS_MOTION_SHOOT_RIGHT:
	case BOSS_MOTION_ABILITY_RIGHT:
	case BOSS_MOTION_HIT_RIGHT:
	case BOSS_MOTION_ELIMINATED_RIGHT:

		if (_direction.x < 0) {
			_motionNum--;
		}
		break;
	}
}


// モーションの計算処理
void Boss::MotionProcess(int motionNum_old)
{
	_playTime += 1.5;
	
	

	// モーションが変更された場合に、変更前のモーションをデタッチして変更後のモーションをアタッチする
	if (motionNum_old != _motionNum) {
		// 変更前のモーションのデタッチ
		MV1DetachAnim(_modelHandle, _anim[motionNum_old].index);
		// 変更後のモーションのアタッチ
		_anim[_motionNum].index = MV1AttachAnim(_modelHandle, _motionNum, -1, FALSE);

		_playTime = 0;
	}
	else {
		// 現在のモーションを最後まで再生したら
		if (_playTime >= _anim[_motionNum].totalTime) {
			_playTime = 0;
			switch (_motionNum)
			{
			case BOSS_MOTION_JUMP_UP:
				_playTime = 5.0f;
				break;
			case BOSS_MOTION_JUMP_DOWN:
				_playTime = 10.0f;
				break;
			case BOSS_MOTION_DASH:
				_playTime = 5.0f;
				break;
			case BOSS_MOTION_HIT_LEFT:
			case BOSS_MOTION_HIT_RIGHT:
				_isHit = 0;

				_motionNum = BOSS_MOTION_WAIT_RIGHT;
				SetMotionReverse();

				// 変更前のモーションのデタッチ
				MV1DetachAnim(_modelHandle, _anim[motionNum_old].index);
				// 変更後のモーションのアタッチ
				_anim[_motionNum].index = MV1AttachAnim(_modelHandle, _motionNum, -1, FALSE);
				break;
			case BOSS_MOTION_ELIMINATED_LEFT:
			case BOSS_MOTION_ELIMINATED_RIGHT:
				_playTime = _anim[_motionNum].totalTime;
				break;
			}
		}
	}


	// 再生時間をセットする(アニメーションの再生)
	MV1SetAttachAnimTime(_modelHandle, _anim[_motionNum].index, _playTime);



}




// 弾の当たり判定矩形を返す
// 
// 引数「index」が未使用要素やすでに消えた弾をさす場合、矩形のxにそれを判別する値を入れ、返した先でその値によって処理を行う
// 未使用要素をさす場合は、-1
// 消えた弾をさす場合は、    0
MYRECT Boss::GetBulletRect(int index)
{
	MYRECT bulletRect = { -1, -1, -1, -1};
	if (0 <= index && index < BOSS_BULLET_MAX) {
		switch (_attackInfo.bullet[index].use)
		{
		case -1:
			//bulletRect.x = -1;
			break;
		case 0:
			bulletRect.x = 0;
			break;

		default:
			bulletRect = { _attackInfo.bullet[index].pos.x, _attackInfo.bullet[index].pos.y, 	60, 60 };
				break;
		}
	}
	return bulletRect;
}

// 描画位置の補正処理
void Boss::AdjustDrawPos(MATRIX mView)
{
	CharaBase::AdjustDrawPos(mView);

	for (int i = 0; i < BOSS_AFTERIMAGE_MAX; i++) {
		if (_afterImage[i].use == 1) {
			VECTOR tmpPos = _afterImage[i].pos;
			tmpPos = VTransform(tmpPos, mView);
			tmpPos = ConvScreenPosToWorldPos(tmpPos);
			//tmpPos.z = 100 * (45 - _afterImage[i].remainCnt);
			tmpPos.z = 100;
			MV1SetPosition(_afterImage[i].modelHandle, tmpPos);
		}
	}

}

// 描画処理
void Boss::Draw()
{
	if (_use == 1) {
		if (_isEliminated == 0) {

			// オーラ描画処理
			// オーラはキャラクターの前面と背面に重ねて描画する
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 140.f);
			VECTOR auraPos = _centerPos;
			auraPos.y -= 20;
			MyDrawModiGraph(gCam.GetViewMatrix(), auraPos, 1.0f, 0.0f, _bodyRect.w * 6, _bodyRect.h * 3.5, cgAttackAura[(_auraAnimCnt / 4) % 20]);
			
			// 残像の描画
			for (int i = 0; i < BOSS_AFTERIMAGE_MAX; i++) {
				if (_afterImage[i].use == 1) {
					MV1DrawModel(_afterImage[i].modelHandle);
				}
			}
			CharaBase::Draw();

			// オーラ描画処理
			MyDrawModiGraph(gCam.GetViewMatrix(), auraPos, 1.0f, 0.0f, _bodyRect.w * 6, _bodyRect.h * 3.5, cgAttackAura[(_auraAnimCnt / 2) % 20]);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			_auraAnimCnt++;
		}
		else {
			CharaBase::Draw();
		}
	}
}



// 弾の描画処理
void Boss::DrawBullet(MATRIX mView)
{
	for (int i = 0; i < BOSS_BULLET_MAX; i++) {
		if (_attackInfo.bullet[i].use == -1) break; // use == -1は未使用配列

		if (_attackInfo.bullet[i].use == 1) {
			VECTOR bPos = _attackInfo.bullet[i].pos;
			bPos.x += 64 / 2;
			bPos.y += 64 / 2;
			MyDrawModiGraph(mView, bPos, 1.0f, 0.0f, 64, 64, cgBossBullet[(_attackCnt / 1) % 30]);
		}
	}
}

// 前面レイヤーに描画
void Boss::DrawFrontLayer(MATRIX mView)
{
	// 弾の描画処理
	DrawBullet(mView);
}




// ボスの攻撃を開始する
void Boss::SetBossBattle()
{
	if (_isEliminated == 0) { 
		//_use = 1; 
		_isBossBattle = 1;
		//SetAttackInfo(VGet(0, 0, 0));
		//PlaySoundMem(_bgmBossBattle, DX_PLAYTYPE_LOOP);
		PlayBossBGM(1);
	} 
}




// 次の攻撃情報をセットする
void Boss::SetAttackInfo(VECTOR playerPos)
{
	// 出現している弾を全て消す
	DeleteAllBullet();
	// 攻撃パターンをランダムに決める
	RandAttackPattern(playerPos);
	// 攻撃情報を参照してバリアチップを出現させる
	SetBarrier();
}


void Boss::SetIsHit(float damage)
{
	if (_isEliminated != 1) {

		if (_invincibleCnt == 0) {
			_isHit = 1;
			_invincibleCnt = BOSS_INVINCIBLE_CNT_MAX;

			UpdateHp(damage);
			ChangeTexture(1);
			ChangeMaterialDifColor(1);
		}
	}
}

// 出現している弾を全て消す
void Boss::DeleteAllBullet()
{
	for (int i = 0; i < BOSS_BULLET_MAX; i++) {
		if (_attackInfo.bullet[i].use == -1) break;
		if (_attackInfo.bullet[i].use == 1) {
			_attackInfo.bullet[i].use = 0;

			

			// ボス弾破裂エフェクトのセット
			MYRECT bulletRect = { _attackInfo.bullet[i].pos.x, _attackInfo.bullet[i].pos.y, 60, 60 };
			int effectX = bulletRect.x - bulletRect.w;
			int effectY = bulletRect.y - bulletRect.h;
			int effectSize = bulletRect.w * 3;
			SetEffect(EFFECT_TYPE_BOSS_BULLET_HIT, effectX, effectY, effectSize, effectSize);
		}
	}
}

// 攻撃パターンをランダムに決める
void Boss::RandAttackPattern(VECTOR playerPos)
{
	BOSS_ATTACK bossAttack[BOSS_ATTACK_MAX] = {
		// pattern1
		{
			// id
			0,
			// frame
			400,
			// pos
			VGet(224.5f, 38, 0),
			// blockPos
			{
				VGet(222, 39, 0),
				VGet(223, 39, 0),
				VGet(224, 39, 0),
				VGet(225, 39, 0),
				VGet(226, 39, 0),
				VGet(227, 39, 0),
				VGet(-1, -1, -1),
			},

			// bulletType
			// 反射弾
			0,

			// bullet
			{
			// use, shotFrame, speed, pos, direction
			{ 0, 0,	18, VGet(224.5f, 37, 0), VGet(-1, 1.73, 0)},
			{ 0, 0, 18,	VGet(224.5f, 37, 0), VGet(1, 1.73, 0)},

			//{ 0, 400,	18, VGet(224.5f, 37, 0), VGet(-1, 2.0, 0)},
			//{ 0, 400, 18,	VGet(224.5f, 37, 0), VGet(1, 2.0, 0)},


			{-1, -1, -1, VGet(-1, -1, -1), VGet(-1, -1, -1)},
		}

	},


		// pattern2
		// 中央バリア 上と左に設置弾
		{
			// id
			1,
			// frame
			260,
			// pos
			VGet(224, 36, 0),
			// blockPos
			{


				VGet(222, 39, 0),
				VGet(223, 39, 0),
				VGet(224, 39, 0),
				VGet(225, 39, 0),
				VGet(226, 39, 0),
				VGet(227, 39, 0),


				VGet(-1, -1, -1),

			},

			// bulletType
			// 設置弾
			1,
			// bullet
			{
			// use, shotFrame, speed, pos, direction
				{ 0, 60,	17, VGet(212, 41, 0), VGet(1, 0, 0)},
				{ 0, 60,	17, VGet(212, 42, 0), VGet(1, 0, 0)},
				{ 0, 60,	17, VGet(212, 43, 0), VGet(1, 0, 0)},

				{ 0, 140, 20, VGet(212, 31, 0), VGet(0, 1, 0)},
				{ 0, 140, 20, VGet(213, 31, 0), VGet(0, 1, 0)},
				{ 0, 140, 20, VGet(214, 31, 0), VGet(0, 1, 0)},
				{ 0, 140, 20, VGet(215, 31, 0), VGet(0, 1, 0)},
				{ 0, 140, 20, VGet(216, 31, 0), VGet(0, 1, 0)},
				{ 0, 140, 20, VGet(217, 31, 0), VGet(0, 1, 0)},
				{ 0, 140, 20, VGet(218, 31, 0), VGet(0, 1, 0)},
				{ 0, 140, 20, VGet(219, 31, 0), VGet(0, 1, 0)},
				{ 0, 140, 20, VGet(220, 31, 0), VGet(0, 1, 0)},
				{ 0, 140, 20, VGet(221, 31, 0), VGet(0, 1, 0)},

				{ 0, 140, 20, VGet(228, 31, 0), VGet(0, 1, 0)},
				{ 0, 140, 20, VGet(229, 31, 0), VGet(0, 1, 0)},
				{ 0, 140, 20, VGet(230, 31, 0), VGet(0, 1, 0)},
				{ 0, 140, 20, VGet(231, 31, 0), VGet(0, 1, 0)},
				{ 0, 140, 20, VGet(232, 31, 0), VGet(0, 1, 0)},
				{ 0, 140, 20, VGet(233, 31, 0), VGet(0, 1, 0)},
				{ 0, 140, 20, VGet(234, 31, 0), VGet(0, 1, 0)},
				{ 0, 140, 20, VGet(235, 31, 0), VGet(0, 1, 0)},
				{ 0, 140, 20, VGet(236, 31, 0), VGet(0, 1, 0)},
				{ 0, 140, 20, VGet(237, 31, 0), VGet(0, 1, 0)},

				{-1, -1, -1, VGet(-1, -1, -1), VGet(-1, -1, -1)},
			}
		},

		// pattern3
		// 中央バリア 上と右に設置弾
	{
		// id
		2,
		// frame
		260,
		// pos
		VGet(224, 36, 0),
		// blockPos
		{


			VGet(222, 39, 0),
			VGet(223, 39, 0),
			VGet(224, 39, 0),
			VGet(225, 39, 0),
			VGet(226, 39, 0),
			VGet(227, 39, 0),


			VGet(-1, -1, -1),

		},

		// bulletType
		// 設置弾
		1,
		// bullet
		{
			// use, shotFrame, speed, pos, direction
				{ 0, 60,	17, VGet(237, 41, 0), VGet(-1, 0, 0)},
				{ 0, 60,	17, VGet(237, 42, 0), VGet(-1, 0, 0)},
				{ 0, 60,	17, VGet(237, 43, 0), VGet(-1, 0, 0)},

				{ 0, 140, 20, VGet(212, 31, 0), VGet(0, 1, 0)},
				{ 0, 140, 20, VGet(213, 31, 0), VGet(0, 1, 0)},
				{ 0, 140, 20, VGet(214, 31, 0), VGet(0, 1, 0)},
				{ 0, 140, 20, VGet(215, 31, 0), VGet(0, 1, 0)},
				{ 0, 140, 20, VGet(216, 31, 0), VGet(0, 1, 0)},
				{ 0, 140, 20, VGet(217, 31, 0), VGet(0, 1, 0)},
				{ 0, 140, 20, VGet(218, 31, 0), VGet(0, 1, 0)},
				{ 0, 140, 20, VGet(219, 31, 0), VGet(0, 1, 0)},
				{ 0, 140, 20, VGet(220, 31, 0), VGet(0, 1, 0)},
				{ 0, 140, 20, VGet(221, 31, 0), VGet(0, 1, 0)},

				{ 0, 140, 20, VGet(228, 31, 0), VGet(0, 1, 0)},
				{ 0, 140, 20, VGet(229, 31, 0), VGet(0, 1, 0)},
				{ 0, 140, 20, VGet(230, 31, 0), VGet(0, 1, 0)},
				{ 0, 140, 20, VGet(231, 31, 0), VGet(0, 1, 0)},
				{ 0, 140, 20, VGet(232, 31, 0), VGet(0, 1, 0)},
				{ 0, 140, 20, VGet(233, 31, 0), VGet(0, 1, 0)},
				{ 0, 140, 20, VGet(234, 31, 0), VGet(0, 1, 0)},
				{ 0, 140, 20, VGet(235, 31, 0), VGet(0, 1, 0)},
				{ 0, 140, 20, VGet(236, 31, 0), VGet(0, 1, 0)},
				{ 0, 140, 20, VGet(237, 31, 0), VGet(0, 1, 0)},

				{-1, -1, -1, VGet(-1, -1, -1), VGet(-1, -1, -1)},
			}
	},

		// pattern4
		// 左側バリア
		{
			// id
			3,
			// frame
			300,
			// pos
			VGet(215, 33, 0),
			// blockPos
			{
				VGet(219, 31, 0),
				VGet(219, 32, 0),
				VGet(219, 33, 0),
				VGet(219, 34, 0),
				VGet(219, 35, 0),
				VGet(219, 36, 0),
				VGet(219, 37, 0),
				VGet(219, 38, 0),

				VGet(-1, -1, -1),

			},

			// bulletType
			// 設置弾
			1,
			// bullet
			{
			// use, shotFrame, speed, pos, direction
			
			{ 0, 60,	15, VGet(212, 41, 0), VGet(1, 0, 0)},
			{ 0, 60,	15, VGet(212, 42, 0), VGet(1, 0, 0)},
			{ 0, 60,	15, VGet(212, 43, 0), VGet(1, 0, 0)},

			{ 0, 184, 20, VGet(220, 31, 0), VGet(0, 1, 0)},
			{ 0, 182, 20, VGet(221, 31, 0), VGet(0, 1, 0)},
			{ 0, 180, 20, VGet(222, 31, 0), VGet(0, 1, 0)},
			{ 0, 178, 20, VGet(223, 31, 0), VGet(0, 1, 0)},
			{ 0, 176, 20, VGet(224, 31, 0), VGet(0, 1, 0)},
			{ 0, 174, 20, VGet(225, 31, 0), VGet(0, 1, 0)},
			{ 0, 172, 20, VGet(226, 31, 0), VGet(0, 1, 0)},
			{ 0, 170, 20, VGet(227, 31, 0), VGet(0, 1, 0)},
			{ 0, 168, 20, VGet(228, 31, 0), VGet(0, 1, 0)},
			{ 0, 166, 20, VGet(229, 31, 0), VGet(0, 1, 0)},
			{ 0, 164, 20, VGet(230, 31, 0), VGet(0, 1, 0)},
			{ 0, 162, 20, VGet(231, 31, 0), VGet(0, 1, 0)},
			{ 0, 160, 20, VGet(232, 31, 0), VGet(0, 1, 0)},
			{ 0, 158, 20, VGet(233, 31, 0), VGet(0, 1, 0)},
			{ 0, 156, 20, VGet(234, 31, 0), VGet(0, 1, 0)},
			{ 0, 154, 20, VGet(235, 31, 0), VGet(0, 1, 0)},
			{ 0, 152, 20, VGet(236, 31, 0), VGet(0, 1, 0)},
			{ 0, 150, 20, VGet(237, 31, 0), VGet(0, 1, 0)},

			{-1, -1, -1, VGet(-1, -1, -1), VGet(-1, -1, -1)},
		}
	},

		// pattern5
		// 右側バリア
		{
			// id
			4,
			// frame
			300,
			// pos
			VGet(234, 33, 0),
			// blockPos
			{
				VGet(230, 31, 0),
				VGet(230, 32, 0),
				VGet(230, 33, 0),
				VGet(230, 34, 0),
				VGet(230, 35, 0),
				VGet(230, 36, 0),
				VGet(230, 37, 0),
				VGet(230, 38, 0),
				

				VGet(-1, -1, -1),

			},

			// bulletType
			// 設置弾
			1,
			// bullet
			{
			// use, shotFrame, speed, pos, direction
			
			{ 0, 60,	15, VGet(237, 41, 0), VGet(-1, 0, 0)},
			{ 0, 60,	15, VGet(237, 42, 0), VGet(-1, 0, 0)},
			{ 0, 60,	15, VGet(237, 43, 0), VGet(-1, 0, 0)},

			{ 0, 150, 20, VGet(212, 31, 0), VGet(0, 1, 0)},
			{ 0, 152, 20, VGet(213, 31, 0), VGet(0, 1, 0)},
			{ 0, 154, 20, VGet(214, 31, 0), VGet(0, 1, 0)},
			{ 0, 156, 20, VGet(215, 31, 0), VGet(0, 1, 0)},
			{ 0, 158, 20, VGet(216, 31, 0), VGet(0, 1, 0)},
			{ 0, 160, 20, VGet(217, 31, 0), VGet(0, 1, 0)},
			{ 0, 162, 20, VGet(218, 31, 0), VGet(0, 1, 0)},
			{ 0, 164, 20, VGet(219, 31, 0), VGet(0, 1, 0)},
			{ 0, 166, 20, VGet(220, 31, 0), VGet(0, 1, 0)},
			{ 0, 168, 20, VGet(221, 31, 0), VGet(0, 1, 0)},
			{ 0, 170, 20, VGet(222, 31, 0), VGet(0, 1, 0)},
			{ 0, 172, 20, VGet(223, 31, 0), VGet(0, 1, 0)},
			{ 0, 174, 20, VGet(224, 31, 0), VGet(0, 1, 0)},
			{ 0, 176, 20, VGet(225, 31, 0), VGet(0, 1, 0)},
			{ 0, 178, 20, VGet(226, 31, 0), VGet(0, 1, 0)},
			{ 0, 180, 20, VGet(227, 31, 0), VGet(0, 1, 0)},
			{ 0, 182, 20, VGet(228, 31, 0), VGet(0, 1, 0)},
			{ 0, 184, 20, VGet(229, 31, 0), VGet(0, 1, 0)},

			{-1, -1, -1, VGet(-1, -1, -1), VGet(-1, -1, -1)},
		}
	},

	};


	int nextAttackId;
	while (true) {
		nextAttackId = rand() % BOSS_ATTACK_MAX;

		// 次の攻撃が今の攻撃と同じパターンにならないようにする
		if (nextAttackId != _attackInfo.id) {
			float xCenter = 224.5f * CHIPSIZE_W;

			// 左側に壁や弾が出る攻撃の場合
			if (nextAttackId == 1 || nextAttackId == 3) {
				// プレイヤーがボスエリアの中心より左側にいたら再抽選
				if (playerPos.x < xCenter) {
					continue;
				}
			}

			// 右側に壁や弾が出る攻撃の場合
			else	if (nextAttackId == 2 || nextAttackId == 4) {
				// プレイヤーがボスエリアの中心より右側にいたら再抽選
				if (xCenter < playerPos.x) {
					continue;
				}
			}

			break;
		}

	}
	_attackInfo = bossAttack[nextAttackId];
}

// 構造体「TEXTURE」の初期化
void Boss::TextureInit(int modelHandle)
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
void Boss::ChangeTexture(int isDamaged)
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
void Boss::MaterialInit(int modelHandle)
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
void Boss::ChangeMaterialDifColor(int isDamaged)
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

// ボス戦用BGMの読み込み
void Boss::LoadBossBGM()
{
	_bgmBossBattle = -1;
	_bgmBossBattle = LoadSoundMem("res/sound/BGM/bgm_boss.mp3");
}

// ボス戦用BGMの破棄
void Boss::DeleteBossBGM()
{
	if (_bgmBossBattle != -1) {
		DeleteSoundMem(_bgmBossBattle);
		_bgmBossBattle = -1;
	}
}

// ボス戦用BGMの再生・停止
void Boss::PlayBossBGM(int n)
{
	if (n == 0) {
		StopSoundMem(_bgmBossBattle);
	}
	else {
		PlaySoundMem(_bgmBossBattle, DX_PLAYTYPE_LOOP);
	}
}



// デバッグ情報の表示
void Boss::DrawDebugInfo(MATRIX mView, int fillFrag)
{
	VECTOR rectPos = VGet(_bodyRect.x, _bodyRect.y, 0);
	rectPos = VTransform(rectPos, mView);

	int colorTbl[] = {
		COLOR_WHITE,
		COLOR_BLUE
	};

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 140.f);
	//DrawBox(_bodyRect.x, _bodyRect.y, _bodyRect.x + _bodyRect.w, _bodyRect.y + _bodyRect.h, COLOR_BLUE, TRUE);
	DrawBox(rectPos.x, rectPos.y, rectPos.x + _bodyRect.w, rectPos.y + _bodyRect.h, colorTbl[1], TRUE);
	DrawFormatStringToHandle(rectPos.x, rectPos.y, COLOR_WHITE, fontHandle, "%d", _hp);

	for (int i = 0; i < BOSS_BULLET_MAX; i++) {
		if (_attackInfo.bullet[i].use == -1) break; // use == -1は未使用配列

		if (_attackInfo.bullet[i].use == 1) {

			MYRECT bulletRect = {
				_attackInfo.bullet[i].pos.x,
				_attackInfo.bullet[i].pos.y,
				60,
				60
			};

			MyDrawBox(mView, bulletRect.x, bulletRect.y, bulletRect.x + bulletRect.w, bulletRect.y + bulletRect.h, COLOR_RED, TRUE);


			VECTOR world = VGet(bulletRect.x, bulletRect.y, 0);
			world = VTransform(world, mView);

			if (_attackInfo.bullet[i].direction.x == 1) {
				DrawFormatString(world.x, world.y, COLOR_WHITE, "→");
			}
			else if (_attackInfo.bullet[i].direction.x == -1) {
				DrawFormatString(world.x, world.y, COLOR_WHITE, "←");

			}
			else if (_attackInfo.bullet[i].direction.y == 1) {
				DrawFormatString(world.x, world.y, COLOR_WHITE, "↓");
			}
			else if (_attackInfo.bullet[i].direction.y == -1) {
				DrawFormatString(world.x, world.y, COLOR_WHITE, "↑");
			}
		}
	}


	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	int x = 1500;
	int y = 500;
	int l = 0;
	DrawFormatString(x, y + 16 * l, COLOR_WHITE, "pos: x %3.2f, y %3.2f, z %3.2f", _pos.x, _pos.y, _pos.z); l++;
	DrawFormatString(x, y + 16 * l, COLOR_WHITE, "screenPos: x %3.2f, y %3.2f, z %3.2f", _screenPos.x, _screenPos.y, _screenPos.z); l++;
	DrawFormatString(x, y + 16 * l, COLOR_WHITE, "direction: x %3.2f, y %3.2f, z %3.2f", _direction.x, _direction.y, _direction.z); l++;
	l++;
	DrawFormatString(x, y + 16 * l, COLOR_WHITE, "_attackInfo: id %d, frame %d", _attackInfo.id, _attackInfo.frame); l++;
	DrawFormatString(x, y + 16 * l, COLOR_WHITE, "_attackCnt:  %d, _attackCoolTime: %d", _attackCnt, _attackCoolTime); l++;





	//for (int i = 0; i < BOSS_BULLET_MAX; i++) {
	//	DrawFormatString(x, y + 16 * l, COLOR_WHITE, "pos: x %3.2f, y %3.2f, z %3.2f", _pos.x, _pos.y, _pos.z); l++;
	//}
}