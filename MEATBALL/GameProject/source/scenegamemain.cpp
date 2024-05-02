#include "DxLib.h"
#include "winmain.h"
#include "gamemain.h"
#include "mymath.h"
#include "mydraw.h"

#include "SceneGameMain.h"

#include "C_Scenario.h"
#include "B_Scenario.h"
#include "talk_end.h"
#include "explanation.h"




#include "player.h"
#include "meatball.h"

#include "enemybase.h"
#include "enemy_float.h"
#include "enemy_shooting.h"
#include "enemy_ground.h"
#include "enemy_shield.h"
#include "enemy_tracer.h"

#include "enemy_data.h"

#include "Heart.h"




#include "resource.h"

#include "effect.h"

#include "gimmick.h"
#include "gimmick_data.h"
#include "spike.h"
#include "lever.h"
#include "jumppad.h"
#include "bomb.h"
#include "dashpanel.h"
#include "UIPosBar.h"


#include "boss.h"

#include "gamepad.h"


#define COOLTIME_WIDTH 455


Player* player;
EnemyBase* enemy[ENEMY_MAX];
MeatBall* meatBall;
UIPosBar* uiposbar;

Boss* boss;
explanation* Explanation;



Camera gCam;
MapData	gMap;


JumpPad* jumpPad[JUMPPAD_MAX];
Lever* lever[LEVER_MAX];
Spike* spike[SPIKE_MAX];

BOMB* bomb[BOMB_MAX];
DASH_PANEL* dashPanel[DASH_PANEL_MAX];




int lightHandle;





int fontHandle;

// �G�̍��v��
int enemyNum = 0;


int gStageNo;


//
// 3D���f��
//

// �v���C���[
int playerModel = -1;
int plAfterImageModel = -1;
// �G
int enemyModel[3] = { -1, -1, -1 };
int shieldModel = -1;

// �~�[�g�{�[��
int meatBallModel = -1;
int mbAfterImageModel = -1;
// �{�X
int bossModel = -1;
int bossAfterImageModel = -1;



SceneGameMain::SceneGameMain() {
	gStageNo = 0;

	// �t�F�[�h�C���֘A�̏���������
	_fadeInStep = 0;
	ColorMask(0, 0, 0, 255);
	step = 0;
	

	_sceneMovie = nullptr;
	_sceneMovie = new SceneMovie(MOVIE_PROLOGUE);


	_sceneGameOver = nullptr;


	//_sceneMovie = new SceneMovie(MOVIE_CLEAR);

	player = nullptr;
	for (int i = 0; i < ENEMY_MAX; i++) {
		enemy[i] = nullptr;
	}
	meatBall = nullptr;
	uiposbar = nullptr;
	boss = nullptr;

	Explanation = nullptr;

	_respawn = nullptr;



	/*VECTOR gazePoint = ConvScreenPosToWorldPos(VGet(SCREEN_W / 2, SCREEN_H / 2, 0));
	SetCameraPositionAndTarget_UpVecY(VGet(gazePoint.x, gazePoint.y, -1000), VGet(gazePoint.x, gazePoint.y, 0));*/
	//SetCameraPositionAndAngle(VGet(0, 0, -1000), 45.f * DX_PI_F / 180.f, 0, 0);

	//�`���[�g���A���C�[�W���O�̏�����
	guide_start = 1080;
	guide_end = 808;
	guide_frame = 30;
	guide_cnt = 0;
	return_cnt = 0;
	start_cnt = 0;
	throw_cnt = 0;
	dash_cnt = 0;



	// �f�o�b�O�p�t�H���g�n���h���̐���
	fontHandle = CreateFontToHandle(NULL, 50, -1, -1);


	SceneGameMainResourceInit();


}

