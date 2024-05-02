#include "enemy_shield.h"
#include "enemy_data.h"

// �e�N�X�`�����
std::vector<TEXTURE>	EnemyShield::_texture;
// �}�e���A�����
std::vector<MATERIAL>	EnemyShield::_material;

// �����f���n���h��
int EnemyShield::_shieldModelBase;

// �����v���C���[�̕����ɍ\���āA�~�[�g�{�[���ł̍U�����K�[�h����G
EnemyShield::EnemyShield(int modelHandle, int size, int index, VECTOR startPos, int type) : EnemyBase(modelHandle, index, startPos)
{
	_type = ENEMY_TYPE_SHIELD;
	_index = index;

	// �G�̋����������ݒ肷��
	SetStregth(_type, size);

	_moveType = type;

	_pos.x = _pos.x * CHIPSIZE_W + _difBodyRect.w / 2;
	_pos.y = _pos.y * CHIPSIZE_W + _difBodyRect.h / 2;

	/*_pos.x = _pos.x * CHIPSIZE_W + 64;
	_pos.y = _pos.y * CHIPSIZE_W + 128;*/

	_speed = 1;

	_screenPos = _pos;
	UpdateWorldPos();
	MV1SetPosition(_modelHandle, _worldPos);


	_difCenterY = 0;
	// �̂̒��S�ʒu���X�V
	UpdateCenterPos();
	// �����蔻���`���X�V
	SetCollision();



	struct SHIELD_SIZE
	{
		float r;
		int h;
	};
	SHIELD_SIZE sSize[3] = {
		{32, 96},
		{32 * 2, 96 * 2},
		{32 * 3, 96 * 3},
	};
	_shieldCenterPos = VGet(0, 0, 0);
	_shieldDirection = VGet(0, 0, 0);
	_shieldScreenPos = VGet(0, 0, 0);
	_shieldWorldPos = VGet(0, 0, 0);
	SetShieldInfo(_index, sSize[size].r, sSize[size].h);
	// ���̒��S���W�Ɗp�x���v�Z����
	UpdateShieldCenterPos();
	UpdateShieldInfo(_index, 1, _shieldCenterPos, _shieldDirection);




	// ������Ԃ̃��[�V�����̐ݒ�
	_motionNum = SHIELD_MOTION_NORMAL;

	// ���[�V�����̏���������
	MotionInit(SHIELD_MOTION_MAX);







	_shieldModelHandle = -1;
	_shieldModelHandle = MV1DuplicateModel(_shieldModelBase);

	float rad = DegToRad(100.f);
	_defaultShieldRot = VGet(rad, 0, 0);
	MV1SetRotationXYZ(_shieldModelHandle, _defaultShieldRot);
	MV1SetScale(_shieldModelHandle, VGet(2.8f, 2.8f, 2.8f));
}

EnemyShield::~EnemyShield()
{
	//CharaBase::~CharaBase();

	if (_shieldModelHandle != -1) {
		MV1DeleteModel(_shieldModelHandle);
		_shieldModelHandle = -1;
	}
}


void EnemyShield::Process(VECTOR playerPos) {
	// ���̓G���|����Ă��Ȃ�������
	if (_use == 1) {
		int motionNum_old = _motionNum;

		// ���ʏ���
		// �q�b�g�X�g�b�v����
		EnemyBase::Process(playerPos);

		// �q�b�g�X�g�b�v���łȂ����
		if (_isHitStop == 0) {

			// ��ʓ��ɓ����Ă�����
			if (_isInScreen == 1) {
				// ���G��ԂȂ�
				if (_isSpotPlayer == 1) {
					// �ړ������̌v�Z
					VECTOR vDir = VSub(playerPos, _centerPos);
					_direction = VNorm(vDir);


					if (_moveType == ENSHIELD_TYPE_MOVE) {
						VECTOR vMove = VScale(_direction, _speed);
						_pos.x += vMove.x;
						_pos.y += vMove.y;

						// �̂̒��S�ʒu���X�V
						UpdateCenterPos();
						// �����蔻���`���X�V
						SetCollision();
					}


					_motionNum = SHIELD_MOTION_ENCOUNT;
				}
				else {
					MATRIX mRot = MGetRotZ(DegToRad(2.f));
					_direction = VTransform(_direction, mRot);
				}
			}
			else {
				SetIsSpotPlayer(0);
				MATRIX mRot = MGetRotZ(DegToRad(2.f));
				_direction = VTransform(_direction, mRot);
			}

			if (_isHitStop == 0) {
				// �����o���ʒu�Ə��̊p�x���v�Z����
				UpdateShieldCenterPos();
				// ���̏����Z�b�g����
				UpdateShieldInfo(_index, 1, _shieldCenterPos, _shieldDirection);
				// �����f���̉�]�l���Z�b�g����
				SetShieldModelRotation();
			}
			else {
				// ���𖳌��ɂ���
				UpdateShieldInfo(_index, 0);
			}


			//// �G���f���̉�]�l���Z�b�g����
			//SetModelRotation();

		}


		MotionProcess(motionNum_old);
	}
	else {
		// ���𖳌��ɂ���
		UpdateShieldInfo(_index, 0);
	}
}

