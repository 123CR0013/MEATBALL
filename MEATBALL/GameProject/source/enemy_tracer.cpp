#include "enemy_tracer.h"
#include "enemy_data.h"
#include "resource.h"

// �e�N�X�`�����
std::vector<TEXTURE>	EnemyTracer::_texture;
// �}�e���A�����
std::vector<MATERIAL>	EnemyTracer::_material;

// �v���C���[��ǂ������Ă���G
EnemyTracer::EnemyTracer(int modelHandle, int size, int index, VECTOR startPos, VECTOR direction) : EnemyBase(modelHandle, index, startPos, direction)
{
	_type = ENEMY_TYPE_TRACER;
	_index = index;

	// �G�̋����������ݒ肷��
	SetStregth(_type, size);


	_pos.x = _pos.x * CHIPSIZE_W + _difBodyRect.w / 2;
	_pos.y = _pos.y * CHIPSIZE_W + _difBodyRect.h / 2;



	/*_pos.x = _pos.x * CHIPSIZE_W + 32;
	_pos.y = _pos.y * CHIPSIZE_W + 64;*/


	// ������Ԃ̃��[�V�����̐ݒ�
	_motionNum = TRACER_MOTION_NORMAL;

	// ���[�V�����̏���������
	MotionInit(TRACER_MOTION_MAX);

	_speed = 2;

	_screenPos = _pos;
	// ���[���h���W���X�V����
	UpdateWorldPos();
	// 3D���f���̍��W���X�V����
	MV1SetPosition(_modelHandle, _worldPos);

	_difCenterY = 32;

	// �̂̒��S�ʒu���X�V
	UpdateCenterPos();
	// �����蔻���`���X�V
	SetCollision();

	_canPlaySe = 1;
}

EnemyTracer::~EnemyTracer()
{
	//CharaBase::~CharaBase();
}

// �v�Z����
void EnemyTracer::Process(VECTOR playerPos) {
	// ���̓G���|����Ă��Ȃ�������
	if (_use == 1) {
		int motionNum_old = _motionNum;

		// ���ʏ���
		// �q�b�g�X�g�b�v����
		EnemyBase::Process(playerPos);

		// �q�b�g�X�g�b�v���Ŗ������
		if (_isHitStop == 0) {
			// ��ʓ��ɂ�����
			if (_isInScreen == 1) {
				// ����G��ԂȂ�
				if (_isSpotPlayer == 1) {

					// �@�B��SE�̍Đ�
					// ���G��Ԃ�1�x�����Đ�����
					// ��ʊO�ɏo����ĂэĐ��\�ɂ���
					if (_canPlaySe == 1){
						PlaySoundMem(se[21], DX_PLAYTYPE_BACK);
						// SE���Đ��s�\�ɂ���
						_canPlaySe = 0;
					}

					// �ړ������̌v�Z
					VECTOR vDir = VSub(playerPos, _centerPos);
					_direction = VNorm(vDir);
					VECTOR vMove = VScale(_direction, _speed);

					//
					// x���̈ړ�
					//
					_pos.x += vMove.x;

					// �̂̒��S�ʒu���X�V
					UpdateCenterPos();
					// �����蔻���`���X�V
					SetCollision();

					// �}�b�v�`�b�v�Ƃ̓����蔻��
					HITCHIP hitChip = gMap.IsHitBoxAndMapChip(_bodyRect);
					// �}�b�v�`�b�v�ɓ������Ă�����
					if (hitChip.chip_no != -1) {
						float w;
						if (vMove.x < 0) {
							w = (hitChip.x * CHIPSIZE_W + CHIPSIZE_W) - _bodyRect.x;
						}
						else if (vMove.x > 0) {
							w = hitChip.x * CHIPSIZE_W - (_bodyRect.x + _bodyRect.w);
							w--;
						}
						// �����߂�����
						_pos.x += w;

						// �̂̒��S�ʒu���X�V
						UpdateCenterPos();
						// �����蔻���`���X�V
						SetCollision();
					}

					//
					// y���̈ړ�
					//
					_pos.y += vMove.y;

					// �̂̒��S�ʒu���X�V
					UpdateCenterPos();
					// �����蔻���`���X�V
					SetCollision();

					// �}�b�v�`�b�v�Ƃ̓����蔻��
					hitChip = gMap.IsHitBoxAndMapChip(_bodyRect);
					// �}�b�v�`�b�v�ɓ������Ă�����
					if (hitChip.chip_no != -1) {
						float h;
						// ���Ɉړ����Ă���Ƃ�
						if (vMove.y > 0) {
							h = hitChip.y * CHIPSIZE_H - (_bodyRect.y + _bodyRect.h);
							_pos.y--;
						}
						// ��Ɉړ����Ă���Ƃ�
						else {
							h = (hitChip.y * CHIPSIZE_H + CHIPSIZE_H) - _bodyRect.y;
						}
						// �����߂�����
						_pos.y += h;

						// �̂̒��S�ʒu���X�V
						UpdateCenterPos();
						// �����蔻���`���X�V
						SetCollision();
					}
					_motionNum = TRACER_MOTION_ENCOUNT;
				}
				else {
					_motionNum = TRACER_MOTION_NORMAL;
				}
			}
			else {
				// SE���Đ��\�ɂ���
				_canPlaySe = 1;
				// ����G��Ԃɂ���
				SetIsSpotPlayer(0);
			}

		}

		// ���[�V�����̌v�Z����
		MotionProcess(motionNum_old);
	}
}


// �\���́uTEXTURE�v�̏�����
void EnemyTracer::TextureInit(int modelHandle)
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
void EnemyTracer::ChangeTexture(int isDamaged)
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
void EnemyTracer::MaterialInit(int modelHandle)
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
void EnemyTracer::ChangeMaterialDifColor(int isDamaged)
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

//void EnemyTracer::DrawDebugInfo(MATRIX mView)
//{
//	if (_use == 1) {
//		VECTOR rectPos;
//
//		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 140.f);
//
//		// ���G�͈�
//		rectPos = VGet(_searchRect.x, _searchRect.y, 0);
//		rectPos = VTransform(rectPos, mView);
//		DrawBox(rectPos.x, rectPos.y, rectPos.x + _searchRect.w, rectPos.y + _searchRect.h, COLOR_GREEN, TRUE);
//		
//
//		// �G�̓����蔻��
//		rectPos = VGet(_bodyRect.x, _bodyRect.y, 0);
//		rectPos = VTransform(rectPos, mView);
//		DrawBox(rectPos.x, rectPos.y, rectPos.x + _bodyRect.w, rectPos.y + _bodyRect.h, COLOR_BLUE, TRUE);
//
//		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
//
//
//
//		DrawFormatString(rectPos.x, rectPos.y, COLOR_WHITE, "dir: x %3.2f, y %3.2f", _direction.x, _direction.y);
//	}
//}