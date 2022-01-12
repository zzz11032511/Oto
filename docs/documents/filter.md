---
layout: default
title: "ドキュメント - フィルター"
---

# フィルター

フィルターは、Otoで音源を自作する際に使用する機能です。oscillator型で定義した基本波形に、様々な効果を加えることができます。

```basic
osc = OSCIL[1]
snd = SOUND[osc]

snd -> FADE_IN[1.2] -> DETUNE[2] -> snd2

PLAY 500, 1, 60, snd2
```

フィルターとsoundオブジェクトをコネクター演算子「->」で接続することができ、そうして出来たものを新しいsoundオブジェクトとして定義できます。

ここでは、PRINTWAV命令で表示した波形とともに、Otoで用意されているフィルターについて解説していきます。

<h2 id="clip">CLIP</h2>

大きすぎる音をカットするフィルターです。

```basic
CLIP[]
```
![CLIPフィルター]({{ '/assets/images/filter-clip.png' | relative_url }})

音の大きさが-1.0-1.0の範囲外のものを-1.0または1.0に補正します。
今後のアップデートでは、任意の大きさ以上の音をカットできるようにする予定です。

<h2 id="fadein">FADE_IN</h2>

最初の音を小さくし、だんだんと大きくしていくようにするフィルターです。

```basic
FADE_IN[<時間(秒)>]
```
![FADE_INフィルター]({{ '/assets/images/filter-fadein.png' | relative_url }})

引数には、音の始まりから何秒目で100%の音量になるのかを指定します。

<h2 id="fadeout">FADE_OUT</h2>

音が終わるにつれて徐々に小さくなるようにするフィルターです。

```basic
FADE_OUT[<時間(秒)>]
```
![FADE_OUTフィルター]({{ '/assets/images/filter-fadeout.png' | relative_url }})

引数には、音の終わりから何秒目で音を小さくし始めるのかを指定します。

<h2 id="fade">FADE</h2>

FADE_INフィルターとFADE_OUTフィルターを両方合わせたものです。

```basic
FADE[<100%になるまでの時間(秒)>, <音が小さくなり始める時間(秒)>]
```
![FADEフィルター]({{ '/assets/images/filter-fade.png' | relative_url }})

Otoの標準では、クリップノイズ防止のためにフィルター無しの状態でもフェード効果がかかっています。詳しくは<a href="option.html">実行時オプション</a>を参照してください。

<h2 id="amp">AMP</h2>

音を大きくするフィルターです。

```basic
AMP[<増幅率>]
```
![AMPフィルター]({{ '/assets/images/filter-amp.png' | relative_url }})

このフィルターを通すことによって、音の大きさが増幅率分大きくなります。潰れた音色を作ることができます。

<h2 id="tremolo">TREMOLO</h2>

音の大きさを周期的に揺らすフィルターです。

```basic
TREMOLO[<深さ>, <速さ>]
```
![TREMOLOフィルター]({{ '/assets/images/filter-tremolo.png' | relative_url }})

<h2 id="detune">DETUNE</h2>

現在の音に音程のわずかに違う音を重ねることで、コーラス効果を与えるフィルターです。

```basic
DETUNE[<音のずれ(Hz)>]
```
![DETUNEフィルター]({{ '/assets/images/filter-detune.png' | relative_url }})

例えば、500Hzの正弦波にDETUNEフィルターを音のずれ2Hzで通すと、502Hzの波形が足し算されることになります。
音のずれが大きすぎると不安定な音になります。

<h2 id="chop">CHOP</h2>

音の波形を切り刻むフィルターです。

```basic
CHOP[<速さ>]
```
![CHOPフィルター]({{ '/assets/images/filter-chop.png' | relative_url }})

<h2 id="lpf">LPF</h2>

指定した周波数以上の成分を除去します。

```basic
LPF[<カットオフ周波数(Hz)>]
```
![LPFフィルター]({{ '/assets/images/filter-lpf1.png' | relative_url }})

現在、LPF、HPF、WAH、RADIOのいずれかを同時に接続すると不具合が発生します。詳しくは、<a href="issue.html">現在発生している不具合</a>を参考にしてください。

<h2 id="hpf">HPF</h2>

指定した周波数以下の成分を除去します。

```basic
HPF[<カットオフ周波数(Hz)>]
```
![HPFフィルター]({{ '/assets/images/filter-hpf1.png' | relative_url }})

<h2 id="wah">WAH</h2>

カットオフ周波数を周期的に変化させるフィルターです。

```basic
WAH[<レゾナンス>, <ワウの深さ>, <ワウの速さ>]
```
![WAHフィルター]({{ '/assets/images/filter-wah.png' | relative_url }})

1000Hzを基準として第2引数に指定した分だけ変化します。レゾナンスはカットオフ周波数付近での増幅率のことで、値を変えると音の雰囲気を変えることができます。

<h2 id="radio">RADIO</h2>

ラジオのような効果を与えます。

```basic
RADIO[]
```
![RADIOフィルター]({{ '/assets/images/filter-radio.png' | relative_url }})