// ���̒��S���W�Ɗp�x���v�Z����
void EnemyShield::UpdateShieldCenterPos()
{
	VECTOR vDir = VScale(_direction, 64 + 32 + 32);
	VECTOR vPos = VAdd(_centerPos, vDir);
	_shieldCenterPos = vPos;

	_shieldDirection.x = -_direction.y;
	_shieldDirection.y = _direction.x;

	_shieldDirection = VNorm(_shieldDirection);
}


// ����3D���f������]����
void EnemyShield::SetShieldModelRotation()
{
	// ��]����
	float rad;
	////VGet(0, -1, 0)���N�_�Ƃ�����]�p���v�Z
	//VECTOR vOrigin = VGet(0, -1, 0);

	// VGet(0, 1, 0)���N�_�Ƃ�����]�p���v�Z
	VECTOR vOrigin = VGet(0, 1, 0);
	float dot = VDot(vOrigin, _direction);
	float cos = dot / (VSize(vOrigin) * VSize(_direction));
	rad = acos(cos);

	//�N�_���獶�ɉ�]����ꍇ
	if (_direction.x < 0) {
		rad *= -1;
	}
	MV1SetRotationXYZ(_shieldModelHandle, VGet(_defaultShieldRot.x, 0, rad));
}

// �`��ʒu�̕␳����
void EnemyShield::AdjustDrawPos(MATRIX mView)
{
	CharaBase::AdjustDrawPos(mView);

	_shieldScreenPos = _shieldCenterPos;
	_shieldScreenPos = VTransform(_shieldScreenPos, mView);
	// �X�N���[�����W�����[���h���W�ɕϊ�
	_shieldWorldPos = ConvScreenPosToWorldPos(_shieldScreenPos);
	_shieldWorldPos.z = 0; // z���\���ʒu�̒���
	// ���f���\�����W�̔��f
	MV1SetPosition(_shieldModelHandle, _shieldWorldPos);

}

// �`�揈��
void EnemyShield::Draw()
{
	if (_use == 1) {
		EnemyBase::Draw();
	}
}

// �O�ʃ��C���[�ɕ`��
void EnemyShield::DrawFrontLayer(MATRIX mView)
{
	if (_use == 1) {
		// ���̕`��
		MV1DrawModel(_shieldModelHandle);
	}
}


// �f�o�b�O�p
// �J�v�Z���̕`��
void MyDrawCapsule(
	float cx, float cy, float r, // �J�v�Z�� �[�̈ʒu(x,y), �J�v�Z���̉����i�~�̔��a�jr
	VECTOR direction,						 // �J�v�Z����L�΂�����
	float h,										 // �J�v�Z���̏c�̒���
	int color, 
	int fillFrag
) {

	float x = cx;
	float y = cy;
	float length = r * 2;

	// �J�v�Z���̊J�n�ʒu�̌v�Z
	// 1�ڂ̉~�̈ʒu��cx,cy���甼�~�����炷
	VECTOR vDir = VNorm(direction);
	VECTOR vTmp = VScale(vDir, r);

	x += vTmp.x;
	y += vTmp.y;

	// ���炷�����v�Z
	MATRIX m = MGetScale(VGet(r / 4, r / 4, 0)); // ���鐔���͔C��(3���傫���Ȃ��ƃJ�v�Z���̌`��ɂȂ�Ȃ�)
	vTmp = VTransform(vDir, m);

	while (length < h) {
		DrawCircle(x, y, r, color, fillFrag);

		x += vTmp.x;
		y += vTmp.y;
		length += VSize(vTmp);
	}

	length -= length - h;
	VECTOR v = VGet(x, y, 0);
	v = VSub(v, VScale(vDir, length - h));
	DrawCircle(v.x, v.y, r, color, fillFrag);
}


