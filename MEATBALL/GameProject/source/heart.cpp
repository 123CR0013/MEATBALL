#include "heart.h"
#include "scenegamemain.h"
#include "resource.h"
#include "effect.h"


Heart::Heart()
{
	_modelHandle = MV1LoadModel("res/model/heart/heart.mv1");
	danger[0] = LoadGraph("res/UI/noheart_00.png");
	danger[1] = LoadGraph("res/UI/noheart_01.png");
	dangerCnt = 0;

	MV1SetScale(_modelHandle, VGet(2.3f, 2.3f, 2.3f));

	_use = 0;

	_remainCount = HEART_COUNT_MAX;
	_isLost = 0;

	_pos = VGet(0, 0, 0);
	_screenPos = VGet(0, 0, 0);
	_worldPos = VGet(0, 0, 0);

	_direction = 1;

	_gravity = 0;
	_speed = 8;

	_difBodyRect = { -32, -64, 64, 64 };
	_bodyRect = { -1, -1, -1, -1 };
}

Heart::~Heart()
{
	if (_modelHandle != -1) {
		MV1DeleteModel(_modelHandle);
		_modelHandle = -1;
	}

	for (int i = 0; i < 2; i++) {
		if (danger[i] != -1) {
			DeleteGraph(danger[i]);
			danger[i] = -1;
		}
	}
}

// �v�Z����
void Heart::Process()
{
	// �n�[�g����яo���Ă�����
	if (_use == 1) {
		dangerCnt++;
		// ��ʊO�ɏo�Ȃ��悤��
		// x���̔���
		if (_pos.x - _difBodyRect.w / 2 < gCam._pos.x - SCREEN_W / 2) {
			_direction = 1;
		}
		else if (gCam._pos.x + SCREEN_W / 2 < _pos.x + _difBodyRect.w / 2) {
			_direction = -1;
		}


		// y�������̈ړ�����
		_gravity += 1;			// �L�����́A�d�͂ɂ������l��傫������
		_pos.y += _gravity;	// �d�͂ɂ������l�̕������ړ�����



		// �}�b�v�`�b�v�Ƃ̓����蔻��
		// y�������̓����蔻��
		MYRECT tmpRect = _bodyRect;
		tmpRect.y += _gravity;
		HITCHIP hitChip = gMap.IsHitBoxAndMapChip(tmpRect);
		// �������Ă���ꍇ�A�ړ�������ύX����
		if (hitChip.chip_no != -1) {
			float h;
			// ���ɓ������Ă����ꍇ
			if (_gravity > 0) {
				h = hitChip.y * CHIPSIZE_H - (tmpRect.y + tmpRect.h);
				_pos.y--;
				// ���̃t���[�����������Ɉړ�������
				_gravity = -16;
			}
			// �V��ɓ������Ă����ꍇ
			else {
				h = tmpRect.y - hitChip.y * CHIPSIZE_H;
				_gravity = 0;
			}
			_pos.y += h;			
		}

		// �����蔻���`�̍X�V
		UpdateBodyRect();


		// x�������̈ړ�����
		float moveX = 5 * _direction;
		_pos.x += moveX;
		// �}�b�v�`�b�v�Ƃ̓����蔻��
		// x�������̓����蔻��
		tmpRect = _bodyRect;
		tmpRect.x += moveX;
		hitChip = gMap.IsHitBoxAndMapChip(tmpRect);
		if (hitChip.chip_no != -1) {
			// �ړ�������������
			_pos.x -= moveX;

			// �ړ������𔽓]������
			_direction *= -1;
		}
		// �����蔻���`�̍X�V
		UpdateBodyRect();





		_remainCount--;
		// �����܂ł̎c��J�E���g��0�ȉ��ɂȂ�����
		if (_remainCount <= 0) {
			SetUse(0);
			_isLost = 1;

			// �n�[�g�j��G�t�F�N�g�̃Z�b�g�i�����G���j�G�t�F�N�g�Ɠ������̂��g�p����j
			VECTOR effectPos = VGet(_bodyRect.x + _bodyRect.w / 2, _bodyRect.y + _bodyRect.h / 2, 0);
			float effectSize = _bodyRect.w * 3;
			SetEffect(EFFECT_TYPE_DEFEAT_CREATURE, effectPos.x - effectSize / 2, effectPos.y - effectSize / 2, effectSize, effectSize);

			// �n�[�g�j��SE�̍Đ��i�����G���jSE�������̂��g�p����j
			PlaySoundMem(se[7], DX_PLAYTYPE_BACK);
		}
	}	
}

