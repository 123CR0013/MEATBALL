#pragma once
#include "scenebase.h"

class C_Scenario :public SceneBase {
public:
	C_Scenario();
	virtual ~C_Scenario();

	void	Input() override;	// ����
	void	Process() override;	// �v�Z
	void	Draw() override;	// �`��



int _talkFlg = 0;

protected:
	int use = 0;

	int talkStartCnt02 = 0;

	int startCnt = 0;

	int instX = 1400;
	int instY = 910;

	
	int j = 0;
	int talkstart_02[8];
	
};
