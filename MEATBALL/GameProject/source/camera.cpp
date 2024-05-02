#include "Camera.h"
#include "mymath.h"
#include "winmain.h"

#include "resource.h"

Camera::Camera() {
	_pos = VGet(SCREEN_W / 2, SCREEN_H / 2, 0);
	_lookat = VGet(SCREEN_W / 2, SCREEN_H / 2, 0);
	_camControl = 0;
	_scale = 1.0f;
	_rotate = 0.0f;

	_stageNo = 0;


	_phase = 0;
	_isFollowingPlayer = 0;
	_adjustValueY = 0;

	_isChangingFollowMode = 0;

	_isRespawn = 0;


	// カメラのリミット設定
	_rcLimit.x = 0;
	_rcLimit.y = 0;
	_rcLimit.w = gMap.GetWorldSizeW();
	_rcLimit.h = gMap.GetWorldSizeH();


	// View行列の生成
	MATRIX m = MGetTranslate(VScale(_pos, -1));				// カメラ位置を*-1する
	m = MMult(m, MGetScale(VGet(_scale, _scale, 1)));		// Z方向の倍率は1のまま
	m = MMult(m, MGetRotZ(DegToRad(_rotate)));				// 2Dでx,yを回転させるには、3DならZ軸回転になる
	m = MMult(m, MGetTranslate(_lookat));					// 注視点分ずらす
	_mView = m;

	_volume = 255;
}
Camera::~Camera() {
	// 何もしない
}

struct FOLLOWING
{
	int phase = -1;
	MYRECT limit;
};

// ステージ開始時の処理
void Camera::Init(int stageNo)
{
	_phase = 0;
	_adjustValueY = 0;
	_isChangingFollowMode = 0;

}

