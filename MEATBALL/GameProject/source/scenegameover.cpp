#include "DxLib.h"
#include "winmain.h"
#include "gamemain.h"
#include "resource.h"
#include "scenebase.h"
#include "scenegamemain.h"
#include "scenegameover.h"
#include "gamepad.h"
#include "mymath.h"
#include "SceneGameTitle.h"



SceneGameOver::SceneGameOver() {

	_isRetry = 0;


	_step = 0;
	_stepcnt = 0;

	// siroからフェードインする
	ColorMask(255, 255, 255, 255);

	//3Dモデル読み込み
	_playerModel = MV1LoadModel("res/model/cg_player_girl/cg_player_girl.mv1");
	_floorModel = MV1LoadModel("res/model/gameover_bg/gameover_bg.mv1");
	_meatBallModel = MV1LoadModel("res/model/meatball/cg_player_meatball.mv1");



	// カメラの座標を初期化
	_CameraPos.x = 4.0f;
	_CameraPos.y = 1319.0f;
	_CameraPos.z = 1377.0f;
	SetCameraPositionAndTarget_UpVecY(_CameraPos, VGet(0, 0, 0));



	//モデルスケール
	MV1SetScale(_playerModel, VGet(4.0f, 4.0f, 4.0f));
	MV1SetScale(_floorModel, VGet(14.0f, 14.0f, 14.0f));


	_meatBallDefaultScale = VGet(1.4f, 1.4f, 1.4f);
	MV1SetScale(_meatBallModel, _meatBallDefaultScale);




	//モデルの初期位置
	_playerPos = VGet(0, 0, 500);
	MV1SetPosition(_playerModel, _playerPos);


	MV1SetPosition(_floorModel, VGet(0, 0, 500));

	_meatBallPos = VAdd(_playerPos, VGet(300, 0, 0));
	MV1SetPosition(_meatBallModel, _meatBallPos);



	int playerMotionIndex = MV1AttachAnim(_playerModel, 12, -1, TRUE);
	MV1SetAttachAnimTime(_playerModel, playerMotionIndex, 200.0f);


	_faceFrameNo = MV1SearchFrame(_playerModel, "head_center");

	_viewRoute = 0;

	// 動きの種類
	_moveCnt = 0;


	// 横揺れ
	_zTwist = 0.0f;
	_twistOrder = 1;
	_twistCnt = 0;

	// ジャンプ
	_meatBallY = 0;
	_isJump = 1;
	_jumpCnt = 0;

	PlaySoundMem(bgm[5], DX_PLAYTYPE_LOOP);
}

SceneGameOver::~SceneGameOver() {
	
	
	// モデルハンドルの削除
	MV1DeleteModel(_playerModel);
	MV1DeleteModel(_floorModel);
}

void SceneGameOver::Input() {


	

}

void SceneGameOver::Process() {
	
	_viewRoute+=0.4;
	MV1SetRotationXYZ(_playerModel, VGet(0, DegToRad(_viewRoute), 0));

	MeatBallProcess();
	

	MenuProcess();

	FadeProcess();


	
}