void SceneGameMain::SceneGameMainResourceInit()
{
	SetUseASyncLoadFlag(TRUE);

	//
	// �}�b�v�`�b�v
	//

	for (auto& cg : _cgChip) {
		cg = -1;
	}

	//�}�b�v�`�b�v�̓ǂݍ���
	LoadDivGraph("res/map/mapchip_01.png", 9, 3, 3, 64, 64, &_cgChip[CHIP_NOMAL]);
	LoadDivGraph("res/map/mapchip_02.png", 4, 2, 2, 64, 64, &_cgChip[CHIP_DIAGONAL_UPWARD]);
	LoadDivGraph("res/map/mapchip_03.png", 4, 2, 2, 64, 64, &_cgChip[CHIP_DIAGONAL_DOWNWARD]);
	//�w�i�}�b�v�`�b�v�ǂݍ���
	LoadDivGraph("res/map/stage_bgchip_01.png", 5, 1, 5, 64, 64, &_cgChip[CHIP_BACK]);
	LoadDivGraph("res/map/stage_bgchip_02.png", 5, 1, 5, 64, 64, &_cgChip[CHIP_BACK_WARNING]);
	LoadDivGraph("res/map/stage_bgchip_03.png", 9, 3, 3, 64, 64, &_cgChip[CHIP_BACK_FENCE]);
	LoadDivGraph("res/map/bgchip_hole_01.png", 9, 3, 3, 64, 64, &_cgChip[CHIP_BACK_HOLE]);
	LoadDivGraph("res/map/bgchip_hole_02.png", 4, 2, 2, 64, 64, &_cgChip[CHIP_BACK_HOLE_CORNER]);
	LoadDivGraph("res/map/bgchip_hole_03.png", 9, 3, 3, 64, 64, &_cgChip[CHIP_BACK_WARNING_HOLE]);
	LoadDivGraph("res/map/bgchip_hole_04.png", 4, 2, 2, 64, 64, &_cgChip[CHIP_BACK_WARNING_HOLE_CORNER]);
	//���C�g�t���w�i�}�b�v�`�b�v
	LoadDivGraph("res/map/zlight_bgchip_01_01.png", 5, 1, 5, 64, 64, &_cgChip[CHIP_BACK_LIGHT]);
	LoadDivGraph("res/map/zlight_bgchip_02_01.png", 5, 1, 5, 64, 64, &_cgChip[CHIP_BACK_WARNING_LIGHT]);
	//��
	LoadDivGraph("res/map/zbgchip_separate_01.png", 1, 1, 1, 64, 64, &_cgChip[CHIP_SEPERATE]);
	//���ƈł̃��C���[
	LoadDivGraph("res/map/light_01.png", 9, 3, 3, 64, 64, &_cgChip[CHIP_LIGHT]);
	LoadDivGraph("res/map/lightlayer_01.png", 9, 3, 3, 64, 64, &_cgChip[CHIP_LIGHT_LAYER]);
	LoadDivGraph("res/map/darklayer_01.png", 9, 3, 3, 64, 64, &_cgChip[CHIP_DARK]);
	LoadDivGraph("res/map/darklayer_02.png", 4, 2, 2, 64, 64, &_cgChip[CHIP_DARK_CORNER]);

	// �󂹂�u���b�N
	_cgChip[CHIP_BREAK + 0] = LoadGraph("res/map/block_01.png");
	_cgChip[CHIP_BREAK + 1] = LoadGraph("res/map/block_02.png");
	_cgChip[CHIP_BREAK + 2] = LoadGraph("res/map/block_03.png");
	_cgChip[CHIP_BREAK + 3] = LoadGraph("res/map/block_04.png");

	//
	LoadDivGraph("res/map/gimmick_barrier_sheet_01.png", 4, 4, 1, 64, 64, &_cgChip[CHIP_DOOR]);

	// �M�~�b�N�`�b�v
	for (auto& cg : cgDashPanel) {
		cg = -1;
	}
	for (auto& cg : cgBomb) {
		cg = -1;
	}
	LoadDivGraph("res/map/gimmick_dashpanel.png", 4, 4, 1, 64, 64, &cgDashPanel[0]);
	LoadDivGraph("res/map/gimmick_bomb.png", 6, 6, 1, 64, 64, &cgBomb[0]);

	//
	// UI�摜
	//
	
	for (auto& cg : meat_face) {
		cg = -1;
	}
	
	_cgUI_LevelBar = LoadGraph("res/UI/ui_player.png");
	_cgUI_BossBar = LoadGraph("res/UI/ui_boss.png");
	_cgUI_Bottom = LoadGraph("res/UI/UI_bottom_bar.png");

	meat_face[0] = LoadGraph("res/UI/meat.png");
	meat_face[1] = LoadGraph("res/UI/ui_frame_liquid.png");
	
	talkBG=LoadGraph("res/UI/talk_bg.png");


	//�`���[�g���A��UI
	instlower_right[0] = LoadGraph("res/UI/ingame_00.png");
	instlower_right[1] = LoadGraph("res/UI/ingame_01.png");
	instlower_right[2] = LoadGraph("res/UI/ingame_02.png");
	instlower_right[3] = LoadGraph("res/UI/ingame_03.png");
	instlower_right[4] = LoadGraph("res/UI/ingame_04.png");
	instlower_right[5] = LoadGraph("res/UI/ingame_05.png");



	_cgReturnFrame = LoadGraph("res/UI/frame_return.png");
	_cgReturnBar = LoadGraph("res/UI/bar_return.png");




	// �n�[�gUI�摜
	_cgUiHeart = LoadGraph("res/UI/heart/UI_heart.png");
	// ����UI�摜�i�n�[�g�̎c��J�E���g���̕\���p�j
	_cgUiNumber[0] = LoadGraph("res/UI/heart/0.png");
	_cgUiNumber[1] = LoadGraph("res/UI/heart/1.png");
	_cgUiNumber[2] = LoadGraph("res/UI/heart/2.png");
	_cgUiNumber[3] = LoadGraph("res/UI/heart/3.png");
	_cgUiNumber[4] = LoadGraph("res/UI/heart/4.png");
	_cgUiNumber[5] = LoadGraph("res/UI/heart/5.png");
	_cgUiNumber[6] = LoadGraph("res/UI/heart/6.png");
	_cgUiNumber[7] = LoadGraph("res/UI/heart/7.png");
	_cgUiNumber[8] = LoadGraph("res/UI/heart/8.png");
	_cgUiNumber[9] = LoadGraph("res/UI/heart/9.png");




	//
	// �G�t�F�N�g�摜
	//
	for (int i = 0; i < EFFECT_TYPE_MAX; i++) {
		for (auto& cg : cgEffect[i]) {
			cg = -1;
		}
	}
	// ���j�G�t�F�N�g
	LoadDivGraph("res/effect/effect_defeat_01/effect_defeat_01.png", 16, 16, 1, 368, 368, &cgEffect[EFFECT_TYPE_DEFEAT_MACHINE][0]);
	LoadDivGraph("res/effect/effect_defeat_creature/effect_defeat_creature.png", 25, 25, 1, 736, 736, &cgEffect[EFFECT_TYPE_DEFEAT_CREATURE][0]);
	// �ˌ��G �e
	LoadDivGraph("res/effect/effect_gun_shot_hit_01/effect_gun_shot_hit_01.png", 17, 17, 1, 368, 368, &cgEffect[EFFECT_TYPE_BULLET_HIT][0]);
	LoadDivGraph("res/effect/effect_gun_charge_r/effect_gun_charge_r.png", 13, 13, 1, 736, 736, &cgEffect[EFFECT_TYPE_BULLET_CHARGE][0]);
	// �u���b�N�j��
	LoadDivGraph("res/effect/effect_breakblock/effect_breakblock.png", 25, 25, 1, 736, 736, &cgEffect[EFFECT_TYPE_CHIP_BREAK][0]);
	// �{�X �e�G�t�F�N�g
	LoadDivGraph("res/effect/effect_gun_shot_hit_boss/effect_gun_shot_hit_boss.png", 16, 16, 1, 800, 800, &cgEffect[EFFECT_TYPE_BOSS_BULLET_HIT][0]);
	// �~�[�g�{�[���d���G�t�F�N�g
	// �A�ԉ摜��2���ɕ�����Ă��āA��������킹��1�̃G�t�F�N�g�ɂ���
	LoadDivGraph("res/effect/effect_electro/effect_electro_a.png", 20, 20, 1, 736, 736, &cgEffect[EFFECT_TYPE_MEATBALL_ELECTRO][0]);
	LoadDivGraph("res/effect/effect_electro/effect_electro_b.png", 20, 20, 1, 736, 736, &cgEffect[EFFECT_TYPE_MEATBALL_ELECTRO][20]);
	// ���G�t�F�N�g
	LoadDivGraph("res/effect/effect_player_jump/effect_player_jump.png", 20, 20, 1, 736, 736, &cgEffect[EFFECT_TYPE_SMOKE][0]);


	LoadDivGraph("res/effect/effect_back_b/effect_back_b.png", 13, 13, 1, 736, 736, &cgEffect[EEFECT_TYPE_MEATBALL_RETURN][0]);


	


	// �v���C���[ �T�C�R�L�l�V�X
	for (auto& cg : cgAttackAura) {
		cg = -1;
	}
	LoadDivGraph("res/effect/effect_aura_set/effect_aura.png", 20, 20, 1, 736, 736, &cgAttackAura[0]);
	for (auto& cg : cgReturnAura) {
		cg = -1;
	}
	LoadDivGraph("res/effect/effect_player_call_2/effect_player_call.png", 13, 13, 1, 736, 736, &cgReturnAura[0]);


	// �~�[�g�{�[�� �Ăі߂��G�t�F�N�g
	for (auto& cg : cgMeatBallReturn) {
		cg = -1;
	}
	LoadDivGraph("res/effect/effect_back_a/effect_back_a.png", 13, 13, 1, 736, 736, &cgMeatBallReturn[0]);


	// �~�[�g�{�[�� ���˕����̗\����
	MeatBall::_cgAim = -1;
	MeatBall::_cgAim = LoadGraph("res/UI/ui_aim.png");



	// �e
	for (auto& cg : cgBullet) {
		cg = -1;
	}
	for (auto& cg : cgBossBullet) {
		cg = -1;
	}
	// �ˌ��G�E�e
	LoadDivGraph("res/effect/effect_gun_shot.png", 30, 30, 1, 368, 368, &cgBullet[0]);
	// �{�X �e
	LoadDivGraph("res/effect/effect_gun_shot_boss/effect_gun_shot_boss.png", 30, 30, 1, 368, 368, &cgBossBullet[0]);


	
	//
	// �e�N�X�`���摜
	//

	// ��_���[�W�� �^�����̃e�N�X�`��
	// �S�ẴL�����N�^�[����
	CharaBase::_cgTexWhite = -1;
	CharaBase::_cgTexWhite = LoadGraph("res/model/white.png");




	// �n��G �e�N�X�`���摜
	for (auto& cg : EnemyGround::_cgTexEnGround) {
		cg = -1;
	}
	// �ʏ�
	EnemyGround::_cgTexEnGround[0] = LoadGraph("res/model/enemy/ground/ground_low_Enemy_low1_BaseColor.png");
	// ����I�i���^���G�j
	EnemyGround::_cgTexEnGround[1] = LoadGraph("res/model/enemy/ground/ground_low_Enemy_low1_Blood_BaseColor.png");



	

	



	//
	// 3D���f��
	//

	// �v���C���[
	playerModel = MV1LoadModel("res/model/cg_player_girl/cg_player_girl.mv1");
	plAfterImageModel = MV1LoadModel("res/model/cg_player_girl/cg_player_girl_afterimage.mv1");

	Player::TextureInit(playerModel);
	Player::MaterialInit(playerModel);



	// �M�~�b�N
	Spike::_spikeModel = MV1LoadModel("res/model/gimmick/needle/cg_gimmick_needle.mv1");
	Lever::_leverModel[LEVER_MODEL_RED] = MV1LoadModel("res/model/gimmick/lever/cg_gimmick_lever.mv1");
	Lever::_leverModel[LEVER_MODEL_BLUE] = MV1LoadModel("res/model/gimmick/lever_blue/cg_gimmick_lever_blue.mv1");
	JumpPad::_jumpPadModel = MV1LoadModel("res/model/gimmick/jumppad/cg_gimmick_jumppad.mv1");


	// �G
	const char* enemyModelList[3] = {
		{
			// ���V�G
			"res/model/enemy/float/cg_enemy_float.mv1"
		},
		{
			// �ˌ��G �i���g�p�j
			"res/model/enemy/shooting/cg_enemy_shooting.mv1"
		},
		{
			// �n��G
			"res/model/enemy/ground/cg_enemy_ground.mv1"
		}
	};

	for (int i = 0; i < 3; i++) {
		enemyModel[i] = MV1LoadModel(enemyModelList[i]);
	}
	EnemyFloat::TextureInit(enemyModel[0]);
	EnemyFloat::MaterialInit(enemyModel[0]);

	EnemyShooting::TextureInit(enemyModel[0]);
	EnemyShooting::MaterialInit(enemyModel[0]);

	EnemyShield::TextureInit(enemyModel[0]);
	EnemyShield::MaterialInit(enemyModel[0]);

	EnemyTracer::TextureInit(enemyModel[0]);
	EnemyTracer::MaterialInit(enemyModel[0]);


	// ��
	EnemyShield::_shieldModelBase = -1;
	EnemyShield::_shieldModelBase = MV1LoadModel("res/model/enemy/shield/cg_shield.mv1");


	// �~�[�g�{�[��
	meatBallModel = MV1LoadModel("res/model/meatball/cg_player_meatball.mv1");
	mbAfterImageModel = MV1LoadModel("res/model/meatball/cg_player_meatball.mv1");



	// �{�X
	bossModel = MV1LoadModel("res/model/enemy/boss/cg_enemy_boss_alt.mv1");
	bossAfterImageModel = MV1LoadModel("res/model/enemy/boss/cg_enemy_boss_alt_afterimage.mv1");
	Boss::TextureInit(bossModel);
	Boss::MaterialInit(bossModel);
	


	//
	// �@�B�p�[�c���f��
	//

	const char* metalPartsModelList[3] = {
		{
			// ����
			"res/model/mb_mekaparts_mv1/gear.mv1"
		},
		{
			// �{���g
			"res/model/mb_mekaparts_mv1/bolt.mv1"
		},
		{
			// �S��
			"res/model/mb_mekaparts_mv1/metal.mv1"
		}
	};


	for (int i = 0; i < 3; i++) {
		metalPartsModel[i] = -1;
		metalPartsModel[i] = MV1LoadModel(metalPartsModelList[i]);
	}



	// �{�X��BGM
	Boss::LoadBossBGM();

}


