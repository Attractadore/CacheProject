# CacheProject
Добро пожаловать в наш уютный репозиторий! ^-^

Его создавал Миша, а ему помогали Полина и Лиза. 

Здесь мы реализуем наш любимый алгоритм кеширования LIRS.

Для ознакомления с принципом работы LIRS предлагаем вашему вниманию составленную нами презентацию: https://github.com/Attractadore/CacheProject/blob/master/LIRS%20Cache%20Algorithm.pdf

Для очень подробного ознакомления вы можете прочитать следующую статью: http://web.cse.ohio-state.edu/hpcs/WWW/HTML/publications/papers/TR-02-6.pdf

**Использование:**
Проект собирается через cmake.
В корне проекта:
```
mkdir build
cd build
cmake ..
cmake --build .
```
Чтобы выполнить тесты в той же дирректории `build`:
```
ctest .
```
Результат сборки -- исполняемый файл `BenchCache`, который получает на входе длину последовательности и саму последовательность неотрицательных чисел, а как аргументы -- название алгоритма и размера кэша и считает колличество промахов.
Также можно воспользоваться скриптом `adapter.sh` для иного интерфейса. Так размер кэша передается на входе, а не как аргумент.
Приятного чтения и использования! <3

P.S. Предлагаем скрины из рабочей переписки, чтобы воссоздать атмосферу, в которой создавался наш проект:

![](https://github.com/Attractadore/CacheProject/blob/master/memes/if_I_knew.PNG)

Mood - Миша, everyday:

![](https://github.com/Attractadore/CacheProject/blob/master/memes/faces.PNG)

"Аналогии это кринж"

![](https://github.com/Attractadore/CacheProject/blob/master/memes/orange.PNG)
