#include "meatball.h"
#include "winmain.h"
#include <math.h>
#include "mymath.h"
#include "mydraw.h"
#include "resource.h"

#include "enemy_shield.h"
#include "lever.h"

#include "effect.h"


// モーション用定数
#define MEATBALL_MOTION_MAX 2

#define MEATBALL_MOTION_SOFT 0
#define MEATBALL_MOTION_STAND 1


// 方向線画像
int MeatBall::_cgAim;


MeatBall::MeatBall(int modelHandle, VECTOR playerPos) : CharaBase(modelHandle, playerPos)
{
	// 大きさの調整
	_defaultSize = VGet(0.7f, 0.7f, 0.7f);
	MV1SetScale(_modelHandle, _defaultSize);

	_power = POWER_DEFAULT;


	_direction = VGet(0, 0, 0);
	_arrow = VGet(100, 0, 0);

	_r = 16;
	_speed = 0;

	_isAttack = 0;
	_attackCoolTime = 0;
	_isReturn = 1;


	_canReattack = 0;


	_launchCnt = 0;

	_size = 1;
	_level = 1;

	_reflectRect = { 0, 0, 0, 0 };

	// マップチップとの反射用矩形の更新
	UpdateReflectRect();



	_playerHandle = -1;
	_attachNum = -1;

	


	_rad = 0;
	_rotType = 0;
	_rotCoolTime = 0;

	_isInScreen = 1;
	_offScreenTime = 0;


	_beforHitStopPos = VGet(0, 0, 0);
	_hitStopCnt = 0;



	_isDrawFullArrow = 0;


	// モーションの初期化処理
	_motionNum = MEATBALL_MOTION_STAND;
	MotionInit(MEATBALL_MOTION_MAX);


	// アウトラインの初期設定
	MV1SetMaterialOutLineColor(_modelHandle, 0, GetColorF(1.0f, 0.0f, 1.0f, 1.0f));




	_isDrawAura = 0;
	_effectCnt = 0;



	_isScaling = 0;
	_scalingCnt = 0;
}

// ミートボールをプレイヤーのタンク内に配置するための初期化処理
// 引数:
// プレイヤーのモデルハンドル
// タンクのフレーム番号
void MeatBall::SetAttachToPlayer(int pHandle, int n) {
	_playerHandle = pHandle;
	_attachNum = n;
}

MeatBall::~MeatBall()
{
	//CharaBase::~CharaBase();

	// 残像用モデルの削除
	for (int i = 0; i < MEATBALL_AFTERIMAGE_MAX; i++) {
		if (_afterImage[i].modelHandle != -1) {
			MV1DeleteModel(_afterImage[i].modelHandle);
			_afterImage[i].modelHandle = -1;
		}
	}
}

// 残像の初期化処理
void MeatBall::AfterImageInit(int modelHandle)
{
	VECTOR modelScale = MV1GetScale(_modelHandle);
	for (int i = 0; i < MEATBALL_AFTERIMAGE_MAX; i++) {
		_afterImage[i].use = 0;
		_afterImage[i].remainCnt = 0;

		// ３Ｄモデルの読み込み（複製する）
		// 3Dモデルの読み込み MV1LoadModel()は処理に時間がかかるので、3Dモデルの複製 MV1DuplicateModel()を使う
		// 複製元3Dモデルの読み込みはは scenegamemain.cpp の SceneGameMainResourceInit() 内で行う
		_afterImage[i].modelHandle = MV1DuplicateModel(modelHandle);
		MV1SetScale(_afterImage[i].modelHandle, modelScale);

		_afterImage[i].pos = VGet(0, 0, 0);

		//アニメーションのアタッチ
		_afterImage[i].anim.index = MV1AttachAnim(_afterImage[i].modelHandle, MEATBALL_MOTION_SOFT, -1, FALSE);
		// アタッチしたアニメーションの総再生時間を取得する
		_afterImage[i].anim.totalTime = MV1GetAttachAnimTotalTime(_afterImage[i].modelHandle, _afterImage[i].anim.index);



		_afterImage[i].alpha = 1;
	}


}