void SceneGameMain::StageInit(int stageNo)
{
	// �t�F�[�h�C���֘A�̏���������
	_fadeInStep = 0;
	ColorMask(0, 0, 0, 255);
	

	_isTransition = 0;
	_transitionCnt = 0;


	EffectInit();

	gMap.MapInit(stageNo);

	gCam.Init(gStageNo);


	uiposbar = new UIPosBar();
	if (stageNo == 0) {
		Explanation = new explanation(0);
	}

	// �v���C���[�̏��������� 
	if (stageNo == 0) {
		player = new Player(playerModel, VGet(10.5 * CHIPSIZE_W, 24.5 * CHIPSIZE_H, 0));
		/*	player = new Player(playerModel, VGet(383 * CHIPSIZE_W, 12 * CHIPSIZE_H, 0));*/
		/*	player = new Player(playerModel, VGet(410 * CHIPSIZE_W, 23 * CHIPSIZE_H, 0));
			gCam._phase = 13;*/
	}
	else {
		player = new Player(playerModel, VGet(5 * CHIPSIZE_W, 21 * CHIPSIZE_H, 0));

		/*player = new Player(playerModel, VGet(148 * CHIPSIZE_W, 34 * CHIPSIZE_H, 0));*/
	


		// �f�o�b�O�p
		// �{�X�O
		//player = new Player(playerModel, VGet(210 * CHIPSIZE_W, 43 * CHIPSIZE_H, 0));
		//gCam._phase = 98;


	}

	player->AfterImageInit(plAfterImageModel);


	// �M�~�b�N�̏���������
	for (int i = 0; i < LEVER_MAX; i++) {
		lever[i] = new Lever(stageNo, i);
	}
	for (int i = 0; i < SPIKE_MAX; i++) {
		spike[i] = new Spike(stageNo, i);
	}
	for (int i = 0; i < JUMPPAD_MAX; i++) {
		jumpPad[i] = new JumpPad(stageNo, i);
	}




	for (int i = 0; i < BOMB_MAX; i++) {
		GIMMICK_DATA bombData = GetBombData(stageNo, i);
		bomb[i] = new BOMB(bombData.id, bombData.x, bombData.y);
	}

	for (int i = 0; i < DASH_PANEL_MAX; i++) {
		GIMMICK_DATA dashPanelData = GetDashPanelData(stageNo, i);
		dashPanel[i] = new DASH_PANEL(dashPanelData.id, dashPanelData.x, dashPanelData.y);
	}

	// �G�̏���������
	enemyNum = 0;
	int i = 0;
	int j = 0;
	for (i; i < i + ENEMY_FLOAT_MAX; i++, j++) {
		auto eData = GetFloatData(stageNo, j);
		if (eData.pos.x == -1) {
			j = 0;
			break;
		}
		enemy[i] = new EnemyFloat(enemyModel[0], stageNo, eData.size, j, eData.pos, eData.direction);
		enemyNum++;
	}

	// �����̏���������
	InitShieldInfo();

	for (i; i < i + ENEMY_SHOOTING_MAX; i++, j++) {
		auto eData = GetShootingData(stageNo, j);
		if (eData.pos.x == -1) {
			j = 0;
			break;
		}
		enemy[i] = new EnemyShooting(enemyModel[0], eData.size, j, eData.pos);
		enemyNum++;
	}

	for (i; i < i + ENEMY_GROUND_MAX; i++, j++) {
		auto eData = GetGroundData(stageNo, j);
		if (eData.pos.x == -1) {
			j = 0;
			break;
		}
		enemy[i] = new EnemyGround(enemyModel[2], eData.size, j, eData.pos, eData.type);
		enemyNum++;
	}

	for (i; i < i + ENEMY_SHIELD_MAX; i++, j++) {
		auto eData = GetShieldData(stageNo, j);
		if (eData.pos.x == -1) {
			j = 0;
			break;
		}
		enemy[i] = new EnemyShield(enemyModel[0], eData.size, j, eData.pos, eData.type);
		enemyNum++;
	}

	for (i; i < i + ENEMY_TRACER_MAX; i++, j++) {
		auto eData = GetTracerData(stageNo, j);
		if (eData.pos.x == -1) {
			j = 0;
			break;
		}
		enemy[i] = new EnemyTracer(enemyModel[0], eData.size, j, eData.pos);
		enemyNum++;
	}





	// �~�[�g�{�[���̏���������
	meatBall = new MeatBall(meatBallModel, player->GetPos());
	int frameNo = MV1SearchFrame(player->GetModelHandle(), "tank_center");
	meatBall->SetAttachToPlayer(player->GetModelHandle(), frameNo);
	meatBall->AfterImageInit(mbAfterImageModel);
	meatBall->MetalPartsInit(metalPartsModel);





	// �{�X�̏���������
	if (stageNo == 1) {
		boss = new Boss(bossModel, VGet(234 * CHIPSIZE_W, 42 * CHIPSIZE_H, 0));
		boss->AfterImageInit(bossAfterImageModel);
	}



}


SceneGameMain::~SceneGameMain() {
	delete _sceneMovie;
	StageRelease();
	SceneGameMainResourceRelease();

	if (_respawn != NULL) {
		delete _respawn;
		_respawn = nullptr;
	}
}

void SceneGameMain::StageRelease() {
	if (player != NULL) {
		delete player;
		player = nullptr;
	}
	if (meatBall != NULL) {
		delete meatBall;
		meatBall = nullptr;
	}

	if (uiposbar != NULL) {
		delete uiposbar;
		uiposbar = nullptr;
	}
	for (int i = 0; i < enemyNum; i++) {
		if (enemy[i] != NULL) {
			delete enemy[i];
			enemy[i] = nullptr;
		}
	}

	for (int i = 0; i < JUMPPAD_MAX; i++) {
		if (jumpPad[i] != NULL) {
			delete jumpPad[i];
			jumpPad[i] = nullptr;
		}
	}
	for (int i = 0; i < LEVER_MAX; i++) {
		if (lever[i] != NULL) {
			delete lever[i];
			lever[i] = nullptr;
		}
	}
	for (int i = 0; i < SPIKE_MAX; i++) {
		if (spike[i] != NULL) {
			delete spike[i];
			spike[i] = nullptr;
		}
	}
	for (int i = 0; i < BOMB_MAX; i++) {
		if (bomb[i] != NULL) {
			delete bomb[i];
			bomb[i] = nullptr;
		}
	}
	for (int i = 0; i < DASH_PANEL_MAX; i++) {
		if (dashPanel[i] != NULL) {
			delete dashPanel[i];
			dashPanel[i] = nullptr;
		}
	}

}


// �摜�����\�[�X��ǂݍ��񂾃������̉��
void SceneGameMain::SceneGameMainResourceRelease()
{
	// �}�b�v�`�b�v
	for (auto& cg : _cgChip) {
		if (cg != -1) {
			DeleteGraph(cg);
			cg = -1;
		}
	}

	// �M�~�b�N�`�b�v
	for (auto& cg : cgDashPanel) {
		if (cg != -1) {
			DeleteGraph(cg);
			cg = -1;
		}
	}
	for (auto& cg : cgBomb) {
		if (cg != -1) {
			DeleteGraph(cg);
			cg = -1;
		}
	}

	// UI�摜
	
	for (auto& cg : meat_face) {
		if (cg != -1) {
			DeleteGraph(cg);
			cg = -1;
		}
	}


	if (_cgUiHeart != -1) {
		DeleteGraph(_cgUiHeart);
		_cgUiHeart = -1;
	}

	for (auto& cg : _cgUiNumber) {
		if (cg != -1) {
			DeleteGraph(cg);
			cg = -1;
		}
	}


	for (auto& cg : instlower_right) {
		if (cg != -1) {
			DeleteGraph(cg);
			cg = -1;
		}
	}

	// �G�t�F�N�g�摜
	for (int i = 0; i < EFFECT_TYPE_MAX; i++) {
		for (auto& cg : cgEffect[i]) {
			if (cg != -1) {
				DeleteGraph(cg);
				cg = -1;
			}
		}
	}


	// �v���C���[ �T�C�R�L�l�V�X
	for (auto& cg : cgAttackAura) {
		if (cg != -1) {
			int n = DeleteGraph(cg);
			cg = -1;
		}
	}
	for (auto& cg : cgReturnAura) {
		if (cg != -1) {
			int n = DeleteGraph(cg);
			cg = -1;
		}
	}


	// �~�[�g�{�[�� �Ăі߂��G�t�F�N�g
	for (auto& cg : cgMeatBallReturn) {
		if (cg != -1) {
			DeleteGraph(cg);
			cg = -1;
		}
	}

	// �~�[�g�{�[�� ���˕����̗\����
	if (MeatBall::_cgAim != -1) {
		 DeleteGraph(MeatBall::_cgAim);
		 MeatBall::_cgAim = -1;
	}

	// �e
	for (auto& cg : cgBullet) {
		if (cg != -1) {
			DeleteGraph(cg);
			cg = -1;
		}
	}
	for (auto& cg : cgBossBullet) {
		if (cg != -1) {
			DeleteGraph(cg);
			cg = -1;
		}
	}


	// �Ăі߂��Q�[�W
	if (_cgReturnFrame != -1) {
		DeleteGraph(_cgReturnFrame);
		_cgReturnFrame = -1;
	}
	if (_cgReturnBar != -1) {
		DeleteGraph(_cgReturnBar);
		_cgReturnBar = -1;
	}
	
	
	if (CharaBase::_cgTexWhite != -1) {
		DeleteGraph(CharaBase::_cgTexWhite);
		CharaBase::_cgTexWhite = -1;
	}

	// �n��G �e�N�X�`���摜
	for (auto& cg : EnemyGround::_cgTexEnGround) {
		if (cg != -1) {
			DeleteGraph(cg);
			cg = -1;
		}
	}

	//
	// 3D���f��
	// 
	

	// ��
	if (EnemyShield::_shieldModelBase != -1) {
		MV1DeleteModel(EnemyShield::_shieldModelBase);
		EnemyShield::_shieldModelBase = -1;
	}

	// �v���C���[
	if (playerModel != -1) {
		MV1DeleteModel(playerModel);
		playerModel = -1;
	}
	// �v���C���[�c��
	if (plAfterImageModel != -1) {
		MV1DeleteModel(plAfterImageModel);
		plAfterImageModel = -1;
	}
	
	// �Ƃ�
	if (Spike::_spikeModel != -1) {
		MV1DeleteModel(Spike::_spikeModel);
		Spike::_spikeModel = -1;
	}

	// ���o�[
	for (int i = 0; i < 2; i++) {
		if (Lever::_leverModel[i] != -1) {
			MV1DeleteModel(Lever::_leverModel[i]);
			Lever::_leverModel[i] = -1;
		}
	}

	// �W�����v��
	if (JumpPad::_jumpPadModel != -1) {
		MV1DeleteModel(JumpPad::_jumpPadModel);
		JumpPad::_jumpPadModel = -1;
	}

	// �G
	for (int i = 0; i < 3; i++) {
		if (enemyModel[i] != -1) {
			MV1DeleteModel(enemyModel[i]);
			enemyModel[i] = -1;
		}
	}

	// �~�[�g�{�[��
	if (meatBallModel != -1) {
		MV1DeleteModel(meatBallModel);
		meatBallModel = -1;
	}
	// �~�[�g�{�[���c��
	if (mbAfterImageModel != -1) {
		MV1DeleteModel(mbAfterImageModel);
		mbAfterImageModel = -1;
	}
	
	// �{�X
	if (bossModel != -1) {
		MV1DeleteModel(bossModel);
		bossModel = -1;
	}
	// �{�X�c��
	if (bossAfterImageModel != -1) {
		MV1DeleteModel(bossAfterImageModel);
		bossAfterImageModel = -1;
	}

	// �@�B�p�[�c���f��
	for (int i = 0; i < 3; i++) {
		if (metalPartsModel[i] != -1) {
			MV1DeleteModel(metalPartsModel[i]);
			metalPartsModel[i] = -1;

		}
	}

	// �{�X��BGM
	Boss::DeleteBossBGM();

}

