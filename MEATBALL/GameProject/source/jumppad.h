#pragma once
#include "gimmick.h"



class JumpPad : public Gimmick
{
public:
	JumpPad(int stageNo, int index);
	~JumpPad();

	void Process();

	void SetCollision() override;

	MYRECT GetBoundRect() { return _boundRect; };


	int GetId() { return _id; };

	void SetActive();
	int  GetActive() { return _isActive; };



	void SetPlayMotion() { _isPlayMotion = 1; };






	void DrawDebugInfo(MATRIX mView) override;


	static int _jumpPadModel;

protected:
	int _id;
	int _isAlwaysActive;
	int _isActive;
	int _remainActiveTime;


	int _defaultMotionTime;
	int _isPlayMotion;


	// �����蔻���`
	// �W�����v�䂪�N�������Ƃ��ɁA���͈̔͂ɂ���v���C���[���΂�
	MYRECT _difBoundRect;				// �z�u���W����̍��� (x,y / �傫�� w,h)
	MYRECT _boundRect;				// �����蔻���`(������W x,y / �傫�� w,h) 
};

extern JumpPad* jumpPad[];
