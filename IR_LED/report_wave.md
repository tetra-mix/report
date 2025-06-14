
# 赤外線リモコン（搬送波）

## 目的
赤外線リモコンは搬送波を用いた通信機器である。ここでは、搬送波を作成および受信し、通信
の原理を学ぶ。

## 搬送波
マイコンのクロックは 20M Hz である。ここから、搬送波 38kHz を作成する。搬送波は周期を T (=`26.3us`) とすると、T/3 の点灯、2T/3 の消灯を繰り返した点滅信号である。ここでは、タイマ 0 割り込みを用いて点灯時間、消灯時間を交互に設定する。ここで、T/3=`8.77us`、 2T/3=`17.54`us である。点灯・消灯を状態変数 LED_state で管理する。

T/3 を、タイマ 0 割り込みを用いて作成する。タイマを用いれば
```
クロック       プリスケーラー       カウンタ
 20MHz    ->     1分周        ->    1分周    ->
          A                   B              C
```

Aの周期は1/20MHz = `0.05`us
Bの周期は`0.05`us × `175.4` = `8.77`us
Cの周期は`8.77`us × `2`     = `17.54`us

となるから、B 信号をカウンタで`2`回数えたタイミングで、マイコンに割り込みを行うと`17.54`us刻みの間隔を作ることができる。このとき、カウンタの最大値（top）は`2`になる。同様に、2T/3 の時間は最大値（top）を`2`として実現できる。



## 実験

LEDをD0に、スイッチをD1に接続する。

