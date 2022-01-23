---
layout: default
title: "サンプル - GUIソフトシンセ"
---

## GUIソフトシンセ

includeファイル : [stdosc.oto](stdlib/stdosc.html)

```basic
@include "stdosc.oto"

snd = SOUND[SINE]
a = 1
b = 1
c = 1
d = 1
snd -> CHOP[a] -> DETUNE[b] -> TREMOLO[c, d] -> snd

/* 1から10まで */
range = [1, 10]

/* 1番のシンセに変数aを登録し、値のとりうる範囲はrangeの分だけとする */
SETSYNTH 1, snd, a, range

/* 2番のシンセに変数bを登録し、値のとりうる範囲はrangeの分だけとする */
SETSYNTH 2, snd, b, range

/* 3番のシンセに変数cを登録し、値のとりうる範囲はrangeの分だけとする */
SETSYNTH 3, snd, c, range

/* 4番のシンセに変数dを登録し、値のとりうる範囲はrangeの分だけとする */
SETSYNTH 4, snd, d, range
```

### 出力結果

![出力結果]({{ '/assets/images/samples/guisynth.png' | relative_url }})
