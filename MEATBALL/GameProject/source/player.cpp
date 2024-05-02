#include "player.h"
#include <math.h>
#include "mymath.h"
#include "mydraw.h"

#include "scenegamemain.h"

#include "gamepad.h"
#include "resource.h"
#include "meatball.h"

#include "jumppad.h"
#include "gimmick_data.h"

#include "effect.h"

// モーション用定数
#define PLAYER_MOTION_RUN 0

#define PLAYER_MOTION_WAIT_LEFT 1
#define PLAYER_MOTION_WAIT_RIGHT 2

#define PLAYER_MOTION_JUMP_UP 3
#define PLAYER_MOTION_JUMP_DOWN 4

#define PLAYER_MOTION_SHOOT_WAIT_LEFT 5
#define PLAYER_MOTION_SHOOT_WAIT_RIGHT 6

#define PLAYER_MOTION_SHOOT_LEFT 7
#define PLAYER_MOTION_SHOOT_RIGHT 8

#define PLAYER_MOTION_ABILITY_LEFT 9
#define PLAYER_MOTION_ABILITY_RIGHT 10

#define PLAYER_MOTION_GAMEOVER_LEFT 11
#define PLAYER_MOTION_GAMEOVER_RIGHT 12

#define PLAYER_MOTION_DASH 13

#define PLAYER_MOTION_HIT_LEFT 14
#define PLAYER_MOTION_HIT_RIGHT 15

#define PLAYER_MOTION_PROLOGUE 16
#define PLAYER_MOTION_CLEAR 17


// 移動速度
#define PLAYER_SPEED 8

// ダッシュの持続時間
#define DASH_COUNT_MAX 10





// 被ダメージ後の無敵時間
#define PLAYER_INVINCIBLE_TIME_MAX 120
// 被ダメージ後の移動不可・ハート取得不可の時間
#define PLAYER_KNOCKBACK_TIME_MAX 60



// テクスチャ情報
std::vector<TEXTURE>	Player::_texture;
// マテリアル情報
std::vector<MATERIAL>	Player::_material;

Player::Player(int modelHandle, VECTOR startPos) : CharaBase(modelHandle, startPos) {
	// 大きさの調整
	MV1SetScale(_modelHandle, VGet(1.8f * 1.1f, 1.8f * 1.1f, 1.8f * 1.1f));


	// 初期状態のモーションの設定
	_motionNum = PLAYER_MOTION_WAIT_RIGHT;

	// 複数のアニメーションをアタッチしている状態だと3Dモデルの状態がブレンドされたものになる
	// ブレンドさせたくない場合は
	// ①1つのモーションだけをアタッチする（他のモーションを外すにはデタッチをする）　※DxLibではこちらを想定している
	// ②再生したくないモーションのブレンドレートを0にする (使用メモリや処理負荷が多少上昇する) 
	// MV1SetAttachAnimBlendRate

	// ここでは①の方法をとる
	// 以下のループは totalTime を取るために行う
	for (int i = 0; i < PLAYER_MOTION_MAX; i++) {
		//アニメーションのアタッチ
		_anim[i].index = MV1AttachAnim(_modelHandle, i, -1, FALSE);
		// アタッチしたアニメーションの総再生時間を取得する
		_anim[i].totalTime = MV1GetAttachAnimTotalTime(_modelHandle, _anim[i].index);
		if (i != _motionNum) {
			// アニメーションのデタッチ
			MV1DetachAnim(_modelHandle, _anim[i].index);
		}
	}

	_difBodyRect = { -32, -128, 64, 120 };
	_useGravity = 1;

	_difCenterY = 60;

	// 体の中心位置を更新
	UpdateCenterPos();
	// 当たり判定矩形を更新
	SetCollision();


	_isAttacked = 0;
	_isValidHit = 1;
	_invincibleTime = 0;
	_canPlayHitMotion = 0;


	_canDash = 0;
	_remainDashTime = 0;
	_dashCoolTime = 0;


	_returnCnt = 0;



	_isGameOver = 0;
	_isPlayingGameOverMotion = 0;

	_isDrawAura = 0;
	_auraCnt = 0;
}