// �f�o�b�O���̕\��
void EnemyShield::DrawDebugInfo(MATRIX mView, int fillFrag)
{
	if (_use == 1) {
		EnemyBase::DrawDebugInfo(mView, fillFrag);

		VECTOR rectPos;

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 140.f);

		// ���̓����蔻��
		VECTOR vShieldPos = _shieldCenterPos;
		VECTOR sDir = _shieldDirection;
		vShieldPos = VTransform(vShieldPos, mView);		

		DrawCircle(vShieldPos.x, vShieldPos.y, shieldInfo[_index].r, COLOR_RED, fillFrag);
		MyDrawCapsule(vShieldPos.x, vShieldPos.y, shieldInfo[_index].r, sDir, shieldInfo[_index].h, COLOR_RED, fillFrag);
		sDir = VScale(sDir, -1);
		MyDrawCapsule(vShieldPos.x, vShieldPos.y, shieldInfo[_index].r, sDir, shieldInfo[_index].h, COLOR_RED, fillFrag);


		//VECTOR vShieldPos = _shieldCenterPos;
		//vShieldPos = VTransform(vShieldPos, mView);
		//DrawCircle(vShieldPos.x, vShieldPos.y, 32, COLOR_RED, TRUE);



		//VECTOR vShieldPos2 = VAdd(vShieldPos, VScale(_shieldDirection, 96 - 32));
		//DrawCircle(vShieldPos2.x, vShieldPos2.y, 32, COLOR_RED, TRUE);

		//VECTOR vShieldPos3 = VAdd(vShieldPos, VScale(_shieldDirection, -(96 -32)));
		//DrawCircle(vShieldPos3.x, vShieldPos3.y, 32, COLOR_RED, TRUE);

		/*VECTOR v = VSub(vShieldPos2, vShieldPos3);
		
		float s = VSize(v);
		s = s + 32 + 32;*/

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		/*VECTOR shieldRot = MV1GetRotationXYZ(_shieldModelHandle);
		DrawFormatString(SCREEN_W / 2, SCREEN_H / 2, COLOR_RED, "shieldRot : x %3.2f, y %3.2f, z %3.2f", shieldRot.x, shieldRot.y, shieldRot.z);
		DrawFormatString(SCREEN_W / 2, SCREEN_H / 2 + 16, COLOR_RED, "shieldDir : x %3.2f, y %3.2f, z %3.2f", _shieldDirection.x, _shieldDirection.y, _shieldDirection.z);
	*/
	}

}



//
// �\���́uSHIELD�v�ɑ΂��鏈��
// 

// ����������������i�S�ă��Z�b�g����j
void InitShieldInfo()
{
	for (int i = 0; i < ENEMY_SHIELD_MAX; i++) {
		shieldInfo[i].use = 0;

		shieldInfo[i].r = 0;
		shieldInfo[i].h = 0;

		shieldInfo[i].centerPos = VGet(0, 0, 0);
		shieldInfo[i].direction = VGet(0, 0, 0);
	}
}

// �������Z�b�g����
void SetShieldInfo(int index, float r, int h) {
	shieldInfo[index].r = r;
	shieldInfo[index].h = h;
}

// �������X�V����
void UpdateShieldInfo(int index, int use, VECTOR pos, VECTOR dir) {
	shieldInfo[index].use = use;
	if (use == 1) {
		shieldInfo[index].centerPos = pos;
		shieldInfo[index].direction = dir;
	}
}

// �������擾����
SHIELD GetShieldInfo(int index) {
	return shieldInfo[index];
}


// �\���́uTEXTURE�v�̏�����
void EnemyShield::TextureInit(int modelHandle)
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
void EnemyShield::ChangeTexture(int isDamaged)
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
void EnemyShield::MaterialInit(int modelHandle)
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
void EnemyShield::ChangeMaterialDifColor(int isDamaged)
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



