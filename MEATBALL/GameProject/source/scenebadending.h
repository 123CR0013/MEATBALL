#pragma once
#include "scenebase.h"

class SceneBadEnding :public SceneBase {
public:
	SceneBadEnding();
	virtual ~SceneBadEnding();

	void	Input() override;	// “ü—Í
	void	Process() override;	// ŒvŽZ
	void	Draw() override;	// •`‰æ

#define BadRollSpeed    -2*Abadspeed

public:

	
protected:
	int EndRoll;
	int BadBGM;
	int title_base[7];
	int title_bg[7];
	int title_bodyL;
	int title_faceL[4];
	int title_faceS[4];
	int title_hair[7];
	int title_bubble[55];

	int Abadspeed=1;
	int eyetime = 0;

	int badCnt = 0;
	int y = 0;
	int time = 0;
	int _step;
};