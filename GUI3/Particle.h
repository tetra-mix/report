#pragma once

#include "Windows.h";
#include "Timer.h";

class Particle
{
private:
	double x;
	double y;
	double vx;
	double vy;
	double ax;
	double ay;
	double omega;
	double phi;
	double radius;
	double degree;
	int mx;
	int my;
	Timer timer;
	double lastupdate;
	double life;
	COLORREF color;
	HDC hdc;
public:
	Particle();
	~Particle();
	void init();
	void clean();
	void update();
	void draw();
	void setCursorPos(int mx, int my);
	void fireworksPos();
	void setHDC(HDC hdc);
	double rand();

};

