#pragma once	// .hの先頭に記述。#includeでこのファイルを何度読み込みしても、1度しか読み込まない
#include "DxLib.h"
#include "gamemain.h"
#include "mymath.h"

class UIBase {
public:
	UIBase();
	virtual ~UIBase();

	virtual void	Process();
	virtual void	Draw();

	virtual void	SetBasePos(int base_x, int base_y);

protected:
	int		_base_x, _base_y;	// UIベース位置（左上）
};