void	Camera::Process(VECTOR playerPos) {

	// リスポーン処理をしない場合
	if (_isRespawn == 0) {

		//
		// エリア(_phase)の管理
		// 
		
		// BGMの設定
		if (_stageNo == 1) {
			// 再生し直し
			if (_phase == 80) {
				if (_volume < 255) {
					_volume += 5;
					if (_volume > 255) _volume = 255;
					ChangeVolumeSoundMem(_volume, bgm[1]);
				}

			}
			// 再生停止
			else if (_phase == 81) {
				if (_volume > 0) {
					_volume--;
					if (_volume < 0) _volume = 0;
					ChangeVolumeSoundMem(_volume, bgm[1]);
				}

			}
		}


		if (_phase == 99 && _stageNo == 1) {
			StopSoundMem(bgm[1]);

			VECTOR bossArea = VGet(225 * CHIPSIZE_W, 44 * CHIPSIZE_H, 0);
			bossArea.y -= 400;

			if (_pos.x != bossArea.x && _pos.y != bossArea.y) {
				VECTOR vDir = VSub(bossArea, _pos);

				float size = VSize(vDir);
				if (size > 10) {

					vDir = VNorm(vDir);
					vDir = VScale(vDir, 10);

					_pos = VAdd(_pos, vDir);




				}
				else {
					// カメラが移動しきったら

					// カメラを固定
					_pos = bossArea;

					// フェーズをボス戦用にする
					_phase = 100;


					// 通路をふさぐマップチップのセットを開始する
					gMap.StartBossBattle();
				}

			}

		}
		else {
			if (_phase != 100) {
				// プレイヤーの位置にカメラを合わせる
				_pos = playerPos;
				_pos.y -= 400;
				_pos.y += 73;
			}
		}


		CheckPhase(playerPos);


		AdjustCam();


		// 位置がリミットを超えないように（回転は加味せず）
		float left, top, right, bottom;

		// カメラがプレイヤーの縦移動に追従しないとき（カメラのy座標を固定するとき）
		if (_isFollowingPlayer == 0) {
			// 初期地点 + カメラの高さを移動させる値
			_pos.y = 17.5f * CHIPSIZE_H + _adjustValueY;


			// リミットの設定
			left = _rcLimit.x + _lookat.x / _scale;
			right = (_rcLimit.x + _rcLimit.w) - _lookat.x / _scale;
			top = _rcLimit.y + _lookat.y / _scale;
			bottom = (_rcLimit.y + _rcLimit.h) - _lookat.y / _scale;

			// リミットを超えていたら、範囲内に戻す
			_pos.x = Clamp(left, right, _pos.x);
			_pos.y = Clamp(top, bottom, _pos.y);



			_isChangingFollowMode = 1;

		}
		else { // カメラがプレイヤー縦移動に追従するとき（カメラのy座標を変化させるとき）


			if (_isChangingFollowMode == 1) {




				float tmpPosY = 17.5f * CHIPSIZE_H + _adjustValueY;


				int camSpeed = 16;
				if (fabs(_pos.y - tmpPosY) > camSpeed) {

					if (_pos.y > tmpPosY) {
						_adjustValueY += camSpeed;
					}
					else if (_pos.y < tmpPosY) {
						_adjustValueY -= camSpeed;
					}


					tmpPosY = 17.5f * CHIPSIZE_H + _adjustValueY;
					_pos.y = tmpPosY;
				}
				else {
					_isChangingFollowMode = 0;
				}


			}
			else {
				FOLLOWING following[STAGE_MAX][5] = {
					// phase, MYRECT（画面内に表示したい範囲 : 左上 x, y / 大きさ w, h）
					{
						{18, {282 * CHIPSIZE_W, 0 * CHIPSIZE_H, 142 * CHIPSIZE_W, 35 * CHIPSIZE_H}},

						{-1}
					},
					{
						{ 7, {113 * CHIPSIZE_W, 17 * CHIPSIZE_H, 110 * CHIPSIZE_W, 58 * CHIPSIZE_H}},
						{ 80, {113 * CHIPSIZE_W, 17 * CHIPSIZE_H, 110 * CHIPSIZE_W, 58 * CHIPSIZE_H}},
						//{ 81, {113 * CHIPSIZE_W, 17 * CHIPSIZE_H, 110 * CHIPSIZE_W, 58 * CHIPSIZE_H}},



					 //{99 , {210 * CHIPSIZE_W, 30 * CHIPSIZE_H, 30 * CHIPSIZE_W, 20 * CHIPSIZE_H}},


						{-1}


					}
				};

				int i = 0;
				while (following[_stageNo][i].phase != -1) {
					if (following[_stageNo][i].phase == _phase) break;
					i++;
				}

				if (following[_stageNo][i].phase != -1) {
					left = following[_stageNo][i].limit.x + _lookat.x / _scale;
					right = (following[_stageNo][i].limit.x + following[_stageNo][i].limit.w) - _lookat.x / _scale;
					top = following[_stageNo][i].limit.y + _lookat.y / _scale;
					bottom = (following[_stageNo][i].limit.y + following[_stageNo][i].limit.h) - _lookat.y / _scale;

					/*left = _rcLimit.x + _lookat.x / _scale;
					right = (_rcLimit.x + _rcLimit.w) - _lookat.x / _scale;
					top = _rcLimit.y + _lookat.y / _scale;
					bottom = (_rcLimit.y + _rcLimit.h) - _lookat.y / _scale;*/




					_adjustValueY = _pos.y - 17.5f * CHIPSIZE_H;


					_pos.x = Clamp(left, right, _pos.x);
					_pos.y = Clamp(top, bottom, _pos.y);
				}
			}




		}





	}
	// リスポーン処理をする場合
	else {

		//// プレイヤーの位置にカメラを合わせる
		//_pos = playerPos;
		//_pos.y -= 400;
		//_pos.y += 73;

		//AdjustCam();


		//// 位置がリミットを超えないように（回転は加味せず）
		//float left, top, right, bottom;

		//// リミットの設定
		//left = _rcLimit.x + _lookat.x / _scale;
		//right = (_rcLimit.x + _rcLimit.w) - _lookat.x / _scale;
		//top = _rcLimit.y + _lookat.y / _scale;
		//bottom = (_rcLimit.y + _rcLimit.h) - _lookat.y / _scale;

		//// リミットを超えていたら、範囲内に戻す
		//_pos.x = Clamp(left, right, _pos.x);
		//_pos.y = Clamp(top, bottom, _pos.y);

		_adjustValueY = _pos.y - 17.5f * CHIPSIZE_H;



		_isRespawn = 0;

	}

	// View行列の生成
	MATRIX m = MGetTranslate(VScale(_pos, -1));				// カメラ位置を*-1する
	m = MMult(m, MGetScale(VGet(_scale, _scale, 1)));		// Z方向の倍率は1のまま
	m = MMult(m, MGetRotZ(DegToRad(_rotate)));				// 2Dでx,yを回転させるには、3DならZ軸回転になる
	m = MMult(m, MGetTranslate(_lookat));					// 注視点分ずらす
	_mView = m;
}



