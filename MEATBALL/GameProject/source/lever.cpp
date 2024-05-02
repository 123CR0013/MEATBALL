#include "lever.h"
#include "effect.h"
#include "resource.h"


int Lever::_leverModel[2] = { -1, -1 };

Lever::Lever(int stageNo, int index)
{
	
	_index = index;

	_isActive = 0;

	LEVER_DATA leverData= GetLeverData(stageNo, _index);
	if (leverData.x != -1) {
		_use = 1;

		_returnInactive = leverData.returnInactive;
		_remainActiveTime = 0;

		_pos = VGet(leverData.x * CHIPSIZE_W + 64, leverData.y * CHIPSIZE_H + 64, 0);
		_screenPos = _pos;



		_gimmickType = leverData.gimmickType;
		_gimmickId = leverData.gimmickId;



		


		_difRect = { -64, -64, 128, 128 };
		SetCollision();


		_motionNum = 0;
		_playTime = 0;


		int leverColor = _gimmickType == GIMMICK_TYPE_JUMPPAD ? LEVER_MODEL_RED : LEVER_MODEL_BLUE;

		ModelInit(_leverModel[leverColor], VGet(6, 6, 6), 1, stageNo);

	}
	else {
		_use = 0;
		_returnInactive = 0;
		_remainActiveTime = 0;
		_isPlayMotion = 0;
		_gimmickType = -1;
		_gimmickId = -1;
	}
}

Lever::~Lever()
{
	//Gimmick::~Gimmick();
}


void Lever::ModelInit(int modelHandle, VECTOR size, int motionMax, int stageNo)
{
	if (_use == 1) {
		// モデルの読み込み
		Gimmick::ModelInit(modelHandle, size, motionMax);

		// モデルの回転の設定
		LEVER_DATA leverData = GetLeverData(stageNo, _index);
		_rotation = leverData.rotation;
		float rot = DegToRad(90 * _rotation);
		MV1SetRotationXYZ(_modelHandle, VGet(0, 0, rot));

		// アウトラインの初期設定
		SetOutLine(TRUE);
		// アウトラインカラーの設定
		for (int i = 0; i < 8; i++) {
			MV1SetMaterialOutLineColor(_modelHandle, i, GetColorF(0, 1, 0, 1));
		}

	}
}



void Lever::Process()
{
	if (_use == 1) {

		if (_returnInactive == 1) {
			if (_remainActiveTime > 0) {
				_remainActiveTime--;
			}
			else {
				_isActive = 0;
			}
		}


		switch (_isPlayMotion)
		{
		case 1:
			_playTime++;
			if (_playTime >= _anim[_motionNum].totalTime) {
				_playTime = _anim[_motionNum].totalTime;
				if (_returnInactive == 1) {
					_isPlayMotion = -1;
				}
				else {
					_isPlayMotion = 0;
				}
				// アウトラインの無効化
				SetOutLine(FALSE);
			}
			break;
		case -1:
			_playTime -= 1;
			if (_playTime <= 0) {
				_playTime = 0;
				_isPlayMotion = 0;

				// アウトラインの有効化
				SetOutLine(TRUE);
			}
			break;
		}

		// 再生時間をセットする(アニメーションの再生)
		MV1SetAttachAnimTime(_modelHandle, _anim[_motionNum].index, _playTime);


	}
}

void Lever::SetActive()
{
	if (_remainActiveTime <= 0) {
		_isActive = 1;
		_isPlayMotion = 1;

		_remainActiveTime = 40;

		// レバー作動エフェクトのセット （機械敵撃破エフェクトと同じものを使用する）
		float effectSize = _rect.w * 2;
		int effectX = _rect.x - effectSize / 4;
		int effectY = _rect.y - effectSize / 4;
		SetEffect(EFFECT_TYPE_DEFEAT_MACHINE, effectX, effectY, effectSize, effectSize);

		// レバー作動SEの再生
		PlaySoundMem(se[4], DX_PLAYTYPE_BACK);
		switch (_gimmickType)
		{
		case GIMMICK_TYPE_DOOR:
			// 特定のバリアチップを消す処理
			gMap.VanishDoor(_gimmickId);
			// バリアチップSEの再生
			PlaySoundMem(se[19], DX_PLAYTYPE_BACK);
			break;

		case GIMMICK_TYPE_JUMPPAD:
			for (int i = 0; i < JUMPPAD_MAX; i++) {
				if (jumpPad[i]->GetId() == _gimmickId) {
					jumpPad[i]->SetActive();
					_playTime = 0;
				}
			}
			break;
		}
	}
}

// 描画位置の補正処理
void Lever::AdjustDrawPos(MATRIX mView)
{
	if (_use == 1) {
		VECTOR adjustPos = _pos;
		float adjustValue = 37;
		switch (_rotation)
		{
		case GIMMICK_ROTATION_FLOOR:
			adjustPos.y += adjustValue;
			break;
		case GIMMICK_ROTATION_RIGHT_WALL:
			adjustPos.x += adjustValue;
			break;
		case GIMMICK_ROTATION_CEILING:
			adjustPos.y -= adjustValue;
			break;
		case GIMMICK_ROTATION_LEFT_WALL:
			adjustPos.x -= adjustValue;
			break;
		}
		_screenPos = adjustPos;
		_screenPos = VTransform(_screenPos, mView);
		UpdateWorldPos();
		// 移動処理の反映
		MV1SetPosition(_modelHandle, _worldPos);
	}
}


// アウトラインの設定
void Lever::SetOutLine(bool n)
{
	// 有効化
	if (n == TRUE) {
		for (int i = 0; i < 8; i++) {
			MV1SetMaterialOutLineWidth(_modelHandle, i, 0.001f);
			MV1SetMaterialOutLineDotWidth(_modelHandle, i, 0.02f);
		}
	}
	// 無効化
	else {
		for (int i = 0; i < 8; i++) {
			MV1SetMaterialOutLineWidth(_modelHandle, i, 0.0f);
			MV1SetMaterialOutLineDotWidth(_modelHandle, i, 0.0f);
		}
	}
}


void Lever::DrawDebugInfo(MATRIX mView)
{
	if (_use == 1) {

		VECTOR rectPos;

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 140.f);


		int colorTble[] = {
			COLOR_BLUE,
			COLOR_RED
		};

		// 当たり判定矩形
		rectPos = VGet(_rect.x, _rect.y, 0);
		rectPos = VTransform(rectPos, mView);
		DrawBox(rectPos.x, rectPos.y, rectPos.x + _rect.w, rectPos.y + _rect.h, colorTble[_isActive], TRUE);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}