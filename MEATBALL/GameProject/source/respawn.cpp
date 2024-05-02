#include "respawn.h"
#include "winmain.h"
#include "mydraw.h"
#include "mapdata.h"

// 1�X�e�[�W������̃��X�|�[���n�_�ő吔�i�����n�_�͊܂߂Ȃ��j
#define RESPAWN_POINT_MAX 1


Respawn::Respawn(int stageNo, VECTOR playerPos)
{
	_stageNo = stageNo;
	_respawnPointNo = 0;


	_playerPos = playerPos;
	_meatBallSize = 1;


	_camPhase = 0;

	VECTOR camStartPos[2] = {
		// stage01
		VGet(960, 1120, 0),
		// stage02
		VGet(960, 1120, 0),
	};
	_camPos = camStartPos[_stageNo];


	// �摜�̓ǂݍ���
	_cgHologram = LoadGraph("res/map/save_point/savepoint.png");
	LoadDivGraph("res/map/save_point/savepoint_sheet.png", 12, 12, 1, 192, 192, _cgHologramAnimation);
	_cgProjector = LoadGraph("res/map/save_point/savepoint_frame.png");


	// �A�j���[�V�����p�ϐ��̏�����
	_isPlayingAnimation = 0;
	_animCnt = 1;


	// �T�E���h�n���h���̏�����
	_sound = LoadSoundMem("res/sound/SE/se_save.mp3");
}

Respawn::~Respawn()
{
	if (_cgHologram != -1) {
		DeleteGraph(_cgHologram);
		_cgHologram = -1;
	}

	for (int i = 0; i < 12; i++) {
		if (_cgHologramAnimation[i] != -1) {
			DeleteGraph(_cgHologramAnimation[i]);
			_cgHologramAnimation[i] = -1;
		}
	}


	if (_cgProjector != -1) {
		DeleteGraph(_cgProjector);
		_cgProjector = -1;
	}

	if (_sound != -1) {
		DeleteSoundMem(_sound);
		_sound = -1;
	}
}


// ���X�|�[���n�_�����擾����
auto Respawn::GetRespawnPointInfo(int stageNo, int respawnPointNo)
{
	RESPAWN_POINT respawnPoint[][RESPAWN_POINT_MAX] = {
		// stage01
		{
			// respawnPointNo, �}�b�v�`�b�v���W
			{1, VGet(236, 41, 0)}
		},

		// stage02
		{
			// respawnPointNo, �}�b�v�`�b�v���W
			   {1,VGet(165, 41, 0)}
		   }
	};

	return respawnPoint[stageNo][respawnPointNo];
}

void Respawn::Process(MYRECT playerRect, int meatBallSize)
{
	if (_isPlayingAnimation == 0) {

		// ���X�|�[���n�_�̍X�V�����邩�𒲂ׂ�
		int nextRespawnPoint = CheckRespawnPoint(playerRect);

		// ���X�|�[���n�_�̍X�V������ꍇ
		if (nextRespawnPoint != -1) {
			_isPlayingAnimation = 1;


			_respawnPointNo = nextRespawnPoint;

			// ���X�|�[���ʒu�̐ݒ�
			auto respawnPoint = GetRespawnPointInfo(_stageNo, _respawnPointNo - 1);

			// ����̃}�b�v�`�b�v���W����v���C���[�̋N�_���W���v�Z����
			VECTOR tmpPos = respawnPoint.pos;
			tmpPos.x = (tmpPos.x + 1) * CHIPSIZE_W + playerRect.w / 2;
			tmpPos.y = (tmpPos.y + 1) * CHIPSIZE_H + playerRect.h + 40;
			_playerPos = tmpPos;

			// ���X�|�[���n�_�X�V���̃~�[�g�{�[���̃T�C�Y��ێ�����
			_meatBallSize = meatBallSize;



			//�J�������
			// �������ł����̏����󂯎��Ȃ��̂́A���X�|�[���n�_�X�V���ɃJ�������ړ����Ă���ƃ��X�|�[����ɂ��J�����̈ړ����������邽��

			// �t�F�[�Y
			int camPhase[2][RESPAWN_POINT_MAX] = {
				// stage01
				{
					11,
				},

				// stage02
				{
					8,
				}
			};

			_camPhase = camPhase[_stageNo][_respawnPointNo - 1];

			// �J�������W
			VECTOR camPos[2][RESPAWN_POINT_MAX] = {
				// stage01
				{
					VGet(0, 2344, 0),
				},

				// stage02
				{
					VGet(0, 2520, 0),
				}
			};
			_camPos = camPos[_stageNo][_respawnPointNo - 1];

			
			PlaySoundMem(_sound, DX_PLAYTYPE_BACK);
		}
	}
	else {
		_animCnt++;
	}
}


