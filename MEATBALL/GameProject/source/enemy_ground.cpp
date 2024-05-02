#include "enemy_ground.h"
#include "enemy_data.h"
#include "jumppad.h"
#include "gimmick_data.h"
#include "resource.h"

// テクスチャ画像ハンドル
int EnemyGround::_cgTexEnGround[2] = { -1, -1 };
// マテリアル情報
std::vector<MATERIAL>	EnemyGround::_material;

// 地上をうろつき、プレイヤーを見つけると追いかけてくる敵
// 通常状態とメタル状態がある
EnemyGround::EnemyGround(int modelHandle, int size, int index, VECTOR startPos, int type) : EnemyBase(modelHandle, index, startPos)
{
	_type = ENEMY_TYPE_GROUND;
	_index = index;
	_metalModel = -1;
	_isChange = 0;
	_attackCoolTime = 0;

	_isMetal = type;
	if (_isMetal == 1) {
		_metalModel = MV1DuplicateModel(metalPartsModel[METAL_PARTS_GEAR]);
		MV1SetTextureGraphHandle(_modelHandle, 0, _cgTexEnGround[1], FALSE);
	}

	// 敵の強さを初期設定する
	SetStregth(_type, size);

	_pos.x = _pos.x * CHIPSIZE_W + _difBodyRect.w / 2;
	_pos.y = _pos.y * CHIPSIZE_W + _difBodyRect.h;


	//_pos.x = _pos.x * CHIPSIZE_W + 48;
	//_pos.y = _pos.y * CHIPSIZE_H + 160;



	_walkStartPos = _pos;


	_direction = VGet(-1, 0, 0);
	_speed = ENEMY_GROUND_SPEED_WALK;
	_useGravity = 1;

	_screenPos = _pos;
	UpdateWorldPos();
	MV1SetPosition(_modelHandle, _worldPos);

	_difCenterY = 80;

	// 体の中心位置を更新
	UpdateCenterPos();
	// 当たり判定矩形を更新
	SetCollision();

	_fallStartPos = VGet(0, 0, 0);
	_fallEndPos = VGet(0, 0, 0);
	_fallCnt = 0;

	_canPlaySe = 1;


	// 初期状態のモーションの設定
	_motionNum = GROUND_MOTION_WALK;

	// モーションの初期化処理
	MotionInit(GROUND_MOTION_MAX);
}

EnemyGround::~EnemyGround()
{
	//CharaBase::~CharaBase();
}




