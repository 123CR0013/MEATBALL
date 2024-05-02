#include "scenemovie.h"
#include "gamepad.h"
#include "gamemain.h"
#include "resource.h"
#include "mymath.h"

#define MOTION_PROLOGUE 16
#define MOTION_CLEAR 17

SceneMovie::SceneMovie(int movieNum)
{
	/*ColorMask(0, 0, 0, 255);*/
	_step = 0;
	_stepcnt = 0;
	SetupCamera_Perspective(DegToRad(55));


	_movieNo = movieNum;

	_isEndMovie = 0;


	_cgBgMovie = -1;
	_cgBgMovie = LoadGraph("res/bg/movie_bg.png");


	_player.modelHandle = MV1LoadModel("res/model/cg_player_girl/cg_player_girl.mv1");
	_player.pos = VGet(0, 0, 0);
	MV1SetPosition(_player.modelHandle, _player.pos);
	MV1SetScale(_player.modelHandle, VGet(10, 10, 10));

	int motionNum = -1;
	const char* camName[] = {
		"prologue_camera",
		"clear_camera"
	};
	int camNum = -1;

	const char* gazeName[] = {
		"prologue_camera_aim_01",
		"camera1_aim"
	};
	int gazeNum = -1;
	switch (_movieNo)
	{
	case MOVIE_PROLOGUE:
		motionNum = MOTION_PROLOGUE;
		//camNum = MOVIE_PROLOGUE;
		//gazeNum = MOVIE_PROLOGUE;
		break;
	case MOVIE_CLEAR:
		motionNum = MOTION_CLEAR;
		//camNum = MOVIE_CLEAR;
		//gazeNum = MOVIE_CLEAR;
		break;
	}

	_camFrameNo = MV1SearchFrame(_player.modelHandle, camName[_movieNo]);

	_gazeFrameNo = MV1SearchFrame(_player.modelHandle, gazeName[_movieNo]);
	_gazePoint = MV1GetFramePosition(_player.modelHandle, _gazeFrameNo);







	_player.animIndex = MV1AttachAnim(_player.modelHandle, motionNum, -1, FALSE);
	_player.totalTime = MV1GetAttachAnimTotalTime(_player.modelHandle, _player.animIndex);
	_player.playTime = 0;
	MV1SetAttachAnimTime(_meatBall.modelHandle, _meatBall.animIndex, _meatBall.playTime);


	_frameNo = MV1SearchFrame(_player.modelHandle, "tank_center");
	_meatBall.modelHandle = MV1LoadModel("res/model/meatBall/cg_player_meatball.mv1");
	_rot = 0;

	_meatBall.animIndex = MV1AttachAnim(_meatBall.modelHandle, 1, -1, FALSE);
	_meatBall.totalTime = MV1GetAttachAnimTotalTime(_meatBall.modelHandle, _meatBall.animIndex);
	_meatBall.playTime = 0;



	// ミートボールの挙動関連
	_isDrawAura = 0;

	_rotType = 0;
	_rotation = VGet(0, 0, 0);

	_isScaling = 0;
	_scalingCnt = 0; 




	_gazePoint = MV1GetFramePosition(_player.modelHandle, _gazeFrameNo);
	_camPos = MV1GetFramePosition(_player.modelHandle, _camFrameNo);
	SetCameraPositionAndTarget_UpVecY(_camPos, _gazePoint);
}

SceneMovie::~SceneMovie()
{
	if (_cgBgMovie != -1) {
		DeleteGraph(_cgBgMovie);
	}
	if (_player.modelHandle != -1) {
		MV1DeleteModel(_player.modelHandle);
	}
	if (_meatBall.modelHandle != -1) {
		MV1DeleteModel(_meatBall.modelHandle);
	}
	StopSoundMem(bgm[0]);
	StopSoundMem(bgm[6]);
}

void SceneMovie::Input()
{
}

