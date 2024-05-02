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
	// ３Ｄモデルの読み込み
	_modelHandle = MV1DuplicateModel(modelHandle);
	MV1SetScale(_modelHandle, size);

	// 複数のアニメーションをアタッチしている状態だと3Dモデルの状態がブレンドされたものになる
	// ブレンドさせたくない場合は
	// ①1つのモーションだけをアタッチする（他のモーションを外すにはデタッチをする）　※DxLibではこちらを想定している
	// ②再生したくないモーションのブレンドレートを0にする (使用メモリや処理負荷が多少上昇する) 
	// MV1SetAttachAnimBlendRate

	// ここでは①の方法をとる
	// 以下のループは totalTime を取るために行う
	for (int i = 0; i < motionMax; i++) {
		//アニメーションのアタッチ
		_anim[i].index = MV1AttachAnim(_modelHandle, i, -1, FALSE);
		// アタッチしたアニメーションの総再生時間を取得する
		_anim[i].totalTime = MV1GetAttachAnimTotalTime(_modelHandle, _anim[i].index);
		if (i != _motionNum) {
			// アニメーションのデタッチ
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

	// モーションが変更された場合に、変更前のモーションをデタッチして変更後のモーションをアタッチする
	if (motionNum_old != _motionNum) {
		// 変更前のモーションのデタッチ
		MV1DetachAnim(_modelHandle, _anim[motionNum_old].index);
		// 変更後のモーションのアタッチ
		_anim[_motionNum].index = MV1AttachAnim(_modelHandle, _motionNum, -1, FALSE);

		_playTime = 0;
	}


	// 再生時間をセットする(アニメーションの再生)
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




	// 当たり判定矩形
	rectPos = VGet(_rect.x, _rect.y, 0);
	rectPos = VTransform(rectPos, mView);
	DrawBox(rectPos.x, rectPos.y, rectPos.x + _rect.w, rectPos.y + _rect.h, COLOR_BLUE, TRUE);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}


// 描画位置の補正処理
void Gimmick::AdjustDrawPos(MATRIX mView)
{
	_screenPos = _pos;
	_screenPos = VTransform(_screenPos, mView);
	UpdateWorldPos();
	// 移動処理の反映
	MV1SetPosition(_modelHandle, _worldPos);
}

void Gimmick::Draw()
{
	if (_use == 1) {
		//// ３Ｄモデルの描画
		MV1DrawModel(_modelHandle);
	}
}

void Gimmick::UpdateWorldPos()
{
	// スクリーン座標をワールド座標に変換
	_worldPos = ConvScreenPosToWorldPos(_screenPos);
	_worldPos.z = 400; // z軸表示位置の調整
}
