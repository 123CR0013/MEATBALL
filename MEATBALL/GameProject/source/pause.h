#pragma once
#include "scenebase.h"
#include "gamepad.h"

class ScenePause :public SceneBase {
public:
	ScenePause();
	virtual ~ScenePause();

	void	Input() override;	// 入力
	void	Process() override;	// 計算
	void	Draw() override;	// 描画

	

protected:
	int use = 0;
	int instuse = 0;
	int Enduse = 0;
	int pauseCnt = 0;







	
	int insttime = 0;
	int insterror;
	int insterrorCnt = 0;
	int volume[2];
	int inst[2];
	int back[12];
	int volumeChangeFlag = 0;
	int volume_change = 0;
	int gradation;
	int yes;
	int no;
	int yessheet[12];
	int nosheet[12];
	int confirmation;
	int error[12];
	int error_L[12];
	int info;
	

	//終了フレーム変数
	int returnend;
	int trueend;
	
	//flame01
	int easingOUT;
	float easingStart;
	float easingEnd;
	float easingCnt;
	float easingFlame;
	int easingMax;
	int rerurnInst=0;
	//flame02
	int easingRight;
	float RightStart;
	float RightEnd;
	float RightCnt;
	float RightFlame;
	int RightMax;
	float endcnt;
	int endtime = 0;
	int easingend;
	float endmax;
	float end;
	float endstart;
	float endflame;

	
	

public:
	int _isPause = 1;
	int _isExplanation = 0;
	int _isVoiceChange = 0;
	int _isGoTitle = 0;
	int selectEnd = 0;
};