void SceneMovie::Process()
{
	// BGMの再生
	if (_movieNo == MOVIE_CLEAR) {
		if (CheckSoundMem(bgm[6]) == 0) {
			PlaySoundMem(bgm[6], DX_PLAYTYPE_LOOP);
		}
	}
	else {
		if (CheckSoundMem(bgm[0]) == 0) {
			PlaySoundMem(bgm[0], DX_PLAYTYPE_LOOP);
		}
	}


	_player.playTime += 1.0f;
	press += 1;


	if (_movieNo == MOVIE_PROLOGUE) {
		if (_player.playTime == 310) {
			PlaySoundMem(voice[13], DX_PLAYTYPE_BACK);
		}
		else if (_player.playTime == 450) {
			PlaySoundMem(voice[11], DX_PLAYTYPE_BACK);
		}

		if (_player.playTime == _player.totalTime - 30) {
			ColorFadeOut(0, 0, 0, 30);

		}

	}
	if (_movieNo == MOVIE_CLEAR) {
		if (movievoicecnt == 0) {

			if (_player.playTime == 500) {
				PlaySoundMem(voice[12], DX_PLAYTYPE_BACK);
			}
			if (_player.playTime == 620) {
				PlaySoundMem(voice[11], DX_PLAYTYPE_BACK);
				movievoicecnt++;
			}

		}
		if (_player.playTime == 699) {

			presscnt++;

		}
	}





	if (_player.playTime >= _player.totalTime) {
		if (_movieNo == MOVIE_CLEAR) {
			_player.playTime = 480.0f;
		}
		else {
			_player.playTime = _player.totalTime;
			_rot = 0;
		}
		if (GetASyncLoadNum() == 0) {
			_isEndMovie = 1;
		}
	}
	MV1SetAttachAnimTime(_player.modelHandle, _player.animIndex, _player.playTime);

	_meatBall.playTime += 1.5f;
	if (_meatBall.playTime >= _meatBall.totalTime) {
		_meatBall.playTime = 0.0f;
	}
	MV1SetAttachAnimTime(_meatBall.modelHandle, _meatBall.animIndex, _meatBall.playTime);


	AttachMeatBallToPlayer();
	_rot += 1.5;


	_gazePoint = MV1GetFramePosition(_player.modelHandle, _gazeFrameNo);
	_camPos = MV1GetFramePosition(_player.modelHandle, _camFrameNo);

	if (_movieNo == MOVIE_CLEAR) {
		VECTOR vDir = VSub(_gazePoint, _camPos);
		vDir = VNorm(vDir);
		_camPos = VAdd(_camPos, VScale(vDir, 300));
	}
	else {
		VECTOR vDir = VSub(_gazePoint, _camPos);
		vDir = VNorm(vDir);

		_camPos = VAdd(_camPos, VScale(vDir, 1000));
	}


	SetCameraPositionAndTarget_UpVecY(_camPos, _gazePoint);






	//// 開発用
	//// ムービーを最初から流す
	//if (gPad->_trg[PAD_BUTTON_BACK] != 0) {
	//	_player.playTime = 0;
	//	_meatBall.playTime = 0;
	//	_rot = 0;

	//	switch (_movieNo)
	//	{
	//	case MOVIE_PROLOGUE:
	//		_gazePoint = _player.pos;
	//		_gazePoint.y += 1400;
	//		break;
	//	case MOVIE_CLEAR:
	//		_gazePoint = _meatBall.pos;
	//		break;
	//	}
	//}



	//if (gKey & PAD_INPUT_1) {
	//	if (gKey & PAD_INPUT_UP) {
	//		_gazePoint.z += 10;
	//	}
	//	else if (gKey & PAD_INPUT_DOWN) {
	//		_gazePoint.z -= 10;
	//	}
	//}

	//if (gKey & PAD_INPUT_2) {
	//	if (gKey & PAD_INPUT_UP) {
	//		_gazePoint.x += 10;
	//	}
	//	else if (gKey & PAD_INPUT_DOWN) {
	//		_gazePoint.x -= 10;
	//	}
	//}

	//if (gKey & PAD_INPUT_3) {
	//	if (gKey & PAD_INPUT_UP) {
	//		_gazePoint.y += 10;
	//	}
	//	else if (gKey & PAD_INPUT_DOWN) {
	//		_gazePoint.y -= 10;
	//	}
	//}






	if (gPad->_trg[PAD_BUTTON_RSTICK] != 0) {
		_isDrawAura = (_isDrawAura + 1) % 2;

		MV1SetMaterialOutLineColor(_meatBall.modelHandle, 0, GetColorF(1.0f, 0.0f, 1.0f, 1.0f));

		if (_isDrawAura == 1) {
			MV1SetMaterialOutLineWidth(_meatBall.modelHandle, 0, 0.01f);
			MV1SetMaterialOutLineDotWidth(_meatBall.modelHandle, 0, 0.05f);
		}
		else {
			MV1SetMaterialOutLineWidth(_meatBall.modelHandle, 0, 0.0f);
			MV1SetMaterialOutLineDotWidth(_meatBall.modelHandle, 0, 0.0f);
		}

	}


	if (gPad->_trg[PAD_BUTTON_LSTICK] != 0) {
		_rotType = (_rotType + 1) % 4;
	}

	//if (gPad->_trg[PAD_BUTTON_RT] != 0) {
	//	_isScaling = 1;
	//	_scalingCnt = 0;
	//}
	if (gPad->_input.Buttons[PAD_BUTTON_RT] != 0) {
		if (_isScaling == 0) {
			_isScaling = 1;
			_scalingCnt = 0;
		}
	}

	if (_rotType == 3) {
		VECTOR vRot = VGet(gPad->_input.Rz, gPad->_input.Z, 0);
		vRot.x *= -1;
		vRot.y *= -1;

		if (vRot.x != 0 || vRot.y != 0) {
			vRot = VNorm(vRot);
			vRot = VScale(vRot, 0.05f);
			_rotation = VAdd(_rotation, vRot);
		}
	}
	else {
		_rotation = VGet(0, 0, 0);
	}
	if (presscnt >= 1) {
		if (gPad->_trg[PAD_BUTTON_A]) {
			PlaySoundMem(se[26], DX_PLAYTYPE_BACK);
		}
	}

	switch (_step) {
	case 0:
		// フェードイン開始
		ColorFadeIn(60);
		_step++;
		break;
	case 1:
		if (IsColorFade() == 0) {
			// フェードイン終了
			_step++;

		}
		break;
	}
}

