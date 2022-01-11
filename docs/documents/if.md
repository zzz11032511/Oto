---
layout: default
title: "ドキュメント - if文"
---

# if文

条件分岐を表現するにはif文を使用します。

```basic
i = 1
IF [i >= 1] THEN
    PRINT i
END
```

ifの後の括弧の中には条件式を代入します。式の結果が0以外の値ならif文の中身を実行します、そうでないなら実行しません。

<h2 id="elsif-else">elsif, else</h2>

elsif文、else文を使えばさらに複雑な条件分岐も表現できます。

```basic
a = 2
IF [a > 10] THEN
    PRINT 1
ELSIF [a > 5] THEN
    PRINT 2
ELSE
    PRINT 3
END
```

上の例ではまず「aが10より大きい」かを判定して、もし正しいなら"PRINT 1"、そうでないならつぎに「aが5より大きい」かを判定して、正しいなら"PRINT 2"、そうでもないなら"PRINT 3"を実行します。

else文にはTHENが付かないことに注意してください。

<h2 id="not">not</h2>

時には、否定を用いた方が条件式がすっきりする場合があります。

```basic
IF [a != 1 OR b != 2] THEN
    PRINT 1
END
```

そのときにはnot文を使うことができます。

```basic
IF NOT [a == 1 AND b == 2] THEN
    PRINT 1
END
```

notは演算子ではないことに注意してください。