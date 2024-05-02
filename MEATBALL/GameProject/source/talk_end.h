#pragma once
#include "scenebase.h"
class talk_end :public SceneBase {
public:
	talk_end();
	virtual ~talk_end();

	void Input()override;
	void Process() override;
	void Draw() override;


public:

protected:
	
	int talkBadEnd[25];

	int instX = 1400;
	int instY = 910;

	

	int i = 0;
	int badcnt = 0;

	int endCnt = 0;

	int _step;
	int badstepcnt;

};



