#include <cstdlib>
#include <cmath>

#include "Particle.h"

Particle::Particle(void) : x(0), y(0), vx(0), vy(0), ax(0), ay(0), omega(0), degree(0), phi(0), radius(0), mx(0), my(0), lastupdate(0), life(0), color(0), hdc(0) // TODO : 初期化
{

}

Particle::~Particle(void)
{

}

void Particle::init(void)
{
	int t = timer.get();

	// TODO : パーティクルの中心座標をマウスカーソルの座標にする。
	x = -100;
	y = -100;
	
	phi = 3.14;

	degree = 50;

	radius = 10;

	life = (20 * rand() );


	// TODO : パーティクルの色を青にする。
	color = RGB(0, 0, 255);
	// TODO : タイマーをリセットする。
	timer.reset();
	// TODO : 最終更新時刻を 0 にする。
	lastupdate = 0;
}
void Particle::clean(void)
{

}
// パーティクルの挙動計算
void Particle::update(void)
{
	double t; // 現在の時刻
	// TODO : t に現在の時刻（タイマー値）をセットする。
	t = timer.get();
	// TODO : もし現在の時刻 t が寿命 life 以下なら，
	if (t <= life)
	{
		double dt; // 最終更新時刻 lastupdate から経過時間
		// TODO : dt に最終更新時刻からの経過時間をセットする。
		dt = t - lastupdate;

		//ax = vx / dt;
		//ay = vy / dt;

		// TODO : ax,ay,dt を用いて現在の速度 vx,vy を計算する。
		//vx += ax * dt;
		//vy += ay * dt;
		// TODO : vx,vy,dt を用いて現在の位置 x,y を計算する。
		x = cos(t) * degree + mx;
		y = sin(t) * degree + my;

		degree += 0.5;


		// TODO : t を用いて現在の半径 radius を計算する。
		//radius = -(10/life) * t + 10;
		// TODO : t を用いて現在の色 color を計算する。
		int r = sin(t) * 128 + 128;
		int g = sin(t + 128) * 128 + 128;
		int b = sin(t + 256) * 128 + 128;
		color = RGB(r, g, b);

		// TODO : 最終更新時刻を現在の時刻をセットする。
		lastupdate = t;
	}
	else { // TODO : そうでなければ，
		// TODO : 終了処理をする。
		clean();
		// TODO : 初期処理をする。
		init();
	}
	
}
// パーティクルの描画
void Particle::draw(void)
{
	// TODO : 中心座標(x, y), 半径 radius, 色 color の円を塗りつぶし描画する。
	SelectObject(hdc, GetStockObject(DC_PEN));
	SelectObject(hdc, GetStockObject(DC_BRUSH));
	SetDCPenColor(hdc, color);
	SetDCBrushColor(hdc, color);
	Ellipse(hdc, x - radius, y + radius, x + radius, y - radius);
	
}
void Particle::setCursorPos(int mx, int my)
{
	// TODO : 仮引数の mx, my をメンバ変数の mx, my へセットする。
	this->mx = mx;
	this->my = my;
}


void Particle::setHDC(HDC hdc)
{
	// TODO : 仮引数の hdc をメンバ変数の hdc にセットする。
	this->hdc = hdc;
}


double Particle::rand(void)
{
	return static_cast<double>(::rand()) / RAND_MAX;
}