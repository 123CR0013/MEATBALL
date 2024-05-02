#include "enemy_shooting.h"
#include "enemy_data.h"
#include "resource.h"
#include "mydraw.h"
#include "effect.h"

// �e�������Ă��玟�̒e�����܂ł̊Ԋu
#define BULLET_COOLTIME_MAX 2 * 60

// �e�N�X�`�����
std::vector<TEXTURE>	EnemyShooting::_texture;
// �}�e���A�����
std::vector<MATERIAL>	EnemyShooting::_material;

// �v���C���[�Ɍ������Ēe�����G
EnemyShooting::EnemyShooting(int modelHandle, int size, int index, VECTOR startPos, VECTOR direction) : EnemyBase(modelHandle, index, startPos, direction)
{
	_type = ENEMY_TYPE_SHOOTING;
	_index = index;


	// �G�̋����������ݒ肷��
	SetStregth(_type, size);

	_pos.x = _pos.x * CHIPSIZE_W + _difBodyRect.w / 2;
	_pos.y = _pos.y * CHIPSIZE_W + _difBodyRect.h / 2;


	/*_pos.x = _pos.x * CHIPSIZE_W + 64;
	_pos.y = _pos.y * CHIPSIZE_W + 128;*/
	

	_speed = 0;

	_screenPos = _pos;
	UpdateWorldPos();
	MV1SetPosition(_modelHandle, _worldPos);
	

	_difCenterY = 0;
	// �̂̒��S�ʒu���X�V
	UpdateCenterPos();
	// �����蔻���`���X�V
	SetCollision();



	for (int i = 0; i < BULLET_MAX; i++) {
		bullet[i].use = 0;
	}
	_bulletSpeed = 7;
	_bulletCoolTime = 27;

	_bulletScreenPos = VGet(-1, -1, -1);



	// ������Ԃ̃��[�V�����̐ݒ�
	_motionNum = SHOOTING_MOTION_NORMAL;

	// ���[�V�����̏���������
	MotionInit(SHOOTING_MOTION_MAX);



	_eyeEmiColorFade = 0.0f;

}

EnemyShooting::~EnemyShooting()
{
	//CharaBase::~CharaBase();
}


// �v�Z����
void EnemyShooting::Process(VECTOR playerPos) {
	// ���̓G���|����Ă��Ȃ�������
	if (_use == 1) {
		int motionNum_old = _motionNum;

		// ���ʏ���
		// �q�b�g�X�g�b�v����
		EnemyBase::Process(playerPos);

		// �q�b�g�X�g�b�v���łȂ����
		if (_isHitStop == 0) {
			// ��ʓ��ɂ�����
			if (_isInScreen == 1) {
				// ���G��ԂȂ�
				if (_isSpotPlayer == 1) {
					// �e����������̃N�[���^�C�����I����Ă�����
					if (_bulletCoolTime <= 0) {
						// �e��������������
						ShotBullet(playerPos);
					}
					else if(_bulletCoolTime == 27){
						_eyeEmiColorFade = 1.0f;


						// �`���[�W�G�t�F�N�g�̃Z�b�g
						int effectX = _bodyRect.x - _bodyRect.w * 1.5;
						int effectY = _bodyRect.y - _bodyRect.h * 1.5;
						int effectSize = _bodyRect.w * 4;
						SetEffect(EFFECT_TYPE_BULLET_CHARGE, effectX, effectY, effectSize, effectSize);
					}
				}
			}
			else {
				// ��ʔ����ɏo����A����G��Ԃɂ���
				SetIsSpotPlayer(0);
			}

			// ���[�V�����̐ݒ�
			if (_isSpotPlayer == 1) {
				_motionNum = SHOOTING_MOTION_ENCOUNT;
			}
			else {
				_motionNum = SHOOTING_MOTION_NORMAL;
			}
		}

		// �ڂ̃p�[�c�����点��
		MV1SetMaterialEmiColor(_modelHandle, 8, GetColorF(_eyeEmiColorFade, _eyeEmiColorFade, _eyeEmiColorFade, 1.0f));
		_eyeEmiColorFade -= 0.1f;
		if (_eyeEmiColorFade < 0) {
			_eyeEmiColorFade = 0.0f;
		}

		// ���[�V�����̌v�Z����
		MotionProcess(motionNum_old);
	}


	// �L�����N�^�[�̉�]����
	VECTOR vDir = VSub(playerPos, _centerPos);
	_direction = VNorm(vDir);
	float rad;
	if (VSize(_direction) != 0) {
		////VGet(0, -1, 0)���N�_�Ƃ�����]�p���v�Z
		//VECTOR vOrigin = VGet(0, -1, 0);

		// VGet(0, 1, 0)���N�_�Ƃ�����]�p���v�Z
		VECTOR vOrigin = VGet(0, 1, 0);
		float dot = VDot(vOrigin, _direction);
		float cos = dot / (VSize(vOrigin) * VSize(_direction));
		rad = acos(cos);

		//�N�_���獶�ɉ�]����ꍇ
		if (_direction.x > 0) {
			rad *= -1;
		}
	}
	//��]�����̔��f
	MV1SetRotationXYZ(_modelHandle, VGet(0.0f, rad, 0.0f));

	// �e�̌v�Z����
	BulletProcess();


	// �N�[���^�C�����̏���
	if (_bulletCoolTime > 0) {
		if (_isSpotPlayer == 1 || _isAttack == 1) {
			_bulletCoolTime--;
		}
	}
}