void SceneGameMain::Input() {

}



void SceneGameMain::Process() {

	switch (_fadeInStep) {
	case 0:
		// �t�F�[�h�C���J�n
		ColorFadeIn(60);
		_fadeInStep++;
		break;
	case 1:
		if (IsColorFade() == 0) {
			// �t�F�[�h�C���I��
			_fadeInStep++;

		}
		break;
	}

	if (_sceneMovie == NULL && _sceneGameOver ==NULL) {





		//// �J���p
		//// ���X�|�[������
		//if (gPad->_trg[PAD_BUTTON_LSTICK] != 0) {
		//	SetRespawn();
		//}



		// �G�t�F�N�g
		EffectProcess();

		// �G
		for (int i = 0; i < enemyNum; i++) {
			enemy[i]->Process(player->GetCenterPos());
		}


		// ���o�[
		for (int i = 0; i < LEVER_MAX; i++) {
			lever[i]->Process();
		}
		// �W�����v��
		for (int i = 0; i < JUMPPAD_MAX; i++) {
			jumpPad[i]->Process();
		}

		// �v���C���[
		player->Process();
		// �~�[�g�{�[��
		meatBall->Process(player->GetCenterPos());


		// ���X�|�[���n�_�̍X�V�����邩�𒲂ׂ�
		//respawn->CheckRespawnPoint(player->GetMyRect(COLLISION_TYPE_BODY));
		_respawn->Process(player->GetMyRect(COLLISION_TYPE_BODY), meatBall->GetSize());





		if (gStageNo == 1) {
			boss->Process(player->GetPos());
		}


		for (int i = 0; i < BOMB_MAX; i++) {
			bomb[i]->Process();
		}

		//
		// �{�[���Ƒ��I�u�W�F�N�g�̓����蔻��
		//
		if (meatBall->GetIsAttack() == 1) {
			VECTOR meatBallPos = meatBall->GetPos();
			float meatBallRadius = meatBall->GetRadius();

			// �M�~�b�N�Ƃ̓����蔻��
			// ���e�Ƃ̓����蔻��
			for (int i = 0; i < BOMB_MAX; i++) {
				if (bomb[i]->GetUse() == 1 && bomb[i]->GetExplosion() == 0) {
					MYRECT bRect = bomb[i]->GetRect(BOMB_COLLISION_TYPE_BODY);
					if (IsHitCircleAndBox(meatBallPos.x, meatBallPos.y, meatBallRadius, bRect) == 1) {
						bomb[i]->SetExplosion();
					}
				}
			}

			// �����p�l���Ƃ̓����蔻��
			for (int i = 0; i < DASH_PANEL_MAX; i++) {
				if (dashPanel[i]->GetUse() == 1) {
					MYRECT panelRect = dashPanel[i]->GetRect();
					if (IsHitCircleAndBox(meatBallPos.x, meatBallPos.y, meatBallRadius, panelRect) == 1) {
						meatBall->SpeedUp();
						//meatBall->SetHitDashPanel();
						dashPanel[i]->SetHit();
						PlaySoundMem(se[18], DX_PLAYTYPE_BACK);
					}
				}
			}


		}



		//
		// �����͈͂̓����蔻��
		// 
		
		// ���̔��e�ɓ��������ꍇ�A���̔��e�̃J�E���g�_�E���^�C�}�[���Z�b�g����
		for (int i = 0; i < BOMB_MAX; i++) {

			if (bomb[i]->GetUse() == 1 && bomb[i]->GetExplosion() == 1) {
				bomb[i]->ExplodeChip();

				MYRECT iRect = bomb[i]->GetRect(BOMB_COLLISION_TYPE_EXPLOSION);

				// �ʂ̔��e�Ƃ̓����蔻��
				for (int j = 0; j < BOMB_MAX; j++) {
					if (i == j) continue;
					if (bomb[j]->GetUse() == 1 && bomb[j]->GetExplosion() == 0) {

						MYRECT jRect = bomb[j]->GetRect(BOMB_COLLISION_TYPE_BODY);
						if (IsHitBox(iRect.x, iRect.y, iRect.w, iRect.h, jRect.x, jRect.y, jRect.w, jRect.h) == 1) {
							bomb[j]->SetCountDown();
						}
					}
				}


				// �v���C���[�Ƃ̓����蔻��
				if (player->GetIsValidHit() == 1 && player->GetGameOverMotion() != 1) {
					MYRECT pRect = player->GetMyRect(COLLISION_TYPE_BODY);
					if (IsHitBox(iRect.x, iRect.y, iRect.w, iRect.h, pRect.x, pRect.y, pRect.w, pRect.h) == 1) {
						player->SetIsAttacked(1);
						player->SetGravity(0);

						if (player->_heart.GetUse() != 1) {
							VECTOR pPos = player->GetPos();
							VECTOR bPos = VGet(bomb[i]->GetPosX() * CHIPSIZE_W + CHIPSIZE_W / 2, bomb[i]->GetPosY() * CHIPSIZE_H + CHIPSIZE_H / 2, 0);
							int heartDirX = pPos.x < bPos.x ? -1 : 1;
							player->_heart.SetDirection(heartDirX);
						}
					}
				}

				// �G�Ƃ̓����蔻��
				for (int i = 0; i < enemyNum; i++) {
					if (enemy[i]->GetUse() == 1 && enemy[i]->GetHitStop() == 0) {
						MYRECT eRect = enemy[i]->GetMyRect(COLLISION_TYPE_BODY);
						if (IsHitBox(iRect.x, iRect.y, iRect.w, iRect.h, eRect.x, eRect.y, eRect.w, eRect.h) == 1) {
							//if (enemy[i]->GetHP() <= 0) {


							enemy[i]->SetUse(0);


							/*enemy[i]->SetHitStop(meatBall->GetDirection());*/

							switch (enemy[i]->GetType())
							{
							case ENEMY_TYPE_SHIELD:
								UpdateShieldInfo(enemy[i]->GetIndex(), 0);
								break;

							case ENEMY_TYPE_GROUND:
								enemy[i]->ChangeToNormal();
								break;
							}

						/*	if (enemy[i]->GetType() == ENEMY_TYPE_SHIELD) {
								UpdateShieldInfo(enemy[i]->GetIndex(), 0);
							}*/




							VECTOR effectPos = enemy[i]->GetPos();
							if (enemy[i]->GetType() == ENEMY_TYPE_GROUND) {
								// �����G���j�G�t�F�N�g�̃Z�b�g
								float effectSize = eRect.w * 3;
								SetEffect(EFFECT_TYPE_DEFEAT_CREATURE, eRect.x - eRect.w, eRect.y - eRect.h / 2, effectSize, effectSize);

								// �����G���jSE�̃Z�b�g
								PlaySoundMem(se[7], DX_PLAYTYPE_BACK);
							}
							else {
								// �@�B�G���j�G�t�F�N�g�̃Z�b�g
								float effectSize = eRect.w * 3;
								SetEffect(EFFECT_TYPE_DEFEAT_MACHINE, eRect.x - eRect.w, eRect.y - eRect.h, effectSize, effectSize);

								// �@�B�G���j�G�t�F�N�g�̃Z�b�g
								PlaySoundMem(se[6], DX_PLAYTYPE_BACK);
							}
							//}
						}
					}
				}


				bomb[i]->Delete();
			}
		}






		//
		// �{�[���ƓG�̓����蔻��
		//
		if (meatBall->GetIsAttack() == 1) {
			VECTOR meatBallPos = meatBall->GetPos();
			float meatBallRadius = meatBall->GetRadius();

			// �{�[���ƓG�̓����蔻��
			for (int i = 0; i < enemyNum; i++) {
				if (enemy[i]->GetUse() == 1 && enemy[i]->GetHitStop() == 0) {
					MYRECT eRect = enemy[i]->GetMyRect(COLLISION_TYPE_BODY);
					if (IsHitCircleAndBox(meatBallPos.x, meatBallPos.y, meatBallRadius, eRect) == 1) {
						PlaySoundMem(voice[(rand() % 2) + 9], DX_PLAYTYPE_BACK);
						
						meatBall->SetScaling();

						int mPower = meatBall->GetPower();

						if (enemy[i]->GetType() == ENEMY_TYPE_GROUND) {
							if (enemy[i]->GetIsMetal() == 0) {
								enemy[i]->UpdateHp(-mPower);
								enemy[i]->SetIsHit();
							}
							else {
								if (meatBall->GetSpeed() >= 30) {
									enemy[i]->ChangeToNormal();
									enemy[i]->SetIsHit();
								}
							}
						}
						else {
							enemy[i]->UpdateHp(-mPower);
							enemy[i]->SetIsHit();
						}

						if (enemy[i]->GetHP() <= 0) {
							meatBall->UpdateSize(1);
							meatBall->SetHitStopCnt();


							//enemy[i]->SetUse(0);


							enemy[i]->SetHitStop(meatBall->GetDirection());
							if (enemy[i]->GetType() == ENEMY_TYPE_SHIELD) {
								UpdateShieldInfo(enemy[i]->GetIndex(), 0);
							}




							VECTOR effectPos = enemy[i]->GetPos();
							if (enemy[i]->GetType() == ENEMY_TYPE_GROUND) {

								// �����G���j�G�t�F�N�g�̃Z�b�g
								float effectSize = eRect.w * 3;
								SetEffect(EFFECT_TYPE_DEFEAT_CREATURE, eRect.x - eRect.w, eRect.y - eRect.h / 2, effectSize, effectSize);

								// �����G���jSE�̃Z�b�g
								PlaySoundMem(se[7], DX_PLAYTYPE_BACK);
							}
							else {
								// �@�B�G���j�G�t�F�N�g�̃Z�b�g
								float effectSize = eRect.w * 3;
								SetEffect(EFFECT_TYPE_DEFEAT_MACHINE, eRect.x - eRect.w, eRect.y - eRect.h, effectSize, effectSize);

								// �@�B�G���j�G�t�F�N�g�̃Z�b�g
								PlaySoundMem(se[6], DX_PLAYTYPE_BACK);
							}
						}
						else {
							enemy[i]->SetIsSpotPlayer(1);
							meatBall->Reflect(eRect);
							PlaySoundMem(se[28], DX_PLAYTYPE_BACK);
						}
					}
				}
			}






			// �{�X�Ƃ̓����蔻��
			if (gStageNo == 1 && boss->GetUse() == 1 && boss->GetBossBattle() == 1) {
				MYRECT bRect = boss->GetMyRect(COLLISION_TYPE_BODY);

				MYRECT mRect = meatBall->GetReflectRect();
				if (IsHitBox(mRect.x, mRect.y, mRect.w, mRect.h,
					bRect.x, bRect.y, bRect.w, bRect.h) == 1) {
					//if (IsHitCircleAndBox(meatBallPos.x, meatBallPos.y, meatBallRadius, bRect) == 1) {
					float mPower = meatBall->GetPower();
					//boss->UpdateHp(-mPower);
					boss->SetIsHit(-mPower);
					PlaySoundMem(se[28], DX_PLAYTYPE_BACK);
					PlaySoundMem(voice[(rand() % 2) + 9], DX_PLAYTYPE_BACK);
					meatBall->Reflect(bRect);
				}
			}


		}

		//
		// �v���C���[�ƓG�̓����蔻��
		// 

		// �v���C���[�ƈȉ�3�_�Ƃ̓����蔻����s��
		// 1.�G�{��
		// 2.���G�͈�
		// 3.�e
		if (player->GetIsValidHit() == 1 && player->GetGameOverMotion() != 1) {
			MYRECT pRect = player->GetMyRect(COLLISION_TYPE_BODY);
			for (int i = 0; i < enemyNum; i++) {
				if (enemy[i]->GetUse() == 1 && enemy[i]->GetIsInScreen() == 1) {

					// 1.�G�{�̂Ƃ̓����蔻��
					MYRECT eRect = enemy[i]->GetMyRect(COLLISION_TYPE_BODY);
					if (IsHitBox(pRect.x, pRect.y, pRect.w, pRect.h, eRect.x, eRect.y, eRect.w, eRect.h) == 1) {
						player->SetIsAttacked(1);
						player->SetGravity(0);

						if (player->_heart.GetUse() != 1) {
							VECTOR pPos = player->GetPos();
							VECTOR ePos = enemy[i]->GetPos();
							int heartDirX = pPos.x < ePos.x ? -1 : 1;
							player->_heart.SetDirection(heartDirX);
						}
					}

					MYRECT eSearchRect;
					MYRECT eAttackRect;

					switch (enemy[i]->GetType())
					{
					case ENEMY_TYPE_SHOOTING:
						// 2.���G�͈͂Ƃ̓����蔻��i���G�͈͂Ƀv���C���[�������Ă��邩�j
						eSearchRect = enemy[i]->GetMyRect(COLLISION_TYPE_SEARCH);
						if (IsHitBox(pRect.x, pRect.y, pRect.w, pRect.h, eSearchRect.x, eSearchRect.y, eSearchRect.w, eSearchRect.h) == 1) {
							enemy[i]->SetIsSpotPlayer(1);
						}
						break;
					case ENEMY_TYPE_GROUND:
						// 2.���G�͈͂Ƃ̓����蔻��i���G�͈͂Ƀv���C���[�������Ă��邩
						// �v���C���[��Ǐ]����͈͂ɓ����Ă��邩
						eSearchRect = enemy[i]->GetMyRect(COLLISION_TYPE_SEARCH);
						if (IsHitBox(pRect.x, pRect.y, pRect.w, pRect.h, eSearchRect.x, eSearchRect.y, eSearchRect.w, eSearchRect.h) == 1) {
							enemy[i]->SetIsSpotPlayer(1);
						}
						// �_�b�V���U��������͈͂ɓ����Ă��邩
						eAttackRect = enemy[i]->GetMyRect(COLLISION_TYPE_ATTACK);
						if (eAttackRect.x != -1) {
							if (IsHitBox(pRect.x, pRect.y, pRect.w, pRect.h, eAttackRect.x, eAttackRect.y, eAttackRect.w, eAttackRect.h) == 1) {
								if (enemy[i]->GetAttack() == 0 && enemy[i]->GetCoolTime() == 0) {
									enemy[i]->SetAttack(1);

									VECTOR pPos = player->GetPos();
									VECTOR ePos = enemy[i]->GetPos();

									VECTOR vDir;
									if (pPos.x < ePos.x) {
										vDir = VGet(-1, 0, 0);
									}
									else {
										vDir = VGet(1, 0, 0);
									}
									enemy[i]->SetDirection(vDir);
								}
							}
							else {
								//enemy[i]->SetAttack(0);
							}
						}
						break;
					case ENEMY_TYPE_SHIELD:
					case ENEMY_TYPE_TRACER:
						// 2.���G�͈͂Ƃ̓����蔻��i���G�͈͂Ƀv���C���[�������Ă��邩�j
						eSearchRect = enemy[i]->GetMyRect(COLLISION_TYPE_SEARCH);
						if (IsHitBox(pRect.x, pRect.y, pRect.w, pRect.h, eSearchRect.x, eSearchRect.y, eSearchRect.w, eSearchRect.h) == 1) {
							enemy[i]->SetIsSpotPlayer(1);
						}
						break;
					}
				}

				if (enemy[i]->GetType() == ENEMY_TYPE_SHOOTING) {
					// 3.�e�Ƃ̓����蔻��
					for (int j = 0; j < BULLET_MAX; j++) {
						MYRECT eAttackRect = enemy[i]->GetMyRect(COLLISION_TYPE_ATTACK);
						if (eAttackRect.x != -1) {
							if (IsHitBox(pRect.x, pRect.y, pRect.w, pRect.h, eAttackRect.x, eAttackRect.y, eAttackRect.w, eAttackRect.h) == 1) {
								player->SetIsAttacked(1);
								player->SetGravity(0);

								enemy[i]->SetAttack(0);

								// �e�j��G�t�F�N�g�̃Z�b�g
								VECTOR centerPos = VGet(eAttackRect.x, eAttackRect.y, 0);
								centerPos.x += eAttackRect.w / 2;
								centerPos.y += eAttackRect.h / 2;
								float effectSize = eAttackRect.w * 3 * 1.5f;
								SetEffect(EFFECT_TYPE_BULLET_HIT, centerPos.x - effectSize / 2, centerPos.y - effectSize / 2, effectSize, effectSize);


								if (player->_heart.GetUse() != 1) {
									VECTOR pPos = player->GetPos();
									VECTOR ePos = enemy[i]->GetPos();
									int heartDirX = pPos.x < ePos.x ? -1 : 1;
									player->_heart.SetDirection(heartDirX);
								}
							}
						}
					}
				}
			}

			// �Ƃ� �Ƃ̓����蔻��
			for (int i = 0; i < SPIKE_MAX; i++) {
				if (spike[i]->GetUse() == 1) {
					MYRECT sRect = spike[i]->GetRect();
					if (IsHitBox(pRect.x, pRect.y, pRect.w, pRect.h, sRect.x, sRect.y, sRect.w, sRect.h) == 1) {
						player->SetIsAttacked(1);
						player->SetGravity(0);

						if (player->_heart.GetUse() != 1) {
							VECTOR pPos = player->GetPos();
							VECTOR sPos = spike[i]->GetPos();
							int heartDirX = pPos.x < sPos.x ? -1 : 1;
							player->_heart.SetDirection(heartDirX);
						}
					}
				}
			}



			// �{�X�̒e�Ƃ̓����蔻��
			if (gStageNo == 1 && boss->GetUse() == 1) {
				int i = 0;
				MYRECT bulletRect = boss->GetBulletRect(i);

				// �Ԃ�l��-1�Ȃ�Abullet�z���S�ĎQ�Ƃ���
				while (bulletRect.x != -1) {
					// �Ԃ�l��0�Ȃ�A���̗v�f��use��0
					// ����ȊO�̂Ƃ��̂݁A�����蔻��̏������s��
					if (bulletRect.x != 0) {

						if (IsHitBox(pRect.x, pRect.y, pRect.w, pRect.h, bulletRect.x, bulletRect.y, bulletRect.w, bulletRect.h) == 1) {
							player->SetIsAttacked(1);

							if (player->_heart.GetUse() != 1) {
								VECTOR pPos = player->GetPos();
								VECTOR bPos = VGet(bulletRect.x + bulletRect.w, bulletRect.y + bulletRect.h, 0);
								int heartDirX = pPos.x < bPos.x ? -1 : 1;
								player->_heart.SetDirection(heartDirX);
							}
						}

					}
					i++;
					bulletRect = boss->GetBulletRect(i);
				}

			}
		}


		// �~�[�g�{�[���̌Ăі߂������Ă���Ƃ��́A�~�[�g�{�[���ɃG�t�F�N�g��`�悷��
		// �i�����ł͂��̐ݒ������B���ۂ̕`���MeatBall�N���X��Draw�����ōs���j
		int pCnt = player->GetReturnCnt();
		int mCnt = meatBall->GetOffScreenTime();
		if (pCnt != 0 || mCnt != 0) {
			meatBall->SetIsDrawAura(1);

			// �Ăі߂�SE�̍Đ�
			if (CheckSoundMem(se[20]) == 0) {
				PlaySoundMem(se[20], DX_PLAYTYPE_BACK);
			}
		}
		else {
			meatBall->SetIsDrawAura(0);

			StopSoundMem(se[20]);
		}




		// �{�X���j���̉�ʑJ��
		if (gStageNo == 1 && boss->GetHp() <= 0) {

			_isEliminatedBoss = 1;
			StopSoundMem(bgm[0]);
			StopSoundMem(bgm[1]);

			// �ۓ�SE�̒�~
			StopSoundMem(se[15]);

			switch (step) {
			case 0:

				ColorFadeOut(255, 255, 255, 255);
				step++;

				break;
			case 1:
				if (IsColorFade() == 0) {
					// �t�F�[�h�A�E�g�I��
					SceneBase* scene = new talk_end();
					ChangeScene(scene);
				}
				break;
			}

			//}
		}



		gMap.Process();


		// �J��������
		VECTOR pPos = player->GetPos();
		gCam.Process(pPos);


		// �Q�[���I�[�o�[����
		if (player->GetGameOver() == 1) {
			StopSoundMem(se[15]);
			StopSoundMem(se[17]);
			StopSoundMem(bgm[0]);
			StopSoundMem(bgm[1]);
			
			Boss::PlayBossBGM(0);

			switch (_instep) {
			case 0:

				ColorFadeOut(255, 255, 255, 60);
				_instep++;

				break;
			case 1:
				if (IsColorFade() == 0) {
					// �t�F�[�h�A�E�g�I��

					_sceneGameOver = new SceneGameOver();


					/*SceneBase* scene = new SceneGameOver();
					ChangeScene(scene);*/
				}
				break;

			}



		}


		// 
		// �J���p
		// 

		if (gTrg & PAD_INPUT_1) {
			_debug = 1 - _debug;
		}

		if (gTrg & PAD_INPUT_2) {
			meatBall->UpdateSize(1);
		}
		if (gTrg & PAD_INPUT_3) {
			meatBall->UpdateSize(-1);
		}

		if (_debug == 1) {
			if (gTrg & PAD_INPUT_4) {
				_enemyRectFill = 1 - _enemyRectFill;
			}
		}

		if (gTrg & PAD_INPUT_5) {
			if (gStageNo == 1) {
				VECTOR pPos = VGet(205 * CHIPSIZE_W, 43 * CHIPSIZE_H, 0);
				player->SetPos(pPos);

				gCam._phase = 98;
			}
			else {
				VECTOR pPos = VGet(410 * CHIPSIZE_W, 23 * CHIPSIZE_H, 0);
				player->SetPos(pPos);

				gCam._phase = 18;
			}
		}

		//if (gTrg & PAD_INPUT_6) {
		//	player->SetIsAttacked(1);
		//}



		//if (gStageNo == 1) {
		//	if (gPad->_trg[PAD_BUTTON_BACK]) {
		//		boss->UpdateHp(-5000);
		//	}
		//}







		if (player->GetGameOverMotion() == 0) {
			// �{�X��|���Ă�����|�[�Y�͊J���Ȃ��悤�ɂ���
			if (_isEliminatedBoss != 1) {
				//�|�[�Y��ʈڍs
				if (gPad->_trg[PAD_BUTTON_START]) {
					SetScenePause();
				}
			}

			if(gStageNo == 0) {
				//�|�[�Y�`���[�g���A���\���ڍs
				if(speed != 1) {
					if(meatBall->GetSpeed() == 5.0) {
						speed++;
						Explanation->SetChange(0);
						PlaySoundMem(sound_explanation, DX_PLAYTYPE_BACK);
						Set_Explanation(Explanation->GetChange());


					}
				}
				if(damage != 1) {
					if(player->_heart.GetUse() == 1) {
						damage++;
						Explanation->SetChange(1);
						PlaySoundMem(sound_explanation, DX_PLAYTYPE_BACK);
						Set_Explanation(Explanation->GetChange());
					}
				}
			}

		}

		//�`���[�g���A��
		if (gStageNo == 0) {
			MYRECT pRect = player->GetMyRect(COLLISION_TYPE_BODY);
			MYRECT mStart = {
					4 * CHIPSIZE_W,
				19 * CHIPSIZE_H,
					CHIPSIZE_W * 20,
					CHIPSIZE_H * 5
			};
			MYRECT mThrow = {
				28 * CHIPSIZE_W,
				19 * CHIPSIZE_H,
					CHIPSIZE_W * 20,
					CHIPSIZE_H * 5
			};
			/*MYRECT mThrow_two = {
				38 * CHIPSIZE_W,
				23 * CHIPSIZE_H,
					CHIPSIZE_W * 6,
					CHIPSIZE_H * 3
			};*/
		/*	MYRECT mReturn = {
				38 * CHIPSIZE_W,
				23 * CHIPSIZE_H,
					CHIPSIZE_W * 9,
					CHIPSIZE_H * 3
			};*/
			MYRECT pDash = {
				64 * CHIPSIZE_W,
				25 * CHIPSIZE_H,
					CHIPSIZE_W * 8,
					CHIPSIZE_H * 7
			};
			if (IsHitBox(
				pRect.x, pRect.y, pRect.w, pRect.h,
				mStart.x, mStart.y, mStart.w, mStart.h
			) == 1) {

				if (start_cnt <= guide_frame) {
					start_cnt++;
					start_UI = 1;
				}
				

			}
			else {
				start_cnt = 0;
					start_UI = 0;
				}
			
			if (IsHitBox(
				pRect.x, pRect.y, pRect.w, pRect.h,
				mThrow.x, mThrow.y, mThrow.w, mThrow.h
			) == 1) {


				if (throw_cnt <= guide_frame) {
					throw_cnt++;
				throw_UI = 1;
				}

				}
			else {
					throw_UI = 0;
				throw_cnt = 0;
				}
		


	
			if (IsHitBox(
				pRect.x, pRect.y, pRect.w, pRect.h,
				pDash.x, pDash.y, pDash.w, pDash.h
			) == 1) {


				if (dash_cnt <= guide_frame) {
					Dash_UI = 1;
					dash_cnt++;
					
				}
				
			}
			else {
					Dash_UI = 0;
				dash_cnt = 0;
				}
	
		}


		if (gStageNo == 1) {
			MYRECT pRect = player->GetMyRect(COLLISION_TYPE_BODY);
			MYRECT ctalkRect = {
				5 * CHIPSIZE_W,
					18 * CHIPSIZE_H,
				CHIPSIZE_W,
				CHIPSIZE_H * 3
			};
			if (_C_BoxFlg != 1) {
				if (IsHitBox(pRect.x, pRect.y, pRect.w, pRect.h,
					ctalkRect.x, ctalkRect.y, ctalkRect.w, ctalkRect.h
				) == 1) {
					_Flag_talk = 1;

				}
			}
			if (_Flag_talk == 1) {
				_C_BoxFlg = 1;
				_Flag_talk = 0;
				gSetC_Scenario();

			}



			MYRECT MetalEnemyGuide = {
				30 * CHIPSIZE_W,
				21 * CHIPSIZE_H,
				CHIPSIZE_W * 15,
				CHIPSIZE_H * 7

			};

			MYRECT HARDWALL = {
				51 * CHIPSIZE_W,
				23 * CHIPSIZE_H,
				CHIPSIZE_W * 13,
				CHIPSIZE_H* 5
			};

			if (IsHitBox(pRect.x, pRect.y, pRect.w, pRect.h,
				MetalEnemyGuide.x, MetalEnemyGuide.y, MetalEnemyGuide.w, MetalEnemyGuide.h
			) == 1) {
				if (guide_cnt <= guide_frame) {
					METALENEMYGUIDE = 1;

					guide_cnt++;

				}


			}
			else {
				METALENEMYGUIDE = 0;

				guide_cnt = 0;
		}
			if(IsHitBox(pRect.x, pRect.y, pRect.w, pRect.h, 
				HARDWALL.x, HARDWALL.y, HARDWALL.w, HARDWALL.h
			) == 1){
				if (start_cnt <= guide_frame) {
					hardwallguide = 1;

					start_cnt++;

				}


			}
			else {

				hardwallguide = 0;
				start_cnt = 0;
			}
		}
		

		// �X�e�[�W�N���A����
		if (gStageNo == 0) {
			MYRECT pRect = player->GetMyRect(COLLISION_TYPE_BODY);
			MYRECT goalRect = {
				423 * CHIPSIZE_W,
					22 * CHIPSIZE_H,
				CHIPSIZE_W,
				CHIPSIZE_H * 3
			};
			if (IsHitBox(
				pRect.x, pRect.y, pRect.w, pRect.h,
				goalRect.x, goalRect.y, goalRect.w, goalRect.h
			) == 1) {
				_isTransition = 1;

				if (_Box_Flag != 1) {
					_BFlag_talk = 1;
				}
			}
		}

		if (_BFlag_talk == 1) {
			PlaySoundMem(bgm[6], DX_PLAYTYPE_LOOP);
			gSetB_Scenario();
			_Box_Flag = 1;
			_BFlag_talk = 0;
		}


		// ���[�r�[�։�ʑJ�ڂ����鏈��
		if (_isTransition == 1) {
			_transitionCnt++;
			if (_transitionCnt >= 1) {
				_isTransition = 0;
				StopSoundMem(bgm[0]);

				// �t�F�[�h�C���֘A�̏���������
				_fadeInStep = 0;
				ColorMask(0, 0, 0, 255);


				_sceneMovie = new SceneMovie(MOVIE_CLEAR);

				//
				StageRelease();
				gStageNo++;
			}
		}
	}
	else {

		// ���[�r�[�V�[���̏���
		if (_sceneMovie != NULL) {
			_sceneMovie->Process();

			// ���[�r�[���I����Ă�����
			if (_sceneMovie->GetEndMovie() == 1) {
				//�X�e�[�W�N���A���[�r�[�̏ꍇ
				if (_sceneMovie->GetMovieNo() == MOVIE_CLEAR) {
					if (gPad->_trg[PAD_BUTTON_A] != 0) {
						SetUseASyncLoadFlag(FALSE);

						delete _sceneMovie;
						_sceneMovie = nullptr;

						//// �J�����̃��~�b�g�ݒ�
						//gCam._rcLimit.x = 0;
						//gCam._rcLimit.y = 0;
						//gCam._rcLimit.w = gMap.GetWorldSizeW();
						//gCam._rcLimit.h = gMap.GetWorldSizeH();

						gCam._stageNo = gStageNo;


						// �J�����֘A
						SetupCamera_Ortho(2000.0f);
						SetCameraPositionAndAngle(VGet(960.000000, 540.000000, -935.307434), 0, 0, 0);

						StageRelease();
						StageInit(gStageNo);

						// Respawn�N���X�̏���������
						if (_respawn != NULL) {
							delete _respawn;
							_respawn = nullptr;
						}
						_respawn = new Respawn(gStageNo, player->GetPos());


						StopSoundMem(bgm[1]);
						PlaySoundMem(bgm[1], DX_PLAYTYPE_LOOP);
						ChangeVolumeSoundMem(255, bgm[1]);
					}
				}
				// �v�����[�O���[�r�[�̏ꍇ
				else {
					SetUseASyncLoadFlag(FALSE);

					delete _sceneMovie;
					_sceneMovie = nullptr;

					//// �J�����̃��~�b�g�ݒ�
					//gCam._rcLimit.x = 0;
					//gCam._rcLimit.y = 0;
					//gCam._rcLimit.w = gMap.GetWorldSizeW();
					//gCam._rcLimit.h = gMap.GetWorldSizeH();

					gCam._stageNo = gStageNo;


					//�J�����֘A
					SetupCamera_Ortho(2000.0f);
					SetCameraPositionAndAngle(VGet(960.000000, 540.000000, -935.307434), 0, 0, 0);

					StageRelease();
					StageInit(gStageNo);

					// Respawn�N���X�̏���������
					if (_respawn != NULL) {
						delete _respawn;
						_respawn = nullptr;
					}
					_respawn = new Respawn(gStageNo, player->GetPos());

					StopSoundMem(bgm[3]);
					StopSoundMem(bgm[0]);
					PlaySoundMem(bgm[0], DX_PLAYTYPE_LOOP);

				}



			}
		}
		else	if (_sceneGameOver != NULL) {
			_sceneGameOver->Process();

			if (_sceneGameOver->GetIsRetry() == 1) {
				delete _sceneGameOver;
				_sceneGameOver = nullptr;


				SetRespawn();
			}
		}








	}

	Guide_Easing = EasingOutQuad(guide_cnt, guide_start, guide_end, guide_frame);
	Start_Easing = EasingOutQuad(start_cnt, guide_start, guide_end, guide_frame);
	Return_Easing = EasingOutQuad(return_cnt, guide_start, guide_end, guide_frame);
	Dash_Easing = EasingOutQuad(dash_cnt, guide_start, guide_end, guide_frame);
	Throw_Easing = EasingOutQuad(throw_cnt, guide_start, guide_end, guide_frame);

	
}



