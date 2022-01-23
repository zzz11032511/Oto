---
layout: default
title: "サンプル - 波形出力"
---

## 波形出力2

includeファイル : [stdosc.oto](stdlib/stdosc.html)  
includeファイル : [doremi.oto](stdlib/doremi.html)

```basic
@include "stdosc.oto"
@include "doremi.oto"

s = SOUND[TRIANGLE]
chord_G = [So1, Si1, Le2]

s -> FADE_OUT[0.4] -> s

/* Gの和音、音量80の音を音源はsで1秒間鳴らす */
PRINTWAV chord_G, 1, 80, s
```

### 出力結果

![出力結果]({{ '/assets/images/samples/printwav.png' | relative_url }})
