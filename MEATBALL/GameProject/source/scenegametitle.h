#pragma once	// .h�̐擪�ɋL�q�B#include�ł��̃t�@�C�������x�ǂݍ��݂��Ă��A1�x�����ǂݍ��܂Ȃ�
#include "SceneBase.h"

class SceneGameTitle : public SceneBase {
public:
	SceneGameTitle();			// �R���X�g���N�^
	virtual ~SceneGameTitle();	// �f�X�g���N�^

	void	Input() override;	// ����
	void	Process() override;	// �v�Z
	void	Draw() override;	// �`��

#define TITLE_MAX       (50)





protected:
	int title_scene[TITLE_MAX];
	int title_logo_sheet[12];
	int yes_sheet[12];
	int no_sheet[12];
	int error_R[12];
	int error_L[12];
	int movie;


	

	int		_cgBg;		// �w�i�摜
	int    time = 0;
	

	int		_step;		// �t�F�[�h�C���A�E�g�p�X�e�b�v
	int     _stepcnt;

	
	int    use = 0;
	int    enduse=0;

	int fadetalk=0;

	int startA = 4;
	int flamecnt = 0;

	int trueend=0;
	int meatvoicecnt = 0;
	

	//easing
	int easingTitle;
	float start;
	float end;
	float cnt;
	float flame;
	int max;
	int moreeasing;
	float morecnt;
	float morestart;
	int returning;
	
	int returntitle=0;
	int returnframe = 0;
};

