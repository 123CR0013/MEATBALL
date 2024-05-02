#include "boss.h"
#include "mapdata.h"
#include "winmain.h"
#include "mydraw.h"
#include "effect.h"
#include "resource.h"


// ���[�V�����p�萔
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


// �U�����I�����Ă��玟�̍U���Ɉڂ�܂ł̃t���[����
#define ATTACK_COOLTIME_MAX 90

// ���G����
#define BOSS_INVINCIBLE_CNT_MAX 15


// �e�N�X�`�����
std::vector<TEXTURE>	Boss:: _texture;
// �}�e���A�����
std::vector<MATERIAL>	Boss:: _material;

// �{�X��pBGM
int	 Boss::_bgmBossBattle;

Boss::Boss(int modelHandle, VECTOR startPos) : CharaBase(modelHandle, startPos)
{
	// �傫���̒���
	MV1SetScale(_modelHandle, VGet(1.8f * 1.1f, 1.8f * 1.1f, 1.8f * 1.1f));
	_direction = VGet(-1, 0, 0);

	// ������Ԃ̃��[�V�����̐ݒ�
	_motionNum = BOSS_MOTION_WAIT_RIGHT;

	// �����̃A�j���[�V�������A�^�b�`���Ă����Ԃ���3D���f���̏�Ԃ��u�����h���ꂽ���̂ɂȂ�
	// �u�����h���������Ȃ��ꍇ��
	// �@1�̃��[�V�����������A�^�b�`����i���̃��[�V�������O���ɂ̓f�^�b�`������j�@��DxLib�ł͂������z�肵�Ă���
	// �A�Đ��������Ȃ����[�V�����̃u�����h���[�g��0�ɂ��� (�g�p�������⏈�����ׂ������㏸����) 
	// MV1SetAttachAnimBlendRate
	
	// �����ł͇@�̕��@���Ƃ�
	// �ȉ��̃��[�v�� totalTime ����邽�߂ɍs��
	for (int i = 0; i < BOSS_MOTION_MAX; i++) {
		//�A�j���[�V�����̃A�^�b�`
		_anim[i].index = MV1AttachAnim(_modelHandle, i, -1, FALSE);
		// �A�^�b�`�����A�j���[�V�����̑��Đ����Ԃ��擾����
		_anim[i].totalTime = MV1GetAttachAnimTotalTime(_modelHandle, _anim[i].index);
		if (i != _motionNum) {
			// �A�j���[�V�����̃f�^�b�`
			MV1DetachAnim(_modelHandle, _anim[i].index);
		}
	}

	// �A�E�g���C���̏����ݒ�
	for (int i = 0; i < 6; i++) {
		MV1SetMaterialOutLineWidth(_modelHandle, i, 1);
		MV1SetMaterialOutLineColor(_modelHandle, i, GetColorF(0.8, 0.5, 1, 1));

	}

	_difBodyRect = { -32, -128, 64, 120 };
	_useGravity = 1;

	_difCenterY = 60;

	// �̂̒��S�ʒu���X�V
	UpdateCenterPos();
	// �����蔻���`���X�V
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
	// �U���p�^�[���������_���Ɍ��߂�
	//SetAttackInfo();



	_useGravity = 1;


	_isEliminated = 0;
	_isHit = 0;


	_auraAnimCnt = 0;
};


// �c���̏���������
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

		//�A�j���[�V�����̃A�^�b�`
		// �v���C���[�̃��f�������Ƃɂ��Ă���̂ŁA�A�j���[�V�����̓{�X�̃A�j���[�V�����p�萔���g�킸�ɒ��ړ��͂��Ă���
		// �A�j���[�V�����ԍ�13�� �uPLAYER_MOTION_DASH�v
		_afterImage[i].anim.index = MV1AttachAnim(_afterImage[i].modelHandle, 13, -1, FALSE);
		// �A�^�b�`�����A�j���[�V�����̑��Đ����Ԃ��擾����
		_afterImage[i].anim.totalTime = MV1GetAttachAnimTotalTime(_afterImage[i].modelHandle, _afterImage[i].anim.index);


		// �A�E�g���C���̏����ݒ�
		for (int j = 0; j < 6; j++) {
			MV1SetMaterialOutLineWidth(_afterImage[i].modelHandle, j, 1);
			MV1SetMaterialOutLineColor(_afterImage[i].modelHandle, j, GetColorF(0.8, 0.7, 1, 0.1f));

		}


		_afterImage[i].alpha = 1;
	}



}

