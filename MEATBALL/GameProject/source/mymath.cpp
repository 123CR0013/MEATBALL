// �Ǝ��v�Z���C�u����
#include "mymath.h"

// degree��radian
float DegToRad(float deg) {
	return deg * PI / 180.0f;		// �p�x��degree����radian�ɕϊ�
}

// radian��degree
float RadToDeg(float rad) {
	return rad * 180.0f / PI;		// �p�x��radian����degree�ɕϊ�
}



// �����蔻��p�B2��box�������������𔻒�
// �������Ă�����1, �������Ă��Ȃ�������0��Ԃ�
int IsHitBox(
	int x1, int y1, int w1, int h1,		// �ЂƂ߂�box ����(x,y), �傫��w,h
	int x2, int y2, int w2, int h2		// �ӂ��߂�box ����(x,y), �傫��w,h
) {
	if (x1 < x2 + w2 && x2 < x1 + w1		// x�����̔���
		&& y1 < y2 + h2 && y2 < y1 + h1		// y�����̔���
		&& w1 != 0 && h1 != 0           // box1�̑傫��������
		&& w2 != 0 && h2 != 0           // box2�̑傫��������
		)
	{
		// 2��box�͓������Ă���
		return 1;
	}

	// 2��box�͓������Ă��Ȃ�
	return 0;
}


// �����蔻��p�B2�̉~�������������𔻒�
// �������Ă�����1, �������Ă��Ȃ�������0��Ԃ�
int IsHitCircle(
	int x1, int y1, int r1,		// �ЂƂ߂�circle ���S(x,y), ���ar
	int x2, int y2, int r2		// �ӂ��߂�circle ���S(x,y), ���ar
) {
	int w, h, r;
	w = x1 - x2;
	h = y1 - y2;
	r = r1 + r2;
	if (r * r > w * w + h * h) {
		return 1;
	}
	return 0;
}

// �����蔻��p�B�~�Ƌ�`�������������𔻒�
// �������Ă�����1, �������Ă��Ȃ�������0��Ԃ�
// �Q�l�T�C�g: https://takap-tech.com/entry/2019/09/26/004257
int IsHitCircleAndBox(
	float cx, float cy, float r,	 // circle ���S(x,y), ���ar
	MYRECT box				  // box ����(x, y), �傫��w, h
) {
	float left = box.x;
	float right = left + box.w;
	float top = box.y;
	float bottom = top + box.h;

	// �ǂꂩ�̏����ɓ��Ă͂܂��Ă����瓖�����Ă���
	if (
		// �ȉ���2�s�ő�܂��ȓ����蔻������i��`�̒��_�t�߂̓����蔻��͎��Ȃ��j
		((left < cx) && (cx < right) && (top < cy + r) && (cy - r < bottom)) ||
		((left < cx + r) && (cx - r < right) && (top < cy) && (cy < bottom)) ||

		// ��`�̒��_�t�߂̓����蔻������
		((pow(left - cx, 2) + pow(top - cy, 2)) < pow(r, 2)) ||		// ����
		((pow(right - cx, 2) + pow(top - cy, 2)) < pow(r, 2)) ||		// �E��
		((pow(left - cx, 2) + pow(bottom - cy, 2)) < pow(r, 2)) ||		// ����
		((pow(right - cx, 2) + pow(bottom - cy, 2)) < pow(r, 2))			// �E��
		)
	{
		return 1;
	}
	return 0;
}


// �����蔻��p�B�J�v�Z���Ƌ�`�������������𔻒�
// �~�̈ʒu�����炵�Ȃ��� IsHitCircleAndBox�J��Ԃ����s����
// �������Ă�����1, �������Ă��Ȃ�������0��Ԃ�
int IsHitCapsuleAndBox(
	float cx, float cy, float r,	 // �J�v�Z�� �[�̈ʒu(x,y), ���ar
	VECTOR direction,					 // �J�v�Z����L�΂�����
	float capsuleH,					 // �J�v�Z���̏c�̒���
	MYRECT box							 //box ����(x,y), �傫��w,h
) {
	float x = cx;
	float y = cy;
	float length = r * 2;

	// �J�v�Z���̊J�n�ʒu�̌v�Z
	// 1�ڂ̉~�̈ʒu��cx,cy���甼�~�����炷
	VECTOR vDir = VNorm(direction);
	VECTOR vTmp = VScale(vDir, r);
	x += vTmp.x;
	y += vTmp.y;

	// ���炷�����v�Z
	MATRIX m = MGetScale(VGet(r / 4, r / 4, 0)); // ���鐔���͔C��(3���傫���Ȃ��ƃJ�v�Z���̌`��ɂȂ�Ȃ�)
	vTmp = VTransform(vDir, m);

	// �~��vDir�����ɏ��������炵�Ȃ���, �~�Ƌ�`�̓����蔻����s��
	while (length < capsuleH) {
		if (IsHitCircleAndBox(x, y, r, box) == 1) {
			return 1;
		}
		x += vTmp.x;
		y += vTmp.y; 
		//y -= vTmp.y;	 
		length += VSize(vTmp);
	}

	// ��̃��[�v���ƁA�����蔻��p�̃J�v�Z���̒�����capsuleH�ɖ����Ȃ����Ƃ�����
	// ����Ȃ����̓����蔻��
	length -= length - capsuleH;
	VECTOR v = VGet(x, y, 0);
	v = VSub(v, VScale(vDir, length - capsuleH));

	if (IsHitCircleAndBox(v.x, v.y, r, box) == 1) {
		return 1;
	}


	return 0;
}


