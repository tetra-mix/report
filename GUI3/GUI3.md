# GUIアプリケーション3

2I44 吉髙 僚眞

## 目的
簡単なパーティクルシステムの作成を通して、GUIアプリケーションの設計方法の一つであるMVCモデルについて学ぶ。

## MVCモデルの説明・注意点
> イベント駆動型プログラミングに関する次のキーワードを調査し説明せよ。
<dl>
	<dt>イベント</dt>
	<dd>アプリケーションに対して発生する事象のことである。例えば，「キーを押した/放した」「ある時間経過した」「描画しなければならなくなった」などである。</dd>
	<dt>ハンドラ</dt>
	<dd>イベントが発生した際に実行すべきまとまった処理のこと。イベントフック、イベントリスナーなどの呼び方がある。</dd>
</dl>

参考：[Wikipedia](https://ja.wikipedia.org/wiki/%E3%82%A4%E3%83%99%E3%83%B3%E3%83%88%E9%A7%86%E5%8B%95%E5%9E%8B%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%9F%E3%83%B3%E3%82%B0)

## 調査１
> パーティクルシステムについて調査し説明せよ。

## 考察１
> ``` c++
>srand(static_cast<unsigned int>(time(NULL)));
> ```
> の役割と必要性を述べよ。


## 考察2
> double Particle:rand(void)において、
> 1. static_cast<double>() とする理由を述べよ。
> 2. 下線部::rand()において「::」をつける理由を述べよ。

## 演習1

### ソースコード

Particle.h
``` c++
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

```

Partivle.cpp
``` c++
#include <cstdlib>
#include <cmath>
#include "Particle.h"

Particle::Particle(void) : x(0), y(0), vx(0), vy(0), ax(0), ay(0), omega(0), degree(0), phi(0), radius(0), mx(0), my(0), lastupdate(0), life(0), color(0), hdc(0) // TODO : ������
{

}

Particle::~Particle(void){ }

void Particle::init(void)
{
	int t = timer.get();
	x = -100;
	y = -100;
	degree = 50;
	radius = 10;
	life = (20 * rand() );
	color = RGB(0, 0, 255);
	timer.reset();
	lastupdate = 0;
}
void Particle::clean(void)
{

}

void Particle::update(void)
{
	double t;
	t = timer.get();
	if (t <= life)
	{
		double dt;
		dt = t - lastupdate;

		x = cos(t) * degree + mx;
		y = sin(t) * degree + my;

		degree += 0.5;

		int r = sin(t) * 128 + 128;
		int g = sin(t + 128) * 128 + 128;
		int b = sin(t + 256) * 128 + 128;
		color = RGB(r, g, b);

		lastupdate = t;
	}
	else {
		clean();
		init();
	}
	
}
void Particle::draw(void)
{
	SelectObject(hdc, GetStockObject(DC_PEN));
	SelectObject(hdc, GetStockObject(DC_BRUSH));
	SetDCPenColor(hdc, color);
	SetDCBrushColor(hdc, color);
	Ellipse(hdc, x - radius, y + radius, x + radius, y - radius);
	
}
void Particle::setCursorPos(int mx, int my)
{
    this->mx = mx;
	this->my = my;
}


void Particle::setHDC(HDC hdc)
{
	this->hdc = hdc;
}


double Particle::rand(void)
{
	return static_cast<double>(::rand()) / RAND_MAX;
}
```


## 感想

