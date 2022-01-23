---
layout: default
title: "サンプル - "
---

## 音声出力2

includeファイル : [stdosc.oto](stdlib/stdosc.html)  
includeファイル : [doremi.oto](stdlib/doremi.html)

```basic
@include "stdosc.oto"
@include "doremi.oto"

s1 = SOUND[SINE]      /* サイン波 */
s2 = SOUND[SAW]       /* のこぎり波 */
s3 = SOUND[SQUARE]    /* 矩形波 */
s4 = SOUND[TRIANGLE]  /* 三角波 */
s5 = SOUND[NOISE]     /* ノイズ */

PLAY Do1, 0.5, 30, s1  /* ド(サイン波) */
PLAY Re1, 0.5, 30, s2  /* レ(のこぎり波) */
PLAY Mi1, 0.5, 30, s3  /* ミ(矩形波) */
PLAY Fa1, 0.5, 30, s4  /* ファ(三角波) */
PLAY So1, 0.5, 30, s5  /* ソ(ノイズ) */
```