// 残像の初期化処理
void Player::AfterImageInit(int modelHandle)
{
	VECTOR modelScale = MV1GetScale(_modelHandle);
	// 残像を出現させるカウント数（_remainDashTime）
	int afterImageFrame[PLAYER_AFTERIMAGE_MAX] = {
		10, 9, 8, 7, 6, 5, 4, 3, 2, 1
	};
	for (int i = 0; i < PLAYER_AFTERIMAGE_MAX; i++) {
		_afterImage[i].use = 0;
		_afterImage[i].frame = afterImageFrame[i];
		_afterImage[i].remainCnt = 0;

		_afterImage[i].modelHandle = MV1DuplicateModel(modelHandle);
		MV1SetScale(_afterImage[i].modelHandle, modelScale);

		_afterImage[i].pos = VGet(0, 0, 0);

		//アニメーションのアタッチ
		_afterImage[i].anim.index = MV1AttachAnim(_afterImage[i].modelHandle, PLAYER_MOTION_DASH, -1, FALSE);
		// アタッチしたアニメーションの総再生時間を取得する
		_afterImage[i].anim.totalTime = MV1GetAttachAnimTotalTime(_afterImage[i].modelHandle, _afterImage[i].anim.index);


		
		for (int j = 0; j < 6; j++) {
			MV1SetMaterialOutLineWidth(_afterImage[i].modelHandle, j, 1);
			MV1SetMaterialOutLineColor(_afterImage[i].modelHandle, j, GetColorF(0.7, 0.7, 1, 0.1f));

		}

		_afterImage[i].alpha = 1;
	}

}






Player::~Player() {
	//CharaBase::~CharaBase();

	for (int i = 0; i < PLAYER_AFTERIMAGE_MAX; i++) {
		if (_afterImage[i].modelHandle != -1) {
			MV1DeleteModel(_afterImage[i].modelHandle);
			_afterImage[i].modelHandle = -1;
		}
	}
}




// 再生中のSEを停止する
// サイコキネシスSEと呼び戻しSEを停止する
void Player::StopSE()
{
	StopSoundMem(se[11]);
	//StopSoundMem(se[20]);
}

// 煙エフェクトのセット
void Player::SetSmokeEffect()
{
	VECTOR effectPos = _pos;
	effectPos.y -= 20;
	float effectSize = 160;
	SetEffect(EFFECT_TYPE_SMOKE, effectPos.x - effectSize / 2, effectPos.y - effectSize / 2, effectSize, effectSize);
}

