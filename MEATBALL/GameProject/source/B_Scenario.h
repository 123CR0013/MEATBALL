#pragma once
#include "scenebase.h"

class B_Scenario :public SceneBase {
public:
	B_Scenario();
	virtual ~B_Scenario();

	void	Input() override;	// ����
	void	Process() override;	// �v�Z
	void	Draw() override;	// �`��

public:

	int _BtalkFlg = 0;

protected:
	int use = 0;

	int talkClearCnt=0;
	int clearCnt = 0;

	int instX = 1400;
	int instY = 910;

	int _step;
	int _stepcnt;

	int j = 0;
	int talkclear[11];
};