// ��`���m�̓����蔻����Ƃ�����ɁA�����o���������c�������𔻒肷��
// �߂�l �F 0 ������, 1 �c����
// �������A�����蔻��̏d�Ȃ��������̉����Əc���������ɂȂ����ꍇ�͂��܂����肷�邱�Ƃ��ł��Ȃ�
int CheckExtrusionDirection(MYRECT rect1, MYRECT rect2)
{
	float sx, sy, ex, ey;
	sx = rect1.x; if (sx < rect2.x) { sx = rect2.x; }
	sy = rect1.y; if (sy < rect2.y) { sy = rect2.y; }
	ex = rect1.x + rect1.w; if (ex > rect2.x + rect2.w) { ex = rect2.x + rect2.w; }
	ey = rect1.y + rect1.h; if (ey > rect2.y + rect2.h) { ey = rect2.y + rect2.h; }
	float w, h;
	w = ex - sx;
	h = ey - sy;


	

	if (w > h) {
		return 1;
	}

	if (w < h) {
		return 0;
	}

	if (w == h) {
		return 2;
	}






	return -1;
}

#define TwoPI   (PI * 2.0f)
#define PIOver2 (PI / 2.0f)

// �C�[�W���O
float EasingLinear(float cnt, float start, float end, float frames) {
	return (end - start) * cnt / frames + start;
}
float EasingInQuad(float cnt, float start, float end, float frames) {
	cnt /= frames;
	return (end - start) * cnt * cnt + start;
}
float EasingOutQuad(float cnt, float start, float end, float frames) {
	cnt /= frames;
	return -(end - start) * cnt * (cnt - 2) + start;
}
float EasingInOutQuad(float cnt, float start, float end, float frames) {
	cnt /= frames / 2.0;
	if (cnt < 1) {
		return (end - start) / 2.0 * cnt * cnt + start;
	}
	cnt--;
	return -(end - start) / 2.0 * (cnt * (cnt - 2) - 1) + start;
}
float EasingInCubic(float cnt, float start, float end, float frames) {
	cnt /= frames;
	return (end - start) * cnt * cnt * cnt + start;
}
float EasingOutCubic(float cnt, float start, float end, float frames) {
	cnt /= frames;
	cnt--;
	return (end - start) * (cnt * cnt * cnt + 1) + start;
}
float EasingInOutCubic(float cnt, float start, float end, float frames) {
	cnt /= frames / 2.0;
	if (cnt < 1) {
		return (end - start) / 2.0 * cnt * cnt * cnt + start;
	}
	cnt -= 2;
	return (end - start) / 2.0 * (cnt * cnt * cnt + 2) + start;
}
float EasingInQuart(float cnt, float start, float end, float frames) {
	cnt /= frames;
	return (end - start) * cnt * cnt * cnt * cnt + start;
}
float EasingOutQuart(float cnt, float start, float end, float frames) {
	cnt /= frames;
	cnt--;
	return -(end - start) * (cnt * cnt * cnt * cnt - 1) + start;
}
float EasingInOutQuart(float cnt, float start, float end, float frames) {
	cnt /= frames / 2.0;
	if (cnt < 1) {
		return (end - start) / 2.0 * cnt * cnt * cnt * cnt + start;
	}
	cnt -= 2;
	return -(end - start) / 2.0 * (cnt * cnt * cnt * cnt - 2) + start;
}
float EasingInQuint(float cnt, float start, float end, float frames) {
	cnt /= frames;
	return (end - start) * cnt * cnt * cnt * cnt * cnt + start;
}
float EasingOutQuint(float cnt, float start, float end, float frames) {
	cnt /= frames;
	cnt--;
	return (end - start) * (cnt * cnt * cnt * cnt * cnt + 1) + start;
}
float EasingInOutQuint(float cnt, float start, float end, float frames) {
	cnt /= frames / 2.0;
	if (cnt < 1) {
		return (end - start) / 2.0 * cnt * cnt * cnt * cnt * cnt + start;
	}
	cnt -= 2;
	return (end - start) / 2.0 * (cnt * cnt * cnt * cnt * cnt + 2) + start;
}
float EasingInSine(float cnt, float start, float end, float frames) {
	return -(end - start) * cos(cnt / frames * (PIOver2)) + end + start;
}
float EasingOutSine(float cnt, float start, float end, float frames) {
	return (end - start) * sin(cnt / frames * PIOver2) + start;
}
float EasingInOutSine(float cnt, float start, float end, float frames) {
	return -(end - start) / 2.0 * (cos(PI * cnt / frames) - 1) + start;
}
float EasingInExpo(float cnt, float start, float end, float frames) {
	return (end - start) * pow(2.0, 10 * (cnt / frames - 1)) + start;
}
float EasingOutExpo(float cnt, float start, float end, float frames) {
	return (end - start) * (-pow(2.0, -10 * cnt / frames) + 1) + start;
}
float EasingInOutExpo(float cnt, float start, float end, float frames) {
	cnt /= frames / 2.0;
	if (cnt < 1) {
		return (end - start) / 2.0 * pow(2.0, 10 * (cnt - 1)) + start;
	}
	cnt--;
	return (end - start) / 2.0 * (-pow(2.0, -10 * cnt) + 2) + start;
}
float EasingInCirc(float cnt, float start, float end, float frames) {
	cnt /= frames;
	return -(end - start) * (sqrt(1 - cnt * cnt) - 1) + start;
}
float EasingOutCirc(float cnt, float start, float end, float frames) {
	cnt /= frames;
	cnt--;
	return (end - start) * sqrt(1 - cnt * cnt) + start;
}
float EasingInOutCirc(float cnt, float start, float end, float frames) {
	cnt /= frames / 2.0;
	if (cnt < 1) {
		return -(end - start) / 2.0 * (sqrt(1 - cnt * cnt) - 1) + start;
	}
	cnt -= 2;
	return (end - start) / 2.0 * (sqrt(1 - cnt * cnt) + 1) + start;
}