// �e��������������
void EnemyShooting::ShotBullet(VECTOR playerPos)
{
	for (int i = 0; i < BULLET_MAX; i++) {
		if (bullet[i].use == 0) {
			_isAttack = 1;
			PlaySoundMem(se[2], DX_PLAYTYPE_BACK);
			bullet[i].use = 1;

			bullet[i].pos = _pos;

			// �v���C���[����������ɒe�����������i�ړ������͓r���ŕύX���Ȃ��j
			VECTOR vTmp = VSub(playerPos, bullet[i].pos);
			vTmp = VNorm(vTmp);
			bullet[i].direction = vTmp;

			_bulletCoolTime = BULLET_COOLTIME_MAX;

			bullet[i].animCnt = 0;
			break;
		}
	}
}

// �e�̌v�Z����
void EnemyShooting::BulletProcess()
{
	for (int i = 0; i < BULLET_MAX; i++) {
		if (_isAttack == 0) {
			bullet[i].use = 0;
			_attackRect.x = -1;
		}
		if (bullet[i].use == 1) {
			// �ړ�����
			VECTOR vDir = VNorm(bullet[i].direction);
			VECTOR move = VScale(vDir, _bulletSpeed);
			bullet[i].pos.x += move.x;
			bullet[i].pos.y += move.y;

			// �e�̓����蔻��
			MYRECT rc = { 0, 0, 0, 0 };
			rc.x = bullet[i].pos.x + _difAttackRect.x;
			rc.y = bullet[i].pos.y + _difAttackRect.y;
			rc.w = _difAttackRect.w;
			rc.h = _difAttackRect.h;
			bullet[i].rect = rc;

			_attackRect = bullet[i].rect;

			// �}�b�v�`�b�v�Ƃ̓����蔻��
			HITCHIP hitChip = gMap.IsHitBoxAndMapChip(bullet[i].rect);
			if (hitChip.chip_no != -1) {
				bullet[i].use = 0;

				// �e�j��G�t�F�N�g�̃Z�b�g
				SetBulletHitEffect();


				_attackRect.x = -1;
			}



			// ��ʊO�ɂł���e������
			if (
				(bullet[i].rect.x + bullet[i].rect.w < gCam._pos.x - SCREEN_W / 2) ||
				(bullet[i].rect.x > gCam._pos.x + SCREEN_W / 2) ||
				(bullet[i].rect.y + bullet[i].rect.h < gCam._pos.y - SCREEN_H / 2) ||
				(bullet[i].rect.y > gCam._pos.y + SCREEN_H / 2)
				) {
				bullet[i].use = 0;

				// �e�j��G�t�F�N�g�̃Z�b�g
				SetBulletHitEffect();

				_attackRect.x = -1;
			}

			bullet[i].animCnt++;
		}
	}
}

// �e�j��G�t�F�N�g�̃Z�b�g
void EnemyShooting::SetBulletHitEffect()
{
	VECTOR centerPos = VGet(_attackRect.x, _attackRect.y, 0);
	centerPos.x += _attackRect.w / 2;
	centerPos.y += _attackRect.h / 2;
	float effectSize = _attackRect.w * 3 * 1.5f;
	SetEffect(EFFECT_TYPE_BULLET_HIT, centerPos.x - effectSize / 2, centerPos.y - effectSize / 2, effectSize, effectSize);
}


// �`��ʒu�̕␳����
void EnemyShooting::AdjustDrawPos(MATRIX mView)
{
	CharaBase::AdjustDrawPos(mView);

	_bulletScreenPos = VGet(_attackRect.x, _attackRect.y, 0);
	//_bulletScreenPos = VTransform(_bulletScreenPos, mView);
}

// �`�揈��
void EnemyShooting::Draw()
{
	EnemyBase::Draw();
}