void EnemyGround::Process(VECTOR playerPos) {

	// メタル状態なら
	if (_isMetal == 1) {
		// 通常状態へ移行させるなら
		if (_isChange == 1) {
			_isMetal = 0;

			// 機械パーツのモデルを地上敵本体から分離する
			MV1SetMatrix(_metalModel, MGetIdent());

			// 機械パーツの落下開始座標を保持する
			_fallStartPos = _pos;
			_fallStartPos.y -= _bodyRect.h / 2;
			// 機械パーツの落下終了座標を保持する
			_fallEndPos = _pos;
			_fallEndPos.y -= _bodyRect.h / 8;
			// 落下のカウントをリセットする
			_fallCnt = 0;

			// 地上敵本体のサイズにあわせて機械パーツモデルのサイズを調整する
			VECTOR eSize = MV1GetScale(_modelHandle);
			MV1SetScale(_metalModel, VScale(eSize, 5));
		}
	}

	// この敵が倒されていなかったら
	if (_use == 1) {
		int motionNum_old = _motionNum;

		// 共通処理
		// 重力処理とヒットストップ処理
		EnemyBase::Process(playerPos);

		// ヒットストップ中でなければ
		if (_isHitStop == 0) {

			// ジャンプ台との当たり判定
			for (int i = 0; i < JUMPPAD_MAX; i++) {
				if (jumpPad[i]->GetUse() == 0) continue;
				// ジャンプ台本体との当たり判定
				MYRECT jRect = jumpPad[i]->GetRect();
				if (IsHitBox(_bodyRect.x, _bodyRect.y, _bodyRect.w, _bodyRect.h, jRect.x, jRect.y, jRect.w, jRect.h) == 1) {

					// 押し出し処理
					VECTOR pPos = _pos;
					VECTOR jPos = jumpPad[i]->GetPos();
					
					// 押し出し方向を計算する
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
			}


			// 移動状態ごとの処理

			// 攻撃状態
			if (_isAttack == 1) {
				// 移動速度の設定
				if (_isMetal != 1) {
					_speed = ENEMY_GROUND_SPEED_ATTACK;
				}
				else {
					_speed = ENEMY_METAL_SPEED_ATTACK;
				}
				_motionNum = GROUND_MOTION_RUN;

				// SEを再生可能なら
				if (_canPlaySe == 1) {
					// 叫び声SEの再生
					PlaySoundMem(se[0], DX_PLAYTYPE_BACK);
					// SE再生不可状態にする
					_canPlaySe = 0;
				}
			}
			// 追跡状態
			else if (_isSpotPlayer == 1) {
				// 移動速度の設定
				if (_isMetal != 1) {
					_speed = ENEMY_GROUND_SPEED_RUN;
				}
				else {
					_speed = ENEMY_METAL_SPEED_RUN;
				}

				// ダッシュ攻撃のクールタイム（疲れ状態）でなければ向きを変える
				if (_attackCoolTime <= 0) {
					if (playerPos.x < _pos.x) {
						_direction = VGet(-1, 0, 0);
					}
					else {
						_direction = VGet(1, 0, 0);
					}
				}

				_motionNum = GROUND_MOTION_WALK;
			}
			// 非索敵状態
			else {
				// 移動速度の設定
				if (_isMetal != 1) {
					_speed = ENEMY_GROUND_SPEED_WALK;
				}
				else {
					_speed = ENEMY_METAL_SPEED_WALK;
				}

				_motionNum = GROUND_MOTION_WALK;
			}

			// 攻撃中の処理
			if (_attackRemainTime > 0) {
				_attackRemainTime--;
				// 攻撃が終わったらクールタイムをセットする
				if (_attackRemainTime == 0) {
					SetAttack(0);
					_attackCoolTime = ENEMY_GROUND_COOLTIME;					
				}
			}

			// クールタイム中の処理
			if (_attackCoolTime > 0) {
				_speed = 0;
				_motionNum = GROUND_MOTION_TIRED;

				_attackCoolTime--;
				// 攻撃のクールタイムが終わったら
				if (_attackCoolTime <= 0) {
					_attackCoolTime = 0;
					// SE再生可能状態にする
					_canPlaySe = 1;
				}
			}

			// 落下中は左右移動をしない
			if(_stand != 0) {
				// 移動処理
				VECTOR vDir = _direction;
				VECTOR move = VScale(vDir, _speed);
				_pos.x += move.x;
				//_pos.y += move.y;

				MYRECT tmpRect = _bodyRect;
				tmpRect.x += move.x;

				// マップチップとの当たり判定
				HITCHIP hitChip = gMap.IsHitBoxAndMapChip(tmpRect);
				// 押し出し距離
				float w = 0;
				// 足元が斜めのマップチップに当たっているかどうか
				int hitDiagonalChip = 0;

				// マップチップに当たっていた場合
				switch(hitChip.chip_no)
				{
					// マップチップに当たっていない場合
				case -1:
					break;

					// マップチップに当たっていた場合
				default:
					if(move.x < 0) {
						w = (hitChip.x * CHIPSIZE_W + CHIPSIZE_W) - tmpRect.x;

						if(_isSpotPlayer == 0) {
							_direction = VGet(1, 0, 0);
						}
					}
					else if(move.x > 0) {
						w = hitChip.x * CHIPSIZE_W - (tmpRect.x + tmpRect.w);
						w--;

						if(_isSpotPlayer == 0) {
							_direction = VGet(-1, 0, 0);
						}
					}
					_pos.x += w;
					break;

					// 足元が斜めのマップチップに当たっていた場合
				case LOWER_RIGHT:
				case LOWER_LEFT:
					hitDiagonalChip = 1;
					break;
				}

				// 体の中心位置を更新
				UpdateCenterPos();
				// 当たり判定矩形を更新
				SetCollision();


				//
				// 進行方向に穴があるかを調べる
				// 

				// 足元に斜めのマップチップがある、穴があるかの判定を行わない
				// 坂を下っているときは乗っているマップチップの1つ前にマップチップが無いため、進行方向に穴があると判定されてしまうため
				if(hitDiagonalChip != 1) {

					// プレイヤー追従・ダッシュ攻撃時は、穴の前にとどまる
					// 非索敵時は、折り返す


					// _bodyRectの下両端がマップチップと当たっているかを調べる

					tmpRect = _bodyRect;
					// _bodyRectの左下
					float tmpX = tmpRect.x;
					float tmpY = tmpRect.y + tmpRect.h + 10; // + 10をして、キャラの下にマップチップがあるかを判定する

					int chipX = tmpX / CHIPSIZE_W;
					int chipY = tmpY / CHIPSIZE_H;

					int stop = 0;

					// 足元に斜めのマップチップがない場合のみ、前方に穴がないかを調べる
					switch(gMap.IsHitMapchip(_pos.x, _pos.y + 10))
					{
					case LOWER_RIGHT:
					case LOWER_LEFT:

						break;

					default:

						if(_direction.x < 0) {
							tmpX = tmpRect.x;
							tmpY = tmpRect.y + tmpRect.h + 10; // + 10をして、キャラの下にマップチップがあるかを判定する

							if(gMap.IsHitMapchip(tmpX, tmpY) == -1) {
								// プレイヤー追従・ダッシュ攻撃時
								if(_isSpotPlayer == 1) {
									// 穴の前まで押し戻し停止させる
									float w;
									w = (chipX * CHIPSIZE_W + CHIPSIZE_W) - tmpRect.x;
									_pos.x += w;

									stop = 1;
								}
								// 非索敵時
								else {
									// 移動方向を反転させる
									_direction = VGet(1, 0, 0);
								}
							}


						}
						else {
							// _bodyRectの右下
							tmpX += tmpRect.w;
							chipX = tmpX / CHIPSIZE_W;

							if(gMap.IsHitMapchip(tmpX, tmpY) == -1) {
								// プレイヤー追従・ダッシュ攻撃時
								if(_isSpotPlayer == 1) {
									// 穴の前まで押し戻し停止させる
									float w;
									w = chipX * CHIPSIZE_W - (tmpRect.x + tmpRect.w);
									w--;
									_pos.x += w;

									stop = 1;
								}
								// 非索敵時
								else {
									// 移動方向を反転させる
									_direction = VGet(-1, 0, 0);
								}
							}
						}

						break;
					}


					// 穴の前にとどまる場合に当たり判定を更新する
					// (とどまらない場合は押し戻し処理をしないので、当たり判定更新の必要がない)
					if(stop == 1) {
						// 体の中心位置を更新
						UpdateCenterPos();
						// 当たり判定矩形を更新
						SetCollision();
					}
				}
			}

			// 追跡・攻撃状態なら
			if (_isSpotPlayer == 1) {
				// その後の非索敵時にする左右移動の起点位置を更新する
				_walkStartPos = _pos;
			}
			// 非索敵状態なら
			else {
				// _walkStartPosを起点に左右移動をする
				if (_pos.x < _walkStartPos.x - ENEMY_GROUND_WALK_RANGE / 2) {
					_direction = VGet(1, 0, 0);
				}
				else if (_walkStartPos.x + ENEMY_GROUND_WALK_RANGE / 2 < _pos.x) {
					_direction = VGet(-1, 0, 0);
				}
			}


			// ダッシュ攻撃とそのクールタームが終わったときに画面内に入っているかの判定を行う（画面外に出てもダッシュ攻撃・クールタイムを継続させる）
			if (_attackRemainTime == 0 && _attackCoolTime == 0) {
				// 画面内にいるかを判定
				CheckIsInScreen();

				// 画面外に出ていたら
				if (_isInScreen != 1) {
					SetIsSpotPlayer(0);
					SetAttack(0);
				}
			}

			// モデルの回転処理
			UpdateModelRotation();
		}

		// モーションの計算処理
		MotionProcess(motionNum_old);
	}
}


// モーションの計算処理
void EnemyGround::MotionProcess(int motionNum_old)
{
	_playTime += 1;

	// モーションの再生が終わったら、そのモーションの始めに戻す
	if (_motionNum == GROUND_MOTION_TIRED) {
		// 3Dモデル自体に不備があるため、数字を直接入力した
		if (_playTime >= 179.f) {
			_playTime = 0;
		}
	}
	else if (_motionNum == GROUND_MOTION_WALK) {
		// 3Dモデル自体に不備があるため、数字を直接入力した
		if (_playTime >= 59) {
			_playTime = 0;
		}
	}
	else {
		if (_playTime >= _anim[_motionNum].totalTime) {
			_playTime = 0;
		}
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

// 攻撃状態の設定
// 引数n
// 0: 無効化, 1: 有効化
void EnemyGround::SetAttack(int n)
{
	if(_isAttack == 0 && n == 1) {
		_attackRemainTime = ENEMY_GROUND_ATTACKTIME;
	}
	EnemyBase::SetAttack(n);
}

// 描画位置の補正処理
void EnemyGround::AdjustDrawPos(MATRIX mView)
{
	EnemyBase::AdjustDrawPos(mView);
	MetalPartsAdjustDrawPos(mView);
}

// 描画処理
void EnemyGround::Draw()
{
	
	EnemyBase::Draw();
	if (_metalModel != -1 && _fallCnt <= 60) {
		MV1DrawModel(_metalModel);
	}
}

// テクスチャ画像の貼り替え
// 引数isDamaged によって貼るテクスチャを変える
// 0 : 通常時, 1 : 被ダメージ時
void EnemyGround::ChangeTexture(int isDamaged)
{
	// 被ダメージ時はテクスチャを白くする
	if (isDamaged == 1) {
		MV1SetTextureGraphHandle(_modelHandle, 0, _cgTexWhite, FALSE);
	}
	else
	{
		// 通常時は元に戻す
		if (_metalModel == -1) {
			MV1SetTextureGraphHandle(_modelHandle, 0, _cgTexEnGround[0], FALSE);
		}
		else {
			MV1SetTextureGraphHandle(_modelHandle, 0, _cgTexEnGround[1], FALSE);
		}
	}


}

// 構造体「MATERIAL」の初期化
void EnemyGround::MaterialInit(int modelHandle)
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
void EnemyGround::ChangeMaterialDifColor(int isDamaged)
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

// 機械パーツの描画位置の補正処理
void EnemyGround::MetalPartsAdjustDrawPos(MATRIX mView)
{
	if (_metalModel != -1) {
		if (_isMetal == 1) {
			// アタッチするモデルのフレーム〇番の座標を取得する
			VECTOR weaponFrame1Position = MV1GetFramePosition(_metalModel, -1);
			// アタッチするモデルをフレーム〇番の座標を原点にするための平行移動行列を作成
			MATRIX TransMat = MGetTranslate(VScale(weaponFrame1Position, -1.0f));
			// アタッチされるモデルのフレーム〇番の行列を取得
			MATRIX characterFrame10Mat = MV1GetFrameLocalWorldMatrix(_modelHandle, 19);

			// 大きさ調整
			MATRIX mScale = MGetScale(VGet(5.0f, 5.0f, 5.0f));
			//MATRIX mScale = MGetScale(_defaultSize);

			// 回転調整
			MATRIX mRotation = MGetRotX(DegToRad(90.0f));
			mRotation = MMult(mRotation, MGetRotY(0.0f));
			mRotation = MMult(mRotation, MGetRotZ(0.0f));

			// 行列の合成
			MATRIX MixMatrix = MMult(mScale, mRotation);
			MixMatrix = MMult(MixMatrix, TransMat);
			MixMatrix = MMult(MixMatrix, characterFrame10Mat);

			/*MATRIX mTranslate = MGetTranslate(VGet(0, 0, -2));
			MixMatrix = MMult(MixMatrix, mTranslate);*/


			// 合成した行列をアタッチするモデルにセット
			MV1SetMatrix(_metalModel, MixMatrix);
		}
		else {
			
			int frame = 20;
			if (_fallCnt <= frame) {
				float x = EasingLinear(_fallCnt, _fallStartPos.x, _fallEndPos.x, frame);
				float y = EasingLinear(_fallCnt, _fallStartPos.y, _fallEndPos.y, frame);

				VECTOR mPos = VGet(x, y, 0);
				mPos = VTransform(mPos, mView);
				mPos = ConvScreenPosToWorldPos(mPos);
				mPos.z = -200;
				MV1SetPosition(_metalModel, mPos);
				_fallCnt++;
			}
			else if(_fallCnt <= 60){
				VECTOR mPos = _fallEndPos;
				mPos = VTransform(mPos, mView);
				mPos = ConvScreenPosToWorldPos(mPos);
				mPos.z = -200;
				MV1SetPosition(_metalModel, mPos);
				_fallCnt++;
			}
		}
	}
}

//void EnemyGround::DrawDebugInfo(MATRIX mView)
//{
//	if (_use == 1) {
//		VECTOR rectPos;
//
//		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 140.f);
//
//		
//
//
//		// 索敵範囲
//		rectPos = VGet(_searchRect.x, _searchRect.y, 0);
//		rectPos = VTransform(rectPos, mView);
//		DrawBox(rectPos.x, rectPos.y, rectPos.x + _searchRect.w, rectPos.y + _searchRect.h, COLOR_GREEN, FALSE);
//
//		// 索敵範囲
//		rectPos = VGet(_attackRect.x, _attackRect.y, 0);
//		rectPos = VTransform(rectPos, mView);
//		DrawBox(rectPos.x, rectPos.y, rectPos.x + _attackRect.w, rectPos.y + _attackRect.h, COLOR_RED, FALSE);
//
//
//
//		// 敵の当たり判定
//		rectPos = VGet(_bodyRect.x, _bodyRect.y, 0);
//		rectPos = VTransform(rectPos, mView);
//		DrawBox(rectPos.x, rectPos.y, rectPos.x + _bodyRect.w, rectPos.y + _bodyRect.h, COLOR_BLUE, FALSE);
//
//		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
//
//
//
//		DrawFormatString(rectPos.x, rectPos.y, COLOR_WHITE, "dir: x %3.2f, y %3.2f", _direction.x, _direction.y);
//
//		//DrawFormatStringToHandle(rectPos.x, rectPos.y, COLOR_WHITE, fontHandle, "%d", 2 - _type);
//
//		//int x = SCREEN_W / 2;
//		//int y = 0;
//		//int w = 300;
//		//int h = 300;
//		//DrawBox(x - 16, y, x + w, y +h, COLOR_WHITE, TRUE);
//		//DrawFormatString(x, 0 + y * 16, COLOR_RED, "_pos : x %3.2f ,  y %3.2f", _pos.x, _pos.y); y++;
//		//DrawFormatString(x, 0 + y * 16, COLOR_RED, "_startPos : x %3.2f ,  y %3.2f", _startPos.x, _startPos.y); y++;
//
//		//DrawFormatString(x, 0 + y * 16, COLOR_RED, "RemainTime %d, CoolTime %d", _attackRemainTime, _attackCoolTime); y++;
//	}
//}