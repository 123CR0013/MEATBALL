#include "enemy_ground.h"
#include "enemy_data.h"
#include "jumppad.h"
#include "gimmick_data.h"
#include "resource.h"

// �e�N�X�`���摜�n���h��
int EnemyGround::_cgTexEnGround[2] = { -1, -1 };
// �}�e���A�����
std::vector<MATERIAL>	EnemyGround::_material;

// �n���������A�v���C���[��������ƒǂ������Ă���G
// �ʏ��Ԃƃ��^����Ԃ�����
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

	// �G�̋����������ݒ肷��
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

	// �̂̒��S�ʒu���X�V
	UpdateCenterPos();
	// �����蔻���`���X�V
	SetCollision();

	_fallStartPos = VGet(0, 0, 0);
	_fallEndPos = VGet(0, 0, 0);
	_fallCnt = 0;

	_canPlaySe = 1;


	// ������Ԃ̃��[�V�����̐ݒ�
	_motionNum = GROUND_MOTION_WALK;

	// ���[�V�����̏���������
	MotionInit(GROUND_MOTION_MAX);
}

EnemyGround::~EnemyGround()
{
	//CharaBase::~CharaBase();
}




void EnemyGround::Process(VECTOR playerPos) {

	// ���^����ԂȂ�
	if (_isMetal == 1) {
		// �ʏ��Ԃֈڍs������Ȃ�
		if (_isChange == 1) {
			_isMetal = 0;

			// �@�B�p�[�c�̃��f����n��G�{�̂��番������
			MV1SetMatrix(_metalModel, MGetIdent());

			// �@�B�p�[�c�̗����J�n���W��ێ�����
			_fallStartPos = _pos;
			_fallStartPos.y -= _bodyRect.h / 2;
			// �@�B�p�[�c�̗����I�����W��ێ�����
			_fallEndPos = _pos;
			_fallEndPos.y -= _bodyRect.h / 8;
			// �����̃J�E���g�����Z�b�g����
			_fallCnt = 0;

			// �n��G�{�̂̃T�C�Y�ɂ��킹�ċ@�B�p�[�c���f���̃T�C�Y�𒲐�����
			VECTOR eSize = MV1GetScale(_modelHandle);
			MV1SetScale(_metalModel, VScale(eSize, 5));
		}
	}

	// ���̓G���|����Ă��Ȃ�������
	if (_use == 1) {
		int motionNum_old = _motionNum;

		// ���ʏ���
		// �d�͏����ƃq�b�g�X�g�b�v����
		EnemyBase::Process(playerPos);

		// �q�b�g�X�g�b�v���łȂ����
		if (_isHitStop == 0) {

			// �W�����v��Ƃ̓����蔻��
			for (int i = 0; i < JUMPPAD_MAX; i++) {
				if (jumpPad[i]->GetUse() == 0) continue;
				// �W�����v��{�̂Ƃ̓����蔻��
				MYRECT jRect = jumpPad[i]->GetRect();
				if (IsHitBox(_bodyRect.x, _bodyRect.y, _bodyRect.w, _bodyRect.h, jRect.x, jRect.y, jRect.w, jRect.h) == 1) {

					// �����o������
					VECTOR pPos = _pos;
					VECTOR jPos = jumpPad[i]->GetPos();
					
					// �����o���������v�Z����
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
			}


			// �ړ���Ԃ��Ƃ̏���

			// �U�����
			if (_isAttack == 1) {
				// �ړ����x�̐ݒ�
				if (_isMetal != 1) {
					_speed = ENEMY_GROUND_SPEED_ATTACK;
				}
				else {
					_speed = ENEMY_METAL_SPEED_ATTACK;
				}
				_motionNum = GROUND_MOTION_RUN;

				// SE���Đ��\�Ȃ�
				if (_canPlaySe == 1) {
					// ���ѐ�SE�̍Đ�
					PlaySoundMem(se[0], DX_PLAYTYPE_BACK);
					// SE�Đ��s��Ԃɂ���
					_canPlaySe = 0;
				}
			}
			// �ǐՏ��
			else if (_isSpotPlayer == 1) {
				// �ړ����x�̐ݒ�
				if (_isMetal != 1) {
					_speed = ENEMY_GROUND_SPEED_RUN;
				}
				else {
					_speed = ENEMY_METAL_SPEED_RUN;
				}

				// �_�b�V���U���̃N�[���^�C���i����ԁj�łȂ���Ό�����ς���
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
			// ����G���
			else {
				// �ړ����x�̐ݒ�
				if (_isMetal != 1) {
					_speed = ENEMY_GROUND_SPEED_WALK;
				}
				else {
					_speed = ENEMY_METAL_SPEED_WALK;
				}

				_motionNum = GROUND_MOTION_WALK;
			}

			// �U�����̏���
			if (_attackRemainTime > 0) {
				_attackRemainTime--;
				// �U�����I�������N�[���^�C�����Z�b�g����
				if (_attackRemainTime == 0) {
					SetAttack(0);
					_attackCoolTime = ENEMY_GROUND_COOLTIME;					
				}
			}

			// �N�[���^�C�����̏���
			if (_attackCoolTime > 0) {
				_speed = 0;
				_motionNum = GROUND_MOTION_TIRED;

				_attackCoolTime--;
				// �U���̃N�[���^�C�����I�������
				if (_attackCoolTime <= 0) {
					_attackCoolTime = 0;
					// SE�Đ��\��Ԃɂ���
					_canPlaySe = 1;
				}
			}

			// �������͍��E�ړ������Ȃ�
			if(_stand != 0) {
				// �ړ�����
				VECTOR vDir = _direction;
				VECTOR move = VScale(vDir, _speed);
				_pos.x += move.x;
				//_pos.y += move.y;

				MYRECT tmpRect = _bodyRect;
				tmpRect.x += move.x;

				// �}�b�v�`�b�v�Ƃ̓����蔻��
				HITCHIP hitChip = gMap.IsHitBoxAndMapChip(tmpRect);
				// �����o������
				float w = 0;
				// �������΂߂̃}�b�v�`�b�v�ɓ������Ă��邩�ǂ���
				int hitDiagonalChip = 0;

				// �}�b�v�`�b�v�ɓ������Ă����ꍇ
				switch(hitChip.chip_no)
				{
					// �}�b�v�`�b�v�ɓ������Ă��Ȃ��ꍇ
				case -1:
					break;

					// �}�b�v�`�b�v�ɓ������Ă����ꍇ
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

					// �������΂߂̃}�b�v�`�b�v�ɓ������Ă����ꍇ
				case LOWER_RIGHT:
				case LOWER_LEFT:
					hitDiagonalChip = 1;
					break;
				}

				// �̂̒��S�ʒu���X�V
				UpdateCenterPos();
				// �����蔻���`���X�V
				SetCollision();


				//
				// �i�s�����Ɍ������邩�𒲂ׂ�
				// 

				// �����Ɏ΂߂̃}�b�v�`�b�v������A�������邩�̔�����s��Ȃ�
				// ��������Ă���Ƃ��͏���Ă���}�b�v�`�b�v��1�O�Ƀ}�b�v�`�b�v���������߁A�i�s�����Ɍ�������Ɣ��肳��Ă��܂�����
				if(hitDiagonalChip != 1) {

					// �v���C���[�Ǐ]�E�_�b�V���U�����́A���̑O�ɂƂǂ܂�
					// ����G���́A�܂�Ԃ�


					// _bodyRect�̉����[���}�b�v�`�b�v�Ɠ������Ă��邩�𒲂ׂ�

					tmpRect = _bodyRect;
					// _bodyRect�̍���
					float tmpX = tmpRect.x;
					float tmpY = tmpRect.y + tmpRect.h + 10; // + 10�����āA�L�����̉��Ƀ}�b�v�`�b�v�����邩�𔻒肷��

					int chipX = tmpX / CHIPSIZE_W;
					int chipY = tmpY / CHIPSIZE_H;

					int stop = 0;

					// �����Ɏ΂߂̃}�b�v�`�b�v���Ȃ��ꍇ�̂݁A�O���Ɍ����Ȃ����𒲂ׂ�
					switch(gMap.IsHitMapchip(_pos.x, _pos.y + 10))
					{
					case LOWER_RIGHT:
					case LOWER_LEFT:

						break;

					default:

						if(_direction.x < 0) {
							tmpX = tmpRect.x;
							tmpY = tmpRect.y + tmpRect.h + 10; // + 10�����āA�L�����̉��Ƀ}�b�v�`�b�v�����邩�𔻒肷��

							if(gMap.IsHitMapchip(tmpX, tmpY) == -1) {
								// �v���C���[�Ǐ]�E�_�b�V���U����
								if(_isSpotPlayer == 1) {
									// ���̑O�܂ŉ����߂���~������
									float w;
									w = (chipX * CHIPSIZE_W + CHIPSIZE_W) - tmpRect.x;
									_pos.x += w;

									stop = 1;
								}
								// ����G��
								else {
									// �ړ������𔽓]������
									_direction = VGet(1, 0, 0);
								}
							}


						}
						else {
							// _bodyRect�̉E��
							tmpX += tmpRect.w;
							chipX = tmpX / CHIPSIZE_W;

							if(gMap.IsHitMapchip(tmpX, tmpY) == -1) {
								// �v���C���[�Ǐ]�E�_�b�V���U����
								if(_isSpotPlayer == 1) {
									// ���̑O�܂ŉ����߂���~������
									float w;
									w = chipX * CHIPSIZE_W - (tmpRect.x + tmpRect.w);
									w--;
									_pos.x += w;

									stop = 1;
								}
								// ����G��
								else {
									// �ړ������𔽓]������
									_direction = VGet(-1, 0, 0);
								}
							}
						}

						break;
					}


					// ���̑O�ɂƂǂ܂�ꍇ�ɓ����蔻����X�V����
					// (�Ƃǂ܂�Ȃ��ꍇ�͉����߂����������Ȃ��̂ŁA�����蔻��X�V�̕K�v���Ȃ�)
					if(stop == 1) {
						// �̂̒��S�ʒu���X�V
						UpdateCenterPos();
						// �����蔻���`���X�V
						SetCollision();
					}
				}
			}

			// �ǐՁE�U����ԂȂ�
			if (_isSpotPlayer == 1) {
				// ���̌�̔���G���ɂ��鍶�E�ړ��̋N�_�ʒu���X�V����
				_walkStartPos = _pos;
			}
			// ����G��ԂȂ�
			else {
				// _walkStartPos���N�_�ɍ��E�ړ�������
				if (_pos.x < _walkStartPos.x - ENEMY_GROUND_WALK_RANGE / 2) {
					_direction = VGet(1, 0, 0);
				}
				else if (_walkStartPos.x + ENEMY_GROUND_WALK_RANGE / 2 < _pos.x) {
					_direction = VGet(-1, 0, 0);
				}
			}


			// �_�b�V���U���Ƃ��̃N�[���^�[�����I������Ƃ��ɉ�ʓ��ɓ����Ă��邩�̔�����s���i��ʊO�ɏo�Ă��_�b�V���U���E�N�[���^�C�����p��������j
			if (_attackRemainTime == 0 && _attackCoolTime == 0) {
				// ��ʓ��ɂ��邩�𔻒�
				CheckIsInScreen();

				// ��ʊO�ɏo�Ă�����
				if (_isInScreen != 1) {
					SetIsSpotPlayer(0);
					SetAttack(0);
				}
			}

			// ���f���̉�]����
			UpdateModelRotation();
		}

		// ���[�V�����̌v�Z����
		MotionProcess(motionNum_old);
	}
}


// ���[�V�����̌v�Z����
void EnemyGround::MotionProcess(int motionNum_old)
{
	_playTime += 1;

	// ���[�V�����̍Đ����I�������A���̃��[�V�����̎n�߂ɖ߂�
	if (_motionNum == GROUND_MOTION_TIRED) {
		// 3D���f�����̂ɕs�������邽�߁A�����𒼐ړ��͂���
		if (_playTime >= 179.f) {
			_playTime = 0;
		}
	}
	else if (_motionNum == GROUND_MOTION_WALK) {
		// 3D���f�����̂ɕs�������邽�߁A�����𒼐ړ��͂���
		if (_playTime >= 59) {
			_playTime = 0;
		}
	}
	else {
		if (_playTime >= _anim[_motionNum].totalTime) {
			_playTime = 0;
		}
	}

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

// �U����Ԃ̐ݒ�
// ����n
// 0: ������, 1: �L����
void EnemyGround::SetAttack(int n)
{
	if(_isAttack == 0 && n == 1) {
		_attackRemainTime = ENEMY_GROUND_ATTACKTIME;
	}
	EnemyBase::SetAttack(n);
}

// �`��ʒu�̕␳����
void EnemyGround::AdjustDrawPos(MATRIX mView)
{
	EnemyBase::AdjustDrawPos(mView);
	MetalPartsAdjustDrawPos(mView);
}

// �`�揈��
void EnemyGround::Draw()
{
	
	EnemyBase::Draw();
	if (_metalModel != -1 && _fallCnt <= 60) {
		MV1DrawModel(_metalModel);
	}
}

// �e�N�X�`���摜�̓\��ւ�
// ����isDamaged �ɂ���ē\��e�N�X�`����ς���
// 0 : �ʏ펞, 1 : ��_���[�W��
void EnemyGround::ChangeTexture(int isDamaged)
{
	// ��_���[�W���̓e�N�X�`���𔒂�����
	if (isDamaged == 1) {
		MV1SetTextureGraphHandle(_modelHandle, 0, _cgTexWhite, FALSE);
	}
	else
	{
		// �ʏ펞�͌��ɖ߂�
		if (_metalModel == -1) {
			MV1SetTextureGraphHandle(_modelHandle, 0, _cgTexEnGround[0], FALSE);
		}
		else {
			MV1SetTextureGraphHandle(_modelHandle, 0, _cgTexEnGround[1], FALSE);
		}
	}


}

// �\���́uMATERIAL�v�̏�����
void EnemyGround::MaterialInit(int modelHandle)
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
void EnemyGround::ChangeMaterialDifColor(int isDamaged)
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

// �@�B�p�[�c�̕`��ʒu�̕␳����
void EnemyGround::MetalPartsAdjustDrawPos(MATRIX mView)
{
	if (_metalModel != -1) {
		if (_isMetal == 1) {
			// �A�^�b�`���郂�f���̃t���[���Z�Ԃ̍��W���擾����
			VECTOR weaponFrame1Position = MV1GetFramePosition(_metalModel, -1);
			// �A�^�b�`���郂�f�����t���[���Z�Ԃ̍��W�����_�ɂ��邽�߂̕��s�ړ��s����쐬
			MATRIX TransMat = MGetTranslate(VScale(weaponFrame1Position, -1.0f));
			// �A�^�b�`����郂�f���̃t���[���Z�Ԃ̍s����擾
			MATRIX characterFrame10Mat = MV1GetFrameLocalWorldMatrix(_modelHandle, 19);

			// �傫������
			MATRIX mScale = MGetScale(VGet(5.0f, 5.0f, 5.0f));
			//MATRIX mScale = MGetScale(_defaultSize);

			// ��]����
			MATRIX mRotation = MGetRotX(DegToRad(90.0f));
			mRotation = MMult(mRotation, MGetRotY(0.0f));
			mRotation = MMult(mRotation, MGetRotZ(0.0f));

			// �s��̍���
			MATRIX MixMatrix = MMult(mScale, mRotation);
			MixMatrix = MMult(MixMatrix, TransMat);
			MixMatrix = MMult(MixMatrix, characterFrame10Mat);

			/*MATRIX mTranslate = MGetTranslate(VGet(0, 0, -2));
			MixMatrix = MMult(MixMatrix, mTranslate);*/


			// ���������s����A�^�b�`���郂�f���ɃZ�b�g
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
//		// ���G�͈�
//		rectPos = VGet(_searchRect.x, _searchRect.y, 0);
//		rectPos = VTransform(rectPos, mView);
//		DrawBox(rectPos.x, rectPos.y, rectPos.x + _searchRect.w, rectPos.y + _searchRect.h, COLOR_GREEN, FALSE);
//
//		// ���G�͈�
//		rectPos = VGet(_attackRect.x, _attackRect.y, 0);
//		rectPos = VTransform(rectPos, mView);
//		DrawBox(rectPos.x, rectPos.y, rectPos.x + _attackRect.w, rectPos.y + _attackRect.h, COLOR_RED, FALSE);
//
//
//
//		// �G�̓����蔻��
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