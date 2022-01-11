---
layout: default
title: "ドキュメント - loop文"
---

# loop文

繰り返し処理を行いたい場合にはloop文を使います。

```basic
i = 1
LOOP [5] BEGIN
    PRINT i
    i = 1
END
```

loopの後のカッコ内には繰り返す回数を指定します。また、繰り返し実行したい処理はBEGIN-ENDの中に記述します。

今後のアップデートでは制御変数をシンプルに扱えるようにしていくつもりです。