// デバッグ情報の表示
void Camera::DrawDebugInfo() {
	int y = 0;
	DrawFormatString(1500, 800 + y * 16, COLOR_RED, "Camera"); y++;
	DrawFormatString(1500, 800 + y * 16, COLOR_RED, "_phase %d", _phase); y++;
	DrawFormatString(1500, 800 + y * 16, COLOR_RED, "_adjustValueY %3.2f", _adjustValueY); y++;
	DrawFormatString(1500, 800 + y * 16, COLOR_RED, "_pos: x %3.2f, y %3.2f z %3.2f", _pos.x, _pos.y, _pos.z); y++;
}


CamPhase camPhaseChip[STAGE_MAX][175] = {
	// stage1
	{

		{51, 20, 0},
		{51, 21, 0},
		{51, 22, 0},


		{54, 20, 1},
		{54, 21, 1},
		{54, 22, 1},


		{56, 27, 4},
		{57, 27, 4},
		{58, 27, 4},
		{59, 27, 4},
		{60, 27, 4},
		



		{82, 26, 4},
		{83, 26, 4},
		{84, 26, 4},
		{85, 26, 4},
		{86, 26, 4},
		{87, 26, 4},


		{87, 23, 5},
		{87, 24, 5},
		{87, 25, 5},
		{87, 26, 5},


		{109, 15, 5},
		{109, 16, 5},
		{109, 17, 5},


		{111, 20, 7},
		{112, 20, 7},
		{113, 20, 7},


		{183, 23, 7},
		{183, 24, 7},
		{183, 25, 7},
		{183, 26, 7},


		{186, 23, 5},
		{186, 24, 5},
		{186, 25, 5},
		{186, 26, 5},

		{234, 23, 4},
		{234, 24, 4},
		{234, 25, 4},
		{234, 26, 4},


		{231, 23, 5},
		{231, 24, 5},
		{231, 25, 5},
		{231, 26, 5},


		{235, 32, 11},
		{236, 32, 11},
		{237, 32, 11},
		{238, 32, 11},
		{239, 32, 11},
		{240, 32, 11},
		{241, 32, 11},
		{242, 32, 11},
		{243, 32, 11},
		{244, 32, 11},
		{245, 32, 11},
		{246, 32, 11},


		{252, 35, 16},
		{253, 35, 16},
		{254, 35, 16},
		{255, 35, 16},
		{256, 35, 16},
		{257, 35, 16},
		{258, 35, 16},

		{261, 35, 16},
		{262, 35, 16},
		{263, 35, 16},
		{264, 35, 16},
		{265, 35, 16},
		{266, 35, 16},
		{267, 35, 16},
		{268, 35, 16},
		{269, 35, 16},
		{270, 35, 16},

		{273, 35, 16 },
		{274, 35, 16 },
		{275, 35, 16 },
		{276, 35, 16 },
		{277, 35, 16 },
		{278, 35, 16 },
		{279, 35, 16 },
		{280, 35, 16 },
		{281, 35, 16 },
		{282, 35, 16 },

		{285, 35, 16 },
		{286, 35, 16 },
		{287, 35, 16 },
		{288, 35, 16 },
		{289, 35, 16 },
		{290, 35, 16 },
		{291, 35, 16 },


		{ 252, 36, 11 },
		{ 253, 36, 11 },
		{ 254, 36, 11 },
		{ 255, 36, 11 },
		{ 256, 36, 11 },
		{ 257, 36, 11 },
		{ 258, 36, 11 },

		{ 261, 36, 11 },
		{ 262, 36, 11 },
		{ 263, 36, 11 },
		{ 264, 36, 11 },
		{ 265, 36, 11 },
		{ 266, 36, 11 },
		{ 267, 36, 11 },
		{ 268, 36, 11 },
		{ 269, 36, 11 },
		{ 270, 36, 11 },

		{ 273, 36, 11 },
		{ 274, 36, 11 },
		{ 275, 36, 11 },
		{ 276, 36, 11 },
		{ 277, 36, 11 },
		{ 278, 36, 11 },
		{ 279, 36, 11 },
		{ 280, 36, 11 },
		{ 281, 36, 11 },
		{ 282, 36, 11 },

		{ 285, 36, 11 },
		{ 286, 36, 11 },
		{ 287, 36, 11 },
		{ 288, 36, 11 },
		{ 289, 36, 11 },
		{ 290, 36, 11 },
		{ 291, 36, 11 },

		{ 297, 24, 16 },
		{ 297, 25, 16 },
		{ 297, 26, 16 },
		{ 297, 27, 16 },
		

		{ 298, 24, 18 },
		{ 298, 25, 18 },
		{ 298, 26, 18 },
		{ 298, 27, 18 },
		
		
		{ 390, 11, 18 },
		{ 390, 12, 18 },
		{ 390, 13, 18 },
		{ 390, 14, 18 },

		
		
		{ 391, 11, 0 },
		{ 391, 12, 0 },
		{ 391, 13, 0 },
		{ 391, 14, 0 },

		{-1}
	},
	// stage2
	{
		// phase0
		{18, 18, 0},
		{18, 19, 0},
		{18, 20, 0},
		
		// phase 1
		{21, 18, 1},
		{21, 19, 1},
		{21, 20, 1},

		{64, 23, 1},
		{64, 24, 1},
		{64, 25, 1},
		
		//// phase 2
		{67, 23, 2},
		{67, 24, 2},
		{67, 25, 2},

		{108, 25, 2},
		{108, 26, 2},
		{108, 27, 2},
		{108, 28, 2},
		{108, 29, 2},
		{108, 30, 2},
		{108, 31, 2},
		{108, 32, 2},
		
		//// phase 3
		{111, 29, 3},
		{111, 30, 3},
		{111, 31, 3},
		{111, 32, 3},

		//// phase 4
		{121, 38, 4},
		{122, 38, 4},
		{123, 38, 4},
		
		{50, 50, 4},
		{50, 51, 4},
		{50, 52, 4},
		{50, 53, 4},

		

		

		//// phase 5
		{45, 50, 5},
		{45, 51, 5},
		{45, 52, 5},
		{45, 53, 5},
		

		//// phase 6
		{32, 61, 6},
		{33, 61, 6},
		{34, 61, 6},
		{35, 61, 6},
		{36, 61, 6},
		{37, 61, 6},
		{38, 61, 6},
		{39, 61, 6},
		{40, 61, 6},
		{41, 61, 6},
		{42, 61, 6},
		{43, 61, 6},

		{128, 67, 6},
		{128, 68, 6},
		{128, 69, 6},
		{128, 70, 6},
		{128, 71, 6},
		
		

		//// phase 7
		{129, 67, 7},
		{129, 68, 7},
		{129, 69, 7},
		{129, 70, 7},
		{129, 71, 7},

		{155, 30, 7},
		{155, 31, 7},
		{155, 32, 7},
		{155, 33, 7},
		{155, 34, 7},
		{155, 35, 7},
		{155, 36, 7},
		{155, 37, 7},

		{157, 38, 7},
		{157, 39, 7},
		{157, 40, 7},
		{157, 41, 7},
		{157, 42, 7},
		{157, 43, 7},
		{157, 44, 7},

		{161, 45, 7},
		{160, 45, 7},
		{159, 45, 7},
		{158, 45, 7},
		{157, 45, 7},
		
		// phase 8
		{156, 30, 8},
		{156, 31, 8},
		{156, 32, 8},
		{156, 33, 8},
		{156, 34, 8},
		{156, 35, 8},
		{156, 36, 8},
		{156, 37, 8},
		
		// BGM再生し直し
		{175, 40, 80},
		{175, 41, 80},
		{175, 42, 80},
		{175, 43, 80},
		{175, 44, 80},

		// BGM停止
		{176, 40, 81},
		{176, 41, 81},
		{176, 42, 81},
		{176, 43, 81},
		{176, 44, 81},


		// デバッグ用
		// ボス
		{214, 38, 99},
		{214, 39, 99},
		{214, 40, 99},
		{214, 41, 99},
		{214, 42, 99},
		{214, 43, 99},
		{214, 44, 99},




		{-1}
		
		
	}
};