void SceneMovie::Draw()
{
	DrawGraph(0, 0, _cgBgMovie, FALSE);
	MV1DrawModel(_meatBall.modelHandle);
	MV1DrawModel(_player.modelHandle);

	// トーク画面中はキャラクターを隠す
	if (_movieNo == MOVIE_CLEAR && _player.playTime < 1) {
 		DrawGraph(0, 0, _cgBgMovie, FALSE);
	}

	if (presscnt >= 1) {	
		DrawGraph(1000, 540, titleBottunSheet[press / 8 % 12], TRUE);
	}
}

void SceneMovie::AttachMeatBallToPlayer()
{
	// アタッチするモデルのフレーム〇番の座標を取得する
	VECTOR weaponFramePosition = MV1GetFramePosition(_meatBall.modelHandle, -1);
	// アタッチするモデルをフレーム〇番の座標を原点にするための平行移動行列を作成
	MATRIX TransMat = MGetTranslate(VScale(weaponFramePosition, -1.0f));
	// アタッチされるモデルのフレーム〇番の行列を取得
	MATRIX characterFrameMat = MV1GetFrameLocalWorldMatrix(_player.modelHandle, _frameNo);

	// 大きさ調整

	VECTOR vScale = VGet(0.28f, 0.28f, 0.28f);

	if (_isScaling == 1) {
		if (_scalingCnt <= 36) {
			float x = EasingOutElastic(_scalingCnt, vScale.x * 0.8f, vScale.x, 36);
			float y = EasingOutElastic(_scalingCnt, vScale.y * 0.8f, vScale.y, 36);
			float z = EasingOutElastic(_scalingCnt, vScale.z * 0.8f, vScale.z, 36);

			vScale = VGet(x, y, z);
		}
		else {
			_isScaling = 0;
			_scalingCnt = 0;
		}
		_scalingCnt++;
	}

	MATRIX mScale = MGetScale(vScale);

	// 回転調整
	
	MATRIX mRotation;
	
	switch (_rotType)
	{
	case 0:
		mRotation = MGetRotZ(DegToRad(_rot));
		break;
	case 1:
		mRotation = MGetRotX(DegToRad(_rot));
		break;
	case 2:
		mRotation = MGetRotY(DegToRad(_rot));
		break;
	case 3:
		mRotation = MGetRotX(_rotation.x);
		mRotation = MMult(mRotation, MGetRotY(_rotation.y));
		mRotation = MMult(mRotation, MGetRotZ(_rotation.z));
		break;
	}
	//MATRIX mRotation = MGetRotX(DegToRad(0.0f));
	//mRotation = MMult(mRotation, MGetRotY(DegToRad(0.0f)));
	//mRotation = MMult(mRotation, MGetRotZ(DegToRad(_rot)));

	// 行列の合成
	MATRIX MixMatrix = MMult(mScale, mRotation);
	MixMatrix = MMult(MixMatrix, TransMat);
	MixMatrix = MMult(MixMatrix, characterFrameMat);

	/*MATRIX mTranslate = MGetTranslate(VGet(0, 0, -2));
	MixMatrix = MMult(MixMatrix, mTranslate);*/


	// 合成した行列をアタッチするモデルにセット
	MV1SetMatrix(_meatBall.modelHandle, MixMatrix);
}