// �e�̕`�揈��
void EnemyShooting::DrawBullet(MATRIX mView)
{
	for (int i = 0; i < BULLET_MAX; i++) {
		if (bullet[i].use == 1) {
			//DrawGraph(_bulletScreenPos.x, _bulletScreenPos.y, cgBullet, TRUE);
			//DrawExtendGraph(_bulletScreenPos.x, _bulletScreenPos.y, _bulletScreenPos.x + 64, _bulletScreenPos.y + 64, cgBullet, TRUE);

			VECTOR bPos = _bulletScreenPos;
			bPos.x += 64 / 2;
			bPos.y += 64 / 2;
			MyDrawModiGraph(mView, bPos, 1.0f, 0.0f, 64 *1.5f, 64 * 1.5f, cgBullet[(bullet[i].animCnt / 1) % 30]);
		}
	}}


// �O�ʃ��C���[�ɕ`��
// �e�̕`��
void EnemyShooting::DrawFrontLayer(MATRIX mView)
{
	DrawBullet(mView);
}







// �\���́uTEXTURE�v�̏�����
void EnemyShooting::TextureInit(int modelHandle)
{
	// ������
	_texture.clear();

	// �e�N�X�`���̍��v�����擾����
	int n = MV1GetTextureNum(modelHandle);

	for (int i = 0; i < n; i++) {
		// �e�N�X�`���摜�̃n���h�����\����TEXTURE�ɕێ�����
		TEXTURE t = { i, MV1GetTextureGraphHandle(modelHandle, i)};
		_texture.push_back(t);
	}
}

// �e�N�X�`���摜�̓\��ւ�
// �_���[�W�󂯂��Ƃ�, ���t���[�������^�����ȉ摜��\��
// ���̌�, ���ɖ߂�
// ����isDamaged �ɂ���ē\��e�N�X�`����ύX����
// 0 : �ʏ펞, 1 : ��_���[�W��
void EnemyShooting::ChangeTexture(int isDamaged)
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
void EnemyShooting::MaterialInit(int modelHandle)
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
void EnemyShooting::ChangeMaterialDifColor(int isDamaged)
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

// �f�o�b�O���̕\��
void EnemyShooting::DrawDebugInfo(MATRIX mView, int fillFrag)
{
	if (_use == 1) {
		EnemyBase::DrawDebugInfo(mView, fillFrag);
		VECTOR rectPos;

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 140.f);
		/*DrawFormatStringToHandle(rectPos.x, rectPos.y, COLOR_WHITE, fontHandle, "%d", 2 - _type);
		DrawFormatString(rectPos.x, rectPos.y, COLOR_WHITE, "dir: x %3.2f, y %3.2f", _direction.x, _direction.y);*/

		//// ���G�͈�
		//rectPos = VGet(_searchRect.x, _searchRect.y, 0);
		//rectPos = VTransform(rectPos, mView);
		//DrawBox(rectPos.x, rectPos.y, rectPos.x + _searchRect.w, rectPos.y + _searchRect.h, COLOR_GREEN, FALSE);

		

		////// �G�̓����蔻��
		//rectPos = VGet(_bodyRect.x, _bodyRect.y, 0);
		//rectPos = VTransform(rectPos, mView);
		//DrawBox(rectPos.x, rectPos.y, rectPos.x + _bodyRect.w, rectPos.y + _bodyRect.h, COLOR_BLUE, FALSE);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		//DrawFormatString(rectPos.x, rectPos.y, COLOR_WHITE, "%d", _bulletCoolTime);
	}
	// �e�̓����蔻��
	for (int i = 0; i < BULLET_MAX; i++) {
		if (bullet[i].use == 1) {
			VECTOR rectPos;
			rectPos = VGet(bullet[i].rect.x, bullet[i].rect.y, 0);
			rectPos = VTransform(rectPos, mView);
			DrawBox(rectPos.x, rectPos.y, rectPos.x + bullet[i].rect.w, rectPos.y + bullet[i].rect.h, COLOR_RED, fillFrag);
		}
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//DrawFormatString(SCREEN_H / 2, 0, COLOR_WHITE, "bullet use: x %d", bullet[0].use);
	//DrawFormatString(SCREEN_H / 2, 16, COLOR_WHITE, "bullet pos: x %3.2f, y: %3.2f", bullet[0].pos.x, bullet[0].pos.y);
	//DrawFormatString(SCREEN_H / 2, 32, COLOR_WHITE, "cam pos: x %3.2f, y: %3.2f", gCam._pos.x, gCam._pos.y + SCREEN_H / 2);

}