void Camera::CheckPhase(VECTOR pos) {
	if (_phase != 100) {
		int mapX = pos.x / CHIPSIZE_W;
		int mapY = pos.y / CHIPSIZE_H;
		int i = 0;
		while (camPhaseChip[_stageNo][i].x != -1) {
			if (mapX == camPhaseChip[_stageNo][i].x) {
				if (mapY == camPhaseChip[_stageNo][i].y) {
					_phase = camPhaseChip[_stageNo][i].phase;
				}
			}
			i++;
		}
	}
}

struct AdjustValue
{
	int phase;
	int value;
};

void Camera::AdjustCam()
{
	// valueの値は開始地点の座標 ( y: 17.5f * CHIPSIZE_H ) からの座分
	AdjustValue adjustValue[STAGE_MAX][20] = {
		// stage1
		{
			{0,   0},
			{1, 512},
			{4, 800},
			{5, 64},
			{7, 400},
			{11, 1200},
			{16, 640},
			{18,-1},

			{-1} // ここまで調べないはず
		},

		// stage2
		{
			{0,   0},
			{1, 240},
			{2, 640},
			{3, 1200},
			{4, 1808},
			{5, 2448},
			{6, 3040},
			{7, -1},
			{8, 1376},



			{80, 1376},
			{81, 1376},

			

			{98, -1},
			{99, -1},


			{-1} // ここまで調べないはず
		
		}
	};

	int camSpeed = 16;

	int i = 0;
	while (adjustValue[_stageNo][i].phase != -1) {
		if (_phase == adjustValue[_stageNo][i].phase) {
			if (_isRespawn == 1) {
				_adjustValueY = adjustValue[_stageNo][i].value;
				break;
			}
			if (adjustValue[_stageNo][i].value == -1) {
				_isFollowingPlayer = 1;
				break;
			}
			else {
				_isFollowingPlayer = 0;
			}

			if (_adjustValueY > adjustValue[_stageNo][i].value) {
				_adjustValueY -= camSpeed;

				if (_adjustValueY <= adjustValue[_stageNo][i].value) {
					_adjustValueY = adjustValue[_stageNo][i].value;
				}
			}
			else if (_adjustValueY < adjustValue[_stageNo][i].value) {
				_adjustValueY += camSpeed;

				if (_adjustValueY >= adjustValue[_stageNo][i].value) {
					_adjustValueY = adjustValue[_stageNo][i].value;
				}
			}
			break;
		}
		i++;
	}

	/*if (adjustValue[_phase].value == -1) {
		_isFollowingPlayer = 1;
	}
	else {
		_isFollowingPlayer = 0;

		if (_adjustValueY > adjustValue[_phase].value) {
			_adjustValueY -= speed;
		}
		else if (_adjustValueY < adjustValue[_phase].value) {
			_adjustValueY += speed;
		}
	}*/
}

void Camera::SetRespawn(int respawnPhase, VECTOR respawnPos)
{
	_phase = respawnPhase;
	_pos = respawnPos;
	_isRespawn = 1;
}


