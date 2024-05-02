#include "spike.h"
#include "gimmick_data.h"


int Spike::_spikeModel = -1;


Spike::Spike(int stageNo, int index)
{
	_index = index;

	SPIKE_DATA spikeData = GetSpikeData(stageNo, _index);

	if (spikeData.x != -1) {
		_use = 1;

		_pos = VGet(spikeData.x * CHIPSIZE_W + 32, spikeData.y * CHIPSIZE_H + 32, 0);


		_difRect = { -32, -32, 64, 64 };
		SetCollision();

		ModelInit(_spikeModel, VGet(1.8, 1.8, 1.8), 0, stageNo);
	}
	else {
		_use = 0;
	}
}

Spike::~Spike()
{
	//Gimmick::~Gimmick();
}


void Spike::ModelInit(int modelHandle, VECTOR size, int motionMax, int stageNo)
{
	if (_use == 1) {
		// モデルの読み込み
		Gimmick::ModelInit(modelHandle, size, motionMax);

		// モデルの回転の設定
		SPIKE_DATA spikeData = GetSpikeData(stageNo, _index);
		float rot = DegToRad(90 * spikeData.rotation);
		MV1SetRotationXYZ(_modelHandle, VGet(0, 0, rot));


		// アウトラインの設定
		for (int i = 0; i < 7; i++) {
			MV1SetMaterialOutLineWidth(_modelHandle, i, 0.001f);
			MV1SetMaterialOutLineDotWidth(_modelHandle, i, 0.7f);
			MV1SetMaterialOutLineColor(_modelHandle, i, GetColorF(1, 0, 0, 0.5f));
		}

	}
}

//void Spike::AdjustDrawPos(MATRIX mView)
//{
//	VECTOR adjustPos = _pos;
//	//float adjustValue = 37;
//	//switch (_rotation)
//	//{
//	//case GIMMICK_ROTATION_FLOOR:
//	//	adjustPos.y += adjustValue;
//	//	break;
//	//case GIMMICK_ROTATION_RIGHT_WALL:
//	//	adjustPos.x += adjustValue;
//	//	break;
//	//case GIMMICK_ROTATION_CEILING:
//	//	adjustPos.y -= adjustValue;
//	//	break;
//	//case GIMMICK_ROTATION_LEFT_WALL:
//	//	adjustPos.x -= adjustValue;
//	//	break;
//	//}
//	_screenPos = adjustPos;
//	_screenPos = VTransform(_screenPos, mView);
//	UpdateWorldPos();
//	// 移動処理の反映
//	MV1SetPosition(_modelHandle, _worldPos);
//}

void Spike::DrawDebugInfo(MATRIX mView)
{
	if (_use == 1) {

		VECTOR rectPos;

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 140.f);




		// 当たり判定矩形
		rectPos = VGet(_rect.x, _rect.y, 0);
		rectPos = VTransform(rectPos, mView);
		DrawBox(rectPos.x, rectPos.y, rectPos.x + _rect.w, rectPos.y + _rect.h, COLOR_RED, TRUE);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


	}

	//for (int i = 0; i < 7; i++) {
	//	MV1SetMaterialOutLineDotWidth(_modelHandle, i, 0.7f);
	//	MV1SetMaterialOutLineColor(_modelHandle, i, GetColorF(1, 0, 0, 1));
	//}
}