// 残像の計算処理
void MeatBall::AfterImageProcess()
{
	for (int i = 0; i < MEATBALL_AFTERIMAGE_MAX; i++) {

		// 毎フレーム透明度をあげる
		_afterImage[i].alpha -= 0.15f;
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

// 残像をセットする
void MeatBall::SetAfterImage()
{
	for (int i = 0; i < MEATBALL_AFTERIMAGE_MAX; i++) {
		if (_afterImage[i].use == 1) continue;
		_afterImage[i].use = 1;

		// 残像の持続時間をミートボールの移動速度に合わせて設定する
		// 移動速度が大きいほど残像の持続時間を長くする（3段階）
		if (_speed < SPEED_MAX) {
			_afterImage[i].remainCnt = 3;
		}
		else if (_speed < SPEED_DASH) {
			_afterImage[i].remainCnt = 6;
		}
		else {
			_afterImage[i].remainCnt = 10;
		}

		_afterImage[i].pos = _pos;

		// 残像用モデルのサイズをミートボールと同じにする
		VECTOR modelScale = MV1GetScale(_modelHandle);
		MV1SetScale(_afterImage[i].modelHandle, modelScale);

		// 残像用モデルの回転をミートボールと同じにする
		VECTOR modelRot = MV1GetRotationXYZ(_modelHandle);
		MV1SetRotationXYZ(_afterImage[i].modelHandle, modelRot);

		// 再生時間をセットする(アニメーションの再生)
		float afterImagePlayTime = _playTime;
		MV1SetAttachAnimTime(_afterImage[i].modelHandle, _afterImage[i].anim.index, afterImagePlayTime);

		_afterImage[i].alpha = 1;
		break;
	}
}

// 計算処理
void MeatBall::Process(VECTOR playerCenterPos)
{
	// 前のフレームのモーション番号を保持する
	int motionNum_old = _motionNum;
	if (_isAttack == 1) {
		_motionNum = MEATBALL_MOTION_SOFT;
	}
	else {
		_motionNum = MEATBALL_MOTION_SOFT;
	}
	// モーションの計算処理
	MotionProcess(motionNum_old);

	// 回転処理
	// 5つのパターンからランダム
	if (_isAttack == 1) {
		_rad += 0.05f;
		_rotCoolTime--;

		VECTOR rot = VGet(0, 0, 0);
		switch (_rotType)
		{
		case 0:
			rot = VGet(_rad, 0, _rad);
			break;
		case 1:
			rot = VGet(_rad, _rad, 0);
			break;
		case 2:
			rot = VGet(_rad, 0, 0);
			break;
		case 3:
			rot = VGet(0, _rad, 0);
			break;
		case 4:
			rot = VGet(0, 0, _rad);
			break;
		}
		MV1SetRotationXYZ(_modelHandle, rot);
	}


	// レベル処理
	if (_size >= 30) {
		SetLevel(3);
	}
	else	if (_size >= 16) {
		SetLevel(2);
	}
	else {
		SetLevel(1);
	}


	VECTOR vDir;

	// 飛ばす方向の設定
	if (gPad->_input.X != 0 || gPad->_input.Y != 0) {
		vDir = VGet(gPad->_input.X, gPad->_input.Y, 0);
		vDir = VNorm(vDir);
		_arrow = VScale(vDir, 100);
	}



	// 半径の計算
	// 半径は32~368の43段階 
	// +8ずつサイズが大きくなっていく
	_r = RADIUS_DEFAULT + (_size - 1) * RADIUS_INCREMENT_VALUE;


	// ヒットストップ中でなければ
	if (_hitStopCnt <= 0) {
		// 攻撃中なら
		if (_isAttack == 1) {

			_attackCoolTime--;
			// 攻撃のクールタイムが終わったら
			if (_attackCoolTime < 0) {
				_attackCoolTime = 0;
				// 再攻撃（方向転換）可能にする
				_canReattack = 1;
			}

			// 現在のサイズをもとに攻撃力を設定する
			_power = POWER_DEFAULT + (_size - 1) * POWER_INCREMENT_VALUE;


			// 現在のサイズをもとに3Dモデルの拡大値を設定する
			float magnification = 0.75f + (_size * 0.25f);
			VECTOR vScale = VScale(_defaultSize, magnification);
			// スケーリング処理中でなければ
			if (_isScaling == 0) {
				MV1SetScale(_modelHandle, vScale);
			}
			else {
				// スケーリング処理
				ScalingProcess(vScale);
			}



			// ボールの移動速度が早すぎるとマップチップをすり抜けることがある
			// 1フレーム内での移動を少しずつ区切って反映させる
			// divSpeedずつ移動を反映させる

			float sum = 0;
			float divSpeed = 1;

			// エフェクトを出すマップチップ座標を保持する
			std::vector<HITCHIP> effectChip;

			// ヒットSEを鳴らすかどうか
			int isPlayHitSound = 0;

			while (true) {
				sum += divSpeed;
				// クランプ
				// このループで、合計移動距離が1フレームでの移動距離 _speed を超える場合、超えない様にdivSpeedを調整する
				if (sum > _speed) {
					/*divSpeed = _speed - (sum - divSpeed);
					finish = 1;*/

					break;
				}

				// 画面外に出ないように
				// x軸の反射
				if (_pos.x - _r < gCam._pos.x - SCREEN_W / 2) {
					float dir = fabs(_direction.x);
					_direction.x = dir;
					isPlayHitSound = 1;
					SetScaling();
				}
				else if (gCam._pos.x + SCREEN_W / 2 < _pos.x + _r) {
					float dir = fabs(_direction.x);
					_direction.x = -dir;
					isPlayHitSound = 1;
					SetScaling();
				}
				// y軸の反射
				if (_pos.y - _r < gCam._pos.y - SCREEN_H / 2) {
					float dir = fabs(_direction.y);
					_direction.y = dir;
					isPlayHitSound = 1;
					SetScaling();
				}
				else	if (gCam._pos.y + SCREEN_H / 2 < _pos.y + _r) {
					float dir = fabs(_direction.y);
					_direction.y = -dir;
					isPlayHitSound = 1;
					SetScaling();
				}


				// 移動処理
				VECTOR vDir = VNorm(_direction);
				VECTOR vMove = VScale(vDir, divSpeed);
				_pos.x += vMove.x;
				_pos.y += vMove.y;

				// マップチップとの反射用矩形の更新
				UpdateReflectRect();


				// マップチップとの当たり判定
				HITCHIP hitChip = gMap.IsHitBoxAndMapChip(_reflectRect);
				// マップチップに当たっていたら
				if(hitChip.chip_no != -1) {


					MYRECT chipRect = { hitChip.x * CHIPSIZE_W, hitChip.y * CHIPSIZE_H, CHIPSIZE_W, CHIPSIZE_H };

					// 押し出し方向を取得する
					// ただし、当たり判定の重なった部分の横幅と縦幅が同じになった場合はうまく反射させることができない
					// そのため、当たり判定の上下左右の4地点での反射ができなかった場合の代わりの措置として使用する
					int extrusionDirection = CheckExtrusionDirection(_reflectRect, chipRect);

					// 反射処理を行うかするかどうか
					int reflect = 0;
					// 斜めのマップチップに当たったかどうか
					int diagonalReflect = 0;

					// 当たった斜めのマップチップの法線ベクトルを保持する
					VECTOR vNormal;

					// 当たったマップチップの種類ごとに処理を行う
					switch(hitChip.chip_no)
					{
						// 壊せるマップチップ
					case BREAK_CHIP_1:
						// マップチップ破壊処理
						gMap.BreakMapChip(hitChip);
						isPlayHitSound = 2;

						// ブロック破壊エフェクトのセット
						SetChipBreakEffect(hitChip);
						break;
						// レベルが2以上なら壊せるマップチップ
					case BREAK_CHIP_2:
						if(_level >= 2) {
							// マップチップ破壊処理
							gMap.BreakMapChip(hitChip);
							isPlayHitSound = 2;

							// ブロック破壊エフェクトのセット
							SetChipBreakEffect(hitChip);
						}
						else {
							reflect = 1;
						}
						break;
						// レベルが3以上なら壊せるマップチップ
					case BREAK_CHIP_3:
						if(_level >= 3) {
							// マップチップ破壊処理
							gMap.BreakMapChip(hitChip);
							isPlayHitSound = 2;

							// ブロック破壊エフェクトのセット
							SetChipBreakEffect(hitChip);
						}
						else {
							reflect = 1;
						}
						break;
						// スピードが30以上なら壊せるマップチップ
					case BREAK_CHIP_4:
						if(_speed >= 30) {
							// マップチップ破壊処理
							gMap.BreakMapChip(hitChip);
							isPlayHitSound = 2;

							// ブロック破壊エフェクトのセット
							SetChipBreakEffect(hitChip);
						}
						else {
							reflect = 1;
						}
						break;

						// 通常のマップチップに当たった場合
					default:
						reflect = 1;
						break;

						// 斜めのマップチップにあったった場合
					case LOWER_RIGHT:
						vNormal = VGet(-1, 1, 0);
						diagonalReflect = 1;

						break;
					case UPPER_RIGHT:
						vNormal = VGet(-1, -1, 0);
						diagonalReflect = 1;

						break;
					case UPPER_LEFT:
						vNormal = VGet(1, -1, 0);
						diagonalReflect = 1;

						break;
					case LOWER_LEFT:
						vNormal = VGet(1, 1, 0);
						diagonalReflect = 1;

						break;
					}

					// 反射処理をする場合
					if(reflect == 1) {



						// 上下左右の点がマップチップに当たっているかを調べる
						// 基点座標から上下左右に移動した点
						EXTRUSION ex[] = {
							// 左
							{0, 1, VGet(_reflectRect.x, _pos.y, 0)},
							// 右
							{0, -1, VGet(_reflectRect.x + _reflectRect.w, _pos.y, 0)},

							// 上
							{1, 1, VGet(_pos.x, _reflectRect.y, 0)},
							// 下
							{1, -1, VGet(_pos.x, _reflectRect.y + _reflectRect.w, 0)},

						};

						// 各軸において、点がマップチップに当たったかどうか（中身にはexの要素番号が入る）
						// すでに反対側の点が当たっていた場合はループから抜ける処理を行うため
						int hitX = -1;
						int hitY = -1;

						// 上下左右の内のどこかの点が当たっているか
						int isHitPoint = 0;

						for(int i = 0; i < 4; i++) {
							// 各点がマップチップに当たっているかを調べる
							int isHit = gMap.IsHitMapchip(ex[i].point.x, ex[i].point.y);
							// マップチップに当たっていた場合
							// 押し出し処理をして、当たらなくなるまで繰り返す
							while(isHit != -1)
							{
								isHitPoint = 1;
								// x軸の処理
								if(ex[i].axis == 0) {
									// すでに反対側の点が当たっていた場合はループから抜ける
									// 例）左の点が当たっていた場合、右の点の当たり判定は行わない
									if(hitX != -1 && hitX != i) break;
									hitX = i;
									// 押し出し処理
									_pos.x += ex[i].direction;

									// 反射方向の設定
									if(ex[i].direction == 1) {
										_direction.x = fabs(_direction.x);
									}
									else {
										_direction.x = -fabs(_direction.x);
									}

								}
								// y軸の処理
								else {
									// すでに反対側の点が当たっていた場合はループから抜ける
									if(hitY != -1 && hitY != i) break;
									hitY = i;

									// 押し出し処理
									_pos.y += ex[i].direction;

									// 反射方向の設定
									if(ex[i].direction == 1) {
										_direction.y = fabs(_direction.y);
									}
									else {
										_direction.y = -fabs(_direction.y);
									}
								}

								// マップチップとの反射用矩形の更新
								UpdateReflectRect();

								ex[0].point = VGet(_reflectRect.x, _pos.y, 0);
								ex[1].point = VGet(_reflectRect.x + _reflectRect.w, _pos.y, 0);
								ex[2].point = VGet(_pos.x, _reflectRect.y, 0);
								ex[3].point = VGet(_pos.x, _reflectRect.y + _reflectRect.w, 0);


								isHit = gMap.IsHitMapchip(ex[i].point.x, ex[i].point.y);
							}
						}


						// 上下左右の点がマップチップに当たっていなかった場合
						// 
						// 
						// ただし、当たり判定の重なった部分の横幅と縦幅が同じになった場合はうまく反射させることができない
						// そのため、当たり判定の上下左右の4地点での反射ができなかった場合の代わりの措置として使用する
						if(isHitPoint == 0) {
							// 反射方向の設定					
							if(extrusionDirection == 0) {
								_direction.x = -_direction.x;

								if(_direction.x == 0) {
									_direction.y = -_direction.y;
								}
							}
							else if(extrusionDirection == 1) {
								_direction.y = -_direction.y;

								if(_direction.y == 0) {
									_direction.x = -_direction.x;
								}
							}
							else {
								// 当たり判定の重なった部分の横幅と縦幅が同じになった場合はうまく反射させることができない
								// ミートボールのとマップチップの当たり判定矩形どうしの角が当たったときなど
								_direction.x = -_direction.x;
								_direction.y = -_direction.y;
							}
						}

						// ミートボールの回転を5つのパターンからランダムで決定する
						SetRotType();


						isPlayHitSound = 1;


						//// このフレームで既に同じマップチップにエフェクトがセットされている場合は、新しくエフェクトはセットしない
						//int effect_ok = 1;
						//for(auto eChip : effectChip) {
						//	if(hitChip.x == eChip.x && hitChip.y == eChip.y) {
						//		effect_ok = 0;
						//		break;
						//	}
						//}

						//if(effect_ok == 1) {
						//	// 煙エフェクトの設定
						//	SetSmokeEffect(hitChip);
						//	effectChip.push_back(hitChip);
						//}

						//// スケーリング処理の開始設定
						//SetScaling();
					}

					// 斜めのマップチップにあったった場合の反射
					else if(diagonalReflect == 1) {
						// 当たった斜めのマップチップの法線ベクトル
						vNormal = VNorm(vNormal);
						VECTOR dirTmp = VNorm(_direction);

						auto dot = 2.0f * VDot(vNormal, dirTmp);

						VECTOR vReflect = VScale(vNormal, dot);
						vReflect = VSub(vReflect, dirTmp);
						vReflect = VNorm(vReflect);
						_direction = vReflect;

						vDir = VNorm(_direction);
						vMove = VScale(vDir, divSpeed);
						_pos.x += vMove.x;
						_pos.y += vMove.y;

						// ミートボールの回転を5つのパターンからランダムで決定する
						SetRotType();

						isPlayHitSound = 1;
					}

					// 既に同じマップチップにエフェクトがセットされている場合は、新しくエフェクトはセットしない
					int effect_ok = 1;
					for(auto eChip : effectChip) {
						if(hitChip.x == eChip.x && hitChip.y == eChip.y) {
							effect_ok = 0;
							break;
						}
					}

					if(effect_ok == 1) {
						// 煙エフェクトの設定
						SetSmokeEffect(hitChip);
						effectChip.push_back(hitChip);
					}
					// スケーリング処理の開始設定
					SetScaling();


					// マップチップとの反射用矩形の更新
					UpdateReflectRect();

				}


				// 盾との当たり判定
				for (int i = 0; i < ENEMY_SHIELD_MAX; i++) {
					SHIELD eShield = GetShieldInfo(i);
					if (eShield.use != 0) {
						// 盾とボールが当たったかを判定する
						int hitShield = 0;
						MYRECT mRect = _reflectRect;
						VECTOR sDir = eShield.direction;

						// 盾（カプセル）とボール（矩形）の当たり判定
						// 盾の当たり判定（カプセル）を3つのパーツに分けて判定を行う
						if (IsHitCircleAndBox(eShield.centerPos.x, eShield.centerPos.y, eShield.r, mRect) == 1) {
							hitShield = 1;
						}
						else	if (IsHitCapsuleAndBox(
							eShield.centerPos.x, eShield.centerPos.y, eShield.r,
							sDir,
							eShield.h,
							mRect)
							) {
							hitShield = 1;
						}
						else if (IsHitCapsuleAndBox(
							eShield.centerPos.x, eShield.centerPos.y, eShield.r,
							VScale(sDir, -1),
							eShield.h,
							mRect)
							) {
							hitShield = 1;
						}

						// 当たっていた場合、盾の角度に応じて反射方向を計算する
						if (hitShield == 1) {
							_pos.x -= vMove.x * 30;
							_pos.y -= vMove.y * 30;

							// 法線ベクトル
							VECTOR vNormal = VNorm(eShield.direction);
							VECTOR dirTmp = VNorm(_direction);

							auto dot = 2.0f * VDot(vNormal, dirTmp);

							VECTOR vReflect = VScale(vNormal, dot);
							vReflect = VSub(vReflect, dirTmp);
							vReflect = VNorm(vReflect);
							_direction = vReflect;


							// スケーリング処理の開始設定
							SetScaling();


							// 煙エフェクト（カラー）のセット
							VECTOR effectPos = _pos;
							float effectSize = _r * 3;
							if (effectSize < 60) {
								effectSize = 60;
							}
							if (effectSize > 300) {
								effectSize = 300;
							}
							SetEffect(EFFECT_TYPE_SMOKE_COLOR, effectPos.x - effectSize / 2, effectPos.y - effectSize / 2, effectSize, effectSize);
						}
					}

				}


				// レバーとの当たり判定
				for (int i = 0; i < LEVER_MAX; i++) {
					// この配列要素が使用中かつレバーが有効化されていなかったら
					if (lever[i]->GetUse() == 1 && lever[i]->GetActive() != 1) {
						MYRECT leverRect = lever[i]->GetRect();
						// 当たっていた場合、そのレバーを有効にする
						if (IsHitCircleAndBox(_pos.x, _pos.y, _r, leverRect) == 1) {							
							lever[i]->SetActive();
						}
					}
				}

			}

			// 一定以上の速度の場合に残像を出す
			if (_speed >= 20) {				
				// 残像をセットする
				SetAfterImage();
			}

			_launchCnt--;
			// 発射（再発射）してから一定フレーム経過したら
			if (_launchCnt < 0) {
				_launchCnt = 0;
				// 毎フレームSPEED_DECREMENT_VALUEの分だけ減速する
				_speed -= SPEED_DECREMENT_VALUE;
				if (_speed < SPEED_MIN) {
					_speed = SPEED_MIN;
				}
			}

			// SEの再生処理
			if (isPlayHitSound == 1) {
				//if (CheckSoundMem(se[5]) == 0) {
					PlaySoundMem(se[5], DX_PLAYTYPE_BACK);
				//}
			}
			else if (isPlayHitSound == 2) {
				//if (CheckSoundMem(se[16]) == 0) {
					PlaySoundMem(se[16], DX_PLAYTYPE_BACK);
				//}
			}


			// 画面外に出て30フレーム経ったらプレイヤーのもとに戻す
			_isInScreen = CheckIsInScreen();
			if (_isInScreen == 0) {

				// 呼び戻しSEの再生
				if (CheckSoundMem(se[20]) == 0) {
					PlaySoundMem(se[20], DX_PLAYTYPE_BACK);
				}

				_offScreenTime++;

				// 画面外にいる時間がMAXを超えていた場合、呼び戻し処理を行う
				if (_offScreenTime >= MEATBALL_RETURN_CNT_MAX) {
					_isAttack = 0;
					_offScreenTime = 0;
				}
			}
			else {
				_offScreenTime = 0;
			}

		}
		// 攻撃中でなければ
		else {
			// プレイヤーの元へ戻す
			ReturnToPlayer(playerCenterPos);
		}
	}
	// ヒットストップ中なら
	else {

		if (_hitStopCnt % 2 == 0) {
			// _hitStopDirの方向（またはその反対方向）に揺らす
			VECTOR tmpDir = _direction;
			if (_hitStopCnt % 4 == 2) {
				tmpDir = VScale(tmpDir, -1);
			}

			// 移動処理
			tmpDir = VNorm(tmpDir);
			VECTOR vMove = VScale(tmpDir, 10);
			_pos.x += vMove.x;
			_pos.y += vMove.y;

		}

		_hitStopCnt--;
		// ヒットストップが終わったら
		if (_hitStopCnt == 0) {
			// ヒットストップ前の座標に戻す
			_pos = _beforHitStopPos;
		}
	}

	// ヒットストップ中には当たり判定の更新を行わない
	if (_hitStopCnt <= 0) {
		// マップチップとの反射用矩形の更新
		UpdateReflectRect();
	}

	// 3Dモデル用座標の更新
	UpdateWorldPos();

	// 残像の計算処理
	AfterImageProcess();
}

// 3Dモデル用座標の更新
void MeatBall::UpdateWorldPos()
{
	// スクリーン座標をワールド座標に変換
	_worldPos = ConvScreenPosToWorldPos(_screenPos);
	_worldPos.z = 350; // z軸表示位置の調整
}

// 攻撃状態を変更する
// 引数 n
// 0: 非攻撃状態に, 1: 攻撃状態に
void MeatBall::SetAttack(VECTOR playerCenterPos, int n)
{
	// 非攻撃状態にする場合
	if (n == 0) {
		_isAttack = 0;
		return;
	}

	// 攻撃可能かどうか
	int attack_ok = 0;
	// プレイヤーのもとにいる状態なら
	if (_isReturn == 1) {
		_isAttack = 1;
		_canReattack = 1;
		_pos = playerCenterPos;

		attack_ok = 1;
	}
	// 攻撃状態かつ再攻撃可能なら
	else if (_isAttack == 1 && _canReattack == 1) {
		_canReattack = 0;

		attack_ok = 1;
	}


	// 攻撃可能なら
	if (attack_ok == 1) {
		_rotCoolTime = 0;
		// ミートボールの回転を5つのパターンからランダムで決定する
		SetRotType();
		_attackCoolTime = MEATBALL_ATTACK_COOLTIME;

		_direction = _arrow;

		// スピードが発射時のときより遅くなっていたら、スピードを設定する
		// 加速状態ならその速さを維持するため（加速状態はSPEED_MAXよりスピードの値が大きくなる）
		if (_speed < SPEED_MAX) {
			_speed = SPEED_MAX;
		}
		_launchCnt = LAUCH_COUNT_MAX;


		_isReturn = 0;


		// 発射時に画面外に出ているときは画面内に押し戻す
		int isInScreen = 0;
		while (isInScreen != 1) {
			isInScreen = 1;

			// x軸
			if (_pos.x - _r < gCam._pos.x - SCREEN_W / 2) {
				_pos.x++;
				isInScreen = 0;
			}
			else if (gCam._pos.x + SCREEN_W / 2 < _pos.x + _r) {
				_pos.x--;
				isInScreen = 0;
			}

			// y軸
			if (_pos.y - _r < gCam._pos.y - SCREEN_H / 2) {
				_pos.y++;
				isInScreen = 0;
			}
			else	if (gCam._pos.y + SCREEN_H / 2 < _pos.y + _r) {
				_pos.y--;
				isInScreen = 0;
			}


		}
		// マップチップとの反射用矩形の更新
		UpdateReflectRect();



		// プレイヤーがマップチップに密着しているときに、ボールを飛ばしてもマップチップにめり込まないようにする
		// 発射時にすでにボールとマップチップがあったている場合は、一度マップチップと当たらない座標まで押し出す

		// 各軸において、点がマップチップに当たったかどうか（中身にはexの要素番号が入る）
		// すでに反対側の点が当たっていた場合はループから抜ける処理を行うため
		int hitX = -1;
		int hitY = -1;

		// 上下左右の点がマップチップに当たっているかを調べる
		// 基点座標から上下左右に移動した点
		EXTRUSION ex[] = {
			// 左
			{0, 1, VGet(_reflectRect.x, _pos.y, 0)},
			// 右
			{0, -1, VGet(_reflectRect.x + _reflectRect.w, _pos.y, 0)},

			// 上
			{1, 1, VGet(_pos.x, _reflectRect.y, 0)},
			// 下
			{1, -1, VGet(_pos.x, _reflectRect.y + _reflectRect.w, 0)},

		};		


		for (int i = 0; i < 4; i++) {
			// 各点がマップチップに当たっているかを調べる
			int isHit = gMap.IsHitMapchip(ex[i].point.x, ex[i].point.y);
			// マップチップに当たっていた場合
			// 押し出し処理をして、当たらなくなるまで繰り返す
			while (isHit != -1)
			{
				// x軸の処理
				if (ex[i].axis == 0) {
					// すでに反対側の点が当たっていた場合はループから抜ける
					// 例）左の点が当たっていた場合、右の点の当たり判定は行わない
					if (hitX != -1 && hitX != i) break;
					hitX = i;
					// 押し出し処理
					_pos.x += ex[i].direction;
				}
				// y塾の処理
				else {
					if (hitY != -1 && hitY != i) break;
					hitY = i;
					// 押し出し処理
					_pos.y += ex[i].direction;
				}

				// マップチップとの反射用矩形の更新
				UpdateReflectRect();

				ex[0].point = VGet(_reflectRect.x, _pos.y, 0);
				ex[1].point = VGet(_reflectRect.x + _reflectRect.w, _pos.y, 0);
				ex[2].point = VGet(_pos.x, _reflectRect.y, 0);
				ex[3].point = VGet(_pos.x, _reflectRect.y + _reflectRect.w, 0);


				isHit = gMap.IsHitMapchip(ex[i].point.x, ex[i].point.y);
			}
		}
	}	
}



// プレイヤーの所に戻る
void MeatBall::ReturnToPlayer(VECTOR playerPos)
{
	_canReattack = 0;
	_attackCoolTime = 0;
	_speed = 0;

	_isReturn = 1;
	_pos = playerPos;
	_pos.y -= 64;



	_isScaling = 0;
	_scalingCnt = 0;
}

// 敵・ボスと当たったときの反射処理
void MeatBall::Reflect(MYRECT objRect)
{
	// 押し出し方向を計算する
	int extrusionDirection = CheckExtrusionDirection(_reflectRect, objRect);

	// 反射方向の設定					
	if (extrusionDirection == 0) {
		_direction.x = -_direction.x;
	}
	else if (extrusionDirection == 1) {
		_direction.y = -_direction.y;
	}

	VECTOR objPos = VGet(objRect.x + objRect.w / 2, objRect.y + objRect.h / 2, 0);

	int isHit = IsHitBox(_reflectRect.x, _reflectRect.y, _reflectRect.w, _reflectRect.h,
		objRect.x, objRect.y, objRect.w, objRect.h);
	while (isHit == 1)
	{
		extrusionDirection = CheckExtrusionDirection(_reflectRect, objRect);
		if (extrusionDirection == -1 || extrusionDirection == 2) break;

		switch (extrusionDirection)
		{
		case 0: // 横方向
			float w;
			if (_pos.x < objPos.x) {
				//w = _reflectRect.x + _reflectRect.w - objRect.x;
				//_pos.x -= w;
				_pos.x--;
			}
			else {
				//w = objRect.x + objRect.w - _reflectRect.x;
				//_pos.x += w;
				_pos.x++;
			}
			break;

		case 1: // 縦方向
			float h;
			if (_pos.y < objPos.y) {
				//h = _reflectRect.y + _reflectRect.h - objRect.y;
				//_pos.y -= h;
				_pos.y--;
			}
			else {
				//h = _reflectRect.y - (objRect.y + objRect.h);
				//_pos.y += h;
				_pos.y++;
			}
			break;
		}


		//_reflectRect = { _pos.x - _r, _pos.y - _r, _r * 2, _r * 2 };
		// マップチップとの反射用矩形の更新
		UpdateReflectRect();
		isHit = IsHitBox(_reflectRect.x, _reflectRect.y, _reflectRect.w, _reflectRect.h,
			objRect.x, objRect.y, objRect.w, objRect.h);
	}



	// マップチップとの反射用矩形の更新
	UpdateReflectRect();
	// 3Dモデル用座標の更新
	UpdateWorldPos();


}


// マップチップとの反射用矩形の更新
void MeatBall::UpdateReflectRect()
{
	float side = _r / sqrt(2);

	_reflectRect.x = _pos.x - side;
	_reflectRect.y = _pos.y - side;
	_reflectRect.w = side * 2;
	_reflectRect.h = side * 2;
}

// サイズの更新処理
void MeatBall::UpdateSize(int n)
{
	switch (n)
	{
	case 1:
		if (_size < 43) _size += n;
		break;
	case -1:
		if (_size > 1) _size += n;
		break;
	}
}


// 描画位置の補正処理
void MeatBall::AdjustDrawPos(MATRIX mView)
{
	// プレイヤーがボールを持っているときに、ボールをプレイヤーの培養ポッド内に配置する
	if(_isReturn == 1) {

		// アタッチするモデルのフレーム〇番の座標を取得する
		VECTOR weaponFramePosition = MV1GetFramePosition(_modelHandle, -1);
		// アタッチするモデルをフレーム〇番の座標を原点にするための平行移動行列を作成
		MATRIX TransMat = MGetTranslate(VScale(weaponFramePosition, -1.0f));
		// アタッチされるモデルのフレーム〇番の行列を取得
		MATRIX characterFrameMat = MV1GetFrameLocalWorldMatrix(_playerHandle, _attachNum);

		// 大きさ調整
		MATRIX mScale = MGetScale(VGet(0.28f, 0.28f, 0.28f));
		//MATRIX mScale = MGetScale(_defaultSize);

		// 回転調整
		MATRIX mRotation = MGetRotY(DegToRad(180.f));
		mRotation = MMult(mRotation, MGetRotZ(0.0f));

		// 行列の合成
		MATRIX MixMatrix = MMult(mScale, mRotation);
		MixMatrix = MMult(MixMatrix, TransMat);
		MixMatrix = MMult(MixMatrix, characterFrameMat);

		/*MATRIX mTranslate = MGetTranslate(VGet(0, 0, -2));
		MixMatrix = MMult(MixMatrix, mTranslate);*/


		// 合成した行列をアタッチするモデルにセット
		MV1SetMatrix(_modelHandle, MixMatrix);
		MV1DrawModel(_modelHandle);


	}
	else {

		// ボールがプレイヤーの手を離れているときのボールの表示座標設定

		// セットした行列を無効化
		MV1SetMatrix(_modelHandle, MGetIdent());


		CharaBase::AdjustDrawPos(mView);


		for(int i = 0; i < MEATBALL_AFTERIMAGE_MAX; i++) {
			if(_afterImage[i].use == 1) {
				VECTOR tmpPos = _afterImage[i].pos;
				tmpPos = VTransform(tmpPos, mView);
				tmpPos = ConvScreenPosToWorldPos(tmpPos);
				// 古い残像が奥に配置されるように調整
				tmpPos.z = 100 * (10 - _afterImage[i].remainCnt);
				MV1SetPosition(_afterImage[i].modelHandle, tmpPos);
			}
		}
	}

	// 機械パーツの計算処理
	// ミートボールの特定のリグに機械パーツを持たせる
	MetalPartsProcess();
}




// 描画処理
void MeatBall::Draw(MATRIX mView)
{

	if(_isReturn == 1) {
		// プレイヤーがボールを持っているときに、ボールをプレイヤーの培養ポッド内に配置する

		//CharaBase::Draw();


		// アウトラインの設定
		MV1SetMaterialOutLineWidth(_modelHandle, 0, 0);

	}
	else {
		// 残像の描画
		DrawAfterImage();

		CharaBase::Draw();

		// 再攻撃可能状態なら
		if(_canReattack == 1) {
			// 電撃エフェクトの描画
			int electroSize = _r * 3;

			// エフェクトサイズの最低値設定
			if(electroSize < 100) {
				electroSize = 100;
			}
			// 描画の透明度設定
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 140.f);
			// エフェクト画像を複数枚重ねて描画する
			MyDrawModiGraph(gCam.GetViewMatrix(), _pos, 1, 0, electroSize, electroSize, cgEffect[EFFECT_TYPE_MEATBALL_ELECTRO][(_effectCnt / 2) % 40]);
			MyDrawModiGraph(gCam.GetViewMatrix(), _pos, 1, 0, electroSize, electroSize, cgEffect[EFFECT_TYPE_MEATBALL_ELECTRO][(_effectCnt / 3) % 40]);
			MyDrawModiGraph(gCam.GetViewMatrix(), _pos, 1, 0, electroSize, electroSize, cgEffect[EFFECT_TYPE_MEATBALL_ELECTRO][(_effectCnt / 1) % 40]);
			MyDrawModiGraph(gCam.GetViewMatrix(), _pos, 1, 0, electroSize, electroSize, cgEffect[EFFECT_TYPE_MEATBALL_ELECTRO][(_effectCnt / 4) % 40]);
			// 描画の透明度を元に戻す
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

			_effectCnt++;

			// アウトラインの設定
			if(_isReturn != 1) {
				switch(_level)
				{
				case 1:
					MV1SetMaterialOutLineDotWidth(_modelHandle, 0, 0.08f);
					MV1SetMaterialOutLineWidth(_modelHandle, 0, 0.05f);
					break;
				case 2:
					MV1SetMaterialOutLineDotWidth(_modelHandle, 0, 0.04f);
					MV1SetMaterialOutLineWidth(_modelHandle, 0, 0.05f);
					break;
				case 3:
					MV1SetMaterialOutLineDotWidth(_modelHandle, 0, 0.025f);
					MV1SetMaterialOutLineWidth(_modelHandle, 0, 0.05f);
					break;
				}
			}
		}
		else {
			MV1SetMaterialOutLineDotWidth(_modelHandle, 0, 0);
			MV1SetMaterialOutLineWidth(_modelHandle, 0, 0);
		}
	}
	// 機械パーツの描画処理
	MetalPartsDraw();
}


// 前面レイヤーに描画
// 方向線の描画
void MeatBall::DrawFrontLayer(MATRIX mView)
{
	// プレイヤーのもとにいる場合
	if (_isReturn == 1) {
		// 方向線を最大の長さで描画する場合（攻撃ボタンのホールド中）
		if (_isDrawFullArrow == 1) {
			// 発射方向の線
			DrawArrow(mView);
		}
	}
	else {
		// 再攻撃可能なら
		if (_canReattack == 1) {
			// 発射方向の線
			DrawArrow(mView);
		}
	}
}

void MeatBall::DrawArrow(MATRIX mView)
{
	// 発射方向の線
	VECTOR startPos = _pos;
	VECTOR vDir = VNorm(_arrow);

	// 方向線の長さを設定
	int arrowLength = _isDrawFullArrow == 1 ? 1000 : 250;

	// 30ピクセル間隔で方向線の画像を描画する
	for (int i = 30; i < arrowLength; i += 30) {
		VECTOR arrowPos = VAdd(startPos, VScale(vDir, i));

		MyDrawModiGraph(mView, arrowPos, 1.0f, 0.0f, 10, 10, _cgAim);
	}
}

// 残像の描画処理
void MeatBall::DrawAfterImage()
{
	for (int i = 0; i < MEATBALL_AFTERIMAGE_MAX; i++) {
		if (_afterImage[i].use == 1) {
			MV1DrawModel(_afterImage[i].modelHandle);
		}
	}
}



// ミートボールの回転を5つのパターンからランダムで決定する
void MeatBall::SetRotType()
{
	if (_rotCoolTime <= 0) {
		int type_old = _rotType;
		while (true)
		{
			_rotType = rand() % 5;

			// 現在の回転から変更された場合にループから抜ける
			if (type_old != _rotType) break;
		}
		
		_rotCoolTime = MEATBALL_ROTAION_COOLTIME;
	}
}

// ミートボールが画面内にいるかを判定する
// 返り値 0: 画面内にいない, 1: 画面内にいる
int MeatBall::CheckIsInScreen()
{
	MYRECT camRect = {
		gCam._pos.x - SCREEN_W / 2,
		gCam._pos.y - SCREEN_H / 2,
		SCREEN_W,
		SCREEN_H
	};

	return (IsHitBox(
		_reflectRect.x, _reflectRect.y, _reflectRect.w, _reflectRect.h,
		camRect.x, camRect.y, camRect.w, camRect.h
	));
}


// デバッグ情報の表示
void MeatBall::DrawDebugInfo(MATRIX mView, int fillFrag)
{
	float alpha = _isAttack ? 160 : 80;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	//DrawBox(_pos.x - _r, _pos.y - _r, _pos.x + _r, _pos.y + _r, COLOR_WHITE, TRUE);

	// マップチップとの当たり判定矩形
	// 円に内接する正方形
	float side = _r / sqrt(2);
	VECTOR rectPos = VGet(_reflectRect.x, _reflectRect.y, 0);
	rectPos = VTransform(rectPos, mView);
	DrawBox(rectPos.x, rectPos.y, rectPos.x + _reflectRect.w, rectPos.y + _reflectRect.h, COLOR_WHITE, TRUE);

	// 円
	VECTOR circlePos = _pos;
	circlePos = VTransform(circlePos, mView);
	DrawCircle(circlePos.x, circlePos.y, _r, COLOR_RED, TRUE);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	int y = 0;
	DrawFormatString(0, 500 + y * 16, COLOR_RED, "Screen pos  : x %3.2f, y %3.2f, z %3.2f", _pos.x, _pos.y, _pos.z); y++;
	DrawFormatString(0, 500 + y * 16, COLOR_RED, "World pos   : x %3.2f, y %3.2f, z %3.2f", _worldPos.x, _worldPos.y, _worldPos.z); y++;
	DrawFormatString(0, 500 + y * 16, COLOR_RED, "Direction   : x %3.2f, y %3.2f, z %3.2f", _direction.x, _direction.y, _direction.z); y++;
	y++;


	DrawFormatString(0, 500 + y * 16, COLOR_RED, "flag : _isAttack %d, _isReturn %d", _isAttack, _isReturn); y++;
	DrawFormatString(0, 500 + y * 16, COLOR_RED, "speed %3.2f", _speed); y++;
	y++;

	DrawFormatString(0, 500 + y * 16, COLOR_RED, "size %d, level %d", _size, _level); y++;
	DrawFormatString(0, 500 + y * 16, COLOR_RED, "_r %3.2f, 直径 %3.2f", _r, _r * 2); y++;

	DrawFormatString(0, 500 + y * 16, COLOR_RED, "_scalingCnt %2d", _scalingCnt); y++;




	//円の　　に×を表示する
	/*int lineSize = 10;
	int color = GetColor(rand() % 256, rand() % 256, rand() % 256);
	for (int i = 0; i < 4; i++) {
		DrawLine(_collisionPos[i].x - lineSize, _collisionPos[i].y - lineSize, _collisionPos[i].x + lineSize + 1, _collisionPos[i].y + lineSize + 1, color);
		DrawLine(_collisionPos[i].x - lineSize, _collisionPos[i].y + lineSize, _collisionPos[i].x + lineSize + 1, _collisionPos[i].y - lineSize - 1, color);
	}*/


	//DrawFormatStringToHandle(_bodyRect.x, _bodyRect.y, COLOR_WHITE, fontHandle, "%d", 2 - _type);
}





// 機械パーツの初期化処理
void MeatBall::MetalPartsInit(int metalPartsModel[])
{
	// 機械パーツの情報
	struct METAL_PARTS_INFO
	{
		int type;						// 種類（METAL, BOLT, GEAR）
		int level;						// 現在のミートボールのレベルがこの数値を超えていたら、このパーツを描画する
		const char* attachFrameName;	// このパーツを配置するリグの名前
		VECTOR scale;					// 3Dモデルの拡大値
		VECTOR rotation;				// 3Dモデルの回転値
	};

	METAL_PARTS_INFO mpInfo[MEATBALL_METAL_PARTS_MAX] = {
		// level2
		{ METAL_PARTS_METAL, 2, "meat5", VGet(3.0f, 3.0f, 3.0f), VGet(0.0f, 0.0f, 2.0f)},
		{ METAL_PARTS_BOLT, 2, "meat8", VGet(3.0f, 3.0f, 3.0f), VGet(0.0f, 0.0f, 2.0f)},
		{ METAL_PARTS_GEAR, 2, "meat7", VGet(4.0f, 4.0f, 4.0f), VGet(0.0f, 0.0f, 3.0f)},
		{ METAL_PARTS_BOLT, 2, "meat14", VGet(2.0f, 2.0f, 2.0f), VGet(1.0f, 1.0f, 2.5f)},
		// level3
		{ METAL_PARTS_BOLT, 3,  "meat2", VGet(2.5f, 2.5f, 2.5f), VGet(6.0f, 0.0f, 0.0f)},
		{ METAL_PARTS_METAL, 3,"meat3", VGet(2.0f, 2.0f, 2.0f), VGet(4.0f, 1.0f, 4.0f)},
		{ METAL_PARTS_METAL, 3,"meat4", VGet(4.0f, 4.0f, 4.0f), VGet(0.5f, 0.0f, 6.0f)},
		{ METAL_PARTS_GEAR, 3,  "meat6", VGet(2.0f, 2.0f, 2.0f), VGet(0.0f, 2.0f, 4.0f)}
	};

	// 上記の情報をメンバ変数 _metal[] に設定する
	for (int i = 0; i < MEATBALL_METAL_PARTS_MAX; i++){
		_metal[i].modelHandle = MV1DuplicateModel(metalPartsModel[mpInfo[i].type]);
		_metal[i].level = mpInfo[i].level;
		// 指定されたリグの番号を取得する
		_metal[i].attachNum = MV1SearchFrame(_modelHandle, mpInfo[i].attachFrameName);
		_metal[i].scale = mpInfo[i].scale;
		_metal[i].rotation = mpInfo[i].rotation;
	}
}

// 機械パーツの計算処理
// ミートボールの特定のリグに機械パーツを持たせる
void MeatBall::MetalPartsProcess()
{
	for(int i = 0; i < MEATBALL_METAL_PARTS_MAX; i++) {
		// 現在のレベルが機械パーツを配置するレベル以上の場合のみ処理を行う
		if(_metal[i].level > _level) continue;

		// アタッチするモデルのフレーム〇番の座標を取得する
		VECTOR weaponFramePosition = MV1GetFramePosition(_metal[i].modelHandle, -1);
		// アタッチするモデルをフレーム〇番の座標を原点にするための平行移動行列を作成
		MATRIX TransMat = MGetTranslate(VScale(weaponFramePosition, -1.0f));
		// アタッチされるモデルのフレーム〇番の行列を取得
		MATRIX characterFrameMat = MV1GetFrameLocalWorldMatrix(_modelHandle, _metal[i].attachNum);

		// 大きさ調整
		MATRIX mScale = MGetScale(_metal[i].scale);

		// 回転調整
		MATRIX mRotation = MGetRotX(_metal[i].rotation.x);
		mRotation = MMult(mRotation, MGetRotY(_metal[i].rotation.y));
		mRotation = MMult(mRotation, MGetRotZ(_metal[i].rotation.z));




		// 行列の合成
		MATRIX MixMatrix = MMult(mScale, mRotation);
		MixMatrix = MMult(MixMatrix, TransMat);
		MixMatrix = MMult(MixMatrix, characterFrameMat);


		// 合成した行列をアタッチするモデルにセット
		MV1SetMatrix(_metal[i].modelHandle, MixMatrix);
	}
}

// 機械パーツの描画処理
void MeatBall::MetalPartsDraw()
{
	for (int i = 0; i < MEATBALL_METAL_PARTS_MAX; i++) {
		// 現在のレベルが機械パーツを配置するレベル以上の場合のみ処理を行う
		if (_metal[i].level > _level) continue;

		MV1DrawModel(_metal[i].modelHandle);
	}
}

// スケーリングの開始設定
void MeatBall::SetScaling()
{
	if (_isScaling == 0) {
		_isScaling = 1; 
		_scalingCnt = 0;
	}
};


// スケーリング処理
void MeatBall::ScalingProcess(VECTOR vScale)
{
	VECTOR vTmp = VScale(vScale, 0.6f);
	VECTOR magnification;

	// 軸ごとにスケーリングする
	// EasingOutElastic() では、ゴムのような質感のイージング処理を行う
	magnification.x = EasingOutElastic(_scalingCnt, vTmp.x, vScale.x, 24);
	magnification.y = EasingOutElastic(_scalingCnt, vTmp.y, vScale.y, 24);
	magnification.z = EasingOutElastic(_scalingCnt, vTmp.z, vScale.z, 24);

	MV1SetScale(_modelHandle, magnification);

	_scalingCnt++;
	// スケーリングが終わったら
	if (_scalingCnt > 3 * 8) {
		_isScaling = 0;
		_scalingCnt = 0;
	}
}

// 煙エフェクトのセット
void MeatBall::SetSmokeEffect(HITCHIP hitChip)
{
	
	VECTOR effectPos = { hitChip.x * CHIPSIZE_W + CHIPSIZE_W / 2, hitChip.y * CHIPSIZE_H + CHIPSIZE_H / 2, 0 };
	VECTOR vDir = VSub(_pos, effectPos);
	vDir = VNorm(vDir);
	effectPos = VAdd(effectPos, VScale(vDir, CHIPSIZE_W * 3 / 4));
	float effectSize = _r * 3;
	if (effectSize < 60) {
		effectSize = 60;
	}
	if (effectSize > 300) {
		effectSize = 300;
	}
	SetEffect(EFFECT_TYPE_SMOKE, effectPos.x - effectSize / 2, effectPos.y - effectSize / 2, effectSize, effectSize);
}

// チップ破壊エフェクトのセット
void MeatBall::SetChipBreakEffect(HITCHIP hitChip)
{
	int effectX = hitChip.x * CHIPSIZE_W - CHIPSIZE_W;
	int effectY = hitChip.y * CHIPSIZE_H - CHIPSIZE_H;
	int effectSize = 64 * 3;

	SetEffect(EFFECT_TYPE_CHIP_BREAK, effectX, effectY, effectSize, effectSize);
}

// オーラ描画の設定
void MeatBall::SetIsDrawAura(int n)
{
	if (n == 1) {
		if (_isDrawAura == 0) {
			_effectCnt = 0;
		}
	}

	_isDrawAura = n;
}

// 呼び戻しエフェクトのセット
void MeatBall::SetReturnEffect()
{
	int effectSize = _r * 4;
	if (effectSize <= 120) {
		effectSize = 120;
	}
	VECTOR effectPos = _pos;

	//SetEffect(effectPos.x, effectPos.y, EEFECT_TYPE_MEATBALL_RETURN, 0, effectSize, effectSize);
	//SetEffect(effectPos.x, effectPos.y, EEFECT_TYPE_MEATBALL_RETURN, 0, effectSize, effectSize);


	SetEffect(EEFECT_TYPE_MEATBALL_RETURN, effectPos.x, effectPos.y, effectSize, effectSize);

}
