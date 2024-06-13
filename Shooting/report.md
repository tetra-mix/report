# プログラミング応用
2I44 吉髙 僚眞

## 実験の目的
* これまで学んだ知識・技術を総合的に活用すること
* これまでの知識を用いて、与えられたソースコードを理解すること
* デザインパターンやフレームワークの基礎知識を身に付けること
* クラス図の基礎知識を身に付けること

## 実験の概要
簡単なシューティングゲームを開発する。
* ウィンドウサイズ
  - 横 800pixel、縦 600pixel
* 自機
  - 前後左右に平行移動する。
  - 画面中に最大 10 発までのミサイルを前方に向けて撃つことができる。
  - 敵機かミサイルに当たると消滅する。
* 敵機
  - 画面中に 5 機まで存在する。
  - 計算により移動経路を求める。
  - 画面中に 1 発のミサイルを前方に向けて撃つことができる。
  - 敵機以外に当たると消滅する。
* ミサイル
  - 直進的に移動する。
  - 他の飛行物体と当たると消滅する。

## 考察
### クラスEnemyB
EnemyB.h

```c++
class EnemyB : public Enemy
{
    private:
        double phase;
    public:
	    EnemyB();
	    ~EnemyB();

        void init();
        void update();
        void draw();
};
```
EnemyB.cpp
```c++
#define _USE_MATH_DEFINES
#include <cmath>
#include "ShootingApp.h"
#include "EnemyB.h"

EnemyB::EnemyB():phase(0),Enemy()
{

}


EnemyB::~EnemyB()
{

}

void EnemyB::init() {
	FlyingObject::init();

	phase = App::rand() * M_PI;
	x = 100 + 600 * App::rand();
	y = 100 + 100 * App::rand();
	vx = 100 + 500 * App::rand();
	vy = 100 + 500 * App::rand();

	point = 500;

	radius = 20;
}


void EnemyB::update() {

	if (status & COLLISION) {
		if (etimer.get() > 0.5)
			cleanup();
		return;
	}

	double dt = elapsed.get();
	double mt = mtimer.get();
	double dx, dy;

	if (dt > 1.0 / 30)
		dt = 1.0 / 30;
	
	dy = vx * sin(2 * mt + phase) * dt;
	dx = vy * cos(5 * mt + phase) * dt;

	x += dx;
	y += dy;

	elapsed.reset();
}

void EnemyB::draw()
{
	if (status & COLLISION) {
		drawExplosion();
		return;
	}

	Ellipse(App::hDC, x - radius, y - radius, x + radius, y + radius);

	LPCWSTR c = TEXT("B");
	TextOut(App::hDC, (int)x - 6, (int)y - 10, c, lstrlen(c));
}
```

### クラスScore
Score.h
```c++
class Score
{
public:
	int score;

	void init();
	void add(int point);
	void draw(int x, int y);
};

```
Score.cpp
```c++
#include "App.h";
#include "Score.h"

void Score::init() {
	score = 0;
}

void Score::add(int point) {
	score += point;
}

void Score::draw(int x, int y) {
	TCHAR str[32];
	wsprintf(str, TEXT("SCORE: %08d"), score);
	TextOut(App::hDC, x, y, str, lstrlen(str));
}
```
### 応用課題
Enemy.h
```c++
  // 省略
	virtual int collide_score();

```
Enemy.cpp
```c++
//省略
int Enemy::collide_score() {
	return point;
}
```

Boss.h
```c++
#pragma once
#include "Enemy.h"
class Boss : public Enemy
{
private:
	int hitpoint;
	double orbit;
	double phase;
public:
	Boss();
	~Boss();

	void init();
	void update();
	void draw();
	int collide_score();
};


```

Boss.cpp
```c++
#define _USE_MATH_DEFINES
#include "Boss.h"
#include "ShootingApp.h"

Boss::Boss():orbit(0),phase(0),hitpoint(0), Enemy()
{

}


Boss::~Boss()
{

}

void Boss::init() {
	FlyingObject::init();

	phase = App::rand() * M_PI;	// M_PIはπ

	x = 100 + 500 * App::rand();
	y = 100 + 100 * App::rand();

	vx = 3 * App::rand();
	vy = 3 * App::rand();

	point = 9999999;
	hitpoint = 10;
	radius = 20;

	phase = App::rand() * M_PI;
	orbit = 5;
}

void Boss::update() {
	
	

	if (status & COLLISION){
		if (hitpoint <= 0)
		{
			if (etimer.get() > 0.5)
				cleanup();
		}
		else {

			status = ACTIVE;
		}
		return;
	}

	double dt = elapsed.get();
	double mt = mtimer.get();

	if (dt > 1.0 / 30)
		dt = 1.0 / 30;

	if(orbit <= 6)
		orbit += 0.01;

	if (vx <= 5)
		vx += 0.001;
	if (vy <= 5)
		vy += 0.001;

	x = cos(mt) * vx * orbit + x;
	y = sin(mt) * vy * orbit + y;

	elapsed.reset();
}

void Boss::draw()
{
	TCHAR hp[20];
	wsprintf(hp, TEXT("BOSSHP: %03d"), hitpoint);
	TextOut(App::hDC, 10, 25, hp, lstrlen(hp));

	if ((status & COLLISION)&&(hitpoint <= 0)) {
		drawExplosion();
		return;
	}

	Ellipse(App::hDC, x - radius, y - radius, x + radius, y + radius);

	LPCWSTR c = TEXT("Boss");
	TextOut(App::hDC, (int)x - 6, (int)y - 10, c, lstrlen(c));
}

int Boss::collide_score()
{
	if (hitpoint > 0)
	{
		hitpoint--;
		return 0;
	}
	else {
		return point;
	}
}

```

### クラス図

### スクリーンショット

## 感想