void SceneGameMain::Draw() {

	if (_sceneMovie == NULL && _sceneGameOver == NULL) {

		// �J������View�s��ŁA��ʏ�̍��W���v�Z�iView���W�j
		MATRIX	mView = gCam.GetViewMatrix();

		// �w�i�̕`��
		gMap.DrawBG(mView);

		// �����p�l���̕`��
		for (int i = 0; i < DASH_PANEL_MAX; i++) {
			dashPanel[i]->Draw(mView);
		}

		// ���X�|�[���n�_�̕`��
		_respawn->Draw(mView);




		player->AdjustDrawPos(mView);
		meatBall->AdjustDrawPos(mView);

		// �����������ɕʂ̃��f����`�悷��Ƃ��́A���̕����̗����ɂ��郂�f�����ɕ`�悵�Ȃ��Ɠ����Č����Ȃ�
		// �n�[�g�̕`������̊֐����ōs��
		player->Draw();

		// �n�[�g�{�̂ɏd�˂ĕ`��
		DrawHeartCount_Body(mView);

		// �w�ʂɔz�u����G�t�F�N�g�̕`��
		DrawBackEffect(mView);


		// �G�̕`��
		for (int i = 0; i < enemyNum; i++) {
			enemy[i]->AdjustDrawPos(mView);
			enemy[i]->Draw();
		}

		// �{�X�̕`��
		if (gStageNo == 1) {
			boss->AdjustDrawPos(mView);
			boss->Draw();
		}

		// �~�[�g�{�[���̕`��
		meatBall->Draw(mView);

		// ���o�[�̕`��
		for (int i = 0; i < LEVER_MAX; i++) {
			lever[i]->AdjustDrawPos(mView);
			lever[i]->Draw();
		}

		// �Ƃ��̕`��
		for (int i = 0; i < SPIKE_MAX; i++) {
			spike[i]->AdjustDrawPos(mView);
			spike[i]->Draw();
		}

		// �W�����v��̕`��
		for (int i = 0; i < JUMPPAD_MAX; i++) {
			jumpPad[i]->AdjustDrawPos(mView);
			jumpPad[i]->Draw();
		}

		// �}�b�v�`�b�v�̕`��
		gMap.Draw(mView);

		for (int i = 0; i < BOMB_MAX; i++) {
			bomb[i]->Draw(mView);
		}




		for (int i = 0; i < enemyNum; i++) {
			enemy[i]->DrawFrontLayer(mView);
		}

		if (gStageNo == 1) {
			boss->DrawFrontLayer(mView);
		}


		gMap.DrawFrontLayer(mView);



		meatBall->DrawFrontLayer(mView);


		// �O�ʂɔz�u����G�t�F�N�g�̕`��
		DrawFrontEffect(mView);




		MYRECT psychoBar = { 205,92, COOLTIME_WIDTH, 50 };

		float maxCoolTime = float(MEATBALL_ATTACK_COOLTIME);
		float cnt = maxCoolTime - float(meatBall->GetAttackCollTime());
		if (meatBall->GetCanReattack() != 1) {
			psychoBar.w = psychoBar.w * (cnt / maxCoolTime);
		}
		/*if (cnt == maxCoolTime || meatBall->GetCanReattack()) {
			DrawGraph(50, 80, meat_face[0], TRUE);
		}
		else {
			DrawGraph(50, 80, meat_face[1], TRUE);
		}*/

		DrawGraph(195, 143, _cgUI_Bottom, TRUE);


		if (meatBall->GetReturn() == 1) {
			DrawGraph(55, 87, meat_face[0], TRUE);
		}
		DrawGraph(55, 87, meat_face[1], TRUE);
		uiposbar->DrawBar(205, 92, psychoBar.w, 45, 255);
		uiposbar->Draw();

		//DrawGraph(505, 143, ui_heart, TRUE);



		DrawGraph(224, 148, level, TRUE);
		DrawGraph(462, 148, UI_Number[meatBall->GetLevel() - 1], TRUE);

		//DrawFormatString(0, 100, COLOR_RED, "%d", UI_Number[0]);



		// �{�XHP�Q�[�W
		if (gStageNo == 1 && boss->GetBossBattle() == 1) {
			if (boss->GetHp() >= 0) {

				int w;
				w = boss->GetHp() / 10;

				uiposbar->DrawBar(473, 995, w, 50, 255);
				DrawGraph(360, 860, _cgUI_BossBar, TRUE);
			}
		}

		if (start_UI == 1) {
			DrawGraph(0, Start_Easing, instlower_right[5], TRUE);
		}
		if (throw_UI == 1) {
			DrawGraph(0, Throw_Easing, instlower_right[0], TRUE);
		}
		if (return_UI == 1) {
			DrawGraph(0, Return_Easing, instlower_right[1], TRUE);
		}
		if (Dash_UI == 1) {
			DrawGraph(0, Dash_Easing, instlower_right[2], TRUE);
		}
		if (throw_UI_two == 1) {
			DrawGraph(0, Guide_Easing, instlower_right[0], TRUE);
		}
		if (METALENEMYGUIDE == 1) {
			DrawGraph(0, Guide_Easing, instlower_right[3], TRUE);
		}
		if (hardwallguide == 1) {
			DrawGraph(0, Start_Easing, instlower_right[4], TRUE);
		}
		//DrawFormatString(0, 900, COLOR_RED, "cnt%f", guide_cnt);




		if (meatBall->GetReturn() != 1) {

			// �~�[�g�{�[�����Ăі߂��܂ł̃J�E���g���o�[�ŕ\������
			// �v���C���[���Ăі߂��A�N�V���������s���Ă���Ƃ�, �܂���, �~�[�g�{�[������ʊO�ɏo�Ă���Ƃ��Ƀo�[��\������
			int pCnt = player->GetReturnCnt();
			int mCnt = meatBall->GetOffScreenTime();
			if (pCnt != 0 || mCnt != 0) {
				//	�u�v���C���[�̌Ăі߂��J�E���g�v �� �u�~�[�g�{�[������ʊO�ɏo�Ă���J�E���g�v �̑傫�� ����\������
				int returnCnt = pCnt >= mCnt ? pCnt : mCnt;

				// ���݂̃J�E���g���ő�J�E���g�̉����ł��邩
				float rate = float(returnCnt) / 30;

				// �v���C���[�̓���ɕ\������			
				VECTOR barPos = player->GetPos();
				MYRECT pRect = player->GetMyRect(COLLISION_TYPE_BODY);
				barPos.y -= (pRect.h + 70);
				barPos = VTransform(barPos, mView);


				// �o�[�̃T�C�Y
				int w = 64;
				int h = 16;

				// �Q�[�W�����̕`��
				DrawExtendGraph(barPos.x - w / 2, barPos.y - h / 2, (barPos.x - w / 2) + w * rate, barPos.y + h / 2, _cgReturnBar, TRUE);
				// �t���[�������̕`��
				DrawGraph(barPos.x - w / 2, barPos.y - h / 2, _cgReturnFrame, TRUE);
			}
		}


		// �����UI�ւ̕`��
		DrawHeartCount_UI();
		player->_heart.DrawFrontLayer();



#ifdef _DEBUG
		if (_debug == 1) {
			player->DrawDebugInfo(mView);
			meatBall->DrawDebugInfo(mView);
			for (int i = 0; i < enemyNum; i++) {
				enemy[i]->DrawDebugInfo(mView, _enemyRectFill);
			}

			//DrawFormatString(0, 100, GetColor(255, 255, 255), "%d", gRel);



			gMap.DrawDebugInfo(mView);

			gCam.DrawDebugInfo();


			for (int i = 0; i < LEVER_MAX; i++) {
				lever[i]->DrawDebugInfo(mView);
			}

			for (int i = 0; i < SPIKE_MAX; i++) {
				spike[i]->DrawDebugInfo(mView);
			}

			for (int i = 0; i < JUMPPAD_MAX; i++) {
				jumpPad[i]->DrawDebugInfo(mView);
			}


			for (int i = 0; i < BOMB_MAX; i++) {
				bomb[i]->DrawDebugInfo(mView);
			}


			if (gStageNo == 1) {
				boss->DrawDebugInfo(mView);
			}


			_respawn->DrawDebugInfo(mView);
			//DrawFormatString(1800, 0, COLOR_RED, "phase : %d", gCam._phase);
			//DrawFormatString(1800, 100, COLOR_WHITE, "trg_A: %d", gPad->_trg[PAD_BUTTON_A]);
		}

#endif




	}
	else {
		if (_sceneMovie != NULL) {
			_sceneMovie->Draw();

		}
		else	if (_sceneGameOver != NULL) {
			_sceneGameOver->Draw();
		}
	}
}


