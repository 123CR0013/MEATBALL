#pragma once
#include "scenebase.h"

class Title_Talk :public SceneBase {
public:
	Title_Talk();
	virtual ~Title_Talk();

	void	Input() override;	// “ü—Í
	void	Process() override;	// ŒvŽZ
	void	Draw() override;	// •`‰æ

public:
	


protected:
	int use = 0;
    int _step;
	int _stepcnt;

	int titleCnt = 0;


	int instX = 1336;
	int instY = 920;

	int talkStartCnt=0;
	int i = 0;
	int talkstart[21];

	
	
};
