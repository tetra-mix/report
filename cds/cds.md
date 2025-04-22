# AVR入門 明るさ測定 レポート

* 実験日：2024年1月16日
* 実験者：2I44 吉髙 僚眞

## 実験目的
* マイコンによるセンサを用いた物理量測定（明るさ）ができる

## 実験
* CdSの抵抗値をマルチメータを使って測定する

### 結果
* 通常の明るさの場合
  * 800Ω程度の値を示した
* 手で覆った場合
  * 20kΩ前後の値を示した

## ワーク1 暗くなったらLEDが点灯
* CdSセルとマイコンを接続し、周りが暗くなったらLEDが点灯するプログラムを作成せよ。ただし、CdSセルはアナログ出力であるが、ディジタルで考える。

### プログラム
``` armasm
.include "tn2313def.inc"
.CSEG
START:
	LDI R17, 0b00000000
	OUT DDRD, R17
	LDI R17, 0b11111111
	OUT DDRB, R17
SWITCH:
	IN R17, PIND
	SBRS R17, 0
	RJMP ON
	RJMP OFF

ON:
    ;アクティブロー
	LDI R17, 0b00000000
	OUT PORTB, R17
	RJMP SWITCH

OFF:
	LDI R17, 0b11111111
	OUT PORTB, R17
	RJMP SWITCH
```

### 説明
* PD0にCdSの入力をつないだ。
* `START:`でPORTDを出力、PORTBを入力として初期化する
* `SWITCH:`の部分で入力の判定をする。
  * 明るい場合
    * CdSは明るいとき抵抗が小さくなり、PD0の入力はHIGHになる
    * SBRSで1ビット目を調べて`1`になっていれば次の命令をスキップするので`OFF`に移動する。
  * 暗い場合
  * * CdSは暗いとき抵抗が大きくなり、PD0の入力はLOWになる
    * SBRSで1ビット目を調べて`0`になっているので、`ON`に移動する。
* LEDはアクティブローなので`ON:`でビットを`LOW`にして出力する
* `OFF:`では反対にビットを`HIGH`にしてPORTBに出力する
* `SWITCH:`に戻る。 


### 測定
* LEDが光っているときにCdSの電圧を測ってみると、おおよそ2.6V ~ 4.5Vの値となった。




## ワーク2 暗くなったらLEDが点滅
* ワーク１のプログラムを変更し、周りが暗くなったらLEDが点滅するプログラムを作成せよ

#### プログラム
``` armasm
.include "tn2313def.inc"
.CSEG
START:
	LDI R17, 0b00000000
	OUT DDRD, R17
	LDI R17, 0b11111111
	OUT DDRB, R17
SWITCH:
	IN R17, PIND
	SBRS R17, 0
	RJMP BLINK
	RJMP OFF

BLINK:
    ;アクティブロー
	LDI R17, 0b00000000
	OUT PORTB, R17
	RCALL T500MS
	LDI R17, 0b11111111
	OUT PORTB, R17
	RCALL T500MS
	RJMP SWITCH

OFF:
	LDI R17, 0b11111111
	OUT PORTB, R17
	RJMP SWITCH

T500MS:
	LDI R23, 50
_T10MS:
	RCALL T10MS
	SUBI R23, 1
	BRNE _T10MS
	RET

T10MS:
	LDI R22, 100
_T100US:
	RCALL T100US
	SUBI R22, 1
	BRNE _T100US
	RET

T100US:
	LDI R21, 249
_TUS:
	NOP
	NOP
	NOP
	NOP
	NOP
	SUBI R21, 1
	BRNE _TUS
	NOP
	RET
```
### 説明
* 先ほどのプログラムと同様の処理を行う
* `ON:`の処理を上記のような`BLINK:`に変更する
* `BLINK:`では1回点滅をさせる処理を行っている
  1. PORTBをLOWにし、LEDを光らせる
  1. `RCALL T500MS`を行い、500ミリ秒待つ
  1. PORTBをHIGHにして、LEDを消す。
  1. `RCALL T500MS`を行い、500ミリ秒待つ
  1. `SWITCH:`に戻る

## 感想
* マイコンによるセンサを用いた物理量測定の方法と仕組みが理解できた。