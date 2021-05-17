# CacheProject
Добро пожаловать в наш уютный репозиторий! ^-^

Его создавал Миша, а ему помогали Полина и Лиза. 

Здесь мы реализуем наш любимый алгоритм кеширования LIRS.

Для ознакомления с принципом работы LIRS предлагаем вашему вниманию составленную нами презентацию: https://github.com/Attractadore/CacheProject/blob/master/LIRS%20Cache%20Algorithm.pdf

Для очень подробного ознакомления вы можете прочитать следующую статью: http://web.cse.ohio-state.edu/hpcs/WWW/HTML/publications/papers/TR-02-6.pdf

Дата создания - май 2021

Данный репозиторий тесно связан с репозиторием DataStructures https://github.com/Attractadore/DataStructures.
**Вот основные файлы, используемые в проекте:**

- BaseCachePolicy.c - адаптер для всего алгоритма;

- BaseDoubleList.c - функции для двусвязного списка. Он используется в реализации LRU;

- BaseMonoList.c - функции для односвязного списка, который используется в реализации LIRS;

- BaseOpenHashTable.c - функции хэш-таблицы;

- BaseVector.c - контейнер для данных;

- LIRSCache.c - алгоритм LIRS;

- LRUCache.c - алгоритм LRU.

В папке test хранятся тесты для наших функций.

**Использование:** /.../

Приятного чтения и использования! <3

P.S. Предлагаем скрины из рабочей переписки, чтобы воссоздать атмосферу, в которой создавался наш проект:

![](https://github.com/Attractadore/CacheProject/blob/master/memes/if_I_knew.PNG)

Mood - Миша, everyday:

![](https://github.com/Attractadore/CacheProject/blob/master/memes/faces.PNG)

"Аналогии это кринж"

![](https://github.com/Attractadore/CacheProject/blob/master/memes/orange.PNG)
