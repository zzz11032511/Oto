---
layout: default
---

<link rel="stylesheet" href="{{ '/assets/css/home.css' | relative_url }}">

<h1 class="home-helloworld">Hello Oto world!!!</h1>

<div class="home-text">
    ここは音遊びプログラミング言語Otoの公式サイトです.  
</div>

```basic
    PLAY 500, 1, 70, piano
```

<!-- <div class="home-index">
    <ul>
        <li>
            <a href="tutorial.html">チュートリアル</a>
            <br>
        </li>  
        <li>
            <a href="document.html">ドキュメント</a>
            <br>
        </li>  
        <li>
            <a href="sample.html">サンプル</a>
            <br>
        </li>
    </ul>
</div> -->

<div class="home-news">
    <h2>News</h2>
    <ul>
        {% for post in site.posts %}
            {% capture year %}  {{ post.date | date: '%Y' }} {% endcapture %}
            {% capture month %} {{ post.date | date: '%m' }} {% endcapture %}
            {% capture day %}   {{ post.date | date: '%d' }} {% endcapture %}
            <oi>
                {{ year }}-{{ month }}-{{ day }} : 
                <a href="/Oto{{ post.url }}">{{ post.title }}</a>
            </oi>
        {% endfor %}
    </ul>
</div>