// �����UI�ւ̕`��
void SceneGameMain::DrawHeartCount_UI()
{
	// �n�[�g�������Ă��Ȃ�������
	if (player->_heart.GetLost() != 1) {
		// �c��J�E���g���擾���A�t���[��������b���ɕϊ�����
		float heartCnt = float(player->_heart.GetRemainCnt()) / 60.f;
		heartCnt = ceilf(heartCnt);
		int sec = heartCnt;


		DrawGraph(530, 145, _cgUiHeart, TRUE);

		// �c��J�E���g�̕`��

		// �J�E���g��10�̂Ƃ�
		if (sec == 10) {
			// 1�����`�悷��

			// 10�̈ʂ̕`��
			DrawGraph(585, 148, _cgUiNumber[1], TRUE);
			// 1�̈ʂ̕`��
			DrawGraph(615, 148, _cgUiNumber[0], TRUE);
		}
		else {
			// �J�E���g�̕`��
			DrawGraph(600, 148, _cgUiNumber[sec], TRUE);
		}
	}
}

// �n�[�g�{�̂ɏd�˂ĕ`��
void SceneGameMain::DrawHeartCount_Body(MATRIX mView)
{
	// �n�[�g����яo���Ă�����
	if (player->_heart.GetUse() == 1) {

		// �n�[�g�̓����蔻���`�̈ʒu�ƃT�C�Y���擾����
		MYRECT drawRect = player->_heart.GetBodyRect();
		// �n�[�g�̒��S���W���v�Z����
		VECTOR drawPos = { drawRect.x + drawRect.w / 2, drawRect.y + drawRect.h / 2, 0};

		// �c��J�E���g���擾���A�t���[��������b���ɕϊ�����
		float heartCnt = float(player->_heart.GetRemainCnt()) / 60.f;
		heartCnt = ceilf(heartCnt);
		int sec = heartCnt;


		// �c��J�E���g�̕`��
		
		// �J�E���g��10�̂Ƃ�
		if (sec == 10) {
			// 1�����`�悷��

			int w = 32;
			int h = 45;
			drawPos.x -= w / 2;

			// 10�̈ʂ̕`��
			MyDrawModiGraph(mView, drawPos, 1.0f, 0.0f, w, h, _cgUiNumber[1]);

			drawPos.x += w;
			// 1�̈ʂ̕`��
			MyDrawModiGraph(mView, drawPos, 1.0f, 0.0f, w, h, _cgUiNumber[0]);
		}
		// �J�E���g��1���̂Ƃ�
		else {
			int w = 32;
			int h = 45;
			// �J�E���g�̕`��
			MyDrawModiGraph(mView, drawPos, 1.0f, 0.0f, w, h, _cgUiNumber[sec]);
		}
	}
}

