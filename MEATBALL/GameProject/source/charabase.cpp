#include "charabase.h"
#include <math.h>
#include "mymath.h"

#include "gamepad.h"

// �^�����̃e�N�X�`���摜
int CharaBase::_cgTexWhite = { -1 };

// �L�����N�^�[�̊��N���X
CharaBase::CharaBase(int modelHandle, VECTOR startPos) {
	// �R�c���f���̓ǂݍ��݁i��������j
	// 3D���f���̓ǂݍ��� MV1LoadModel()�͏����Ɏ��Ԃ�������̂ŁA3D���f���̕��� MV1DuplicateModel()���g��
	// ������3D���f���̓ǂݍ��݂͂� scenegamemain.cpp �� SceneGameMainResourceInit() ���ōs��
	_modelHandle = MV1DuplicateModel(modelHandle);



	_use = 1;

	_speed = 0.0f;

	_difBodyRect = { 0,0,0,0 };
	_bodyRect = { -1,-1,-1,-1 };


	_motionNum = -1;
	_playTime = 0.0f;


	_useGravity = 0;
	_gravity = 0;
	_stand = 0;




	// �����ʒu�̐ݒ�	
	_pos = startPos;
	_screenPos = _pos;
	_worldPos = VGet(0, 0, 0);

	_centerPos = VGet(0, 0, 0);
	_difCenterY = 0.0f;
	_direction = VGet(1, 0, 0);

	// 3D���f���\���p���W�̍X�V
	UpdateWorldPos();

	// �����ʒu�Ɉړ�
	MV1SetPosition(_modelHandle, _worldPos);

	_isAttack = 0;

}

CharaBase::~CharaBase() {
	if (_modelHandle != -1) {
		MV1DeleteModel(_modelHandle);
		_modelHandle = -1;
	}
}

// �v�Z����
void CharaBase::Process() {
	// �d�͏����iy���̈ړ������j
	if (_useGravity == 1) {
		_gravity += 1;			// �L�����́A�d�͂ɂ������l��傫������
		_pos.y += _gravity;	// �d�͂ɂ������l�̕������ړ�����

		_stand = 0;			// ���ɓ������ĂȂ����Ƃ�O��ɁA���t���O���Z�b�g

		MYRECT tmpRect = _bodyRect;
		tmpRect.y += _gravity;
		// �}�b�v�`�b�v�Ƃ̓����蔻��
		HITCHIP hitChip = gMap.IsHitBoxAndMapChip(tmpRect);
		// �}�b�v�`�b�v�Ɠ������Ă�����
		if (hitChip.chip_no != -1) {

			float h = 0;

			// �΂߂̃`�b�v�ɓ��������Ƃ��ɕK�v�ȏ��
			// �����������ǂ���
			int isDiagonalHit = 0;
			//�Ζʂ̎n�_�ƏI�_
			VECTOR startPos, endPos;

			// ���������`�b�v�̎�ނ��Ƃɏ������s��
			switch (hitChip.chip_no)
			{

			case -1:
				break;

			default:
				// ���Ɉړ����Ă���Ƃ�
				if (_gravity > 0) {
					h = hitChip.y * CHIPSIZE_H - (tmpRect.y + tmpRect.h);
					_stand = 1;
					_pos.y--;
				}
				// ��Ɉړ����Ă���Ƃ�
				else {
					h = (hitChip.y * CHIPSIZE_H + CHIPSIZE_H) - tmpRect.y;
				}
				_pos.y += h;
				_gravity = 0;
				break;

			
			case LOWER_RIGHT: // �E�� �΂߃`�b�v
				// �������̂ݏ������s���i�������Ȃ��Ǝ΂߂̃}�b�v�`�b�v�ɏ������Ԃł̃W�����v���ł��Ȃ��Ȃ�j
				if (0 < _gravity) {
					isDiagonalHit = 1;
					startPos = VGet(hitChip.x * CHIPSIZE_W, hitChip.y * CHIPSIZE_H + CHIPSIZE_H, 0);
					endPos = VGet(hitChip.x * CHIPSIZE_W + CHIPSIZE_W, hitChip.y * CHIPSIZE_H, 0);

					// 
					//if (_direction.x < 0) {
					_pos.y += CHIPSIZE_H / 2;
					//}
				}
				break;
			case LOWER_LEFT: // ���� �΂߃`�b�v
				// �������̂ݏ������s���i�������Ȃ��Ǝ΂߂̃}�b�v�`�b�v�ɏ������Ԃł̃W�����v���ł��Ȃ��Ȃ�j
				if (0 < _gravity) {
					isDiagonalHit = 1;
					startPos = VGet(hitChip.x * CHIPSIZE_W, hitChip.y * CHIPSIZE_H, 0);
					endPos = VGet(hitChip.x * CHIPSIZE_W + CHIPSIZE_W, hitChip.y * CHIPSIZE_H + CHIPSIZE_H, 0);

					//if (_direction.x < 0) {
					_pos.y += CHIPSIZE_H / 2;
					//}
				}				
				break;
			}

			// �΂߂̃`�b�v�ɓ��������ꍇ
			// �Ζʂ��X���[�Y�Ɉړ�����
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


				_stand = 1;
				_gravity = 0;
			}
		}

		// �̂̒��S�ʒu���X�V
		UpdateCenterPos();
		// �����蔻���`���X�V
		SetCollision();
	}
}



void CharaBase::UpdateCenterPos() {
	_centerPos = _pos;
	_centerPos.y -= _difCenterY;
}


void CharaBase::SetCollision() {
	_bodyRect = SetMyRect(_difBodyRect, _pos);
}

