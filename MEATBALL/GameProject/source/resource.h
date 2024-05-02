#pragma once
#include "gamemain.h"
#include "scenegametitle.h"
#include "scenegameover.h"
#include "mapdata.h"



#define EFFECT_TYPE_MAX	10
#define EFFECT_SHEETS_MAX 40





#define ARROW_DOWN 0
#define ARROW_UP 1
#define ARROW_LEFT 2
#define ARROW_RIGHT 3


#define SE          30
#define BGM         10
#define VOICE       30





//�`�b�v����
#define CHIP_NOMAL             0
#define CHIP_DIAGONAL_UPWARD   9
#define CHIP_DIAGONAL_DOWNWARD 13
#define CHIP_BREAK			   17

#define CHIP_BACK_WARNING              21
#define CHIP_BACK    26
#define CHIP_BACK_FENCE        31
#define CHIP_BACK_HOLE       40
#define CHIP_BACK_WARNING_HOLE   84
#define CHIP_BACK_HOLE_CORNER   49
#define CHIP_BACK_WARNING_HOLE_CORNER   93
#define CHIP_LIGHT     66
#define CHIP_LIGHT_LAYER            75
#define CHIP_DARK     53
#define CHIP_DARK_CORNER     62
#define CHIP_BACK_LIGHT     97
#define CHIP_BACK_WARNING_LIGHT     103
#define CHIP_SEPERATE     102



#define CHIP_DOOR			   150




#define METAL_PARTS_GEAR 0
#define METAL_PARTS_BOLT 1
#define METAL_PARTS_METAL 2



// �摜�f�[�^

// 
// sceneGameMain
//

// �}�b�v�`�b�v
extern int _cgChip[200];
// �M�~�b�N�`�b�v
extern int cgDashPanel[4];
extern int cgBomb[6];

// UI
extern int meat_face[2];
extern int instlower_right[6];


// �G�t�F�N�g
extern int cgEffect[EFFECT_TYPE_MAX][EFFECT_SHEETS_MAX];
// �ˌ��G�E�e
extern int cgBullet[30];
// �{�X �e
extern int cgBossBullet[30];

// �v���C���[ �T�C�R�L�l�V�X
extern int cgAttackAura[20];
extern int cgReturnAura[13];

// �~�[�g�{�[�� �Ăі߂��G�t�F�N�g
extern int cgMeatBallReturn[13];












// SE
extern int se[SE];

// 3D���f��
// �@�B�p�[�c


extern int metalPartsModel[3];








//�^�C�g���摜
extern int title_BG;


//���[�h��ʉ摜
extern int NOW_LOADING[];


//�Q�[���I�[�o�[�摜
extern int		gameover;		// �w�i�摜
extern int      close;
extern int     retry;
extern int      arrow;

//�|�[�Y��ʉ摜
extern int   reStart;
extern int   Operation;






extern int _cgUI_LevelBar;
extern int _cgUI_BossBar;
extern int _cgUI_Bottom;
extern int meat_face[];


extern int UI_Number[3];
extern int level;

extern int talkBG;

extern int retryAnim[];
extern int startAnim[];


extern int closeAnim[];
extern int instAnim[];
extern int jRetryAnim[];
extern int cgDashPanel[4];
extern int flame[];

extern int talkinst_sheet[];
extern int titleBottunSheet[];

// SE
extern int se[SE];
//bgm
extern int bgm[BGM];
//voice
extern int voice[VOICE];
extern int sound_explanation;







//�����f�[�^
extern int _cgBG[2];

void ResourceInit();
void ResourceRelease();