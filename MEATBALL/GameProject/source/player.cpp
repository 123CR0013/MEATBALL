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

// ���[�V�����p�萔
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


// �ړ����x
#define PLAYER_SPEED 8

// �_�b�V���̎�������
#define DASH_COUNT_MAX 10





// ��_���[�W��̖��G����
#define PLAYER_INVINCIBLE_TIME_MAX 120
// ��_���[�W��̈ړ��s�E�n�[�g�擾�s�̎���
#define PLAYER_KNOCKBACK_TIME_MAX 60



// �e�N�X�`�����
std::vector<TEXTURE>	Player::_texture;
// �}�e���A�����
std::vector<MATERIAL>	Player::_material;

Player::Player(int modelHandle, VECTOR startPos) : CharaBase(modelHandle, startPos) {
	// �傫���̒���
	MV1SetScale(_modelHandle, VGet(1.8f * 1.1f, 1.8f * 1.1f, 1.8f * 1.1f));


	// ������Ԃ̃��[�V�����̐ݒ�
	_motionNum = PLAYER_MOTION_WAIT_RIGHT;

	// �����̃A�j���[�V�������A�^�b�`���Ă����Ԃ���3D���f���̏�Ԃ��u�����h���ꂽ���̂ɂȂ�
	// �u�����h���������Ȃ��ꍇ��
	// �@1�̃��[�V�����������A�^�b�`����i���̃��[�V�������O���ɂ̓f�^�b�`������j�@��DxLib�ł͂������z�肵�Ă���
	// �A�Đ��������Ȃ����[�V�����̃u�����h���[�g��0�ɂ��� (�g�p�������⏈�����ׂ������㏸����) 
	// MV1SetAttachAnimBlendRate

	// �����ł͇@�̕��@���Ƃ�
	// �ȉ��̃��[�v�� totalTime ����邽�߂ɍs��
	for (int i = 0; i < PLAYER_MOTION_MAX; i++) {
		//�A�j���[�V�����̃A�^�b�`
		_anim[i].index = MV1AttachAnim(_modelHandle, i, -1, FALSE);
		// �A�^�b�`�����A�j���[�V�����̑��Đ����Ԃ��擾����
		_anim[i].totalTime = MV1GetAttachAnimTotalTime(_modelHandle, _anim[i].index);
		if (i != _motionNum) {
			// �A�j���[�V�����̃f�^�b�`
			MV1DetachAnim(_modelHandle, _anim[i].index);
		}
	}

	_difBodyRect = { -32, -128, 64, 120 };
	_useGravity = 1;

	_difCenterY = 60;

	// �̂̒��S�ʒu���X�V
	UpdateCenterPos();
	// �����蔻���`���X�V
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


// �c���̏���������
void Player::AfterImageInit(int modelHandle)
{
	VECTOR modelScale = MV1GetScale(_modelHandle);
	// �c�����o��������J�E���g���i_remainDashTime�j
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

		//�A�j���[�V�����̃A�^�b�`
		_afterImage[i].anim.index = MV1AttachAnim(_afterImage[i].modelHandle, PLAYER_MOTION_DASH, -1, FALSE);
		// �A�^�b�`�����A�j���[�V�����̑��Đ����Ԃ��擾����
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




// �Đ�����SE���~����
// �T�C�R�L�l�V�XSE�ƌĂі߂�SE���~����
void Player::StopSE()
{
	StopSoundMem(se[11]);
	//StopSoundMem(se[20]);
}

// ���G�t�F�N�g�̃Z�b�g
void Player::SetSmokeEffect()
{
	VECTOR effectPos = _pos;
	effectPos.y -= 20;
	float effectSize = 160;
	SetEffect(EFFECT_TYPE_SMOKE, effectPos.x - effectSize / 2, effectPos.y - effectSize / 2, effectSize, effectSize);
}

// �v�Z����
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

	// �O�̃t���[���̃��[�V�����ԍ���ێ�����
	int motionNum_old = _motionNum;


	// ��x�~�[�g�{�[����_isDrawFullArrow�i���������ő�̒����ŕ`�悷�邩�ǂ����j��0�ɂ���
	meatBall->SetIsDrawFullArrow(0);

	// �O�̃t���[���̏d�͒l��ێ�����
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



	// ���ʏ���
	// �d�͏���
	CharaBase::Process();


	// ���n����
	// �O�̃t���[���̏d�͒l��3�ȏ� ���� ���̃t���[���ŏ��ɐG��Ă�����
	if (gravity_old >= 3 && _stand == 1) {
		PlaySoundMem(se[12], DX_PLAYTYPE_BACK);

		// ���G�t�F�N�g�̃Z�b�g
		SetSmokeEffect();
	}

	// �{�X�o�ꉉ�o���͑�����ł��Ȃ��悤�ɂ���
	if (gCam._phase == 99) {
		StopDash();
		if (_stand == 1) {
			_motionNum = PLAYER_MOTION_WAIT_RIGHT;
		}
	}

	// �Q�[���I�[�o�[���[�V�����Đ����o�Ȃ� ���� �{�X�o�ꉉ�o���łȂ��Ȃ�
	if (_isPlayingGameOverMotion != 1 && gCam._phase != 99) {

		// �W�����v��Ƃ̓����蔻��
		for (int i = 0; i < JUMPPAD_MAX; i++) {
			if (jumpPad[i]->GetUse() == 0) continue;
			// �W�����v��{�̂Ƃ̓����蔻��
			MYRECT jRect = jumpPad[i]->GetRect();
			if (IsHitBox(_bodyRect.x, _bodyRect.y, _bodyRect.w, _bodyRect.h, jRect.x, jRect.y, jRect.w, jRect.h) == 1) {

				// �����o������
				VECTOR pPos = _pos;
				VECTOR jPos = jumpPad[i]->GetPos();

				int hitJumpPad = CheckExtrusionDirection(_bodyRect, jRect);
				switch (hitJumpPad)
				{
				case 0: // ������
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

				case 1: // �c����
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


				// �̂̒��S�ʒu���X�V
				UpdateCenterPos();
				// �����蔻���`���X�V
				SetCollision();

			}

			// �W�����v�͈͂Ƃ̓����蔻��
			// ���͈̔͂ƃv���C���[���������Ă����ꍇ�A��ɃW�����v������
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

			// ���ˍ\��
			// �z�[���h��
			if (gPad->_input.Buttons[PAD_BUTTON_X] != 0 && (meatBall->GetIsAttack() == 0 || meatBall->GetCanReattack() == 1) && _remainDashTime <= 0) {
				_useGravity = 1;

				_returnCnt = 0;

				_motionNum = PLAYER_MOTION_SHOOT_WAIT_RIGHT;
				_isDrawAura = 1;

				// �������̕`��Ɋւ���ݒ�
				meatBall->SetIsDrawFullArrow(1);


				// �T�C�R�L�l�V�XSE�̍Đ�
				if (CheckSoundMem(se[11]) == 0) {
					PlaySoundMem(se[11], DX_PLAYTYPE_BACK);
				}


			}
			// �~�[�g�{�[�����Ăі߂�
			// �z�[���h��
			else if (gPad->_input.Buttons[PAD_BUTTON_B] && meatBall->GetIsAttack() == 1 && _remainDashTime <= 0) {
				_useGravity = 1;

				_returnCnt++;

				_motionNum = PLAYER_MOTION_ABILITY_RIGHT;
				_isDrawAura = 2;

				// �Ăі߂��J�E���g��MAX�𒴂�����A�Ăі߂��������s��
				if (_returnCnt >= MEATBALL_RETURN_CNT_MAX) {
					meatBall->SetAttack(VGet(0, 0, 0), 0);
					meatBall->SetReturnEffect();


					
					PlaySoundMem(voice[rand() % 4], DX_PLAYTYPE_BACK);

				}
			}else{
				StopSE();

				_returnCnt = 0;


				// �~�[�g�{�[���̔��ˏ���
				if (gPad->_rel[PAD_BUTTON_X] != 0) {
					if (meatBall->GetIsAttack() == 0) {
						StopDash();

						VECTOR meatBallStartPos = _centerPos;
						meatBallStartPos.y -= 64;
						meatBall->SetAttack(meatBallStartPos);

						_motionNum = PLAYER_MOTION_SHOOT_RIGHT;
						PlaySoundMem(voice[(rand() % 4) + 5], DX_PLAYTYPE_BACK);

					}
					// �~�[�g�{�[���̕����]��
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

					// �_�b�V���ݒ�
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

					// �_�b�V���ړ�����
					if (_remainDashTime > 0) {

						
						// �c���̃Z�b�g
						SetAfterImage();

						_remainDashTime--;

						_speed = PLAYER_SPEED * 3;

						// �ړ�����
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


						case LOWER_RIGHT: // �E��
							isDiagonalHit = 1;
							startPos = VGet(hitChip.x * CHIPSIZE_W, hitChip.y * CHIPSIZE_H + CHIPSIZE_H, 0);
							endPos = VGet(hitChip.x * CHIPSIZE_W + CHIPSIZE_W, hitChip.y * CHIPSIZE_H, 0);

							// 
							//if (_direction.x < 0) {
							_pos.y += CHIPSIZE_H / 2;
							//}
							break;
						case LOWER_LEFT: // ����
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
								//�̂̒��S�ʒu���X�V
								UpdateCenterPos();
								// �����蔻���`���X�V
								SetCollision();

								// 
								// �Ζʂ�����Ƃ݂Ȃ��āA�ΖʂƑ̂̋�`�̓����蔻����Ƃ�
								isHit = IsHitRectAndLine(_bodyRect, startPos, endPos);
							}
						}
						else {
							// �̂̒��S�ʒu���X�V
							UpdateCenterPos();

							// �v���C���[�����蔻���`�̈ʒu�𒲐�����
							SetCollision();
						}



						_motionNum = PLAYER_MOTION_DASH;


					}
					else {
						_useGravity = 1;


						if (gPad->_input.X != 0) {
							//_direction = VGet(gPad->_input.X, gPad->_input.Y, 0); // ���X�e�b�N���͂��������ꍇ�̂�direction���X�V����
							_direction = VGet(gPad->_input.X, 0, 0); // ���X�e�b�N���͂��������ꍇ�̂�direction���X�V����

							// LT�{�^����������Ă���Ƃ��͓����Ȃ��悤�ɂ���
							//if (gPad->_input.Buttons[PAD_BUTTON_LT] == 0) {




							if (_remainDashTime <= 0) {
								{
									_speed = PLAYER_SPEED;


									// �ړ�����
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

								// �̂̒��S�ʒu���X�V
								UpdateCenterPos();
								// �����蔻���`���X�V
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



					// �W�����v����
					if (_remainDashTime <= 0) {
						if (_stand == 1) {
							_canDash = 1;
							if (gPad->_trg[PAD_BUTTON_A] != 0) {
								_gravity = -25;
								_motionNum = PLAYER_MOTION_JUMP_UP;
								PlaySoundMem(se[10], DX_PLAYTYPE_BACK);

								// ���G�t�F�N�g�̃Z�b�g
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

			// �n�[�g����鏈��
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




	// ���f���̉�]����
	UpdateModelRotation();




	_heart.Process();


	// ��_���[�W��
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

// �_�b�V����r���ŃL�����Z������
void Player::StopDash()
{
	_remainDashTime = 0;
	_useGravity = 1;
	_isDrawAura = 0;

}

// ��_���[�W����
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




// �L�����N�^�[�̌����Ă�������ɂ���ă��[�V������ύX����
// 
// Process�������Ń��[�V�������w�肷��Ƃ��͑S�ĉE�����̃��[�V�������w�肷��
// �L�����N�^�[�����������Ă���ꍇ�́A�������̃��[�V�����ɕύX����
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

	// ����̃��[�V�����̍Đ����Ō�܂ŏI�������ʂ̃��[�V�����ɐ؂�ւ���
	if (motionNum_old == _motionNum) {
		switch (_motionNum)
		{
		// ���˃��[�V�������I�������ҋ@���[�V�����ɐ؂�ւ���
		case PLAYER_MOTION_SHOOT_RIGHT:
		case PLAYER_MOTION_SHOOT_LEFT:
			if (_playTime >= _anim[_motionNum].totalTime - 20) {
				_playTime = 0;
				_isDrawAura = 0;
				_motionNum = PLAYER_MOTION_WAIT_RIGHT;
				SetMotionReverse();
			}
			break;

		// �Ăі߂����[�V�������I�������ҋ@���[�V�����ɐ؂�ւ���
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


		// �_���[�W���[�V�������I�������ҋ@���[�V�����ɐ؂�ւ���
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

		
		

		// �Q�[���I�[�o�[���[�V�������I�������
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

		// ���[�V�������ύX���ꂽ�ꍇ�ɁA�ύX�O�̃��[�V�������f�^�b�`���ĕύX��̃��[�V�������A�^�b�`����
		if (motionNum_old != _motionNum) {
			// �ύX�O�̃��[�V�����̃f�^�b�`
			MV1DetachAnim(_modelHandle, _anim[motionNum_old].index);
			// �ύX��̃��[�V�����̃A�^�b�`
			_anim[_motionNum].index = MV1AttachAnim(_modelHandle, _motionNum, -1, FALSE);

			_playTime = 0;
		}


		// �Đ����Ԃ��Z�b�g����(�A�j���[�V�����̍Đ�)
		MV1SetAttachAnimTime(_modelHandle, _anim[_motionNum].index, _playTime);

	}
}


void Player::AfterImageProcess()
{
	for (int i = 0; i < PLAYER_AFTERIMAGE_MAX; i++) {
		if (_afterImage[i].use == 1) {

			// ���t���[�������x��������
			_afterImage[i].alpha -= 0.2f;
			for (int j = 0; j < 6; j++) {
				MV1SetMaterialDifColor(_afterImage[i].modelHandle, j, GetColorF(1, 1, 1, _afterImage[i].alpha));
			}

			// �c��J�E���g�����炷
			_afterImage[i].remainCnt--;
			// �c���̎c��J�E���g��0�����������c�����폜����
			if (_afterImage[i].remainCnt < 0) {
				_afterImage[i].use = 0;
			}
		}
	}

}

void Player::SetAfterImage()
{
	for (int i = 0; i < PLAYER_AFTERIMAGE_MAX; i++) {
		// �c���̏o���t���[���Ǝc��_�b�V���J�E���g����v���Ă�����c�����o��������
		if (_afterImage[i].frame == _remainDashTime) {
			_afterImage[i].use = 1;
			_afterImage[i].remainCnt = 6;

			_afterImage[i].pos = _pos;
			VECTOR modelRot = MV1GetRotationXYZ(_modelHandle);
			MV1SetRotationXYZ(_afterImage[i].modelHandle, modelRot);
			// �Đ����Ԃ��Z�b�g����(�A�j���[�V�����̍Đ�)
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


// �`��ʒu�̕␳����
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
	// �X�N���[�����W�����[���h���W�ɕϊ�
	_worldPos = ConvScreenPosToWorldPos(_screenPos);
	_worldPos.z = 0; // z���\���ʒu�̒���
}


void Player::Draw()
{
	// �A�E�g���C���̐ݒ�
	SetOutLine();


	switch(_isDrawAura)
	{
		// �ʏ펞
	default:
		// �c���̕`��
		for(int i = 0; i < PLAYER_AFTERIMAGE_MAX; i++) {
			if(_afterImage[i].use == 1) {
				MV1DrawModel(_afterImage[i].modelHandle);
			}
		}
		CharaBase::Draw();

		_auraCnt = 0;
		break;

		// �I�[�����o�Ă�����
		// �~�[�g�{�[�����˃z�[���h��
	case 1:
	{
		VECTOR auraPos = _centerPos;
		auraPos.y -= 20;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 140.f);
		// �I�[���i���j�̕`��
		MyDrawModiGraph(gCam.GetViewMatrix(), auraPos, 1.0f, 0.0f, _bodyRect.w * 6, _bodyRect.h * 3.5, cgAttackAura[(_auraCnt / 4) % 20]);



		CharaBase::Draw();

		// �I�[���i�O�j�̕`��
		MyDrawModiGraph(gCam.GetViewMatrix(), auraPos, 1.0f, 0.0f, _bodyRect.w * 6, _bodyRect.h * 3.5, cgAttackAura[(_auraCnt / 2) % 20]);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		_auraCnt++;
	}
	break;



	// �I�[�����o�Ă�����
	// �~�[�g�{�[���Ăі߂���
	case 2:
	{
		//if (_auraCnt < 4 * 13) {
		VECTOR auraPos = _pos;
		auraPos.y -= _bodyRect.h;
		auraPos.y -= 25;

		float effectSize = _bodyRect.w * 2.5;


		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255.0f * 0.6f);
		// �I�[���i���j�̕`��
		MyDrawModiGraph(gCam.GetViewMatrix(), auraPos, 1.0f, 0.0f, effectSize, effectSize, cgReturnAura[(_auraCnt / 5) % 13]);
		MyDrawModiGraph(gCam.GetViewMatrix(), auraPos, 1.0f, 0.0f, effectSize, effectSize, cgReturnAura[(_auraCnt / 3) % 13]);



		CharaBase::Draw();

		// �I�[���i�O�j�̕`��
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







// �f�o�b�O���̕\��
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

// �\���́uTEXTURE�v�̏�����
void Player::TextureInit(int modelHandle)
{
	// ������
	_texture.clear();

	// �e�N�X�`���̍��v�����擾����
	int n = MV1GetTextureNum(modelHandle);

	for (int i = 0; i < n; i++) {
		// �e�N�X�`���摜�̃n���h�����\����TEXTURE�ɕێ�����
		TEXTURE t = { i, MV1GetTextureGraphHandle(modelHandle, i) };
		_texture.push_back(t);
	}
}

// �e�N�X�`���摜�̓\��ւ�
// �_���[�W�󂯂��Ƃ�, ���t���[�������^�����ȉ摜��\��
// ���̌�, ���ɖ߂�
// ����isDamaged �ɂ���ē\��e�N�X�`����ύX����
// 0 : �ʏ펞, 1 : ��_���[�W��
void Player::ChangeTexture(int isDamaged)
{
	for (auto tex : _texture) {
		int cgTex;

		// �ʏ펞
		if (isDamaged == 0) {
			cgTex = tex.graphHandle;
		}
		// ��_���[�W��
		else {
			cgTex = _cgTexWhite;
		}

		SetTexture(tex.index, cgTex);
	}
}

// �\���́uMATERIAL�v�̏�����
void Player::MaterialInit(int modelHandle)
{
	// ������
	_material.clear();

	// �}�e���A���̍��v�����擾����
	int n = MV1GetMaterialNum(modelHandle);

	for (int i = 0; i < n; i++) {
		// �}�e���A���̃f�B�t���[�Y�J���[���\����MATERIAL�ɕێ�����
		MATERIAL m = { i, MV1GetMaterialDifColor(modelHandle, i) };
		_material.push_back(m);
	}
}

// �}�e���A���̃f�B�t���[�Y�J���[�̕ύX
// ����isDamaged�ɂ���ăf�B�t���[�Y�J���[��ύX����
void Player::ChangeMaterialDifColor(int isDamaged)
{
	for (auto m : _material) {
		COLOR_F difColor;

		// �ʏ펞
		if (isDamaged == 0) {
			difColor = m.difColor;
		}
		// ��_���[�W��
		else {
			difColor = GetColorF(1.0f, 1.0f, 1.0f, 1.0f);

			// �����x�͈ێ�����
			difColor.a = m.difColor.a;
		}

		SetMaterialDifColor(m.index, difColor);
	}
}
