---
layout: default
---

<link rel="stylesheet" href="{{ '/assets/css/home.css' | relative_url }}">

<h1 class="home-helloworld">Hello Oto world!!!</h1>

<div class="home-text">
    ここは音遊びプログラミング言語Otoの公式サイトです。
</div>

```basic
    PLAY Do, 1, 70, piano
```

<div class= "overview">
    Otoは、音源自作や楽曲演奏といった「音遊び」を簡単に楽しめるプログラミング言語です。
</div>

<div class="home-news">
    <h2>News</h2>
    <ul>
        {% for post in site.posts %}
            {% capture year %}  {{ post.date | date: '%Y' }} {% endcapture %}
            {% capture month %} {{ post.date | date: '%m' }} {% endcapture %}
            {% capture day %}   {{ post.date | date: '%d' }} {% endcapture %}
            <oi>
                {{ year }}-{{ month }}-{{ day }} : <a href="/Oto{{ post.url }}">{{ post.title }}</a><br>
            </oi>
        {% endfor %}
    </ul>
</div>