// �c���̌v�Z����
void Boss::AfterImageProcess()
{
	for (int i = 0; i < BOSS_AFTERIMAGE_MAX; i++) {
		if (_afterImage[i].use == 1) {

			// ���t���[�������x��������
			_afterImage[i].alpha -= 0.1f;
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

// �c�����Z�b�g����
void Boss::SetAfterImage()
{
	for (int i = 0; i < BOSS_AFTERIMAGE_MAX; i++) {
		if (_afterImage[i].use == 1) continue;
		_afterImage[i].use = 1;
		_afterImage[i].remainCnt = 10;

		_afterImage[i].pos = _pos;
		VECTOR modelRot = MV1GetRotationXYZ(_modelHandle);
		MV1SetRotationXYZ(_afterImage[i].modelHandle, modelRot);
		// �Đ����Ԃ��Z�b�g����(�A�j���[�V�����̍Đ�)
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

// �v�Z����
void Boss::Process(VECTOR playerPos)
{
	if (_use == 1) {
		int motionNum_old = _motionNum;
		int gravity_old = _gravity;




		// �d�͏���
		CharaBase::Process();


		// �{�X�풆�Ȃ�
		if (_isBossBattle == 1) {

			if (_isEliminated != 1) {
				// ���j���ꂽ�Ƃ��̐ݒ�
				if (_hp <= 0) {
					_motionNum = BOSS_MOTION_ELIMINATED_RIGHT;
					_isEliminated = 1;
					_useGravity = 1;

					// �o�����̒e��S�ď���
					DeleteAllBullet();

					// �o�����̃o���A�`�b�v������
					gMap.VanishBossBarrier();


					// �e�N�X�`�������ɖ߂�
					ChangeTexture(0);
					// �}�e���A���̃f�B�t���[�Y�J���[�����ɖ߂�
					ChangeMaterialDifColor(0);

					// �A�E�g���C��������
					for (int i = 0; i < 6; i++) {
						MV1SetMaterialOutLineWidth(_modelHandle, i, 0);
					}

					// �{�X��pBGM���~����
					PlayBossBGM(0);
				}
			}
			else {
				_isHit = 0;
			}


			if (_attackCoolTime == 0 || _attackCoolTime > 45) {
				// ���n����
				if (gravity_old >= 3 && _stand == 1) {
					// ���G�t�F�N�g�̃Z�b�g
					VECTOR effectPos = _pos;
					effectPos.y -= 20;
					float effectSize = 160;
					int effectX = effectPos.x - effectSize / 2;
					int effectY = effectPos.y - effectSize / 2;
					SetEffect(EFFECT_TYPE_SMOKE, effectX, effectY, effectSize, effectSize);

					// ���nSE�̍Đ�
					PlaySoundMem(se[12], DX_PLAYTYPE_BACK);
				}
			}

			// ���j����Ă��Ȃ�������
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

				// �e�̌v�Z����
				BulletProcess();

				// �U�����̏���
				if (_isAttack == 1) {
					_attackCnt++;

					// �U�����I�������鏈��
					if (_attackCnt == _attackInfo.frame) {
						_changeAttack = 1;
						_isAttack = 0;

						// �o�����̒e��S�ď���
						DeleteAllBullet();
						// �o�����̃o���A�`�b�v������
						gMap.VanishBossBarrier();

						// �o���A�`�b�v�o��SE�̍Đ�
						PlaySoundMem(se[19], DX_PLAYTYPE_BACK);

						_gravity = 0;
					}

				}
				// ��U�����̏���
				else {

					// �U����A���̍U���Ɉڂ�܂ł̏���
					if (_attackCoolTime > 0) {
						_attackCoolTime--;

						// ���̍U���܂ł̃t���[������45�t���[���ɂȂ�����A���̍U���̏����Z�b�g���ړ����J�n����
						if (_attackCoolTime == 45) {
							// ���̍U���̏����Z�b�g����
							SetAttackInfo(playerPos);

							// ���̍U���n�_�܂ł̈ړ��ɕK�v�ȏ���ێ�����
							_startPos = _pos;
							float endX = _attackInfo.pos.x * CHIPSIZE_W + _bodyRect.w / 2;
							if (_startPos.x <= endX) {
								_direction = VGet(1, 0, 0);
							}
							else {
								_direction = VGet(-1, 0, 0);
							}
						}

						// ���̍U���n�_�܂�45�t���[�������Ĉړ�����
						if (_attackCoolTime <= 45) {
							_useGravity = 0;
							_motionNum = BOSS_MOTION_DASH;

							// �c�����Z�b�g����
							SetAfterImage();

							// ���̒n�_�܂ł̈ړ�����
							int cnt = 45 - _attackCoolTime;
							VECTOR endPos = {
								_attackInfo.pos.x * CHIPSIZE_W + _bodyRect.w / 2,
								_attackInfo.pos.y * CHIPSIZE_H + _bodyRect.h,
								0
							};
							_pos.x = EasingInOutSine(cnt, _startPos.x, endPos.x, 45);
							_pos.y = EasingInOutSine(cnt, _startPos.y, endPos.y, 45);


							// �̂̒��S�ʒu���X�V
							UpdateCenterPos();
							// �����蔻���`���X�V
							SetCollision();
						}


						// �U���n�_�ɓ���������A�p�^�[�����Ƃɐݒ������
						if (_attackCoolTime == 0) {
							_isAttack = 1;
							_useGravity = 0;
							_stand = 0;

							// �e�̏���������
							BulletInit();

							switch (_attackInfo.id)
							{
							case 0:
								_useGravity = 1;
								break;
								// �����o���A
							case 1:
							case 2:
								_useGravity = 1;
								break;
								// ����
							case 3:
								_direction = VGet(1, 0, 0);
								break;
								// �E��
							case 4:
								_direction = VGet(-1, 0, 0);
								break;
							}
						}




					}
				}

				// �U�����I��������A���̍U���܂ł̏���������
				if (_changeAttack == 1) {
					_changeAttack = 0;
					_attackCnt = 0;
					_attackCoolTime = ATTACK_COOLTIME_MAX;
					_useGravity = 1;
				}

			}

		}


		// ���G���̏���
		if (_invincibleCnt > 0) {
			_invincibleCnt--;
			// ���G���Ԃ��I�������
			if (_invincibleCnt == 0) {
				// �e�N�X�`�������ɖ߂�
				ChangeTexture(0);
				// �}�e���A���̃f�B�t���[�Y�J���[�����ɖ߂�
				ChangeMaterialDifColor(0);
			}
		}


		// 3D���f���̉�]����
		UpdateModelRotation();

		// �L�����N�^�[�̌����Ă�������ɂ���ă��[�V������ύX����
		SetMotionReverse();
		// ���[�V�����̌v�Z����
		MotionProcess(motionNum_old);

		// �c���̌v�Z����
		AfterImageProcess();
	}
}

// �e�̌v�Z����
void Boss::BulletProcess()
{
	// �e�̔��ˏ���
	for (int i = 0; i < BOSS_BULLET_MAX; i++) {
		if (_attackInfo.bullet[i].use == -1) break; // use == -1�͖��g�p�z��

		// ���˒e
		// shotFrame == _attackCnt �̎��ɒe���o��������
		if (_attackInfo.bulletType == 0) {
			if (_isAttack == 1) {
				if (_attackInfo.bullet[i].shotFrame == _attackCnt) {
					_attackInfo.bullet[i].use = 1;
				}
			}
		}

		// ���̔z��v�f���d�l���Ȃ�
		if (_attackInfo.bullet[i].use == 1) {
			if (_attackInfo.bullet[i].shotFrame <= _attackCnt) {
				VECTOR vDir = VNorm(_attackInfo.bullet[i].direction);
				VECTOR vMove = VScale(vDir, _attackInfo.bullet[i].speed);
				_attackInfo.bullet[i].pos.x += vMove.x;
				_attackInfo.bullet[i].pos.y += vMove.y;

				// �e���}�b�v�`�b�v�ɓ����������𒲂ׂ�
				MYRECT bulletRect = { _attackInfo.bullet[i].pos.x, _attackInfo.bullet[i].pos.y, 60, 60 };
				HITCHIP hitChip = gMap.IsHitBoxAndMapChip(bulletRect);

				// �������Ă����ꍇ
				if (hitChip.chip_no != -1) {

					// ���˒e�̏ꍇ�A���ˌ�̈ړ��������v�Z����
					if (_attackInfo.bulletType == 0) {

						// �e�̓����蔻��̏㉺���E��4�_���}�b�v�`�b�v�ɓ����������𒲂ׂ�
						// �㉺���E��4�_���}�b�v�`�b�v�ɓ������Ă��Ȃ������ꍇ�A�e�̓����蔻���`�̊p���}�b�v�`�b�v�ɓ��������Ƃ݂Ȃ�

						VECTOR bulletPos[4] = {
						{_attackInfo.bullet[i].pos.x + 30, _attackInfo.bullet[i].pos.y		  , 0},
						{_attackInfo.bullet[i].pos.x + 30, _attackInfo.bullet[i].pos.y + 60, 0},
						{_attackInfo.bullet[i].pos.x		   , _attackInfo.bullet[i].pos.y + 30, 0},
						{_attackInfo.bullet[i].pos.x + 60, _attackInfo.bullet[i].pos.y + 30, 0},
						};

						// �����߂��Ă��甽�˕����̐ݒ������
						// �����߂����ۂɓ��������`�b�v���甲���o���Ȃ��Ȃ�̂�h�����߂� * 2 ������
						_attackInfo.bullet[i].pos.x -= vMove.x;
						_attackInfo.bullet[i].pos.y -= vMove.y;

						int isHitCorner = 1;

						// �e�̏㉺�̓_���}�b�v�`�b�v�ɓ����������𒲂ׂ�
						for (int j = 0; j < 2; j++) {
							int isHit = gMap.IsHitMapchip(bulletPos[j].x, bulletPos[j].y);
							if (isHit != -1) {
								_attackInfo.bullet[i].direction.y = -_attackInfo.bullet[i].direction.y;
								isHitCorner = 0;
							}
						}

						// �e�̍��E�̓_���}�b�v�`�b�v�ɓ����������𒲂ׂ�
						for (int j = 2; j < 4; j++) {
							int isHit = gMap.IsHitMapchip(bulletPos[j].x, bulletPos[j].y);
							if (isHit != -1) {
								_attackInfo.bullet[i].direction.x = -_attackInfo.bullet[i].direction.x;
								isHitCorner = 0;
							}
						}

						// �e�̓����蔻���`�̊p���}�b�v�`�b�v�ɓ��������ꍇ
						if (isHitCorner == 1) {
							_attackInfo.bullet[i].direction.x = -_attackInfo.bullet[i].direction.x;
							_attackInfo.bullet[i].direction.y = -_attackInfo.bullet[i].direction.y;
						}

					}
					// �ݒu�e�̏ꍇ�A���̒e������
					else {
						_attackInfo.bullet[i].use = 0;
					
						// �e�j��G�t�F�N�g�̃Z�b�g
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

// �e�̏���������
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






// �U�������Q�Ƃ��ăo���A�`�b�v���o��������
void Boss::SetBarrier()
{
	// �o���A�`�b�v�o��SE�̍Đ�
	PlaySoundMem(se[19], DX_PLAYTYPE_BACK);

	// �o���A�`�b�v���o��������
	for (int i = 0; i < BOSS_BLOCK_MAX; i++) {
		if (_attackInfo.blockPos[i].x == -1) break;
		gMap.SetBossBarrier(_attackInfo.blockPos[i]);
	}
}


// �L�����N�^�[�̌����Ă�������ɂ���ă��[�V������ύX����
// 
// Process�������Ń��[�V�������w�肷��Ƃ��͑S�ĉE�����̃��[�V�������w�肷��
// �L�����N�^�[�����������Ă���ꍇ�́A�������̃��[�V�����ɕύX����
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


// ���[�V�����̌v�Z����
void Boss::MotionProcess(int motionNum_old)
{
	_playTime += 1.5;
	
	

	// ���[�V�������ύX���ꂽ�ꍇ�ɁA�ύX�O�̃��[�V�������f�^�b�`���ĕύX��̃��[�V�������A�^�b�`����
	if (motionNum_old != _motionNum) {
		// �ύX�O�̃��[�V�����̃f�^�b�`
		MV1DetachAnim(_modelHandle, _anim[motionNum_old].index);
		// �ύX��̃��[�V�����̃A�^�b�`
		_anim[_motionNum].index = MV1AttachAnim(_modelHandle, _motionNum, -1, FALSE);

		_playTime = 0;
	}
	else {
		// ���݂̃��[�V�������Ō�܂ōĐ�������
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

				// �ύX�O�̃��[�V�����̃f�^�b�`
				MV1DetachAnim(_modelHandle, _anim[motionNum_old].index);
				// �ύX��̃��[�V�����̃A�^�b�`
				_anim[_motionNum].index = MV1AttachAnim(_modelHandle, _motionNum, -1, FALSE);
				break;
			case BOSS_MOTION_ELIMINATED_LEFT:
			case BOSS_MOTION_ELIMINATED_RIGHT:
				_playTime = _anim[_motionNum].totalTime;
				break;
			}
		}
	}


	// �Đ����Ԃ��Z�b�g����(�A�j���[�V�����̍Đ�)
	MV1SetAttachAnimTime(_modelHandle, _anim[_motionNum].index, _playTime);



}




// �e�̓����蔻���`��Ԃ�
// 
// �����uindex�v�����g�p�v�f�₷�łɏ������e�������ꍇ�A��`��x�ɂ���𔻕ʂ���l�����A�Ԃ�����ł��̒l�ɂ���ď������s��
// ���g�p�v�f�������ꍇ�́A-1
// �������e�������ꍇ�́A    0
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

// �`��ʒu�̕␳����
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

// �`�揈��
void Boss::Draw()
{
	if (_use == 1) {
		if (_isEliminated == 0) {

			// �I�[���`�揈��
			// �I�[���̓L�����N�^�[�̑O�ʂƔw�ʂɏd�˂ĕ`�悷��
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 140.f);
			VECTOR auraPos = _centerPos;
			auraPos.y -= 20;
			MyDrawModiGraph(gCam.GetViewMatrix(), auraPos, 1.0f, 0.0f, _bodyRect.w * 6, _bodyRect.h * 3.5, cgAttackAura[(_auraAnimCnt / 4) % 20]);
			
			// �c���̕`��
			for (int i = 0; i < BOSS_AFTERIMAGE_MAX; i++) {
				if (_afterImage[i].use == 1) {
					MV1DrawModel(_afterImage[i].modelHandle);
				}
			}
			CharaBase::Draw();

			// �I�[���`�揈��
			MyDrawModiGraph(gCam.GetViewMatrix(), auraPos, 1.0f, 0.0f, _bodyRect.w * 6, _bodyRect.h * 3.5, cgAttackAura[(_auraAnimCnt / 2) % 20]);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			_auraAnimCnt++;
		}
		else {
			CharaBase::Draw();
		}
	}
}



// �e�̕`�揈��
void Boss::DrawBullet(MATRIX mView)
{
	for (int i = 0; i < BOSS_BULLET_MAX; i++) {
		if (_attackInfo.bullet[i].use == -1) break; // use == -1�͖��g�p�z��

		if (_attackInfo.bullet[i].use == 1) {
			VECTOR bPos = _attackInfo.bullet[i].pos;
			bPos.x += 64 / 2;
			bPos.y += 64 / 2;
			MyDrawModiGraph(mView, bPos, 1.0f, 0.0f, 64, 64, cgBossBullet[(_attackCnt / 1) % 30]);
		}
	}
}

// �O�ʃ��C���[�ɕ`��
void Boss::DrawFrontLayer(MATRIX mView)
{
	// �e�̕`�揈��
	DrawBullet(mView);
}




// �{�X�̍U�����J�n����
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




// ���̍U�������Z�b�g����
void Boss::SetAttackInfo(VECTOR playerPos)
{
	// �o�����Ă���e��S�ď���
	DeleteAllBullet();
	// �U���p�^�[���������_���Ɍ��߂�
	RandAttackPattern(playerPos);
	// �U�������Q�Ƃ��ăo���A�`�b�v���o��������
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

// �o�����Ă���e��S�ď���
void Boss::DeleteAllBullet()
{
	for (int i = 0; i < BOSS_BULLET_MAX; i++) {
		if (_attackInfo.bullet[i].use == -1) break;
		if (_attackInfo.bullet[i].use == 1) {
			_attackInfo.bullet[i].use = 0;

			

			// �{�X�e�j��G�t�F�N�g�̃Z�b�g
			MYRECT bulletRect = { _attackInfo.bullet[i].pos.x, _attackInfo.bullet[i].pos.y, 60, 60 };
			int effectX = bulletRect.x - bulletRect.w;
			int effectY = bulletRect.y - bulletRect.h;
			int effectSize = bulletRect.w * 3;
			SetEffect(EFFECT_TYPE_BOSS_BULLET_HIT, effectX, effectY, effectSize, effectSize);
		}
	}
}

// �U���p�^�[���������_���Ɍ��߂�
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
			// ���˒e
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
		// �����o���A ��ƍ��ɐݒu�e
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
			// �ݒu�e
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
		// �����o���A ��ƉE�ɐݒu�e
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
		// �ݒu�e
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
		// �����o���A
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
			// �ݒu�e
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
		// �E���o���A
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
			// �ݒu�e
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

		// ���̍U�������̍U���Ɠ����p�^�[���ɂȂ�Ȃ��悤�ɂ���
		if (nextAttackId != _attackInfo.id) {
			float xCenter = 224.5f * CHIPSIZE_W;

			// �����ɕǂ�e���o��U���̏ꍇ
			if (nextAttackId == 1 || nextAttackId == 3) {
				// �v���C���[���{�X�G���A�̒��S��荶���ɂ�����Ē��I
				if (playerPos.x < xCenter) {
					continue;
				}
			}

			// �E���ɕǂ�e���o��U���̏ꍇ
			else	if (nextAttackId == 2 || nextAttackId == 4) {
				// �v���C���[���{�X�G���A�̒��S���E���ɂ�����Ē��I
				if (xCenter < playerPos.x) {
					continue;
				}
			}

			break;
		}

	}
	_attackInfo = bossAttack[nextAttackId];
}

// �\���́uTEXTURE�v�̏�����
void Boss::TextureInit(int modelHandle)
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
void Boss::ChangeTexture(int isDamaged)
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
void Boss::MaterialInit(int modelHandle)
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
void Boss::ChangeMaterialDifColor(int isDamaged)
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

// �{�X��pBGM�̓ǂݍ���
void Boss::LoadBossBGM()
{
	_bgmBossBattle = -1;
	_bgmBossBattle = LoadSoundMem("res/sound/BGM/bgm_boss.mp3");
}

// �{�X��pBGM�̔j��
void Boss::DeleteBossBGM()
{
	if (_bgmBossBattle != -1) {
		DeleteSoundMem(_bgmBossBattle);
		_bgmBossBattle = -1;
	}
}

// �{�X��pBGM�̍Đ��E��~
void Boss::PlayBossBGM(int n)
{
	if (n == 0) {
		StopSoundMem(_bgmBossBattle);
	}
	else {
		PlaySoundMem(_bgmBossBattle, DX_PLAYTYPE_LOOP);
	}
}



// �f�o�b�O���̕\��
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
		if (_attackInfo.bullet[i].use == -1) break; // use == -1�͖��g�p�z��

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
				DrawFormatString(world.x, world.y, COLOR_WHITE, "��");
			}
			else if (_attackInfo.bullet[i].direction.x == -1) {
				DrawFormatString(world.x, world.y, COLOR_WHITE, "��");

			}
			else if (_attackInfo.bullet[i].direction.y == 1) {
				DrawFormatString(world.x, world.y, COLOR_WHITE, "��");
			}
			else if (_attackInfo.bullet[i].direction.y == -1) {
				DrawFormatString(world.x, world.y, COLOR_WHITE, "��");
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