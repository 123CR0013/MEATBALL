#pragma once	// .h�̐擪�ɋL�q�B#include�ł��̃t�@�C�������x�ǂݍ��݂��Ă��A1�x�����ǂݍ��܂Ȃ�
#include "SceneBase.h"
#include "MapData.h"
#include "Camera.h"
#include "scenemovie.h"
#include "SceneGameOver.h"


#include "respawn.h"





class SceneGameMain : public SceneBase {
public:
	SceneGameMain();			// �R���X�g���N�^
	virtual ~SceneGameMain();	// �f�X�g���N�^

	void SceneGameMainResourceInit();
	void SceneGameMainResourceRelease();

	void StageInit(int stageNo);
	void StageRelease();



	void	Input() override;	// ����
	void	Process() override;	// �v�Z
	void	Draw() override;	// �`��


	int _Flag_talk = 0;
	int _BFlag_talk = 0;
	int _Box_Flag = 0;
	int _C_BoxFlg = 0;


	
	// �n�[�g�̎c��J�E���g�`��
	// �����UI�ւ̕`��
	void DrawHeartCount_UI();
	// �n�[�g�{�̂ɏd�˂ĕ`��
	void DrawHeartCount_Body(MATRIX mView);



	// ���X�|�[������
	void SetRespawn();

protected:
	// �t�F�[�h�C���E�t�F�[�h�A�E�g�֘A
	int _fadeInStep;
	int _instep;
	int _stepcnt;
	int _isEliminatedBoss = 0;
	int step;
	int start_UI = 0;
	int throw_UI=0;
	int throw_UI_two = 0;
	int return_UI = 0;
	int Dash_UI = 0;
	int METALENEMYGUIDE = 0;
	int hardwallguide = 0;

	//�C�[�W���O�K�p
	int Guide_Easing;
	int Start_Easing;
	int Return_Easing;
	int Dash_Easing;
	int Throw_Easing;
	float guide_start;
	float guide_end;
	float guide_cnt;
	float return_cnt;
	float start_cnt;
	float throw_cnt;
	float dash_cnt;
	float guide_frame;






	int _cgReturnFrame;
	int _cgReturnBar;

	//�`���[�g���A������
	int speed = 0;
	int damage = 0;


	// �n�[�gUI�摜
	int _cgUiHeart;
	// ����UI�摜�i�n�[�g�̎c��J�E���g���̕\���p�j
	// �����UI�����Ɩ{�̂ɏd�˂ĕ\�����镔����2�ӏ��Ŏg�p����
	int _cgUiNumber[10];



	Respawn* _respawn;

	//
	// �V�[��
	//

	// ���[�r�[�V�[��
	SceneMovie* _sceneMovie;
	// ��ʑJ�ڒ����ǂ���
	int _isTransition;
	// ��ʑJ�ڒ��̃J�E���g
	int _transitionCnt;

	//�Q�[���I�[�o�[�V�[��
	SceneGameOver* _sceneGameOver;



	//
	// �J���p
	//

	// �f�o�b�O�\�������邩
	int _debug = 0;
	// �f�o�b�O�\�����ɁA�G�̓����蔻���`��h��Ԃ���
	int _enemyRectFill = 0;
};

// �J����
extern Camera	gCam;