void Heart::SetPos(VECTOR playerPos)
{
	_pos = playerPos;

	UpdateBodyRect();
}

void Heart::UpdateBodyRect() {
	MYRECT rc = { 0, 0, 0, 0 };
	MYRECT h = _difBodyRect;
	VECTOR tmpPos = _pos;

	if (_use == 1) {
		rc.x = tmpPos.x + h.x;
		rc.y = tmpPos.y + h.y;
		rc.w = h.w;
		rc.h = h.h;
	}
	_bodyRect = rc;
}


// �`��ʒu�̕␳����
void Heart::AdjustDrawPos(MATRIX mView)
{
	if (_use == 1) {

		_screenPos = _pos;
		_screenPos = VTransform(_screenPos, mView);

		UpdateWorldPos();
		// �ړ������̔��f
		MV1SetPosition(_modelHandle, _worldPos);
	}
}

// ���[���h���W���X�V����
void Heart::UpdateWorldPos()
{
	// �X�N���[�����W�����[���h���W�ɕϊ�
	_worldPos = ConvScreenPosToWorldPos(_screenPos);
	_worldPos.z = 0; // z���\���ʒu�̒���
}

// �`�揈��
void Heart::Draw()
{
	// �n�[�g����яo���Ă���Ƃ�
	if (_use == 1) {
		// �n�[�g�{�̂̕`��
		MV1DrawModel(_modelHandle);
	}
}

// �f�o�b�O���̕\��
void Heart::DrawDebugInfo(MATRIX mView)
{
	if (_use == 1) {
		VECTOR rectPos = VGet(_bodyRect.x, _bodyRect.y, 0);
		rectPos = VTransform(rectPos, mView);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 140.f);
		//DrawBox(_bodyRect.x, _bodyRect.y, _bodyRect.x + _bodyRect.w, _bodyRect.y + _bodyRect.h, COLOR_BLUE, TRUE);
		DrawBox(rectPos.x, rectPos.y, rectPos.x + _bodyRect.w, rectPos.y + _bodyRect.h, COLOR_RED, TRUE);



		//float cnt = float(_count) / 60.f;
		//cnt = ceilf(cnt);

		//DrawFormatStringToHandle(rectPos.x, rectPos.y, COLOR_WHITE, fontHandle, "%2.0f", cnt);



		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

}

// �O�ʃ��C���[�ւ̕`��
void Heart::DrawFrontLayer()
{
	if (_use == 1) {
		DrawGraph(0, 0, danger[0], TRUE);
		if (dangerCnt % 60 == 0) {

			DrawGraph(0, 0, danger[1], TRUE);
		}

	}
	else if (_isLost == 1) {
		DrawGraph(0, 0, danger[0], TRUE);
	}
}

void Heart::SetUse(int n) {
	_use = n;
	// �n�[�g���Ăяo�����ꍇ�̏���
	if (_use == 1) {
		// �ۓ�SE�̍Đ�
		PlaySoundMem(se[15], DX_PLAYTYPE_LOOP);
	}
	// �n�[�g��������Ƃ��̏���
	else {
		// �b���Ƃ��Ĉ����A�����_�ȉ��͐؂�グ
		int sec = _remainCount / 60;
		_remainCount = (sec + 1) * 60;

		// �ۓ�SE�̒�~
		StopSoundMem(se[15]);
	}
}