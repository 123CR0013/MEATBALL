#include "bomb.h"
#include "mapdata.h"
#include "winmain.h"
#include "effect.h"
#include "resource.h"
#include "mydraw.h"

BOMB::BOMB(int index, int x, int y)
{
	_index = index;
	if (_index != -1) {
		_use = 1;
	}
	else {
		_use = 0;
	}

	_x = x;
	_y = y;

	_isExplosion = 0;

	

	_count = 0;

	_rect = { -1,-1, -1,-1 };
	_explosionRect = { -1,-1, -1,-1 };
	InitCollision();


	_animCnt = 0;
	_animOrder = 1;

	
}

BOMB::~BOMB()
{
}

void BOMB::Process()
{
	if (_use == 1) {

		_animCnt += _animOrder;
		if (_animCnt == 35) {
			_animOrder = -1;
		}
		else if (_animCnt == 0) {
			_animOrder = 1;
		}

		if (_count > 0) {
			_count--;
			if (_count == 0) {
				SetExplosion();
			}
		}

	}
}



void BOMB::InitCollision()
{
	MYRECT difRect = { 0, 0, 64, 64 };
	int tmpX = _x * CHIPSIZE_W;
	int tmpY = _y * CHIPSIZE_H;

	_rect.x = tmpX + difRect.x;
	_rect.y = tmpY + difRect.y;
	_rect.w = difRect.w;
	_rect.h = difRect.h;

	tmpX = (_x - BOMB_RANGE_HARF_W) * CHIPSIZE_W;
	tmpY = (_y - BOMB_RANGE_HARF_H) * CHIPSIZE_H;

	_explosionRect.x = tmpX;
	_explosionRect.y = tmpY;
	_explosionRect.w = 64 * 9;
	_explosionRect.h = 64 * 9;

}

void BOMB::Delete()
{
	_use = 0;
	_isExplosion = 0;

	HITCHIP bChip = { -1, _x, _y };
	gMap.BreakMapChip(bChip);
}


MYRECT BOMB::GetRect(int type)
{
	MYRECT rect = { -1, -1, -1, -1 };

	switch (type)
	{
	case BOMB_COLLISION_TYPE_BODY:
		rect = _rect;
		break;
	case BOMB_COLLISION_TYPE_EXPLOSION:
		rect = _explosionRect;
		break;
	}

	return rect;
}


void BOMB::ExplodeChip()
{
	// �����G�t�F�N�g�̃Z�b�g�i�@�B�G���j�G�t�F�N�g�Ɠ������̂��g�p����j
	int effectX = _x * CHIPSIZE_W - CHIPSIZE_W;
	int effectY = _y * CHIPSIZE_H - CHIPSIZE_H;
	int effectSize = 64 * 3;
	SetEffect(EFFECT_TYPE_DEFEAT_MACHINE, effectX, effectY, effectSize, effectSize);



	int x, y;
	// ��`���쐬����
	int l, t, r, b;		// ����(left,top) - �E��(right,bottom)
	l = _explosionRect.x;
	t = _explosionRect.y;
	r = _explosionRect.x + _explosionRect.w;
	b = _explosionRect.y + _explosionRect.h;

	// box�̍�����W�`�E�����W�ɂ�����}�b�v�`�b�v�ƁA�����蔻����s��
	for (y = t / CHIPSIZE_H; y <= b / CHIPSIZE_H; y++)
	{
		for (x = l / CHIPSIZE_W; x <= r / CHIPSIZE_W; x++)
		{
			// (x,y)�́A�}�b�v�`�b�v�̍��W�i�`�b�v�P�ʁj
			// ���̈ʒu�̃`�b�v�͓����邩�H
			HITCHIP hitChip = { gMap.CheckHitMapChip(x, y) , x, y };
			switch (hitChip.chip_no)
			{
			case BREAK_CHIP_1:
			case BREAK_CHIP_2:
			case BREAK_CHIP_3:
			case BREAK_CHIP_4:
				// �u���b�N�j��G�t�F�N�g�̃Z�b�g
				int effectX = hitChip.x * CHIPSIZE_W - CHIPSIZE_W;
				int effectY = hitChip.y * CHIPSIZE_H - CHIPSIZE_H;
				int effectSize = 64 * 3;
				SetEffect(EFFECT_TYPE_CHIP_BREAK, effectX, effectY, effectSize, effectSize);

				// �u���b�N�j��SE�̍Đ�
				PlaySoundMem(se[17], DX_PLAYTYPE_BACK);

				// �}�b�v�`�b�v�̔j�󏈗�
				gMap.BreakMapChip(hitChip);
				break;
			}
		}
	}



}

void BOMB::Draw(MATRIX mView)
{
	if (_use == 1) {
		VECTOR world = VGet(_x * CHIPSIZE_W, _y * CHIPSIZE_H, 0);
		// �摜�̒��S��world���W�Ƃ���̂ŁA�T�C�Y/2�������炷
		world.x += CHIPSIZE_W / 2;
		world.y += CHIPSIZE_H / 2;
		MyDrawModiGraph(mView, world, 1.f, 0.f, CHIPSIZE_W, CHIPSIZE_H, cgBomb[(_animCnt / 6) % 6]);
	}
}

void BOMB::DrawDebugInfo(MATRIX mView)
{
	VECTOR rectPos;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 140.f);

	if (_use == 1) {
		rectPos = VGet(_explosionRect.x, _explosionRect.y, 0);
		rectPos = VTransform(rectPos, mView);
		DrawBox(rectPos.x, rectPos.y, rectPos.x + _explosionRect.w, rectPos.y + _explosionRect.h, COLOR_RED, FALSE);



		rectPos = VGet(_rect.x, _rect.y, 0);
		rectPos = VTransform(rectPos, mView);
		DrawBox(rectPos.x, rectPos.y, rectPos.x + _rect.w, rectPos.y + _rect.h, COLOR_BLUE, FALSE);
	}


	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