// ���X�|�[���n�_�̍X�V�����邩�𒲂ׂ�
// �Ԃ�l 
// ���X�|�[���n�_�̍X�V�@-1:���Ȃ�, -1�ȊO: ����i�Ԃ�l�͍X�V��̃��X�|�[���n�_�ԍ��ƂȂ�j 
int Respawn::CheckRespawnPoint(MYRECT playerRect)
{
	for (int i = 0; i < RESPAWN_POINT_MAX; i++) {
		auto respawnPoint = GetRespawnPointInfo(_stageNo, i);

		// ���݂̃��X�|�[���n�_�ԍ����傫���ꍇ�ɂ̂ݔ�����s��
		if (_respawnPointNo < respawnPoint.respawnPointNo) {
			MYRECT rRect = { respawnPoint.pos.x * CHIPSIZE_W, respawnPoint.pos.y * CHIPSIZE_H, 192, 192 };


			if (IsHitBox(
				rRect.x, rRect.y, rRect.w, rRect.h,
				playerRect.x, playerRect.y, playerRect.w, playerRect.h
			) == 1) {
				// ���X�|�[���n�_�̍X�V������
				return respawnPoint.respawnPointNo;
			}
		}
	}
	// ���X�|�[���n�_�̍X�V�����Ȃ�
	return -1;
}






void Respawn::Draw(MATRIX mView)
{
	// �`�揈��
	for (int i = 0; i < RESPAWN_POINT_MAX; i++) {

		auto respawnPoint = GetRespawnPointInfo(_stageNo, i);

		VECTOR tPos = VGet(respawnPoint.pos.x * CHIPSIZE_W + 192 / 2, respawnPoint.pos.y * CHIPSIZE_H + 192 / 2, 0);

		if (_isPlayingAnimation == 0) {
			MyDrawModiGraph(mView, tPos, 1.0f, 0.0f, 192, 192, _cgHologram);
		}
		else {
			int animSpeed = 4;
			int animSheetsNum = 12;
			if (_animCnt < animSpeed * animSheetsNum) {
				MyDrawModiGraph(mView, tPos, 1.0f, 0.0f, 192, 192, _cgHologramAnimation[(_animCnt / animSpeed) % animSheetsNum]);
			}
		}
		MyDrawModiGraph(mView, tPos, 1.0f, 0.0f, 192, 192, _cgProjector);
	}
}

void Respawn::DrawDebugInfo(MATRIX mView, int fillFrag)
{

	int y = 0;
	DrawFormatString(1500, 900 + y * 16, COLOR_WHITE, "_respawnPointNo: %d", _respawnPointNo); y++;
	DrawFormatString(1500, 900 + y * 16, COLOR_WHITE, "_playerPos:  x %3.2f, y %3.2f, z %3.2f", _playerPos.x, _playerPos.y, _playerPos.z); y++;
	DrawFormatString(1500, 900 + y * 16, COLOR_WHITE, "_meatBallSize:  %d", _meatBallSize); y++;
	DrawFormatString(1500, 900 + y * 16, COLOR_WHITE, "_camPhase:  %d", _camPhase); y++;	
	DrawFormatString(1500, 900 + y * 16, COLOR_WHITE, "_camPos:  x %3.2f, y %3.2f, z %3.2f", _camPos.x, _camPos.y, _camPos.z); y++;


}
