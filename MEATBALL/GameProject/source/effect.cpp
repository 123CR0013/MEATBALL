#include "DXLib.h"
#include "effect.h"
#include "resource.h"
#include "mydraw.h"


EFFECT effect[EFFECT_MAX];

// ����������
void EffectInit() {
	for (int i = 0; i < EFFECT_MAX; i++) {
		effect[i].use = 0;
		effect[i].type = -1;

		effect[i].x = 0;
		effect[i].y = 0;
		effect[i].w = 0;
		effect[i].h = 0;

		effect[i].cnt = 0;
		effect[i].sheets = 0;
		effect[i].speed = 0;

		effect[i].loop = 0;
	}
}

// �G�t�F�N�g���Z�b�g���A���̗v�f�ԍ���Ԃ�
// �G�t�F�N�g��C�ӂ̃^�C�~���O�ŏ��������ꍇ�ɔz��ԍ����K�v�ɂȂ�
int SetEffect(int type, int x, int y, int w, int h, int loop) {
	int i;

	for (i = 0; i < EFFECT_MAX; i++) {
		// �g�p���Ă��Ȃ��z��v�f�ɃG�t�F�N�g���Z�b�g����
		if (effect[i].use == 0) {
			effect[i].use = 1;
			effect[i].type = type;

			effect[i].x = x;
			effect[i].y = y;
			effect[i].w = w;
			effect[i].h = h;

			effect[i].cnt = 0;


			effect[i].loop = loop;

			switch (effect[i].type)
			{
			case EFFECT_TYPE_DEFEAT_MACHINE:
				effect[i].sheets = 16;
				effect[i].speed = 4;
				break;
			case EFFECT_TYPE_DEFEAT_CREATURE:
				effect[i].sheets = 25;
				effect[i].speed = 2;
				break;
			case EFFECT_TYPE_BULLET_HIT:
				effect[i].sheets = 17;
				effect[i].speed = 4;
				break;

			case EFFECT_TYPE_BULLET_CHARGE:
				effect[i].sheets = 13;
				effect[i].speed = 3;
				break;

			case EFFECT_TYPE_CHIP_BREAK:
				effect[i].sheets = 25;
				effect[i].speed = 4;
				break;

			case EFFECT_TYPE_BOSS_BULLET_HIT:
				effect[i].sheets = 16;
				effect[i].speed = 4;
				break;

			case EFFECT_TYPE_SMOKE:
				effect[i].sheets = 20;
				effect[i].speed = 1;
				break;
			case EFFECT_TYPE_SMOKE_COLOR:
				effect[i].sheets = 20;
				effect[i].speed = 1;
				break;

			case EEFECT_TYPE_MEATBALL_RETURN:
				effect[i].sheets = 13;
				effect[i].speed = 1;
				break;
			}

			break;
		}
	}

	return i;
}


// �v�Z����
void EffectProcess() {
	for (int i = 0; i < EFFECT_MAX; i++) {
		if (effect[i].use == 1) {
			// �J�E���g�𑝂₷
			effect[i].cnt++;
			
			// �G�t�F�N�g�̕`�悪�I�������� use = 0 �ɂ���
			if (effect[i].cnt > effect[i].speed * effect[i].sheets - 1) {
				effect[i].cnt = 0;

				// ���[�v�Đ������Ȃ��ꍇ�̂� use = 0 �ɂ���
				if (effect[i].loop == 0) {
					effect[i].use = 0;
				}
			}
		}
	}
}

// �G�t�F�N�g���~����
// SetEffect�Ŏ擾�����v�f�ԍ����g�p����
void StopEffect(int index)
{
	effect[index].use = 0;
}


// �w�ʃ��C���[�ւ̕`��
void DrawBackEffect(MATRIX mView)
{
	for (int i = 0; i < EFFECT_MAX; i++) {
		if (effect[i].use == 1) {
			VECTOR effectPos = VGet(effect[i].x, effect[i].y, 0);
			effectPos = VTransform(effectPos, mView);
			switch (effect[i].type)
			{
			case EFFECT_TYPE_BULLET_CHARGE:
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 216.0f);
				MyDrawModiGraph(mView, VGet(effect[i].x + effect[i].w / 2, effect[i].y + effect[i].h / 2, 0), 1.0f, 0.0f, effect[i].w, effect[i].h, cgEffect[effect[i].type][(effect[i].cnt / effect[i].speed) % effect[i].sheets]);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				break;
			}
		}
	}
}


// �O�ʃ��C���[�ւ̕`��
void DrawFrontEffect(MATRIX mView)
{
	for (int i = 0; i < EFFECT_MAX; i++) {
		if (effect[i].use == 1) {
			VECTOR effectPos = VGet(effect[i].x, effect[i].y, 0);
			effectPos = VTransform(effectPos, mView);
			switch (effect[i].type)
			{
				// �ˌ��G�̃`���[�W�G�t�F�N�g�͓G�̗��ɕ`�悷��
			case EFFECT_TYPE_BULLET_CHARGE:
				break;
			case EFFECT_TYPE_SMOKE_COLOR:
				//SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
				SetDrawBright(rand() % 256, rand() % 256, rand() % 256);
				MyDrawModiGraph(mView, VGet(effect[i].x + effect[i].w / 2, effect[i].y + effect[i].h / 2, 0), 1.0f, 0.0f, effect[i].w, effect[i].h, cgEffect[EFFECT_TYPE_SMOKE][(effect[i].cnt / effect[i].speed) % effect[i].sheets]);
				SetDrawBright(255, 255, 255);
				//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				break;


			case EEFECT_TYPE_MEATBALL_RETURN:
				MyDrawModiGraph(mView, VGet(effect[i].x, effect[i].y, 0), 1.0f, 0.0f, effect[i].w, effect[i].h, cgEffect[effect[i].type][(effect[i].cnt / effect[i].speed) % effect[i].sheets]);
				break;


			default:
				MyDrawModiGraph(mView, VGet(effect[i].x + effect[i].w / 2, effect[i].y + effect[i].h / 2, 0), 1.0f, 0.0f, effect[i].w, effect[i].h, cgEffect[effect[i].type][(effect[i].cnt / effect[i].speed) % effect[i].sheets]);
				break;
			}
		}
	}
}
