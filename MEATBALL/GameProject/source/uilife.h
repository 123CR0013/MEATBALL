#pragma once	// .hの先頭に記述。#includeでこのファイルを何度読み込みしても、1度しか読み込まない
#include "DxLib.h"
#include "gamemain.h"
#include "mymath.h"
#include "UIBase.h"

class UILife : public UIBase {
public:
	UILife();
	virtual ~UILife();

	void	Process();
	void	Draw();
	void	Set(int life);

private:
	int		_cgHeart;
	int		_life;
};

