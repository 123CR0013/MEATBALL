#pragma once
#include "gimmick.h"
#include "gimmick_data.h"

#include "jumppad.h"


#define LEVER_MODEL_RED 0
#define LEVER_MODEL_BLUE 1

class Lever : public Gimmick
{
public:
	Lever(int stageNo, int index);
	~Lever();
	// 描画位置の補正処理
	void AdjustDrawPos(MATRIX mView) override;


	void ModelInit(int modelHandle, VECTOR size, int motionMax, int stageNo) override;


	void Process();

	void SetActive();
	int GetActive() { return _isActive; };


	// アウトラインの設定
	// 引数 n  
	// 0: 無効化, 1: 有効化
	void SetOutLine(bool n);


	void DrawDebugInfo(MATRIX mView) override;



	static int _leverModel[2];

protected:
	// レバーが作動したかどうか
	int _isActive;

	// 非有効状態に戻るかどうか
	int _returnInactive;

	// レバーの残り作動時間
	// 0になるまで次の有効化は行わない
	// モーション・エフェクトと有効化のタイミングを合わせる
	int _remainActiveTime;

	// モーションの再生方向
	// レバーのモーションは倒れてから元に戻る
	// 1 : 通常, -1 : 逆
	int _isPlayMotion;


	//
	// 作動時にアクションさせるギミック
	// （gimmickdataと対応付ける）
	//

	// アクションさせるギミックのタイプ
	int _gimmickType;
	// アクションさせるギミックのid 
	int _gimmickId;
};


extern Lever* lever[];


