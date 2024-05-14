#include <cstdlib>
#include <cmath>

#include "Particle.h"

Particle::Particle(void) : x(0), y(0), vx(0), vy(0), ax(0), ay(0), omega(0), degree(0), phi(0), radius(0), mx(0), my(0), lastupdate(0), life(0), color(0), hdc(0) // TODO : ������
{

}

Particle::~Particle(void)
{

}

void Particle::init(void)
{
	int t = timer.get();

	// TODO : �p�[�e�B�N���̒��S���W���}�E�X�J�[�\���̍��W�ɂ���B
	x = -100;
	y = -100;
	
	phi = 3.14;

	degree = 50;

	radius = 10;

	life = (20 * rand() );


	// TODO : �p�[�e�B�N���̐F��ɂ���B
	color = RGB(0, 0, 255);
	// TODO : �^�C�}�[�����Z�b�g����B
	timer.reset();
	// TODO : �ŏI�X�V������ 0 �ɂ���B
	lastupdate = 0;
}
void Particle::clean(void)
{

}
// �p�[�e�B�N���̋����v�Z
void Particle::update(void)
{
	double t; // ���݂̎���
	// TODO : t �Ɍ��݂̎����i�^�C�}�[�l�j���Z�b�g����B
	t = timer.get();
	// TODO : �������݂̎��� t ������ life �ȉ��Ȃ�C
	if (t <= life)
	{
		double dt; // �ŏI�X�V���� lastupdate ����o�ߎ���
		// TODO : dt �ɍŏI�X�V��������̌o�ߎ��Ԃ��Z�b�g����B
		dt = t - lastupdate;

		//ax = vx / dt;
		//ay = vy / dt;

		// TODO : ax,ay,dt ��p���Č��݂̑��x vx,vy ���v�Z����B
		//vx += ax * dt;
		//vy += ay * dt;
		// TODO : vx,vy,dt ��p���Č��݂̈ʒu x,y ���v�Z����B
		x = cos(t) * degree + mx;
		y = sin(t) * degree + my;

		degree += 0.5;


		// TODO : t ��p���Č��݂̔��a radius ���v�Z����B
		//radius = -(10/life) * t + 10;
		// TODO : t ��p���Č��݂̐F color ���v�Z����B
		int r = sin(t) * 128 + 128;
		int g = sin(t + 128) * 128 + 128;
		int b = sin(t + 256) * 128 + 128;
		color = RGB(r, g, b);

		// TODO : �ŏI�X�V���������݂̎������Z�b�g����B
		lastupdate = t;
	}
	else { // TODO : �����łȂ���΁C
		// TODO : �I������������B
		clean();
		// TODO : ��������������B
		init();
	}
	
}
// �p�[�e�B�N���̕`��
void Particle::draw(void)
{
	// TODO : ���S���W(x, y), ���a radius, �F color �̉~��h��Ԃ��`�悷��B
	SelectObject(hdc, GetStockObject(DC_PEN));
	SelectObject(hdc, GetStockObject(DC_BRUSH));
	SetDCPenColor(hdc, color);
	SetDCBrushColor(hdc, color);
	Ellipse(hdc, x - radius, y + radius, x + radius, y - radius);
	
}
void Particle::setCursorPos(int mx, int my)
{
	// TODO : �������� mx, my �������o�ϐ��� mx, my �փZ�b�g����B
	this->mx = mx;
	this->my = my;
}


void Particle::setHDC(HDC hdc)
{
	// TODO : �������� hdc �������o�ϐ��� hdc �ɃZ�b�g����B
	this->hdc = hdc;
}


double Particle::rand(void)
{
	return static_cast<double>(::rand()) / RAND_MAX;
}