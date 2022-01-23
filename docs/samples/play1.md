---
layout: default
title: "サンプル - 音声出力1"
---

## 音声出力1

```basic
/* OSCILは基本波形を表現する */
osl = OSCIL[1]

/* SOUNDはエフェクトを含む音源の情報を表現する */
s = SOUND[osl]

/* SOUNDにフィルターを何段も重ねて、独創的な音を作る */
s -> FADE_IN[0.5] -> AMP[0.8] \
  -> DETUNE[2] -> TREMOLO[1.2, 10] -> s

/* 500Hz、音量30の音を音源はsで3秒間鳴らす */
PLAY 500, 3, 30, s
```