float EasingOutElastic(float cnt, float start, float end, float frames)
{
	if (cnt == 0) {
		return start;
	}
	else if (cnt == frames) {
		return end;
	}
	else {


		float time = cnt / frames;
		float rad = (2.0f * PI) / 3.0f;


		float rate = pow(2, -10 * time) * sin((time * 10 - 0.75) * rad) + 1;

		return  start + (end - start) * rate;
	} 
}


// �͈͓���Ԃ�
float Clamp(float min_n, float max_n, float n) {
	if (n <= min_n) { return min_n; }
	if (n >= max_n) { return max_n; }
	return n;
}



// �����Ƌ�`�̓����蔻��
// 0:�������Ă��Ȃ�, 1:��������
int IsHitRectAndLine(MYRECT rect, VECTOR lineStartPos, VECTOR lineEndPos)
{

	// �����̎n�_�ƏI�_����ɍ쐬������`���ɂ��邩�𔻒肷��
	/*���̕������R�����g�A�E�g����ƒ����Ƌ�`�̓����蔻����Ƃ��*/
	float left, right, top, bottom;
	if (lineEndPos.x - lineStartPos.x > 0) {
		left = lineStartPos.x;
		right = lineEndPos.x;
	}
	else {
		left = lineEndPos.x;
		right = lineStartPos.x;
	}
	if (lineEndPos.y - lineStartPos.y > 0) {
		top = lineStartPos.y;
		bottom = lineEndPos.y;
	}
	else {
		top = lineEndPos.y;
		bottom = lineStartPos.y;
	}
	if (rect.x + rect.w < left
		|| right < rect.x
		|| rect.y + rect.h < top
		|| bottom < rect.y
		) {
		return 0;
	}
	/*�����܂�*/


	VECTOR line = VSub(lineEndPos, lineStartPos);
	MATRIX m = MGetRotZ(90.f * DX_PI_F / 180.f);
	line = VTransform(line, m);

	VECTOR rectPos[4] = {
		VGet(rect.x, rect.y, 0),
		VGet(rect.x + rect.w, rect.y, 0),
		VGet(rect.x + rect.w, rect.y + rect.h, 0),
		VGet(rect.x, rect.y + rect.h, 0)
	};


	VECTOR c[4];
	for (int i = 0; i < 4; i++) {
		c[i] = VSub(lineStartPos, rectPos[i]);
	}

	float dot[4];
	for (int i = 0; i < 4; i++) {
		dot[i] = VDot(line, c[i]);
	}

	if (dot[0] * dot[1] <= 0
		|| dot[1] * dot[2] <= 0
		|| dot[2] * dot[3] <= 0
		) {
		return 1;
	}



	return 0;
}
//�J������]�p
static const float ROTATE_SPEED = DX_PI_F / 90;//��]�X�s�[�h

// (x,y)�̓_��(mx,my)�𒆐S��ang�p��]����
void rotate(float* x, float* y, const float ang, const float mx, const float my) {
	const float ox = *x - mx, oy = *y - my;
	*x = ox * cos(ang) + oy * sin(ang);
	*y = -ox * sin(ang) + oy * cos(ang);
	*x += mx;
	*y += my;
}
