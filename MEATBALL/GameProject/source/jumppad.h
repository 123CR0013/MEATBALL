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


	// 当たり判定矩形
	// ジャンプ台が起動したときに、この範囲にいるプレイヤーを飛ばす
	MYRECT _difBoundRect;				// 配置座標からの差分 (x,y / 大きさ w,h)
	MYRECT _boundRect;				// 当たり判定矩形(左上座標 x,y / 大きさ w,h) 
};

extern JumpPad* jumpPad[];