// ���X�|�[������
void SceneGameMain::SetRespawn()
{
	// �X�e�[�W�̏��̃��Z�b�g
	StageRelease();
	StageInit(gStageNo);

	//�J�����֘A
	SetupCamera_Ortho(2000.0f);
	SetCameraPositionAndAngle(VGet(960.000000, 540.000000, -935.307434), 0, 0, 0);

	// �v���C���[�̍��W�����X�|�[���n�_�ɐݒ�
	player->SetPos(_respawn->GetRespawnPlayerPos());
	//player->_heart.SetRespawn();
	
	// �~�[�g�{�[���̃T�C�Y�����X�|�[���n�_�X�V���̃T�C�Y�ɐݒ�
	meatBall->SetSize(_respawn->GetRespawnMeatBallSize());

	// �J�����̃t�F�[�Y�ƍ��W��ݒ�
	gCam.SetRespawn(_respawn->GetRespawnCamPhase(), _respawn->GetRespawnCamPos());





	// BGM�̒�~
	StopSoundMem(bgm[0]);
	StopSoundMem(bgm[1]);
	StopSoundMem(bgm[3]);

	// BGM�̍Đ�
	if (gStageNo == 0) {
		PlaySoundMem(bgm[0], DX_PLAYTYPE_LOOP);
	}
	else {
		PlaySoundMem(bgm[1], DX_PLAYTYPE_LOOP);
		ChangeVolumeSoundMem(255, bgm[1]);
	}
}

