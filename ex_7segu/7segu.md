# AVR入門 点滅回路の製作 レポート

* 実験日：2024年12月19日
* 実験者：2I44 吉髙 僚眞

## 実験目的
* 7セグメントLED回路の点滅ができる


## 実験内容
* LED 点滅回路の製作する。

## ワーク1
* 以下の点灯パターンの場合、P1～P4、P5～P8の出力(H or L)を答えよ。
1. `0000`のように4とも0を光らせる場合
    | P  | H or L |
    | --- | --- |
    | P1 | H |
    | P2 | H |
    | P3 | H |
    | P4 | H |
    | P5 | L |
    | P6 | L |
    | P7 | L |
    | P8 | L |

2. `00  `のようにP1~P2だけ0を光らせる場合
    | P  | H or L |
    | --- | --- |
    | P1 | H |
    | P2 | H |
    | P3 | L |
    | P4 | L |
    | P5 | L |
    | P6 | L |
    | P7 | L |
    | P8 | L |

## ワーク2
* 次のように配線し，テストプログラム １を実行せよ。
* マイコンボードのPB0~3 を7 セグメントLED ボードのP8~P5 
* マイコンボードのPB4~7 を7 セグメントLED ボードのP4~P1 へ

#### テストプログラム1
```armasm
.include "tn2313def.inc"
.CSEG
START: 
    LDI R16, 0b11111111
    OUT DDRB, R16
ON:
    LDI R16, 0b11110000
    OUT PORTB, R16
    RJMP ON
```
#### 結果
すべての7セグメントLEDが0になった。

## ワーク3
* ワーク２のテストプログラム1を変更し、全てのLEDで7が点灯するように構成し、動作を報告せよ

#### プログラム
``` armasm
.include "tn2313def.inc"
.CSEG
START: 
	LDI R16, 0b11111111
	OUT DDRB, R16
ON:
	LDI R16, 0b11110111 ;後ろの4ビットが7を示すように変更
	OUT PORTB, R16
RJMP ON
```

#### 結果
すべての7セグメントLEDが7になった。


## ワーク5
* テストプロラム２を変更し、左から順に7654 と表示できるプログラム作成し、報告せよ。

#### プログラム
```armasm
.include "tn2313def.inc"
.CSEG
START: 
	LDI R16, 0b11111111
	OUT DDRB, R16
ON:
	LDI R16, 0b10000111 ;P1に7
	OUT PORTB, R16
	RCALL T05MS
	LDI R16, 0b01000110 ;P2に6
	OUT PORTB, R16
	RCALL T05MS
	LDI R16, 0b00100101 ;P3に5
	OUT PORTB, R16
	RCALL T05MS
	LDI R16, 0b00010100 ;P4に4
	OUT PORTB, R16
	RCALL T05MS
	RJMP ON

T05MS:
	LDI R18, 50; T100US を50回呼び出し
_T100US:
	RCALL T100US
	SUBI R18, 1
	BRNE _T100US
	RET

T100US:
	LDI R19, 249; 249 回繰り返し
_TUS:
	NOP
	NOP
	NOP
	NOP
	NOP
	SUBI R19,1
	BRNE _TUS 
	NOP
	RET
```

#### 説明
ダイナミック駆動では左のセグメントから順番に指定していくので、まず、P1に7を表示する。
上位4ビットで表示したいポートを下位4ビットで表示したい数を指定するため、`0b10000111`のように指定する。人間の目が認識できる速度に合わせて、遅延させたいので0.5ミリ秒ほど待つために、前回の授業で使ったプログラムを使用した。`RCALL T05MS`で呼び出している部分がその部分である。同様にP2に6を、P3に5をP4に4を遅延させながら表示させると、`7654`と表示されているように見える。



## ワーク6
* スイッチ１を押したときは「1」，スイッチ２を押したときは「2」，どちらも押してないときは「0」と表示する回路，プログラムを完成させ、動作を報告せよ。表示させるのは1 桁のみでよい