// 計算処理
void Player::Process() {

	if (_motionNum != PLAYER_MOTION_SHOOT_RIGHT
		&& _motionNum != PLAYER_MOTION_SHOOT_LEFT
		&& _motionNum != PLAYER_MOTION_SHOOT_WAIT_RIGHT
		&& _motionNum != PLAYER_MOTION_SHOOT_WAIT_LEFT
		&& _motionNum != PLAYER_MOTION_ABILITY_RIGHT
		&& _motionNum != PLAYER_MOTION_ABILITY_LEFT
		) {
		_isDrawAura = 0;
	}

	// 前のフレームのモーション番号を保持する
	int motionNum_old = _motionNum;


	// 一度ミートボールの_isDrawFullArrow（方向線を最大の長さで描画するかどうか）を0にする
	meatBall->SetIsDrawFullArrow(0);

	// 前のフレームの重力値を保持する
	int gravity_old = _gravity;


	if (_motionNum == PLAYER_MOTION_SHOOT_RIGHT
		|| _motionNum == PLAYER_MOTION_SHOOT_LEFT
		|| _motionNum == PLAYER_MOTION_ABILITY_RIGHT
		|| _motionNum == PLAYER_MOTION_ABILITY_LEFT
		) {
		if (_returnCnt == 0) {
			_gravity = 2;
		}
	}



	// 共通処理
	// 重力処理
	CharaBase::Process();


	// 着地処理
	// 前のフレームの重力値が3以上 かつ このフレームで床に触れていたら
	if (gravity_old >= 3 && _stand == 1) {
		PlaySoundMem(se[12], DX_PLAYTYPE_BACK);

		// 煙エフェクトのセット
		SetSmokeEffect();
	}

	// ボス登場演出中は操作をできないようにする
	if (gCam._phase == 99) {
		StopDash();
		if (_stand == 1) {
			_motionNum = PLAYER_MOTION_WAIT_RIGHT;
		}
	}

	// ゲームオーバーモーション再生中出ない かつ ボス登場演出中でないなら
	if (_isPlayingGameOverMotion != 1 && gCam._phase != 99) {

		// ジャンプ台との当たり判定
		for (int i = 0; i < JUMPPAD_MAX; i++) {
			if (jumpPad[i]->GetUse() == 0) continue;
			// ジャンプ台本体との当たり判定
			MYRECT jRect = jumpPad[i]->GetRect();
			if (IsHitBox(_bodyRect.x, _bodyRect.y, _bodyRect.w, _bodyRect.h, jRect.x, jRect.y, jRect.w, jRect.h) == 1) {

				// 押し出し処理
				VECTOR pPos = _pos;
				VECTOR jPos = jumpPad[i]->GetPos();

				int hitJumpPad = CheckExtrusionDirection(_bodyRect, jRect);
				switch (hitJumpPad)
				{
				case 0: // 横方向
					float w;
					if (pPos.x < jPos.x) {
						w = _bodyRect.x + _bodyRect.w - jRect.x;
						pPos.x -= w;
					}
					else {
						w = jRect.x + jRect.w - _bodyRect.x;
						pPos.x += w;
					}
					_pos = pPos;
					break;

				case 1: // 縦方向
					float h;
					if (pPos.y < jPos.y) {
						h = _bodyRect.y + _bodyRect.h - jRect.y;
						pPos.y -= h;
						_stand = 1;
					}
					else {
						h = _bodyRect.y - (jRect.y + jRect.h);
						pPos.y += h;
					}
					_pos = pPos;
					_gravity = 0;
					break;
				}


				// 体の中心位置を更新
				UpdateCenterPos();
				// 当たり判定矩形を更新
				SetCollision();

			}

			// ジャンプ範囲との当たり判定
			// この範囲とプレイヤーが当たっていた場合、上にジャンプさせる
			if (jumpPad[i]->GetActive() == 1) {
				jRect = jumpPad[i]->GetBoundRect();
				if (IsHitBox(_bodyRect.x, _bodyRect.y, _bodyRect.w, _bodyRect.h, jRect.x, jRect.y, jRect.w, jRect.h) == 1) {
					_gravity = -32;
					_motionNum = PLAYER_MOTION_JUMP_UP;
					jumpPad[i]->SetActive();

					//_stand = 1;
					_canDash = 1;
				}
			}
		}


		if (_invincibleTime < PLAYER_INVINCIBLE_TIME_MAX - PLAYER_KNOCKBACK_TIME_MAX) {

			// 発射構え
			// ホールド中
			if (gPad->_input.Buttons[PAD_BUTTON_X] != 0 && (meatBall->GetIsAttack() == 0 || meatBall->GetCanReattack() == 1) && _remainDashTime <= 0) {
				_useGravity = 1;

				_returnCnt = 0;

				_motionNum = PLAYER_MOTION_SHOOT_WAIT_RIGHT;
				_isDrawAura = 1;

				// 方向線の描画に関する設定
				meatBall->SetIsDrawFullArrow(1);


				// サイコキネシスSEの再生
				if (CheckSoundMem(se[11]) == 0) {
					PlaySoundMem(se[11], DX_PLAYTYPE_BACK);
				}


			}
			// ミートボールを呼び戻す
			// ホールド中
			else if (gPad->_input.Buttons[PAD_BUTTON_B] && meatBall->GetIsAttack() == 1 && _remainDashTime <= 0) {
				_useGravity = 1;

				_returnCnt++;

				_motionNum = PLAYER_MOTION_ABILITY_RIGHT;
				_isDrawAura = 2;

				// 呼び戻しカウントがMAXを超えたら、呼び戻し処理を行う
				if (_returnCnt >= MEATBALL_RETURN_CNT_MAX) {
					meatBall->SetAttack(VGet(0, 0, 0), 0);
					meatBall->SetReturnEffect();


					
					PlaySoundMem(voice[rand() % 4], DX_PLAYTYPE_BACK);

				}
			}else{
				StopSE();

				_returnCnt = 0;


				// ミートボールの発射処理
				if (gPad->_rel[PAD_BUTTON_X] != 0) {
					if (meatBall->GetIsAttack() == 0) {
						StopDash();

						VECTOR meatBallStartPos = _centerPos;
						meatBallStartPos.y -= 64;
						meatBall->SetAttack(meatBallStartPos);

						_motionNum = PLAYER_MOTION_SHOOT_RIGHT;
						PlaySoundMem(voice[(rand() % 4) + 5], DX_PLAYTYPE_BACK);

					}
					// ミートボールの方向転換
					else if (meatBall->GetCanReattack() == 1) {
						StopDash();

						meatBall->SetAttack();
						_motionNum = PLAYER_MOTION_SHOOT_RIGHT;
						PlaySoundMem(voice[(rand() % 4)+5], DX_PLAYTYPE_BACK);

					}
				}

				if (_motionNum != PLAYER_MOTION_SHOOT_RIGHT
					&& _motionNum != PLAYER_MOTION_SHOOT_LEFT
					&& _motionNum != PLAYER_MOTION_ABILITY_RIGHT
					&& _motionNum != PLAYER_MOTION_ABILITY_LEFT

					) {

					// ダッシュ設定
					if (_canDash == 1 && _dashCoolTime <= 0 && gPad->_trg[PAD_BUTTON_RT] != 0) {
						PlaySoundMem(se[13], DX_PLAYTYPE_BACK);
						_canDash = 0;
						_remainDashTime = DASH_COUNT_MAX;
						_dashCoolTime = 20;

						_useGravity = 0;
						_gravity = 0;
					}

					if (_dashCoolTime > 0) {
						_dashCoolTime--;
					}

					// ダッシュ移動処理
					if (_remainDashTime > 0) {

						
						// 残像のセット
						SetAfterImage();

						_remainDashTime--;

						_speed = PLAYER_SPEED * 3;

						// 移動処理
						VECTOR vDir = VNorm(_direction);
						VECTOR vMove = VScale(vDir, _speed);
						_pos.x += vMove.x;
						MYRECT tmpRect = _bodyRect;
						tmpRect.x += vMove.x;
						HITCHIP hitChip = gMap.IsHitBoxAndMapChip(tmpRect);


						float w = 0;

						int isDiagonalHit = 0;
						VECTOR startPos, endPos;
						switch (hitChip.chip_no)
						{
						case -1:
							break;

						default:
							if (vMove.x < 0) {
								w = (hitChip.x * CHIPSIZE_W + CHIPSIZE_W) - tmpRect.x;
							}
							else if (vMove.x > 0) {
								w = hitChip.x * CHIPSIZE_W - (tmpRect.x + tmpRect.w);
								w--;
							}
							_pos.x += w;



							break;


						case LOWER_RIGHT: // 右下
							isDiagonalHit = 1;
							startPos = VGet(hitChip.x * CHIPSIZE_W, hitChip.y * CHIPSIZE_H + CHIPSIZE_H, 0);
							endPos = VGet(hitChip.x * CHIPSIZE_W + CHIPSIZE_W, hitChip.y * CHIPSIZE_H, 0);

							// 
							//if (_direction.x < 0) {
							_pos.y += CHIPSIZE_H / 2;
							//}
							break;
						case LOWER_LEFT: // 左下
							isDiagonalHit = 1;
							startPos = VGet(hitChip.x * CHIPSIZE_W, hitChip.y * CHIPSIZE_H, 0);
							endPos = VGet(hitChip.x * CHIPSIZE_W + CHIPSIZE_W, hitChip.y * CHIPSIZE_H + CHIPSIZE_H, 0);

							//if (_direction.x < 0) {
							_pos.y += CHIPSIZE_H / 2;
							//}

							break;

						}

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
						}
						else {
							// 体の中心位置を更新
							UpdateCenterPos();

							// プレイヤー当たり判定矩形の位置を調整する
							SetCollision();
						}



						_motionNum = PLAYER_MOTION_DASH;


					}
					else {
						_useGravity = 1;


						if (gPad->_input.X != 0) {
							//_direction = VGet(gPad->_input.X, gPad->_input.Y, 0); // 左ステック入力があった場合のみdirectionを更新する
							_direction = VGet(gPad->_input.X, 0, 0); // 左ステック入力があった場合のみdirectionを更新する

							// LTボタンが押されているときは動かないようにする
							//if (gPad->_input.Buttons[PAD_BUTTON_LT] == 0) {




							if (_remainDashTime <= 0) {
								{
									_speed = PLAYER_SPEED;


									// 移動処理
									VECTOR vDir = VNorm(_direction);
									VECTOR move = VScale(vDir, _speed);
									_pos.x += move.x;


									MYRECT tmpRect = _bodyRect;
									tmpRect.x += move.x;
									HITCHIP hitChip = gMap.IsHitBoxAndMapChip(tmpRect);

									float w = 0;
									switch (hitChip.chip_no)
									{
									case -1:
										break;

									default:
										if (move.x < 0) {
											w = (hitChip.x * CHIPSIZE_W + CHIPSIZE_W) - tmpRect.x;
											//_pos.y--;
										}
										else if (move.x > 0) {
											w = hitChip.x * CHIPSIZE_W - (tmpRect.x + tmpRect.w);
											w--;
										}
										_pos.x += w;
										break;

									case LOWER_RIGHT:
									case LOWER_LEFT:
										break;
									}


								}

								// 体の中心位置を更新
								UpdateCenterPos();
								// 当たり判定矩形を更新
								SetCollision();

							}
							if (_stand == 1) {
								_motionNum = PLAYER_MOTION_RUN;
							}
							//}

						}
						else {
							if (_stand == 1) {
								_motionNum = PLAYER_MOTION_WAIT_RIGHT;
							}
						}
					}



					// ジャンプ処理
					if (_remainDashTime <= 0) {
						if (_stand == 1) {
							_canDash = 1;
							if (gPad->_trg[PAD_BUTTON_A] != 0) {
								_gravity = -25;
								_motionNum = PLAYER_MOTION_JUMP_UP;
								PlaySoundMem(se[10], DX_PLAYTYPE_BACK);

								// 煙エフェクトのセット
								SetSmokeEffect();
							}
						}
						else {
							if (_gravity >= 0) {
									_motionNum = PLAYER_MOTION_JUMP_DOWN;
							}
						}
					}


				}

			}

		}
		else {
			if (_canPlayHitMotion == 1) {
				_motionNum = PLAYER_MOTION_HIT_RIGHT;
				_canPlayHitMotion = 0;
			}
		}

		if (_isPlayingGameOverMotion != 1) {

			// ハートを取る処理
			if (_heart.GetUse() == 1 && _heart.GetLost() != 1) {
				MYRECT heartRect = _heart.GetBodyRect();

				if (IsHitBox(_bodyRect.x, _bodyRect.y, _bodyRect.w, _bodyRect.h,
					heartRect.x, heartRect.y, heartRect.w, heartRect.h) == 1
					)
				{
					if (_invincibleTime < PLAYER_INVINCIBLE_TIME_MAX - PLAYER_KNOCKBACK_TIME_MAX) {
						_heart.SetUse(0);
					}
				}
			}
		}
	}




	// モデルの回転処理
	UpdateModelRotation();




	_heart.Process();


	// 被ダメージ時
	if (_isAttacked == 1) {
		_remainDashTime = 0;
		_useGravity = 1;
		_isDrawAura = 0;

		if (_heart.GetUse() != 1) {
			if (_heart.GetLost() != 1) {
				SetDamage();
			}
			else {
				_isPlayingGameOverMotion = 1;
				_motionNum = PLAYER_MOTION_GAMEOVER_RIGHT;
				PlaySoundMem(voice[14], DX_PLAYTYPE_BACK);
			}

		}
		else {
			_isPlayingGameOverMotion = 1;
			_motionNum = PLAYER_MOTION_GAMEOVER_RIGHT;
			PlaySoundMem(voice[14], DX_PLAYTYPE_BACK);
		}
		_isAttacked = 0;

	}


	//if (_isGameOverMotion == 1) {
	//	_heart.SetUse(0);
	//}




	SetMotionReverse();
	MotionProcess(motionNum_old);
	AfterImageProcess();



	if (_invincibleTime > 0) {
		_invincibleTime--;


		int cnt = _invincibleTime % 30;
		if (cnt < 20) {
			ChangeTexture(1);
			ChangeMaterialDifColor(1);
		}
		else {
			ChangeTexture(0);
			ChangeMaterialDifColor(0);
		}
	}
	else {
		if (_isPlayingGameOverMotion != 1) {
			_isValidHit = 1;
		}
		ChangeTexture(0);
		ChangeMaterialDifColor(0);
	}

}

