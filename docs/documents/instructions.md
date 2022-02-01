---
layout: default
title: "ドキュメント - 命令一覧"
---

# 命令一覧

Otoには標準で用意されている命令があり、それらを利用して音遊びをしていくことになります。

ほとんどの命令は引数を省略することができ、あらかじめ決められた初期値に従って動作します。
```basic
/* 第2引数と第4引数を省略*/
PLAY 500, ,10

/* 第2引数以降を省略 */
PLAY 500

/* 全て省略 */
PLAY
```

<h2 id="print">PRINT</h2>

変数の値を出力する命令です。

```basic
PRINT <変数>
```

現時点ではfloat型の変数または定数だけしか出力することができません。

<h2 id="beep">BEEP</h2>

ビープ音を出力する命令です。

```basic
BEEP <周波数(Hz)> <長さ(秒)>
```
引数を省略すると、周波数500Hz、長さ1秒のビープ音を出力します。

この命令は他のPLAY命令と比べて低速であり、指定された長さ通りに音声出力が終わるとは限らないことに注意してください。

<h2 id="play">PLAY</h2>

音を出力する命令です。

```basic
PLAY <周波数(Hz)> <長さ(秒)> <音量(0-100)> <sound型の変数>
```
引数を省略すると、周波数500Hz、長さ1秒、音量80の正弦波の音を出力します。

第一引数(周波数)にarray型の変数を渡すと、和音を出力することができます。
```basic
a = [100, 200]
PLAY a, 1, 80
```

sound型は音源を表しています。詳しくは<a href="vartype.html#sound-type">変数とデータ型</a>、<a>フィルター</a>を参照してください。

<h2 id="printwav">PRINTWAV</h2>

音を出力し、その波形を表示する命令です。

```basic
PRINTWAV <周波数(Hz)> <長さ(秒)> <音量(0~100)> <sound型の変数>
```
引数を省略すると、周波数500Hz、長さ1秒、音量80の正弦波の音を出力し、その波形を表示します。

![200Hzの正弦波の例]({{ '/assets/images/doc-instr-printwav.png' | relative_url }})

波形は二種類表示され、上は出力時間全体を見た波形、下は一部を拡大して見た波形になっています。

1つのソースコードに2つ以上PRINTWAV命令を使うと不具合が発生します。詳しくは、<a href="issue.html">現在発生している不具合</a>を参考にしてください。


<h2 id="printvar">PRINTVAR</h2>

現在定義されている変数と、その値を出力する命令です。

```basic
PRINTVAR
```

```basic
- Variable list -
    a(float) : 1.000000
    b(float) : 2.000000
    c(float) : 3.000000
```

<h2 id="sleep">SLEEP</h2>

指定した時間スリープする命令です。

```basic
SLEEP <長さ(秒)>
```

<h2 id="setsynth">SETSYNTH</h2>
シンセサイザーの設定を行う命令です。REPLモードでは実行できません。

```basic
SETSYNTH <シンセ番号>, <sound型の変数>, <float型の変数>, <値の範囲>
```

この命令を実行するとプログラム終了時にGUIソフトシンセが起動します。
![GUIソフトシンセ]({{ '/assets/images/doc-instr-setsynth.png' | relative_url }})

4個のGUIコントロールを使用でき、それらは引数のfloat型の変数と対応させることができます。
下に具体的な使用例を示します。
```basic
snd = SOUND[SQUARE]
a = 1
snd -> CHOP[a] -> snd

range = [0, 5]
/* 1番のシンセにsndを登録し、aのrangeの範囲分動かせるようにする */
SETSYNTH 1, snd, a, range
```

上の例では、変数aをSETSYNTH命令で1番のシンセサイザーに接続します。
変数aがとりうる値の範囲は、array型の変数rangeで指定されたものになります(要素数が2未満だとエラー)。

<h2 id="setloop">SETLOOP</h2>
GUIシンセモードでのループポイント設定を行う命令です。REPLモードでは実行できません。

```basic
SETLOOP <ループ時間(秒)>
```

シンセサイザーモードでは、指定した音が流れ続けます。しかし、Otoの音声出力の都合上音が一瞬停止してから再び鳴り始めます。
SETLOOP命令は、音が停止するまでの時間を指定します。

<h2 id="stop">STOP</h2>

処理を一時停止する命令です。キーが押されると再開します。

```basic
STOP
```
