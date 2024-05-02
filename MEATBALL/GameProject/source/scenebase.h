#pragma once	// .hの先頭に記述。#includeでこのファイルを何度読み込みしても、1度しか読み込まない

class SceneBase {
public:
	SceneBase();			// コンストラクタ
	virtual ~SceneBase();	// デストラクタ

	virtual void	Input();	// 入力
	virtual void	Process();	// 計算
	virtual void	Draw();		// 描画

protected:
};