// ダッシュを途中でキャンセルする
void Player::StopDash()
{
	_remainDashTime = 0;
	_useGravity = 1;
	_isDrawAura = 0;

}

// 被ダメージ処理
void Player::SetDamage()
{	
	_isValidHit = 0;
	_invincibleTime = PLAYER_INVINCIBLE_TIME_MAX;
	_canPlayHitMotion = 1;
	_heart.SetUse(1);
	_heart.SetPos(_centerPos);

	PlaySoundMem(voice[(rand() % 2) + 15], DX_PLAYTYPE_BACK);

	//ChangeTexture(1);
	//ChangeMaterialDifColor(1);
}




// キャラクターの向いている向きによってモーションを変更する
// 
// Process処理内でモーションを指定するときは全て右向きのモーションを指定する
// キャラクターが左を向いている場合は、左向きのモーションに変更する
void Player::SetMotionReverse()
{
	switch (_motionNum)
	{
	default:
		break;

	case PLAYER_MOTION_WAIT_RIGHT:
	case PLAYER_MOTION_SHOOT_WAIT_RIGHT:
	case PLAYER_MOTION_SHOOT_RIGHT:
	case PLAYER_MOTION_ABILITY_RIGHT:
	case PLAYER_MOTION_GAMEOVER_RIGHT:
	case PLAYER_MOTION_HIT_RIGHT:

		if (_direction.x < 0) {
			_motionNum--;
		}
		break;
	}
}

