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





//チップ差分
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



// 画像データ

// 
// sceneGameMain
//

// マップチップ
extern int _cgChip[200];
// ギミックチップ
extern int cgDashPanel[4];
extern int cgBomb[6];

// UI
extern int meat_face[2];
extern int instlower_right[6];


// エフェクト
extern int cgEffect[EFFECT_TYPE_MAX][EFFECT_SHEETS_MAX];
// 射撃敵・弾
extern int cgBullet[30];
// ボス 弾
extern int cgBossBullet[30];

// プレイヤー サイコキネシス
extern int cgAttackAura[20];
extern int cgReturnAura[13];

// ミートボール 呼び戻しエフェクト
extern int cgMeatBallReturn[13];












// SE
extern int se[SE];

// 3Dモデル
// 機械パーツ


extern int metalPartsModel[3];








//タイトル画像
extern int title_BG;


//ロード画面画像
extern int NOW_LOADING[];


//ゲームオーバー画像
extern int		gameover;		// 背景画像
extern int      close;
extern int     retry;
extern int      arrow;

//ポーズ画面画像
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







//音声データ
extern int _cgBG[2];

void ResourceInit();
void ResourceRelease();