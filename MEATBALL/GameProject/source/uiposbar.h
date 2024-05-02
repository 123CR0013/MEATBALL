#pragma once	// .h�̐擪�ɋL�q�B#include�ł��̃t�@�C�������x�ǂݍ��݂��Ă��A1�x�����ǂݍ��܂Ȃ�
#include "DxLib.h"
#include "gamemain.h"
#include "mymath.h"
#include "UIBase.h"

class UIPosBar : public UIBase {
public:
	UIPosBar();
	virtual ~UIPosBar();

	void	Process();
	void	Draw();

	void	SetRange(float rangeMin, float rangeMax);
	void	Set(float data);


	

    void	DrawBar(int x, int y, int bar_w, int bar_h, int colorR);
	


    int		_colorR;	// �o�[�̐F

private:
	

	int		_cgBarBase;
	int     _cgBarMain;
	int		_cgBarR;


	float	_rangeMin, _rangeMax;	// �o�[�̒l�͈́B�ŏ�min�`�ő�max
	float	_data;					// �o�[�ŕ\������l
	int		_drawData;			// �`��p�f�[�^
	int		_easingStartData;	// easing�J�n���̃f�[�^
	int		_easingCnt;			// easing�J�E���^
	int		_easingMax;			// easing�ő�l
	
};


