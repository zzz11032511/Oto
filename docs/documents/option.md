---
layout: default
title: "ドキュメント - 実行時オプション"
---

# 実行時オプション

Oto起動時に、同じディレクトリに.otoconfという名前のファイルを入れておくことで、Otoの設定を変えることができます。

```
(作業ディレクトリ)
├─ XXX.oto
└─ .otoconf
```

.otoconfファイルは以下のように記述します。

```yaml
timecount: true
repl: false
language: kanji
default_srcpath: aaa.oto
sampling_rate: 8000
fade_range: 0.1
```

「<設定名> : <値>」のフォーマットで記述します。yamlに似ていますが違います。

<h2 id="opt-timecount">timecount</h2>

trueにすると実行時間を表示します。標準ではfalseです。

```yaml
timecount: false
```

<h2 id="opt-repl">repl</h2>

trueにするとREPLモードで実行します。標準ではfalseです。
このオプションが無くてもファイル名を指定せずに実行するとREPLが起動しますが、指定すると、仮にファイル名を指定して実行してもREPLが起動するようになります。

```yaml
repl: true
```

<h2 id="opt-language">language</h2>

Otoで使用する言語を指定します。標準ではkanjiです。

```yaml
language: kanji     /* 日本語 */
language: hiragana  /* にほんご */
language: english   /* English */
```

日本語(漢字)、日本語(ひらがな)、英語が選択できます。

<h2 id="opt-srcpath">default_srcpath</h2>

デフォルトで読み込むソースファイルのパスを指定します。標準では設定されていません。

```yaml
default_srcpath: aaa.oto
```

この設定をすると、Otoを起動する際にファイル名を指定しなくても、REPLでなくファイルを読み込んで実行します。
もしこのオプションを書いたままREPLを行いたいなら、replオプションを有効にしてください。

<h2 id="opt-samprate">sampling_rate</h2>

音声出力の際に使用するサンプリング周波数をHzで指定します。標準は44100です。

```yaml
sampling_rate: 8000
```

<h2 id="opt-faderange">fade_range</h2>

音にかかっているフェード効果の大きさを秒で指定します。

Otoではクリップノイズ除去のために、音の始まりと終わりにフェード効果がかかっています。それを制御することができます。標準は0.05秒です。

```yaml
fade_range: 0.1
```