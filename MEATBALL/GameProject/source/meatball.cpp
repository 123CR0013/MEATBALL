#include "meatball.h"
#include "winmain.h"
#include <math.h>
#include "mymath.h"
#include "mydraw.h"
#include "resource.h"

#include "enemy_shield.h"
#include "lever.h"

#include "effect.h"


// ���[�V�����p�萔
#define MEATBALL_MOTION_MAX 2

#define MEATBALL_MOTION_SOFT 0
#define MEATBALL_MOTION_STAND 1


// �������摜
int MeatBall::_cgAim;


MeatBall::MeatBall(int modelHandle, VECTOR playerPos) : CharaBase(modelHandle, playerPos)
{
	// �傫���̒���
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

	// �}�b�v�`�b�v�Ƃ̔��˗p��`�̍X�V
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


	// ���[�V�����̏���������
	_motionNum = MEATBALL_MOTION_STAND;
	MotionInit(MEATBALL_MOTION_MAX);


	// �A�E�g���C���̏����ݒ�
	MV1SetMaterialOutLineColor(_modelHandle, 0, GetColorF(1.0f, 0.0f, 1.0f, 1.0f));




	_isDrawAura = 0;
	_effectCnt = 0;



	_isScaling = 0;
	_scalingCnt = 0;
}

// �~�[�g�{�[�����v���C���[�̃^���N���ɔz�u���邽�߂̏���������
// ����:
// �v���C���[�̃��f���n���h��
// �^���N�̃t���[���ԍ�
void MeatBall::SetAttachToPlayer(int pHandle, int n) {
	_playerHandle = pHandle;
	_attachNum = n;
}

MeatBall::~MeatBall()
{
	//CharaBase::~CharaBase();

	// �c���p���f���̍폜
	for (int i = 0; i < MEATBALL_AFTERIMAGE_MAX; i++) {
		if (_afterImage[i].modelHandle != -1) {
			MV1DeleteModel(_afterImage[i].modelHandle);
			_afterImage[i].modelHandle = -1;
		}
	}
}

// �c���̏���������
void MeatBall::AfterImageInit(int modelHandle)
{
	VECTOR modelScale = MV1GetScale(_modelHandle);
	for (int i = 0; i < MEATBALL_AFTERIMAGE_MAX; i++) {
		_afterImage[i].use = 0;
		_afterImage[i].remainCnt = 0;

		// �R�c���f���̓ǂݍ��݁i��������j
		// 3D���f���̓ǂݍ��� MV1LoadModel()�͏����Ɏ��Ԃ�������̂ŁA3D���f���̕��� MV1DuplicateModel()���g��
		// ������3D���f���̓ǂݍ��݂͂� scenegamemain.cpp �� SceneGameMainResourceInit() ���ōs��
		_afterImage[i].modelHandle = MV1DuplicateModel(modelHandle);
		MV1SetScale(_afterImage[i].modelHandle, modelScale);

		_afterImage[i].pos = VGet(0, 0, 0);

		//�A�j���[�V�����̃A�^�b�`
		_afterImage[i].anim.index = MV1AttachAnim(_afterImage[i].modelHandle, MEATBALL_MOTION_SOFT, -1, FALSE);
		// �A�^�b�`�����A�j���[�V�����̑��Đ����Ԃ��擾����
		_afterImage[i].anim.totalTime = MV1GetAttachAnimTotalTime(_afterImage[i].modelHandle, _afterImage[i].anim.index);



		_afterImage[i].alpha = 1;
	}


}