void Player::MotionProcess(int motionNum_old)
{
	_playTime += 1;

	// 特定のモーションの再生が最後まで終わったら別のモーションに切り替える
	if (motionNum_old == _motionNum) {
		switch (_motionNum)
		{
		// 発射モーションが終わったら待機モーションに切り替える
		case PLAYER_MOTION_SHOOT_RIGHT:
		case PLAYER_MOTION_SHOOT_LEFT:
			if (_playTime >= _anim[_motionNum].totalTime - 20) {
				_playTime = 0;
				_isDrawAura = 0;
				_motionNum = PLAYER_MOTION_WAIT_RIGHT;
				SetMotionReverse();
			}
			break;

		// 呼び戻しモーションが終わったら待機モーションに切り替える
		case PLAYER_MOTION_ABILITY_RIGHT:
		case PLAYER_MOTION_ABILITY_LEFT:

			if (_returnCnt == 0) {
				if (_playTime >= _anim[_motionNum].totalTime - 60) {

					_playTime = 0;

					_motionNum = PLAYER_MOTION_WAIT_RIGHT;
					SetMotionReverse();

				}
			}
			else {
				if (_playTime >= _anim[_motionNum].totalTime) {
					_playTime = 40;
					if (_motionNum == PLAYER_MOTION_ABILITY_LEFT) {
						_playTime = 43;
					}
				}
			}
			break;


		// ダメージモーションが終わったら待機モーションに切り替える
		case PLAYER_MOTION_HIT_RIGHT:
		case PLAYER_MOTION_HIT_LEFT:
			if (_playTime >= _anim[_motionNum].totalTime) {
				_playTime = 0;
				_motionNum = PLAYER_MOTION_WAIT_RIGHT;
				SetMotionReverse();
			}
			break;
		}
	}

	if (_playTime >= _anim[_motionNum].totalTime) {

		
		

		// ゲームオーバーモーションが終わったら
		if (_motionNum == PLAYER_MOTION_GAMEOVER_RIGHT || _motionNum == PLAYER_MOTION_GAMEOVER_LEFT) {
			
			_isGameOver = 1;
			

			//_isGameOverMotion = 0;
			//_motionNum = PLAYER_MOTION_WAIT_RIGHT;
			//SetMotionReverse();

			/*if (_direction.x > 0) {
				
			}
			else {
				_motionNum = PLAYER_MOTION_WAIT_LEFT;
			}*/
			//_isGameOver = 0;
		}
		else	if (_motionNum == PLAYER_MOTION_ABILITY_RIGHT || _motionNum == PLAYER_MOTION_ABILITY_LEFT) {
			_motionNum = PLAYER_MOTION_WAIT_RIGHT;
			SetMotionReverse();

			/*if (_direction.x > 0) {
				
			}
			else {
				_motionNum = PLAYER_MOTION_WAIT_LEFT;
			}*/
		}

		_playTime = 0;

		switch (_motionNum)
		{
		case PLAYER_MOTION_JUMP_UP:
			_playTime = 5.0f;
			break;
		case PLAYER_MOTION_JUMP_DOWN:
			_playTime = 10.0f;
			break; 
		case PLAYER_MOTION_DASH:
			_playTime = 5.0f;
			break;
		}
	}


	if (_isGameOver != 1) {

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
}


void Player::AfterImageProcess()
{
	for (int i = 0; i < PLAYER_AFTERIMAGE_MAX; i++) {
		if (_afterImage[i].use == 1) {

			// 毎フレーム透明度をあげる
			_afterImage[i].alpha -= 0.2f;
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

void Player::SetAfterImage()
{
	for (int i = 0; i < PLAYER_AFTERIMAGE_MAX; i++) {
		// 残像の出現フレームと残りダッシュカウントが一致していたら残像を出現させる
		if (_afterImage[i].frame == _remainDashTime) {
			_afterImage[i].use = 1;
			_afterImage[i].remainCnt = 6;

			_afterImage[i].pos = _pos;
			VECTOR modelRot = MV1GetRotationXYZ(_modelHandle);
			MV1SetRotationXYZ(_afterImage[i].modelHandle, modelRot);
			// 再生時間をセットする(アニメーションの再生)
			float afterImagePlayTime = _playTime;
			if (i == 0) { afterImagePlayTime = 0; }
			MV1SetAttachAnimTime(_afterImage[i].modelHandle, _afterImage[i].anim.index, afterImagePlayTime);

			_afterImage[i].alpha = 1;
			break;
		}
	}
}



void Player::SetOutLine() {


	if (_isDrawAura == 1) {
		for (int i = 0; i < 6; i++) {
			MV1SetMaterialOutLineWidth(_modelHandle, i, 1);
			MV1SetMaterialOutLineColor(_modelHandle, i, GetColorF(1, 1, 1, 1));
		}
	}
	else if (_remainDashTime != 0) {
		for (int i = 0; i < 6; i++) {
			MV1SetMaterialOutLineWidth(_modelHandle, i, 1);
			MV1SetMaterialOutLineColor(_modelHandle, i, GetColorF(0.5, 0.7, 1, 1.0f));
		}
	}
	else {
		for (int i = 0; i < 6; i++) {
			MV1SetMaterialOutLineWidth(_modelHandle, i, 0);
		}
	}
}


// 描画位置の補正処理
void Player::AdjustDrawPos(MATRIX mView)
{
	CharaBase::AdjustDrawPos(mView);
	_heart.AdjustDrawPos(mView);

	
	for (int i = 0; i < PLAYER_AFTERIMAGE_MAX; i++) {
		if (_afterImage[i].use == 1) {
			VECTOR tmpPos = _afterImage[i].pos;
			tmpPos = VTransform(tmpPos, mView);
			tmpPos = ConvScreenPosToWorldPos(tmpPos);
			tmpPos.z = 100 * (10 - _afterImage[i].remainCnt);
			MV1SetPosition(_afterImage[i].modelHandle, tmpPos);
		}
	}
}



void Player::UpdateWorldPos()
{
	// スクリーン座標をワールド座標に変換
	_worldPos = ConvScreenPosToWorldPos(_screenPos);
	_worldPos.z = 0; // z軸表示位置の調整
}


void Player::Draw()
{
	// アウトラインの設定
	SetOutLine();


	switch(_isDrawAura)
	{
		// 通常時
	default:
		// 残像の描画
		for(int i = 0; i < PLAYER_AFTERIMAGE_MAX; i++) {
			if(_afterImage[i].use == 1) {
				MV1DrawModel(_afterImage[i].modelHandle);
			}
		}
		CharaBase::Draw();

		_auraCnt = 0;
		break;

		// オーラが出ている状態
		// ミートボール発射ホールド中
	case 1:
	{
		VECTOR auraPos = _centerPos;
		auraPos.y -= 20;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 140.f);
		// オーラ（後ろ）の描画
		MyDrawModiGraph(gCam.GetViewMatrix(), auraPos, 1.0f, 0.0f, _bodyRect.w * 6, _bodyRect.h * 3.5, cgAttackAura[(_auraCnt / 4) % 20]);



		CharaBase::Draw();

		// オーラ（前）の描画
		MyDrawModiGraph(gCam.GetViewMatrix(), auraPos, 1.0f, 0.0f, _bodyRect.w * 6, _bodyRect.h * 3.5, cgAttackAura[(_auraCnt / 2) % 20]);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		_auraCnt++;
	}
	break;



	// オーラが出ている状態
	// ミートボール呼び戻し中
	case 2:
	{
		//if (_auraCnt < 4 * 13) {
		VECTOR auraPos = _pos;
		auraPos.y -= _bodyRect.h;
		auraPos.y -= 25;

		float effectSize = _bodyRect.w * 2.5;


		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255.0f * 0.6f);
		// オーラ（後ろ）の描画
		MyDrawModiGraph(gCam.GetViewMatrix(), auraPos, 1.0f, 0.0f, effectSize, effectSize, cgReturnAura[(_auraCnt / 5) % 13]);
		MyDrawModiGraph(gCam.GetViewMatrix(), auraPos, 1.0f, 0.0f, effectSize, effectSize, cgReturnAura[(_auraCnt / 3) % 13]);



		CharaBase::Draw();

		// オーラ（前）の描画
		//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255.0f * 0.6f);
		MyDrawModiGraph(gCam.GetViewMatrix(), auraPos, 1.0f, 0.0f, effectSize, effectSize, cgReturnAura[(_auraCnt / 1) % 13]);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		/*}
		else
		{
			CharaBase::Draw();
		}*/
		_auraCnt++;

	}
	break;
	}

	_heart.Draw();

}







// デバッグ情報の表示
void Player::DrawDebugInfo(MATRIX mView, int fillFrag)
{
	VECTOR rectPos = VGet(_bodyRect.x, _bodyRect.y, 0);
	rectPos = VTransform(rectPos, mView);

	int colorTbl[] = {
		COLOR_WHITE,
		COLOR_BLUE
	};

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 140.f);
	//DrawBox(_bodyRect.x, _bodyRect.y, _bodyRect.x + _bodyRect.w, _bodyRect.y + _bodyRect.h, COLOR_BLUE, TRUE);
	DrawBox(rectPos.x, rectPos.y, rectPos.x + _bodyRect.w, rectPos.y + _bodyRect.h, colorTbl[_isValidHit], TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


	_heart.DrawDebugInfo(mView);
}

// 構造体「TEXTURE」の初期化
void Player::TextureInit(int modelHandle)
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
void Player::ChangeTexture(int isDamaged)
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
void Player::MaterialInit(int modelHandle)
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
void Player::ChangeMaterialDifColor(int isDamaged)
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