``` armasm
.include "tn2313def.inc"
.CSEG
START: 
	LDI R16, 0b11111111
	OUT DDRB, R16
	LDI R16, 0b00000000
	OUT DDRD, R16
ON:
	IN R17, PIND
	LDI R16, 0b10000000
FIRST:
	SBRS R17, 0
	LDI R16, 0b10000001
SECOND:
	SBRS R17, 1
	LDI R16, 0b10000010
LIGHT:
	OUT PORTB, R16
	RJMP ON
```
#### 説明
1. 最初にR16に0を光らせる状態の値を保存しておく。
1. もしスイッチ1が押されたら、1を光らせられる値に上書きする
1. もしスイッチ2が押されたら、2を光らせられる値に上書きする
1. R16の値を光らせる。
1. 最初に戻る

## ワーク7
* スイッチ１を押したとき、「0」、「1」、「2」、「3」と順番に点灯するプログラム作成せよ。

``` armasm

.include "tn2313def.inc"
.CSEG
START: 
	LDI R16, 0b11111111
	OUT DDRB, R16
	LDI R16, 0b00000000
	OUT DDRD, R16
ON:
	IN R17, PIND
	SBRS R17, 0
	RJMP LIGHT
	RJMP ON

LIGHT:
	LDI R16, 0b10000000
	OUT PORTB, R16
	RCALL T03S
	LDI R16, 0b01000001
	OUT PORTB, R16
	RCALL T03S
	LDI R16, 0b00100010
	OUT PORTB, R16
	RCALL T03S
	LDI R16, 0b00010011
	OUT PORTB, R16
	RCALL T03S
	LDI R16, 0b00000000
	OUT PORTB, R16
	RJMP ON

T03S:
	LDI R20, 30 ; T10MS を100 回呼び出し
_T10MS:
	RCALL T10MS
	SUBI R20,1
	BRNE _T10MS
	RET

T10MS:
	LDI R21, 100; T100US を100回呼び出し
_T100US:
	RCALL T100US
	SUBI R21, 1
	BRNE _T100US
	RET

T100US:
	LDI R22, 249; 249 回繰り返し
_TUS:
	NOP
	NOP
	NOP
	NOP
	NOP
	SUBI R22,1
	BRNE _TUS 
	NOP
	RET
```
#### 説明
* もしスイッチを押していたら、点灯させるプログラムに飛ぶ。そうでなければ、最初に戻る
* ダイナミック点灯のプログラムと同様にP1に0, P2に1, P3に2, P4に3を表示する。
* この時、0.5秒の遅延を発生させて、順番に表示しているように見えるようにする。



## ワーク8
* ダイナミック駆動による7セグメントLEDの点灯
* スイッチ1を押したとき7セグメントLEDに`3456`と表示せよ。

``` armasm
.include "tn2313def.inc"
.CSEG
START: 
	LDI R16, 0b11111111
	OUT DDRB, R16
	LDI R16, 0b00000000
	OUT DDRD, R16
ON:
	IN R17, PIND
	SBRS R17, 0
	RJMP LIGHT
	RJMP ON

LIGHT:
	LDI R16, 0b10000011
	OUT PORTB, R16
	RCALL T05MS
	LDI R16, 0b01000100
	OUT PORTB, R16
	RCALL T05MS
	LDI R16, 0b00100101
	OUT PORTB, R16
	RCALL T05MS
	LDI R16, 0b00010110
	OUT PORTB, R16
	RCALL T05MS
	LDI R16, 0b00000000
	OUT PORTB, R16
	RJMP ON

T05MS:
	LDI R21, 50; T100US を50回呼び出し
_T100US:
	RCALL T100US
	SUBI R21, 1
	BRNE _T100US
	RET

T100US:
	LDI R22, 249; 249 回繰り返し
_TUS:
	NOP
	NOP
	NOP
	NOP
	NOP
	SUBI R22,1
	BRNE _TUS 
	NOP
	RET
```
#### 説明
* ワーク7のプログラムと同様に、P1に3, P2に4, P3に5, P4に6を表示させられるようにする。
* ダイナミック表示のプログラムと同様に各セグメントの表示切り替え時に0.5ミリ秒の遅延を発生させる。