#include "enemy_float.h"
#include "mydraw.h"

// �e�N�X�`�����
std::vector<TEXTURE>	EnemyFloat::_texture;
// �}�e���A�����
std::vector<MATERIAL>	EnemyFloat::_material;

// ���܂������[�g�����񂷂�G
EnemyFloat::EnemyFloat(int modelHandle, int stageNo, int size, int index, VECTOR startPos, VECTOR direction) : EnemyBase(modelHandle, index, startPos, direction)
{
	_type = ENEMY_TYPE_FLOAT;
	_index = index;


	// �G�̋����������ݒ肷��
	SetStregth(_type, size);
	
	_pos.x = _pos.x * CHIPSIZE_W + _difBodyRect.w / 2;
	_pos.y = _pos.y * CHIPSIZE_W + _difBodyRect.h / 2;


	/*_pos.x = _pos.x * CHIPSIZE_W + 32;
	_pos.y = _pos.y * CHIPSIZE_W + 64;*/


	_direction = direction;


	// �ړ����[�g�����擾���ĕێ�����
	for (int i = 0; i < 10; i++) {
		route[i] = GetFloatRoute(stageNo, _index, i);
	}

	


	// ������Ԃ̃��[�V�����̐ݒ�
	_motionNum = FLOAT_MOTION_NORMAL;

	// ���[�V�����̏���������
	MotionInit(FLOAT_MOTION_MAX);

	_speed = 3;

	_screenPos = _pos;
	UpdateWorldPos();
	MV1SetPosition(_modelHandle, _worldPos);

	_difCenterY = 32;

	// �̂̒��S�ʒu���X�V
	UpdateCenterPos();
	// �����蔻���`���X�V
	SetCollision();

}

EnemyFloat::~EnemyFloat()
{
	//CharaBase::~CharaBase();
}

// �v�Z����
void EnemyFloat::Process(VECTOR playerPos) {
	// ���̓G���|����Ă��Ȃ�������
	if (_use == 1) {
		int motionNum_old = _motionNum;

		// ���ʏ���
		// �q�b�g�X�g�b�v����
		EnemyBase::Process(playerPos);

		// �q�b�g�X�g�b�v���łȂ����
		if (_isHitStop == 0) {

			// �ړ������̍X�V�����邩�𒲂ׂ�
			int i = 0;
			while (route[i].pos.x != -1)
			{
				int x = _centerPos.x / CHIPSIZE_W;
				int y = _centerPos.y / CHIPSIZE_H;

				// ���݂̃}�b�v�`�b�v���W�ƈړ����[�g���̃}�b�v�`�b�v���W����v���Ă�����A�ړ��������X�V����
				if (x == route[i].pos.x && y == route[i].pos.y) {
					_direction = route[i].direction;
					break;
				}
				i++;
			}

			// �ړ�����
			VECTOR vDir = _direction;
			VECTOR move = VScale(vDir, _speed);
			_pos.x += move.x;
			_pos.y += move.y;

			// �̂̒��S�ʒu���X�V
			UpdateCenterPos();
			// �����蔻���`���X�V
			SetCollision();
		}

		// ���[�V�����̌v�Z����
		MotionProcess(motionNum_old);
	}
}

// �`�揈��
void EnemyFloat::Draw()
{
}

// �O�ʃ��C���[�ւ̕`�揈��
void EnemyFloat::DrawFrontLayer(MATRIX mView)
{
	// �G�{�̂̕`��
	if (_use == 1) {
		MV1DrawModel(_modelHandle);
	}
}


// �\���́uTEXTURE�v�̏�����
void EnemyFloat::TextureInit(int modelHandle)
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
void EnemyFloat::ChangeTexture(int isDamaged)
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
void EnemyFloat::MaterialInit(int modelHandle)
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
// 0 : �ʏ펞, 1 : ��_���[�W��
void EnemyFloat::ChangeMaterialDifColor(int isDamaged)
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
void EnemyFloat::DrawDebugInfo(MATRIX mView, int fillFrag)
{
	// 
	// ���񃋁[�g�\��
	// 
	for (int i = 0; i < ROUTE_MAX; i++) {
		if (route[i].pos.x == -1) break;
		VECTOR world = VGet(route[i].pos.x * CHIPSIZE_W, route[i].pos.y * CHIPSIZE_H, 0);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 64);		// �������`��w��
		MyDrawBox(mView, world.x, world.y, world.x + CHIPSIZE_W, world.y + CHIPSIZE_H, COLOR_RED, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		// �s�����`��w��

		world = VTransform(world, mView);


		if (route[i].direction.x == 1) {
			DrawFormatString(world.x, world.y, COLOR_WHITE, "��");
		}
		else if (route[i].direction.x == -1) {
			DrawFormatString(world.x, world.y, COLOR_WHITE, "��");

		}
		if (route[i].direction.y == 1) {
			DrawFormatString(world.x, world.y, COLOR_WHITE, "��");
		}
		else if (route[i].direction.y == -1) {
			DrawFormatString(world.x, world.y, COLOR_WHITE, "��");
		}
	}



	if (_use == 1) {
		EnemyBase::DrawDebugInfo(mView, fillFrag);
	}
}



