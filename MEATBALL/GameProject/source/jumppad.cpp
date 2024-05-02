#include "jumppad.h"
#include "gimmick_data.h"
#include "resource.h"


int JumpPad::_jumpPadModel = -1;

JumpPad::JumpPad(int stageNo, int index)
{
	_index = index;

	JUMPPAD_DATA jumpPadData  = GetJumpPadData(stageNo, _index);

	if (jumpPadData.id != -1) {
		_use = 1;

		_id = jumpPadData.id;
		_pos = VGet(jumpPadData.x * CHIPSIZE_W + 64, jumpPadData.y * CHIPSIZE_H + 64, 0);

		_isAlwaysActive = jumpPadData.isAlwaysActive;
		_isActive = _isAlwaysActive;


		_remainActiveTime = 0;

		_difRect = { -60, -64, 120, 64 };
		_difBoundRect = { -60, -128, 120, 64 };

		SetCollision();

		_motionNum = 0;
		ModelInit(_jumpPadModel, VGet(5, 5, 5), 1);

		if (_isAlwaysActive == 1) {
			_defaultMotionTime = 7;
		}
		else {
			_defaultMotionTime = 0;
		}
		_playTime = _defaultMotionTime;

		_motionNum = 0;
		_isPlayMotion = 0;
	}
	else {
		_use = 0;
		_id = -1;
		_isAlwaysActive = -1;
		_isActive = 0;
		_remainActiveTime = 0;
		_defaultMotionTime = 0;
		_isPlayMotion = 0;
		_difBoundRect = { 0,0,0,0 };
		_boundRect = { -1,-1,-1,-1 };
	}
}

JumpPad::~JumpPad()
{
	//Gimmick::~Gimmick();
}

void JumpPad::Process()
{
	if (_use == 1) {
		if (_isAlwaysActive != 1) {
			if (_isActive == 1) {
				if (_remainActiveTime > 0) {
					_remainActiveTime--;
					_playTime = 6;
				}
				else {
					_isActive = 0;
				}
			}
		}

		switch (_isPlayMotion)
		{
		case 1:
			_playTime++;
			
			if (_playTime >= _anim[_motionNum].totalTime) {
				_playTime = _anim[_motionNum].totalTime;
				_isPlayMotion = -1;
			}
			break;
		case -1:
			_playTime -= 1;
			if (_playTime <= _defaultMotionTime) {
				_playTime = _defaultMotionTime;
				_isPlayMotion = 0;
			}
			break;
		}

		// 再生時間をセットする(アニメーションの再生)
		MV1SetAttachAnimTime(_modelHandle, _anim[_motionNum].index, _playTime);
	}
}

void JumpPad::SetCollision()
{
	Gimmick::SetCollision();

	MYRECT rc = { 0, 0, 0, 0 };
	MYRECT h = { 0, 0, 0, 0 };
	VECTOR tmpPos = { 0, 0, 0 };


	h = _difBoundRect;
	tmpPos = _pos;


	rc.x = tmpPos.x + h.x;
	rc.y = tmpPos.y + h.y;
	rc.w = h.w;
	rc.h = h.h;

	_boundRect = rc;
}

void JumpPad::SetActive()
{
	_isActive = 1;
	_remainActiveTime = 1;
	_isPlayMotion = 1; 
	_playTime = _defaultMotionTime; 
	PlaySoundMem(se[3], DX_PLAYTYPE_BACK);
}


void JumpPad::DrawDebugInfo(MATRIX mView)
{
	if (_use == 1) {

		VECTOR rectPos;

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 140.f);

		// 当たり判定矩形
		rectPos = VGet(_rect.x, _rect.y, 0);
		rectPos = VTransform(rectPos, mView);
		DrawBox(rectPos.x, rectPos.y, rectPos.x + _rect.w, rectPos.y + _rect.h, COLOR_BLUE, TRUE);



		rectPos = VGet(_boundRect.x, _boundRect.y, 0);
		rectPos = VTransform(rectPos, mView);

		int colorTbl[] = {
			COLOR_GREEN,
			COLOR_RED
		};
		DrawBox(rectPos.x, rectPos.y, rectPos.x + _boundRect.w, rectPos.y + _boundRect.h, colorTbl[_isActive], TRUE);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}


}

