# fortran95-interpreter
Вашему вниманию - Интерпретатор Fortran95 (В теории)

По сборке - cmake >= 3.0, bison >= **3.5**, flex >= **2.5**, плюсы версии 17, gtests

Важно! Тесты запускать в директории tests (да, я не умею подтягивать данные к тестам, потому #TODO)

Что поддерживается?

Типы: 

+ "INTEGER" (kind=4 only)
+ "CHARACTER", c поддержкой len, естественно
+ "LOGICAL"


Константы:

+ целые числа (в т.ч. отрицательные)
+ логические ".TRUE." и ".FALSE."
+ строковые


Целочисленная арифметика (без power(**) #TODO)
Логические ">", "<", "==", "<=", ">=" для чисел и строк
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
+ Все переменные требуют описания(так, как того требует implicit none, хотя сам он даже не парсится #TODO)

Что не работает
+ Функции (даже встроенные #TODO)
+ Массивы
+ Real
+ ООП
+ implicit none