void SceneGameOver::MeatBallProcess()
{
	if (_moveCnt == 7) { _moveCnt = 0; };

	if (_moveCnt >= 3) {
		_zTwist = 0;

		if (_isJump == 1) {
			_meatBallY = EasingOutQuad(_jumpCnt, 0, 100, 20);


			VECTOR vTmp = VScale(_meatBallDefaultScale, 0.75f);
			VECTOR magnification;
			magnification.x = EasingOutElastic(_jumpCnt, vTmp.x, _meatBallDefaultScale.x, 20);
			magnification.y = EasingOutElastic(_jumpCnt, vTmp.y, _meatBallDefaultScale.y, 20);
			magnification.z = EasingOutElastic(_jumpCnt, vTmp.z, _meatBallDefaultScale.z, 20);
			MV1SetScale(_meatBallModel, magnification);

			_jumpCnt++;
			if (_jumpCnt == 20) {
				_isJump = -1;
				_jumpCnt = 0;
			}
		}
		else if (_isJump == -1) {
			_meatBallY = EasingInQuad(_jumpCnt, 100, 0, 20);

			_jumpCnt++;
			if (_jumpCnt == 20) {
				_isJump = 1;
				_jumpCnt = 0;
				_moveCnt++;
			}
		}
	}
	else {
		float startTwist = _moveCnt == 0 ? 0.0f : 0.5f * _twistOrder * -1;

		//_zTwist = EasingInOutSine(_twistCnt, 0.5f * _twistOrder * -1, 0.5f * _twistOrder, 60);
		_zTwist = EasingInOutQuad(_twistCnt, startTwist, 0.5f * _twistOrder, 60);

		_twistCnt++;
		if (_twistCnt == 60) {
			_twistOrder *= -1;
			_twistCnt = 0;

			_moveCnt++;
		}
	}


	VECTOR vSub = VGet(-350, 30, -100);
	vSub.y += _meatBallY;


	MATRIX mRot = MGetRotY(DegToRad(_viewRoute));
	vSub = VTransform(vSub, mRot);

	_meatBallPos = VAdd(_playerPos, vSub);
	MV1SetPosition(_meatBallModel, _meatBallPos);

	VECTOR facePos = MV1GetFramePosition(_playerModel, _faceFrameNo);

	// プレイヤーがいる方向を向くようにする
	MV1SetRotationZYAxis(_meatBallModel, VSub(_meatBallPos, facePos), VGet(0.0f, 1.0f, 0.0f), _zTwist);

}

void SceneGameOver::MenuProcess()
{



	noperation++;
	if (noperation >= 130) {
	if (dTrg & PAD_INPUT_LEFT) {

		use--;

		PlaySoundMem(se[1], DX_PLAYTYPE_BACK);
	}

	if (dTrg & PAD_INPUT_RIGHT) {

		use++;

		PlaySoundMem(se[1], DX_PLAYTYPE_BACK);
	}


	if (gPad->_trg[PAD_BUTTON_A]) {
		PlaySoundMem(se[22], DX_PLAYTYPE_BACK);
		StopSoundMem(bgm[5]);
		switch (use) {
		case 0: {
			// ゲームメインシーンへ移行する
		  // 実際のシーン移行はscenegamemain.cpp内

			//SceneBase* scene = new SceneGameMain();
			//ChangeScene(scene);

			_isRetry = 1;
			break;
		}
		case 1: {

			Exit = 1;

		}


		}
	}
	if (use == 2) {
		use = 0;
	}
	if (use == -1) {
		use = 1;
	}


	}

}

void SceneGameOver::FadeProcess()
{

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
			_stepcnt = 0;
		}
		break;
	case 2:
		// 通常処理
		_stepcnt++;
		if (_stepcnt > 60) {
			// クロースフェードアウト開始
			CrossFadeOut(60);
			// クロスフェードは開始したら終了を待たずシーン切り替えしちゃって良い
			// フェードアウト終了
			/*SceneBase* scene = new SceneGameTitle();
			ChangeScene(scene);*/
		}
		break;
	}
}



void SceneGameOver::Draw() {

	MV1DrawModel(_meatBallModel);

	MV1DrawModel(_playerModel);
	MV1DrawModel(_floorModel);

	DrawGraph(320, 64, gameover, TRUE);
	DrawGraph(428, 448, retry, TRUE);
	DrawGraph(1068, 448, close, TRUE);

	if (use == 0) {
		DrawGraph(373, 478, arrow, TRUE);
		setCnt++;
		DrawGraph(428, 448, retryAnim[setCnt / 8 % 12], TRUE);
	}
	if (use == 1) {
		DrawGraph(993, 478, arrow, TRUE);
		setCnt++;
		DrawGraph(1068, 448, closeAnim[setCnt / 8 % 12], TRUE);
	}
	//DrawFormatString(0, 1000, COLOR_WHITE, "trg==%d", gTrg);

}