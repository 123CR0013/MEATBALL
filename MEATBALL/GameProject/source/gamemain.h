#pragma once	// .hの先頭に記述。#includeでこのファイルを何度読み込みしても、1度しか読み込まない
#include "winmain.h"
#include "gamepad.h"
#include "camera.h"
#include "mapdata.h"
#include "mymath.h"
#include "Pause.h"
#include "C_Scenario.h"
#include "B_Scenario.h"
#include "explanation.h"

#include "scenebase.h"



#define PLAYER_MAX 1
#define ENEMY_MAX 150

#define	MAPSIZE_LAYER	1


// ゲーム情報

void GameMain();	// gamemain.cpp に実体を記述
void ChangeScene(SceneBase* scene);	// Sceneを変更する

void SetScenePause();
void gSetC_Scenario();
void gSetB_Scenario();
void Set_Explanation(int changeInst);

extern int Exit  ;

extern ScenePause* gScenePause;

extern C_Scenario* gC_Scenario;
extern B_Scenario* gB_Scenario;

extern explanation* gExplanation;




extern int fontHandle;

void ColorMask(int colorR, int colorG, int colorB, int colorA);		// カラーマスクの指定

// 指定の色にフェードアウト（カラーマスク使用, alpha=255にする）
void ColorFadeOut(int colorR, int colorG, int colorB, int frames);

// 現在のカラーマスクからフェードイン（カラーマスク使用, alpha=0にする）
void ColorFadeIn(int frames);

// フェードインアウトは終了したか？
// 戻り値: 1 = フェード中, 0 = フェード終了(もしくは開始前)
int IsColorFade();

// 現在の画面からクロスフェードアウト
void CrossFadeOut(int frames);

// クロスフェードアウトは終了したか？
// 戻り値: 1 = フェード中, 0 = フェード終了(もしくは開始前)
int IsCrossFade();