// �c���̌v�Z����
void MeatBall::AfterImageProcess()
{
	for (int i = 0; i < MEATBALL_AFTERIMAGE_MAX; i++) {

		// ���t���[�������x��������
		_afterImage[i].alpha -= 0.15f;
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

// �c�����Z�b�g����
void MeatBall::SetAfterImage()
{
	for (int i = 0; i < MEATBALL_AFTERIMAGE_MAX; i++) {
		if (_afterImage[i].use == 1) continue;
		_afterImage[i].use = 1;

		// �c���̎������Ԃ��~�[�g�{�[���̈ړ����x�ɍ��킹�Đݒ肷��
		// �ړ����x���傫���قǎc���̎������Ԃ𒷂�����i3�i�K�j
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

		// �c���p���f���̃T�C�Y���~�[�g�{�[���Ɠ����ɂ���
		VECTOR modelScale = MV1GetScale(_modelHandle);
		MV1SetScale(_afterImage[i].modelHandle, modelScale);

		// �c���p���f���̉�]���~�[�g�{�[���Ɠ����ɂ���
		VECTOR modelRot = MV1GetRotationXYZ(_modelHandle);
		MV1SetRotationXYZ(_afterImage[i].modelHandle, modelRot);

		// �Đ����Ԃ��Z�b�g����(�A�j���[�V�����̍Đ�)
		float afterImagePlayTime = _playTime;
		MV1SetAttachAnimTime(_afterImage[i].modelHandle, _afterImage[i].anim.index, afterImagePlayTime);

		_afterImage[i].alpha = 1;
		break;
	}
}

// �v�Z����
void MeatBall::Process(VECTOR playerCenterPos)
{
	// �O�̃t���[���̃��[�V�����ԍ���ێ�����
	int motionNum_old = _motionNum;
	if (_isAttack == 1) {
		_motionNum = MEATBALL_MOTION_SOFT;
	}
	else {
		_motionNum = MEATBALL_MOTION_SOFT;
	}
	// ���[�V�����̌v�Z����
	MotionProcess(motionNum_old);

	// ��]����
	// 5�̃p�^�[�����烉���_��
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


	// ���x������
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

	// ��΂������̐ݒ�
	if (gPad->_input.X != 0 || gPad->_input.Y != 0) {
		vDir = VGet(gPad->_input.X, gPad->_input.Y, 0);
		vDir = VNorm(vDir);
		_arrow = VScale(vDir, 100);
	}



	// ���a�̌v�Z
	// ���a��32~368��43�i�K 
	// +8���T�C�Y���傫���Ȃ��Ă���
	_r = RADIUS_DEFAULT + (_size - 1) * RADIUS_INCREMENT_VALUE;


	// �q�b�g�X�g�b�v���łȂ����
	if (_hitStopCnt <= 0) {
		// �U�����Ȃ�
		if (_isAttack == 1) {

			_attackCoolTime--;
			// �U���̃N�[���^�C�����I�������
			if (_attackCoolTime < 0) {
				_attackCoolTime = 0;
				// �čU���i�����]���j�\�ɂ���
				_canReattack = 1;
			}

			// ���݂̃T�C�Y�����ƂɍU���͂�ݒ肷��
			_power = POWER_DEFAULT + (_size - 1) * POWER_INCREMENT_VALUE;


			// ���݂̃T�C�Y�����Ƃ�3D���f���̊g��l��ݒ肷��
			float magnification = 0.75f + (_size * 0.25f);
			VECTOR vScale = VScale(_defaultSize, magnification);
			// �X�P�[�����O�������łȂ����
			if (_isScaling == 0) {
				MV1SetScale(_modelHandle, vScale);
			}
			else {
				// �X�P�[�����O����
				ScalingProcess(vScale);
			}



			// �{�[���̈ړ����x����������ƃ}�b�v�`�b�v�����蔲���邱�Ƃ�����
			// 1�t���[�����ł̈ړ�����������؂��Ĕ��f������
			// divSpeed���ړ��𔽉f������

			float sum = 0;
			float divSpeed = 1;

			// �G�t�F�N�g���o���}�b�v�`�b�v���W��ێ�����
			std::vector<HITCHIP> effectChip;

			// �q�b�gSE��炷���ǂ���
			int isPlayHitSound = 0;

			while (true) {
				sum += divSpeed;
				// �N�����v
				// ���̃��[�v�ŁA���v�ړ�������1�t���[���ł̈ړ����� _speed �𒴂���ꍇ�A�����Ȃ��l��divSpeed�𒲐�����
				if (sum > _speed) {
					/*divSpeed = _speed - (sum - divSpeed);
					finish = 1;*/

					break;
				}

				// ��ʊO�ɏo�Ȃ��悤��
				// x���̔���
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
				// y���̔���
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


				// �ړ�����
				VECTOR vDir = VNorm(_direction);
				VECTOR vMove = VScale(vDir, divSpeed);
				_pos.x += vMove.x;
				_pos.y += vMove.y;

				// �}�b�v�`�b�v�Ƃ̔��˗p��`�̍X�V
				UpdateReflectRect();


				// �}�b�v�`�b�v�Ƃ̓����蔻��
				HITCHIP hitChip = gMap.IsHitBoxAndMapChip(_reflectRect);
				// �}�b�v�`�b�v�ɓ������Ă�����
				if(hitChip.chip_no != -1) {


					MYRECT chipRect = { hitChip.x * CHIPSIZE_W, hitChip.y * CHIPSIZE_H, CHIPSIZE_W, CHIPSIZE_H };

					// �����o���������擾����
					// �������A�����蔻��̏d�Ȃ��������̉����Əc���������ɂȂ����ꍇ�͂��܂����˂����邱�Ƃ��ł��Ȃ�
					// ���̂��߁A�����蔻��̏㉺���E��4�n�_�ł̔��˂��ł��Ȃ������ꍇ�̑���̑[�u�Ƃ��Ďg�p����
					int extrusionDirection = CheckExtrusionDirection(_reflectRect, chipRect);

					// ���ˏ������s�������邩�ǂ���
					int reflect = 0;
					// �΂߂̃}�b�v�`�b�v�ɓ����������ǂ���
					int diagonalReflect = 0;

					// ���������΂߂̃}�b�v�`�b�v�̖@���x�N�g����ێ�����
					VECTOR vNormal;

					// ���������}�b�v�`�b�v�̎�ނ��Ƃɏ������s��
					switch(hitChip.chip_no)
					{
						// �󂹂�}�b�v�`�b�v
					case BREAK_CHIP_1:
						// �}�b�v�`�b�v�j�󏈗�
						gMap.BreakMapChip(hitChip);
						isPlayHitSound = 2;

						// �u���b�N�j��G�t�F�N�g�̃Z�b�g
						SetChipBreakEffect(hitChip);
						break;
						// ���x����2�ȏ�Ȃ�󂹂�}�b�v�`�b�v
					case BREAK_CHIP_2:
						if(_level >= 2) {
							// �}�b�v�`�b�v�j�󏈗�
							gMap.BreakMapChip(hitChip);
							isPlayHitSound = 2;

							// �u���b�N�j��G�t�F�N�g�̃Z�b�g
							SetChipBreakEffect(hitChip);
						}
						else {
							reflect = 1;
						}
						break;
						// ���x����3�ȏ�Ȃ�󂹂�}�b�v�`�b�v
					case BREAK_CHIP_3:
						if(_level >= 3) {
							// �}�b�v�`�b�v�j�󏈗�
							gMap.BreakMapChip(hitChip);
							isPlayHitSound = 2;

							// �u���b�N�j��G�t�F�N�g�̃Z�b�g
							SetChipBreakEffect(hitChip);
						}
						else {
							reflect = 1;
						}
						break;
						// �X�s�[�h��30�ȏ�Ȃ�󂹂�}�b�v�`�b�v
					case BREAK_CHIP_4:
						if(_speed >= 30) {
							// �}�b�v�`�b�v�j�󏈗�
							gMap.BreakMapChip(hitChip);
							isPlayHitSound = 2;

							// �u���b�N�j��G�t�F�N�g�̃Z�b�g
							SetChipBreakEffect(hitChip);
						}
						else {
							reflect = 1;
						}
						break;

						// �ʏ�̃}�b�v�`�b�v�ɓ��������ꍇ
					default:
						reflect = 1;
						break;

						// �΂߂̃}�b�v�`�b�v�ɂ����������ꍇ
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

					// ���ˏ���������ꍇ
					if(reflect == 1) {



						// �㉺���E�̓_���}�b�v�`�b�v�ɓ������Ă��邩�𒲂ׂ�
						// ��_���W����㉺���E�Ɉړ������_
						EXTRUSION ex[] = {
							// ��
							{0, 1, VGet(_reflectRect.x, _pos.y, 0)},
							// �E
							{0, -1, VGet(_reflectRect.x + _reflectRect.w, _pos.y, 0)},

							// ��
							{1, 1, VGet(_pos.x, _reflectRect.y, 0)},
							// ��
							{1, -1, VGet(_pos.x, _reflectRect.y + _reflectRect.w, 0)},

						};

						// �e���ɂ����āA�_���}�b�v�`�b�v�ɓ����������ǂ����i���g�ɂ�ex�̗v�f�ԍ�������j
						// ���łɔ��Α��̓_���������Ă����ꍇ�̓��[�v���甲���鏈�����s������
						int hitX = -1;
						int hitY = -1;

						// �㉺���E�̓��̂ǂ����̓_���������Ă��邩
						int isHitPoint = 0;

						for(int i = 0; i < 4; i++) {
							// �e�_���}�b�v�`�b�v�ɓ������Ă��邩�𒲂ׂ�
							int isHit = gMap.IsHitMapchip(ex[i].point.x, ex[i].point.y);
							// �}�b�v�`�b�v�ɓ������Ă����ꍇ
							// �����o�����������āA������Ȃ��Ȃ�܂ŌJ��Ԃ�
							while(isHit != -1)
							{
								isHitPoint = 1;
								// x���̏���
								if(ex[i].axis == 0) {
									// ���łɔ��Α��̓_���������Ă����ꍇ�̓��[�v���甲����
									// ��j���̓_���������Ă����ꍇ�A�E�̓_�̓����蔻��͍s��Ȃ�
									if(hitX != -1 && hitX != i) break;
									hitX = i;
									// �����o������
									_pos.x += ex[i].direction;

									// ���˕����̐ݒ�
									if(ex[i].direction == 1) {
										_direction.x = fabs(_direction.x);
									}
									else {
										_direction.x = -fabs(_direction.x);
									}

								}
								// y���̏���
								else {
									// ���łɔ��Α��̓_���������Ă����ꍇ�̓��[�v���甲����
									if(hitY != -1 && hitY != i) break;
									hitY = i;

									// �����o������
									_pos.y += ex[i].direction;

									// ���˕����̐ݒ�
									if(ex[i].direction == 1) {
										_direction.y = fabs(_direction.y);
									}
									else {
										_direction.y = -fabs(_direction.y);
									}
								}

								// �}�b�v�`�b�v�Ƃ̔��˗p��`�̍X�V
								UpdateReflectRect();

								ex[0].point = VGet(_reflectRect.x, _pos.y, 0);
								ex[1].point = VGet(_reflectRect.x + _reflectRect.w, _pos.y, 0);
								ex[2].point = VGet(_pos.x, _reflectRect.y, 0);
								ex[3].point = VGet(_pos.x, _reflectRect.y + _reflectRect.w, 0);


								isHit = gMap.IsHitMapchip(ex[i].point.x, ex[i].point.y);
							}
						}


						// �㉺���E�̓_���}�b�v�`�b�v�ɓ������Ă��Ȃ������ꍇ
						// 
						// 
						// �������A�����蔻��̏d�Ȃ��������̉����Əc���������ɂȂ����ꍇ�͂��܂����˂����邱�Ƃ��ł��Ȃ�
						// ���̂��߁A�����蔻��̏㉺���E��4�n�_�ł̔��˂��ł��Ȃ������ꍇ�̑���̑[�u�Ƃ��Ďg�p����
						if(isHitPoint == 0) {
							// ���˕����̐ݒ�					
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
								// �����蔻��̏d�Ȃ��������̉����Əc���������ɂȂ����ꍇ�͂��܂����˂����邱�Ƃ��ł��Ȃ�
								// �~�[�g�{�[���̂ƃ}�b�v�`�b�v�̓����蔻���`�ǂ����̊p�����������Ƃ��Ȃ�
								_direction.x = -_direction.x;
								_direction.y = -_direction.y;
							}
						}

						// �~�[�g�{�[���̉�]��5�̃p�^�[�����烉���_���Ō��肷��
						SetRotType();


						isPlayHitSound = 1;


						//// ���̃t���[���Ŋ��ɓ����}�b�v�`�b�v�ɃG�t�F�N�g���Z�b�g����Ă���ꍇ�́A�V�����G�t�F�N�g�̓Z�b�g���Ȃ�
						//int effect_ok = 1;
						//for(auto eChip : effectChip) {
						//	if(hitChip.x == eChip.x && hitChip.y == eChip.y) {
						//		effect_ok = 0;
						//		break;
						//	}
						//}

						//if(effect_ok == 1) {
						//	// ���G�t�F�N�g�̐ݒ�
						//	SetSmokeEffect(hitChip);
						//	effectChip.push_back(hitChip);
						//}

						//// �X�P�[�����O�����̊J�n�ݒ�
						//SetScaling();
					}

					// �΂߂̃}�b�v�`�b�v�ɂ����������ꍇ�̔���
					else if(diagonalReflect == 1) {
						// ���������΂߂̃}�b�v�`�b�v�̖@���x�N�g��
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

						// �~�[�g�{�[���̉�]��5�̃p�^�[�����烉���_���Ō��肷��
						SetRotType();

						isPlayHitSound = 1;
					}

					// ���ɓ����}�b�v�`�b�v�ɃG�t�F�N�g���Z�b�g����Ă���ꍇ�́A�V�����G�t�F�N�g�̓Z�b�g���Ȃ�
					int effect_ok = 1;
					for(auto eChip : effectChip) {
						if(hitChip.x == eChip.x && hitChip.y == eChip.y) {
							effect_ok = 0;
							break;
						}
					}

					if(effect_ok == 1) {
						// ���G�t�F�N�g�̐ݒ�
						SetSmokeEffect(hitChip);
						effectChip.push_back(hitChip);
					}
					// �X�P�[�����O�����̊J�n�ݒ�
					SetScaling();


					// �}�b�v�`�b�v�Ƃ̔��˗p��`�̍X�V
					UpdateReflectRect();

				}


				// ���Ƃ̓����蔻��
				for (int i = 0; i < ENEMY_SHIELD_MAX; i++) {
					SHIELD eShield = GetShieldInfo(i);
					if (eShield.use != 0) {
						// ���ƃ{�[���������������𔻒肷��
						int hitShield = 0;
						MYRECT mRect = _reflectRect;
						VECTOR sDir = eShield.direction;

						// ���i�J�v�Z���j�ƃ{�[���i��`�j�̓����蔻��
						// ���̓����蔻��i�J�v�Z���j��3�̃p�[�c�ɕ����Ĕ�����s��
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

						// �������Ă����ꍇ�A���̊p�x�ɉ����Ĕ��˕������v�Z����
						if (hitShield == 1) {
							_pos.x -= vMove.x * 30;
							_pos.y -= vMove.y * 30;

							// �@���x�N�g��
							VECTOR vNormal = VNorm(eShield.direction);
							VECTOR dirTmp = VNorm(_direction);

							auto dot = 2.0f * VDot(vNormal, dirTmp);

							VECTOR vReflect = VScale(vNormal, dot);
							vReflect = VSub(vReflect, dirTmp);
							vReflect = VNorm(vReflect);
							_direction = vReflect;


							// �X�P�[�����O�����̊J�n�ݒ�
							SetScaling();


							// ���G�t�F�N�g�i�J���[�j�̃Z�b�g
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


				// ���o�[�Ƃ̓����蔻��
				for (int i = 0; i < LEVER_MAX; i++) {
					// ���̔z��v�f���g�p�������o�[���L��������Ă��Ȃ�������
					if (lever[i]->GetUse() == 1 && lever[i]->GetActive() != 1) {
						MYRECT leverRect = lever[i]->GetRect();
						// �������Ă����ꍇ�A���̃��o�[��L���ɂ���
						if (IsHitCircleAndBox(_pos.x, _pos.y, _r, leverRect) == 1) {							
							lever[i]->SetActive();
						}
					}
				}

			}

			// ���ȏ�̑��x�̏ꍇ�Ɏc�����o��
			if (_speed >= 20) {				
				// �c�����Z�b�g����
				SetAfterImage();
			}

			_launchCnt--;
			// ���ˁi�Ĕ��ˁj���Ă�����t���[���o�߂�����
			if (_launchCnt < 0) {
				_launchCnt = 0;
				// ���t���[��SPEED_DECREMENT_VALUE�̕�������������
				_speed -= SPEED_DECREMENT_VALUE;
				if (_speed < SPEED_MIN) {
					_speed = SPEED_MIN;
				}
			}

			// SE�̍Đ�����
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


			// ��ʊO�ɏo��30�t���[���o������v���C���[�̂��Ƃɖ߂�
			_isInScreen = CheckIsInScreen();
			if (_isInScreen == 0) {

				// �Ăі߂�SE�̍Đ�
				if (CheckSoundMem(se[20]) == 0) {
					PlaySoundMem(se[20], DX_PLAYTYPE_BACK);
				}

				_offScreenTime++;

				// ��ʊO�ɂ��鎞�Ԃ�MAX�𒴂��Ă����ꍇ�A�Ăі߂��������s��
				if (_offScreenTime >= MEATBALL_RETURN_CNT_MAX) {
					_isAttack = 0;
					_offScreenTime = 0;
				}
			}
			else {
				_offScreenTime = 0;
			}

		}
		// �U�����łȂ����
		else {
			// �v���C���[�̌��֖߂�
			ReturnToPlayer(playerCenterPos);
		}
	}
	// �q�b�g�X�g�b�v���Ȃ�
	else {

		if (_hitStopCnt % 2 == 0) {
			// _hitStopDir�̕����i�܂��͂��̔��Ε����j�ɗh�炷
			VECTOR tmpDir = _direction;
			if (_hitStopCnt % 4 == 2) {
				tmpDir = VScale(tmpDir, -1);
			}

			// �ړ�����
			tmpDir = VNorm(tmpDir);
			VECTOR vMove = VScale(tmpDir, 10);
			_pos.x += vMove.x;
			_pos.y += vMove.y;

		}

		_hitStopCnt--;
		// �q�b�g�X�g�b�v���I�������
		if (_hitStopCnt == 0) {
			// �q�b�g�X�g�b�v�O�̍��W�ɖ߂�
			_pos = _beforHitStopPos;
		}
	}

	// �q�b�g�X�g�b�v���ɂ͓����蔻��̍X�V���s��Ȃ�
	if (_hitStopCnt <= 0) {
		// �}�b�v�`�b�v�Ƃ̔��˗p��`�̍X�V
		UpdateReflectRect();
	}

	// 3D���f���p���W�̍X�V
	UpdateWorldPos();

	// �c���̌v�Z����
	AfterImageProcess();
}

// 3D���f���p���W�̍X�V
void MeatBall::UpdateWorldPos()
{
	// �X�N���[�����W�����[���h���W�ɕϊ�
	_worldPos = ConvScreenPosToWorldPos(_screenPos);
	_worldPos.z = 350; // z���\���ʒu�̒���
}

// �U����Ԃ�ύX����
// ���� n
// 0: ��U����Ԃ�, 1: �U����Ԃ�
void MeatBall::SetAttack(VECTOR playerCenterPos, int n)
{
	// ��U����Ԃɂ���ꍇ
	if (n == 0) {
		_isAttack = 0;
		return;
	}

	// �U���\���ǂ���
	int attack_ok = 0;
	// �v���C���[�̂��Ƃɂ����ԂȂ�
	if (_isReturn == 1) {
		_isAttack = 1;
		_canReattack = 1;
		_pos = playerCenterPos;

		attack_ok = 1;
	}
	// �U����Ԃ��čU���\�Ȃ�
	else if (_isAttack == 1 && _canReattack == 1) {
		_canReattack = 0;

		attack_ok = 1;
	}


	// �U���\�Ȃ�
	if (attack_ok == 1) {
		_rotCoolTime = 0;
		// �~�[�g�{�[���̉�]��5�̃p�^�[�����烉���_���Ō��肷��
		SetRotType();
		_attackCoolTime = MEATBALL_ATTACK_COOLTIME;

		_direction = _arrow;

		// �X�s�[�h�����ˎ��̂Ƃ����x���Ȃ��Ă�����A�X�s�[�h��ݒ肷��
		// ������ԂȂ炻�̑������ێ����邽�߁i������Ԃ�SPEED_MAX���X�s�[�h�̒l���傫���Ȃ�j
		if (_speed < SPEED_MAX) {
			_speed = SPEED_MAX;
		}
		_launchCnt = LAUCH_COUNT_MAX;


		_isReturn = 0;


		// ���ˎ��ɉ�ʊO�ɏo�Ă���Ƃ��͉�ʓ��ɉ����߂�
		int isInScreen = 0;
		while (isInScreen != 1) {
			isInScreen = 1;

			// x��
			if (_pos.x - _r < gCam._pos.x - SCREEN_W / 2) {
				_pos.x++;
				isInScreen = 0;
			}
			else if (gCam._pos.x + SCREEN_W / 2 < _pos.x + _r) {
				_pos.x--;
				isInScreen = 0;
			}

			// y��
			if (_pos.y - _r < gCam._pos.y - SCREEN_H / 2) {
				_pos.y++;
				isInScreen = 0;
			}
			else	if (gCam._pos.y + SCREEN_H / 2 < _pos.y + _r) {
				_pos.y--;
				isInScreen = 0;
			}


		}
		// �}�b�v�`�b�v�Ƃ̔��˗p��`�̍X�V
		UpdateReflectRect();



		// �v���C���[���}�b�v�`�b�v�ɖ������Ă���Ƃ��ɁA�{�[�����΂��Ă��}�b�v�`�b�v�ɂ߂荞�܂Ȃ��悤�ɂ���
		// ���ˎ��ɂ��łɃ{�[���ƃ}�b�v�`�b�v���������Ă���ꍇ�́A��x�}�b�v�`�b�v�Ɠ�����Ȃ����W�܂ŉ����o��

		// �e���ɂ����āA�_���}�b�v�`�b�v�ɓ����������ǂ����i���g�ɂ�ex�̗v�f�ԍ�������j
		// ���łɔ��Α��̓_���������Ă����ꍇ�̓��[�v���甲���鏈�����s������
		int hitX = -1;
		int hitY = -1;

		// �㉺���E�̓_���}�b�v�`�b�v�ɓ������Ă��邩�𒲂ׂ�
		// ��_���W����㉺���E�Ɉړ������_
		EXTRUSION ex[] = {
			// ��
			{0, 1, VGet(_reflectRect.x, _pos.y, 0)},
			// �E
			{0, -1, VGet(_reflectRect.x + _reflectRect.w, _pos.y, 0)},

			// ��
			{1, 1, VGet(_pos.x, _reflectRect.y, 0)},
			// ��
			{1, -1, VGet(_pos.x, _reflectRect.y + _reflectRect.w, 0)},

		};		


		for (int i = 0; i < 4; i++) {
			// �e�_���}�b�v�`�b�v�ɓ������Ă��邩�𒲂ׂ�
			int isHit = gMap.IsHitMapchip(ex[i].point.x, ex[i].point.y);
			// �}�b�v�`�b�v�ɓ������Ă����ꍇ
			// �����o�����������āA������Ȃ��Ȃ�܂ŌJ��Ԃ�
			while (isHit != -1)
			{
				// x���̏���
				if (ex[i].axis == 0) {
					// ���łɔ��Α��̓_���������Ă����ꍇ�̓��[�v���甲����
					// ��j���̓_���������Ă����ꍇ�A�E�̓_�̓����蔻��͍s��Ȃ�
					if (hitX != -1 && hitX != i) break;
					hitX = i;
					// �����o������
					_pos.x += ex[i].direction;
				}
				// y�m�̏���
				else {
					if (hitY != -1 && hitY != i) break;
					hitY = i;
					// �����o������
					_pos.y += ex[i].direction;
				}

				// �}�b�v�`�b�v�Ƃ̔��˗p��`�̍X�V
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



// �v���C���[�̏��ɖ߂�
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

// �G�E�{�X�Ɠ��������Ƃ��̔��ˏ���
void MeatBall::Reflect(MYRECT objRect)
{
	// �����o���������v�Z����
	int extrusionDirection = CheckExtrusionDirection(_reflectRect, objRect);

	// ���˕����̐ݒ�					
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
		case 0: // ������
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

		case 1: // �c����
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
		// �}�b�v�`�b�v�Ƃ̔��˗p��`�̍X�V
		UpdateReflectRect();
		isHit = IsHitBox(_reflectRect.x, _reflectRect.y, _reflectRect.w, _reflectRect.h,
			objRect.x, objRect.y, objRect.w, objRect.h);
	}



	// �}�b�v�`�b�v�Ƃ̔��˗p��`�̍X�V
	UpdateReflectRect();
	// 3D���f���p���W�̍X�V
	UpdateWorldPos();


}


// �}�b�v�`�b�v�Ƃ̔��˗p��`�̍X�V
void MeatBall::UpdateReflectRect()
{
	float side = _r / sqrt(2);

	_reflectRect.x = _pos.x - side;
	_reflectRect.y = _pos.y - side;
	_reflectRect.w = side * 2;
	_reflectRect.h = side * 2;
}

// �T�C�Y�̍X�V����
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


// �`��ʒu�̕␳����
void MeatBall::AdjustDrawPos(MATRIX mView)
{
	// �v���C���[���{�[���������Ă���Ƃ��ɁA�{�[�����v���C���[�̔|�{�|�b�h���ɔz�u����
	if(_isReturn == 1) {

		// �A�^�b�`���郂�f���̃t���[���Z�Ԃ̍��W���擾����
		VECTOR weaponFramePosition = MV1GetFramePosition(_modelHandle, -1);
		// �A�^�b�`���郂�f�����t���[���Z�Ԃ̍��W�����_�ɂ��邽�߂̕��s�ړ��s����쐬
		MATRIX TransMat = MGetTranslate(VScale(weaponFramePosition, -1.0f));
		// �A�^�b�`����郂�f���̃t���[���Z�Ԃ̍s����擾
		MATRIX characterFrameMat = MV1GetFrameLocalWorldMatrix(_playerHandle, _attachNum);

		// �傫������
		MATRIX mScale = MGetScale(VGet(0.28f, 0.28f, 0.28f));
		//MATRIX mScale = MGetScale(_defaultSize);

		// ��]����
		MATRIX mRotation = MGetRotY(DegToRad(180.f));
		mRotation = MMult(mRotation, MGetRotZ(0.0f));

		// �s��̍���
		MATRIX MixMatrix = MMult(mScale, mRotation);
		MixMatrix = MMult(MixMatrix, TransMat);
		MixMatrix = MMult(MixMatrix, characterFrameMat);

		/*MATRIX mTranslate = MGetTranslate(VGet(0, 0, -2));
		MixMatrix = MMult(MixMatrix, mTranslate);*/


		// ���������s����A�^�b�`���郂�f���ɃZ�b�g
		MV1SetMatrix(_modelHandle, MixMatrix);
		MV1DrawModel(_modelHandle);


	}
	else {

		// �{�[�����v���C���[�̎�𗣂�Ă���Ƃ��̃{�[���̕\�����W�ݒ�

		// �Z�b�g�����s��𖳌���
		MV1SetMatrix(_modelHandle, MGetIdent());


		CharaBase::AdjustDrawPos(mView);


		for(int i = 0; i < MEATBALL_AFTERIMAGE_MAX; i++) {
			if(_afterImage[i].use == 1) {
				VECTOR tmpPos = _afterImage[i].pos;
				tmpPos = VTransform(tmpPos, mView);
				tmpPos = ConvScreenPosToWorldPos(tmpPos);
				// �Â��c�������ɔz�u�����悤�ɒ���
				tmpPos.z = 100 * (10 - _afterImage[i].remainCnt);
				MV1SetPosition(_afterImage[i].modelHandle, tmpPos);
			}
		}
	}

	// �@�B�p�[�c�̌v�Z����
	// �~�[�g�{�[���̓���̃��O�ɋ@�B�p�[�c����������
	MetalPartsProcess();
}




// �`�揈��
void MeatBall::Draw(MATRIX mView)
{

	if(_isReturn == 1) {
		// �v���C���[���{�[���������Ă���Ƃ��ɁA�{�[�����v���C���[�̔|�{�|�b�h���ɔz�u����

		//CharaBase::Draw();


		// �A�E�g���C���̐ݒ�
		MV1SetMaterialOutLineWidth(_modelHandle, 0, 0);

	}
	else {
		// �c���̕`��
		DrawAfterImage();

		CharaBase::Draw();

		// �čU���\��ԂȂ�
		if(_canReattack == 1) {
			// �d���G�t�F�N�g�̕`��
			int electroSize = _r * 3;

			// �G�t�F�N�g�T�C�Y�̍Œ�l�ݒ�
			if(electroSize < 100) {
				electroSize = 100;
			}
			// �`��̓����x�ݒ�
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 140.f);
			// �G�t�F�N�g�摜�𕡐����d�˂ĕ`�悷��
			MyDrawModiGraph(gCam.GetViewMatrix(), _pos, 1, 0, electroSize, electroSize, cgEffect[EFFECT_TYPE_MEATBALL_ELECTRO][(_effectCnt / 2) % 40]);
			MyDrawModiGraph(gCam.GetViewMatrix(), _pos, 1, 0, electroSize, electroSize, cgEffect[EFFECT_TYPE_MEATBALL_ELECTRO][(_effectCnt / 3) % 40]);
			MyDrawModiGraph(gCam.GetViewMatrix(), _pos, 1, 0, electroSize, electroSize, cgEffect[EFFECT_TYPE_MEATBALL_ELECTRO][(_effectCnt / 1) % 40]);
			MyDrawModiGraph(gCam.GetViewMatrix(), _pos, 1, 0, electroSize, electroSize, cgEffect[EFFECT_TYPE_MEATBALL_ELECTRO][(_effectCnt / 4) % 40]);
			// �`��̓����x�����ɖ߂�
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

			_effectCnt++;

			// �A�E�g���C���̐ݒ�
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
	// �@�B�p�[�c�̕`�揈��
	MetalPartsDraw();
}


// �O�ʃ��C���[�ɕ`��
// �������̕`��
void MeatBall::DrawFrontLayer(MATRIX mView)
{
	// �v���C���[�̂��Ƃɂ���ꍇ
	if (_isReturn == 1) {
		// ���������ő�̒����ŕ`�悷��ꍇ�i�U���{�^���̃z�[���h���j
		if (_isDrawFullArrow == 1) {
			// ���˕����̐�
			DrawArrow(mView);
		}
	}
	else {
		// �čU���\�Ȃ�
		if (_canReattack == 1) {
			// ���˕����̐�
			DrawArrow(mView);
		}
	}
}

void MeatBall::DrawArrow(MATRIX mView)
{
	// ���˕����̐�
	VECTOR startPos = _pos;
	VECTOR vDir = VNorm(_arrow);

	// �������̒�����ݒ�
	int arrowLength = _isDrawFullArrow == 1 ? 1000 : 250;

	// 30�s�N�Z���Ԋu�ŕ������̉摜��`�悷��
	for (int i = 30; i < arrowLength; i += 30) {
		VECTOR arrowPos = VAdd(startPos, VScale(vDir, i));

		MyDrawModiGraph(mView, arrowPos, 1.0f, 0.0f, 10, 10, _cgAim);
	}
}

// �c���̕`�揈��
void MeatBall::DrawAfterImage()
{
	for (int i = 0; i < MEATBALL_AFTERIMAGE_MAX; i++) {
		if (_afterImage[i].use == 1) {
			MV1DrawModel(_afterImage[i].modelHandle);
		}
	}
}



// �~�[�g�{�[���̉�]��5�̃p�^�[�����烉���_���Ō��肷��
void MeatBall::SetRotType()
{
	if (_rotCoolTime <= 0) {
		int type_old = _rotType;
		while (true)
		{
			_rotType = rand() % 5;

			// ���݂̉�]����ύX���ꂽ�ꍇ�Ƀ��[�v���甲����
			if (type_old != _rotType) break;
		}
		
		_rotCoolTime = MEATBALL_ROTAION_COOLTIME;
	}
}

// �~�[�g�{�[������ʓ��ɂ��邩�𔻒肷��
// �Ԃ�l 0: ��ʓ��ɂ��Ȃ�, 1: ��ʓ��ɂ���
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


// �f�o�b�O���̕\��
void MeatBall::DrawDebugInfo(MATRIX mView, int fillFrag)
{
	float alpha = _isAttack ? 160 : 80;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	//DrawBox(_pos.x - _r, _pos.y - _r, _pos.x + _r, _pos.y + _r, COLOR_WHITE, TRUE);

	// �}�b�v�`�b�v�Ƃ̓����蔻���`
	// �~�ɓ��ڂ��鐳���`
	float side = _r / sqrt(2);
	VECTOR rectPos = VGet(_reflectRect.x, _reflectRect.y, 0);
	rectPos = VTransform(rectPos, mView);
	DrawBox(rectPos.x, rectPos.y, rectPos.x + _reflectRect.w, rectPos.y + _reflectRect.h, COLOR_WHITE, TRUE);

	// �~
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
	DrawFormatString(0, 500 + y * 16, COLOR_RED, "_r %3.2f, ���a %3.2f", _r, _r * 2); y++;

	DrawFormatString(0, 500 + y * 16, COLOR_RED, "_scalingCnt %2d", _scalingCnt); y++;




	//�~�́@�@�Ɂ~��\������
	/*int lineSize = 10;
	int color = GetColor(rand() % 256, rand() % 256, rand() % 256);
	for (int i = 0; i < 4; i++) {
		DrawLine(_collisionPos[i].x - lineSize, _collisionPos[i].y - lineSize, _collisionPos[i].x + lineSize + 1, _collisionPos[i].y + lineSize + 1, color);
		DrawLine(_collisionPos[i].x - lineSize, _collisionPos[i].y + lineSize, _collisionPos[i].x + lineSize + 1, _collisionPos[i].y - lineSize - 1, color);
	}*/


	//DrawFormatStringToHandle(_bodyRect.x, _bodyRect.y, COLOR_WHITE, fontHandle, "%d", 2 - _type);
}





// �@�B�p�[�c�̏���������
void MeatBall::MetalPartsInit(int metalPartsModel[])
{
	// �@�B�p�[�c�̏��
	struct METAL_PARTS_INFO
	{
		int type;						// ��ށiMETAL, BOLT, GEAR�j
		int level;						// ���݂̃~�[�g�{�[���̃��x�������̐��l�𒴂��Ă�����A���̃p�[�c��`�悷��
		const char* attachFrameName;	// ���̃p�[�c��z�u���郊�O�̖��O
		VECTOR scale;					// 3D���f���̊g��l
		VECTOR rotation;				// 3D���f���̉�]�l
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

	// ��L�̏��������o�ϐ� _metal[] �ɐݒ肷��
	for (int i = 0; i < MEATBALL_METAL_PARTS_MAX; i++){
		_metal[i].modelHandle = MV1DuplicateModel(metalPartsModel[mpInfo[i].type]);
		_metal[i].level = mpInfo[i].level;
		// �w�肳�ꂽ���O�̔ԍ����擾����
		_metal[i].attachNum = MV1SearchFrame(_modelHandle, mpInfo[i].attachFrameName);
		_metal[i].scale = mpInfo[i].scale;
		_metal[i].rotation = mpInfo[i].rotation;
	}
}

// �@�B�p�[�c�̌v�Z����
// �~�[�g�{�[���̓���̃��O�ɋ@�B�p�[�c����������
void MeatBall::MetalPartsProcess()
{
	for(int i = 0; i < MEATBALL_METAL_PARTS_MAX; i++) {
		// ���݂̃��x�����@�B�p�[�c��z�u���郌�x���ȏ�̏ꍇ�̂ݏ������s��
		if(_metal[i].level > _level) continue;

		// �A�^�b�`���郂�f���̃t���[���Z�Ԃ̍��W���擾����
		VECTOR weaponFramePosition = MV1GetFramePosition(_metal[i].modelHandle, -1);
		// �A�^�b�`���郂�f�����t���[���Z�Ԃ̍��W�����_�ɂ��邽�߂̕��s�ړ��s����쐬
		MATRIX TransMat = MGetTranslate(VScale(weaponFramePosition, -1.0f));
		// �A�^�b�`����郂�f���̃t���[���Z�Ԃ̍s����擾
		MATRIX characterFrameMat = MV1GetFrameLocalWorldMatrix(_modelHandle, _metal[i].attachNum);

		// �傫������
		MATRIX mScale = MGetScale(_metal[i].scale);

		// ��]����
		MATRIX mRotation = MGetRotX(_metal[i].rotation.x);
		mRotation = MMult(mRotation, MGetRotY(_metal[i].rotation.y));
		mRotation = MMult(mRotation, MGetRotZ(_metal[i].rotation.z));




		// �s��̍���
		MATRIX MixMatrix = MMult(mScale, mRotation);
		MixMatrix = MMult(MixMatrix, TransMat);
		MixMatrix = MMult(MixMatrix, characterFrameMat);


		// ���������s����A�^�b�`���郂�f���ɃZ�b�g
		MV1SetMatrix(_metal[i].modelHandle, MixMatrix);
	}
}

// �@�B�p�[�c�̕`�揈��
void MeatBall::MetalPartsDraw()
{
	for (int i = 0; i < MEATBALL_METAL_PARTS_MAX; i++) {
		// ���݂̃��x�����@�B�p�[�c��z�u���郌�x���ȏ�̏ꍇ�̂ݏ������s��
		if (_metal[i].level > _level) continue;

		MV1DrawModel(_metal[i].modelHandle);
	}
}

// �X�P�[�����O�̊J�n�ݒ�
void MeatBall::SetScaling()
{
	if (_isScaling == 0) {
		_isScaling = 1; 
		_scalingCnt = 0;
	}
};


// �X�P�[�����O����
void MeatBall::ScalingProcess(VECTOR vScale)
{
	VECTOR vTmp = VScale(vScale, 0.6f);
	VECTOR magnification;

	// �����ƂɃX�P�[�����O����
	// EasingOutElastic() �ł́A�S���̂悤�Ȏ����̃C�[�W���O�������s��
	magnification.x = EasingOutElastic(_scalingCnt, vTmp.x, vScale.x, 24);
	magnification.y = EasingOutElastic(_scalingCnt, vTmp.y, vScale.y, 24);
	magnification.z = EasingOutElastic(_scalingCnt, vTmp.z, vScale.z, 24);

	MV1SetScale(_modelHandle, magnification);

	_scalingCnt++;
	// �X�P�[�����O���I�������
	if (_scalingCnt > 3 * 8) {
		_isScaling = 0;
		_scalingCnt = 0;
	}
}

// ���G�t�F�N�g�̃Z�b�g
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

// �`�b�v�j��G�t�F�N�g�̃Z�b�g
void MeatBall::SetChipBreakEffect(HITCHIP hitChip)
{
	int effectX = hitChip.x * CHIPSIZE_W - CHIPSIZE_W;
	int effectY = hitChip.y * CHIPSIZE_H - CHIPSIZE_H;
	int effectSize = 64 * 3;

	SetEffect(EFFECT_TYPE_CHIP_BREAK, effectX, effectY, effectSize, effectSize);
}

// �I�[���`��̐ݒ�
void MeatBall::SetIsDrawAura(int n)
{
	if (n == 1) {
		if (_isDrawAura == 0) {
			_effectCnt = 0;
		}
	}

	_isDrawAura = n;
}

// �Ăі߂��G�t�F�N�g�̃Z�b�g
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
