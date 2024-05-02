#include "gimmick.h"
#include "gamemain.h"

Gimmick::Gimmick()
{
	_index = -1;
	_use = 0;


	_modelHandle = -1;
	_motionNum = -1;
	_playTime = 0.0f;


	_rotation = 0;

	_pos = VGet(0, 0, 0);
	_screenPos = VGet(0, 0, 0);
	_worldPos = VGet(0, 0, 0);

	_difRect = { 0,0,0,0 };
	_rect = { -1,-1,-1,-1 };
}

Gimmick::~Gimmick()
{
	if (_modelHandle != -1) {
		MV1DeleteModel(_modelHandle);
	}
}


void Gimmick::ModelInit(int modelHandle, VECTOR size, int motionMax, int stageNo)
{
	// �R�c���f���̓ǂݍ���
	_modelHandle = MV1DuplicateModel(modelHandle);
	MV1SetScale(_modelHandle, size);

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

	float rad = DegToRad(90 * _rotation);
	MV1SetRotationXYZ(_modelHandle, VGet(0, 0, rad));
}

void Gimmick::MotionProcess(int motionNum_old)
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




void Gimmick::SetCollision()
{
	MYRECT rc = { 0, 0, 0, 0 };
	MYRECT h = { 0, 0, 0, 0 };
	VECTOR tmpPos = { 0, 0, 0 };


	h = _difRect;
	tmpPos = _pos;


	rc.x = tmpPos.x + h.x;
	rc.y = tmpPos.y + h.y;
	rc.w = h.w;
	rc.h = h.h;

	_rect = rc;
}

void Gimmick::DrawDebugInfo(MATRIX mView)
{
	VECTOR rectPos;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 140.f);




	// �����蔻���`
	rectPos = VGet(_rect.x, _rect.y, 0);
	rectPos = VTransform(rectPos, mView);
	DrawBox(rectPos.x, rectPos.y, rectPos.x + _rect.w, rectPos.y + _rect.h, COLOR_BLUE, TRUE);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}


// �`��ʒu�̕␳����
void Gimmick::AdjustDrawPos(MATRIX mView)
{
	_screenPos = _pos;
	_screenPos = VTransform(_screenPos, mView);
	UpdateWorldPos();
	// �ړ������̔��f
	MV1SetPosition(_modelHandle, _worldPos);
}

void Gimmick::Draw()
{
	if (_use == 1) {
		//// �R�c���f���̕`��
		MV1DrawModel(_modelHandle);
	}
}

void Gimmick::UpdateWorldPos()
{
	// �X�N���[�����W�����[���h���W�ɕϊ�
	_worldPos = ConvScreenPosToWorldPos(_screenPos);
	_worldPos.z = 400; // z���\���ʒu�̒���
}
