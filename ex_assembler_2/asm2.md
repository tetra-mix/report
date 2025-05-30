# AVRアセンブリ言語2 レポート

* 実験日：2024年11月07日
* 実験者：2I44 吉髙 僚眞

## 実験目的
* クロス開発環境を使用できる
* アセンブラ言語（算術論理演算命令、ビット関連命令）について説明できる

## 実験内容
* Microchip StudioでATtiny2313のシュミレーション環境を使ってアセンブリの命令を行った際のレジスタの値の変化やプログラムカウンタの動作、ステータスレジスタの動作等を確認する

## 基本問題


1. 以下の手続きで加算を行ない，なぜそのような結果となるかプログラムを示した上で考察せよ。
   
   手順
   1. R16, R17 に10 進数の255 を入力し
   2. R16 にR16 とR17 を加算した結果を入力
   3. その結果，R16 の値はどうなるか

    プログラム(抜粋)
    ``` armasm
    LDI R16, 255
    LDI R17, 255
    ADD R16, R17
    ```

    結果
    |     |     |
    | --- | --- |
    | R16 | 0xFE |

    考察
    10進数の`255`を2進数に直すと`0b1111 1111`である。255+255は次のようになり足し算によって2進数では一つ位が上がってR16に入る値は`0b1111 1110`となる。16進数に直すと、`0xFE`となる。
    ``` math
    {}
    ```

2.  以下の手続きで減算を行ない，なぜそのような結果となるかプログラムを示した上で考察せよ。
   
    手順
       1. R16, R17 に10 進数の 0 を入力し
       2. R16 にR16 とR17 を減算した結果を入力
       3. その結果，R16 の値はどうなるか

    プログラム(抜粋)
    ``` armasm
    LDI R16, 0
    LDI R17, 1
    SUB R16, R17
    ```

    結果
    |     |     |
    | --- | --- |
    | R16 | 0xFF|

    考察    
    10進数で計算すると`0 - 1 = -1`となる。2進数で10進数の`-1`を2の補数表現を使って表すと`0b1111 1111`となる。それを16進数に直すと`0xFF`となる。
    
3.  ワーク6と同じ働きをするプログラムをADD命令を用いて実現せよ。なぜそのような結果となるかプログラムを示したうえで考察せよ。

    プログラム(抜粋)
    ``` armasm
    LDI R16, 0x01
    ADD R16, R16
    ```
   
    考察    
    2進数で同じ値を加算すると、すべてのビットの桁で1つ位が繰り上がるため結果的に論理左シフトができる。

## 発展問題
1. 以下の手続きで加算を行ない，なぜそのような結果となるかプログラムを示した上で考察せよ。
    1. R16,R17 に10 進数の3 を入力，R18 に10 進数の2を入力
    2. R16 にR16 からR18 を減算した結果を入力する．
    3. R18 の2 の補数をR18 に代入する．
    4. R17 にR17 とR18 を加算した結果を入力する．
    5. 演算後のR16, R17 の値はどうなるか

    プログラム(抜粋)
    ``` armasm
    LDI R16, 3; (1)
    LDI R17, 3; (1)
    LDI R18, 2; (1)

    SUB R16, R18; (2)
    NEG R18;      (3)
    ADD R17, R18; (4)
    ```

    結果
    |     |     |
    | --- | --- |
    | R16 | 0x01|
    | R17 | 0x01|

    考察    
    * (2) R16に` 3 - 2 = 1`より`1`が入る
    * (3) R18に`0b0000 0010`の2の補数が代入されるので、`0b1111 1101`が入る
    * (4) `0b0000 0011` + `0b1111 1101` が行われ、計算していくとすべての桁が繰り上がるため`0b0000 0001`がR17に入る


## 考察/感想
今回の実験ではビットの反転やシフト演算が論理積や単純な加算で表せられることがよく分かった。コンピュータの動作の理解につながったと思う。また、アセンブリ言語の演算に関する命令は深く理解できたと思う。