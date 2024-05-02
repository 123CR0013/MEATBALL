#pragma once
#include "scenebase.h"
#include "gamepad.h"

class explanation :public SceneBase{
	public:
		explanation(int changeInst);
		virtual ~explanation();

		void	Input() override;	// “ü—Í
		void	Process() override;	// ŒvŽZ
		void	Draw() override;	// •`‰æ

		int GetChange() { return explanationChange; };
		void SetChange(int changeEX) { explanationChange = changeEX; };

      int explanationEnd;



	protected:
		int attack_explamation[3];
		int speed_explamation[3];
		int push_a;
	 
		int page;
		int explanationNo;
		
		int exNum;

      int explanationChange;

    int explanationEasing;
	float explanationStart;
	float EexplanationEnd;
	float explanationCnt;
	float explanationFlame;

	




	
	
};

