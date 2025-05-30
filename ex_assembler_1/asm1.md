# AVRアセンブリ言語1 レポート

* 実験日：2024年10月24日
* 実験者：2I44 吉髙 僚眞

## 実験目的
* クロス開発環境を使用できる
* アセンブラ言語（算術論理演算命令、ビット関連命令）について説明できる

## 実験内容
* Microchip StudioでATtiny2313のシュミレーション環境を使ってアセンブリの命令を行った際のレジスタの値の変化やプログラムカウンタの動作、ステータスレジスタの動作等を確認する

## 基本問題


1. ワーク１において、LDI 命令を用いてR16 に10 進数の1、R17 に10進数の2 を代入し動作確認したときの汎用レジスタの値を報告せよ。
    プログラム(抜粋)
    ```
    LDI R16, 1
    LDI R17, 2
    ```

    結果
    |     |     |
    | --- | --- |
    | R16 | 0x01 |
    | R17 | 0x02 |

2. ワーク１において、R16 に10 進数の負の数を入力せよ。その時，R16 に入っている値と，なぜその値となるかを報告せよ。

    プログラム(抜粋)
    ```
    LDI R16, -1
    ```

    結果
    |     |     |
    | --- | --- |
    | R16 | 0xFF|

    2進数の補数表現ではビット反転して、1を加える。
    10進数の1は2進数で`0000 0001`なのでこれを2の補数で表すと、`1111 1111`となる。
    これは16進数で`0xFF`となるため、10進数の-1を入力すると0xFFとなる。

## 発展問題
1. ワーク２において、F11を10回押して動作させると、プログラムカウンタ、サイクルカウンタ、ステータスレジスタがどのように変化するか報告せよ。

    プログラム(抜粋)
    ```
    .INCLUDE "tn2313def.inc"
    .CSEG               
        RJMP RESET      ;0
    RESET:              
	    LDI R17, 0x05   ;1
	    LDI R16, 0x00   ;2
    LOOP:
	    ADD R16, R17    ;3,5,7,9
	    RJMP LOOP       ;4,6,8,10
    ```

    結果
    | F11を押した回数 | プログラムカウンタ | サイクルカウンタ | R16の値 | R17の値 | ストレージレジスタ |
    | --- | --- | --- | --- | --- | --- |
    | 0 | 0x00000000 | 0 | 0x00 | 0x00 |
    | 1 | 0x00000001 | 2 | 0x00 | 0x00 |
    | 2 | 0x00000002 | 3 | 0x00 | 0x05 |
    | 3 | 0x00000003 | 4 | 0x00 | 0x05 |
    | 4 | 0x00000004 | 5 | 0x05 | 0x05 |
    | 5 | 0x00000003 | 7 | 0x05 | 0x05 |
    | 6 | 0x00000004 | 8 | 0x0A | 0x05 |
    | 7 | 0x00000003 | 10 | 0x0A | 0x05 |
    | 8 | 0x00000004 | 11 | 0x0F | 0x05 |
    | 9 | 0x00000003 | 13 | 0x0F | 0x05 |
    | 10 | 0x00000004 | 14 | 0x14 | 0x05 | H


2. ワーク２において、RJMP LOOP の部分を， RJMP RESET に変更し実行し、特にプログラムカウンタの値がどう変化するか報告せよ。

    プログラム(抜粋)
    ```
    .INCLUDE "tn2313def.inc"
    .CSEG               
        RJMP RESET      ;0
    RESET:              
	    LDI R17, 0x05   ;1,5,9
	    LDI R16, 0x00   ;2,6,10
    LOOP:
	    ADD R16, R17    ;3,7
	    RJMP RESET       ;4,8
    ```

    結果
    | F11を押した回数 | プログラムカウンタ | サイクルカウンタ | R16の値 | R17の値 | ステータスレジスタ |
    | --- | --- | --- | --- | --- | --- |
    | 0 | 0x00000000 | 0 | 0x00 | 0x00 |
    | 1 | 0x00000001 | 2 | 0x00 | 0x00 |
    | 2 | 0x00000002 | 3 | 0x00 | 0x05 |
    | 3 | 0x00000003 | 4 | 0x00 | 0x05 |
    | 4 | 0x00000004 | 5 | 0x05 | 0x05 |
    | 5 | 0x00000001 | 7 | 0x05 | 0x05 |
    | 6 | 0x00000002 | 8 | 0x05 | 0x05 |
    | 7 | 0x00000003 | 9 | 0x00 | 0x05 |
    | 8 | 0x00000004 | 10 | 0x05 | 0x05 |
    | 9 | 0x00000001 | 12 | 0x05 | 0x05 |
    | 10 | 0x00000002 | 13 | 0x05 | 0x05 |

    1では`RJMP LOOP`の部分でプログラムカウンタが`0x00000003`にもどるというループになっているが2では`RJMP RESET`の部分でプログラムカウンタが`0x00000001`にもどるようになっている。そのため、R16に値が再代入されるので`0x05`よりも数が増えていない。そのため、ステータスレジスタのフラグも上がらなかった。


## 考察/感想
プログラムカウンタが次に実行するべきアドレスを示していることや計算結果が汎用レジスタに入れられていくことがよくわかった。ステータスレジスタの`C`は`ADD`コマンドで値の繰り上げがあった場合あがり、次に`ADC`コマンドを使うことでくりがったものを含めて計算できた。サイクルカウンタを見ると、コマンドが実行されるのは周期によって決まっていることが分かった。