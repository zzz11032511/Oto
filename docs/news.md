---
layout: default
title: "ニュース"
---

<link rel="stylesheet" href="{{ '/assets/css/style.css' | relative_url }}">

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