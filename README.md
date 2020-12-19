# fortran95-interpreter
Вашему вниманию - Интерпретатор Fortran95 (В теории)

По сборке - cmake >= 3.17, bison >= 3.5, flex >= 2.5, плюсы версии 17, gtests

Важно! Тесты запускать в директории tests (да, я не умею подтягивать данные к тестам, хотя возможно это уже исправлено, потому #TODO)
Что поддерживатся?

Типы: 

+ "INTEGER" (kind=4 only)
+ "CHARACTER", c поддержкой len, естественно
+ "LOGICAL"


Констаты:

+ целые числа (в т.ч. отрицательные)
+ логические ".TRUE." и ".FALSE."
+ строковые


Целочисленная арифметика (без power(**) #TODO)
Логические ">", "<" для чисел и строк ("<=" и ">=" пока нет, #TODO)
Логические ".AND.", ".OR.", ".NOT.", ".EQV."


if:
```
if (..) then
  ...
else if (..) then
  ...
else 
  ..
end if
```

do (аналог for):
```
do i = start, finish [, step]
  ...
end do
```

do-while (аналог while):
```
do while (condition)
  ...
end do
```


Что работает **НЕПРАВИЛЬНО**
+ Нельзя использовать ключевые слова как идентификаторы (program, if, while), в настоящем фортране можно