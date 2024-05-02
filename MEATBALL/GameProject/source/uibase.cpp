#include "UIBase.h"


// UIBaseクラス
UIBase::UIBase() {
}
UIBase::~UIBase() {
}

void	UIBase::SetBasePos(int base_x, int base_y) {
	_base_x = base_x;
	_base_y = base_y;
}

// 毎フレーム呼ばれる前提
void	UIBase::Process() {
}

// 描画
void	UIBase::Draw() {
}

