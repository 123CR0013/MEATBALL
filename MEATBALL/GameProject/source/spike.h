#pragma once
#include "gimmick.h"

class Spike : public Gimmick
{
public:
	Spike(int stageNo, int index);
	~Spike();
	//void AdjustDrawPos(MATRIX mView) override;



	void ModelInit(int modelHandle, VECTOR size, int motionMax, int stageNo) override;

	void DrawDebugInfo(MATRIX mView) override;

	static int _spikeModel;

protected:

};


extern Spike* spike[];