// �����蔻���`�𐶐�����
// ����
// difRect: �L���������蔻��(�z�u���W����̍��� x,y / �傫�� w,h)
// pos : �N�_���W
MYRECT CharaBase::SetMyRect(MYRECT difRect, VECTOR pos) {
	MYRECT rc = { 0, 0, 0, 0 };
	MYRECT h = { 0, 0, 0, 0 };
	VECTOR tmpPos = { 0, 0, 0 };


	h = difRect;
	tmpPos = pos;

	if (_use == 1) {
		rc.x = tmpPos.x + h.x;
		rc.y = tmpPos.y + h.y;
		rc.w = h.w;
		rc.h = h.h;
	}
	return rc;
}

// �����蔻���`���擾����
MYRECT CharaBase::GetMyRect(int collisionType) {
	MYRECT tmpRect = { 0, 0, 0, 0 };
	switch (collisionType)
	{
	case COLLISION_TYPE_BODY:
		tmpRect = _bodyRect;
	}

	return tmpRect;
}

// 3D���f���\���p���W�̍X�V
void CharaBase::UpdateWorldPos()
{
	// �X�N���[�����W�����[���h���W�ɕϊ�
	_worldPos = ConvScreenPosToWorldPos(_screenPos);
	_worldPos.z = -100; // z���\���ʒu�̒���
}

// �`�揈��
void CharaBase::Draw() {
	//// �R�c���f���̕`��
	MV1DrawModel(_modelHandle);
}

// ���[�V�������̏���������
void CharaBase::MotionInit(int motionMax)
{
	// �����̃A�j���[�V�������A�^�b�`���Ă����Ԃ���3D���f���̏�Ԃ��u�����h���ꂽ���̂ɂȂ�
	// �u�����h���������Ȃ��ꍇ��
	// �@1�̃��[�V�����������A�^�b�`����i���̃��[�V�������O���ɂ̓f�^�b�`������j�@��DxLib�ł͂������z�肵�Ă���
	// �A�Đ��������Ȃ����[�V�����̃u�����h���[�g��0�ɂ��� (�g�p�������⏈�����ׂ������㏸����) 
	// MV1SetAttachAnimBlendRate
	
	// �����ł͇@�̕��@���Ƃ�
	// �ȉ��̃��[�v�� totalTime ����邽�߂ɍs��
	for (int i = 0; i < motionMax; i++) {
		//�A�j���[�V�����̃A�^�b�`
		_anim[i].index = MV1AttachAnim(_modelHandle, i, -1, FALSE);
		// �A�^�b�`�����A�j���[�V�����̑��Đ����Ԃ��擾����
		_anim[i].totalTime = MV1GetAttachAnimTotalTime(_modelHandle, _anim[i].index);
		if (i != _motionNum) {
			// �A�j���[�V�����̃f�^�b�`
			MV1DetachAnim(_modelHandle, _anim[i].index);
		}
	}
}

// ���[�V�����̌v�Z����
void CharaBase::MotionProcess(int motionNum_old)
{
	_playTime += 1;
	if (_playTime >= _anim[_motionNum].totalTime) {
		_playTime = 0;
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

// �`��ʒu�̕␳����
void CharaBase::AdjustDrawPos(MATRIX mView)
{
	_screenPos = _pos;
	_screenPos = VTransform(_screenPos, mView);
	UpdateWorldPos();
	// ���f���\�����W�̔��f
	MV1SetPosition(_modelHandle, _worldPos);
}

// 3D���f���̉�]����
void CharaBase::UpdateModelRotation()
{
	// ��]����
	float rad;
	//= old_rad;
	if (VSize(_direction) != 0) {
		////VGet(0, -1, 0)���N�_�Ƃ�����]�p���v�Z
		//VECTOR vOrigin = VGet(0, -1, 0);

		// VGet(0, 1, 0)���N�_�Ƃ�����]�p���v�Z
		VECTOR vOrigin = VGet(0, 1, 0);
		float dot = VDot(vOrigin, VGet(1, 0, 0));
		float cos = dot / (VSize(vOrigin) * VSize(_direction));
		rad = acos(cos);

		//�N�_���獶�ɉ�]����ꍇ
		if (_direction.x > 0) {
			rad *= -1;
		}
	}
	//��]�����̔��f
	MV1SetRotationXYZ(_modelHandle, VGet(0.0f, rad, 0.0f));
}



// �e�N�X�`���摜�̓\��ւ�
// �� �֐����Ōp����̃N���X�̐ÓI�����o�ϐ��ɃA�N�Z�X����̂�, �p���悲�ƂɊ֐���`������
void CharaBase::ChangeTexture(int isDamaged)
{
}

// _modelHandle�̃e�N�X�`���ԍ�index�̃e�N�X�`���摜��graphHandle�̉摜�ɍ����ւ���
void CharaBase::SetTexture(int index, int graphHandle) {
	MV1SetTextureGraphHandle(_modelHandle, index, graphHandle, FALSE);
}

// �}�e���A���̃f�B�t���[�Y�J���[�i�g�U���F�j�̕ύX
// �� �֐����Ōp����̃N���X�̐ÓI�����o�ϐ��ɃA�N�Z�X����̂�, �p���悲�ƂɊ֐���`������
void CharaBase::ChangeMaterialDifColor(int isDamaged)
{
}

// _modelHandle�̃}�e���A���ԍ�index�̃f�B�t���[�Y�J���[�i�g�U���F�j��ύX����
void CharaBase::SetMaterialDifColor(int index, COLOR_F difColor)
{
	MV1SetMaterialDifColor(_modelHandle, index, difColor);
}



// �O�ʃ��C���[�ɕ`��
void CharaBase::DrawFrontLayer(MATRIX mView)
{
}

// �f�o�b�O���̕\��
void CharaBase::DrawDebugInfo(MATRIX mView, int fillFrag)
{
}
