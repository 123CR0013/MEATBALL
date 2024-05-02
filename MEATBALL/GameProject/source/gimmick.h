#pragma once
#include "Dxlib.h"
#include "mymath.h"
#include "mapdata.h"
#include "gamemain.h"

#include "charabase.h"




class Gimmick
{
public:
	Gimmick();
	virtual ~Gimmick();
	// �`��ʒu�̕␳����
	virtual void AdjustDrawPos(MATRIX mView);
	virtual void Draw();

	int GetUse() { return _use; };


	// 3D���f���\���p���W�̍X�V
	void UpdateWorldPos();





	virtual void ModelInit(int modelHandle, VECTOR size, int motionMax, int stageNo = -1);
	void MotionProcess(int motionNum_old);


	virtual void SetCollision();

	MYRECT GetRect() { return _rect; };
	VECTOR GetPos() { return _pos; };


	virtual void DrawDebugInfo(MATRIX mView);

protected:
	int _index;

	int _use;



	int _modelHandle;
	int _motionNum;
	int _playTime;

	CHARA_ANIM _anim[5];


	int _rotation;


	VECTOR _pos;
	VECTOR _screenPos;
	VECTOR _worldPos;


	// �����蔻���`
	// �~�[�g�{�[���Ƃ̓����蔻��Ɏg��
	MYRECT _difRect;				// �z�u���W����̍��� (x,y / �傫�� w,h)
	MYRECT _rect;				// �����蔻���`(������W x,y / �傫�� w,h) 
};

