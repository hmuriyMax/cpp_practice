# Практика C++

## Цель: создать приложение, которое будет отправлять сигналы доернему процессу, которое будет их получать и обрабатывать необходимым образом

В Windows нет сигналов как в Linux, поэтому приходится искать отдельные решения для Windows.
Путём некоторых изысканий в документации были найдены два полезных метода:

### 1. SetConsoleCtrlHandler(HandlerFunc, bool)

Эта функция устанавливает для событий `CTRL+C`, `CTRL+BREAK` и ещё некоторых [функцию-обработчик](<https://docs.microsoft.com/en-us/windows/console/handlerroutine>), которую мы сами можем определить и задать необходимые действия при этих событий. Таким образом мы подменяем действия по умолчанию действиями, которые мы определим.
В моём примере такой функцией являются `CtrlHandler` и `EmptyCtrlHandler`. Первая содержит в себе switch-case, который определяет, что делать с каждым из сигналов. Вторая просто возвращает значение `true`. Если функция возвращает `true`, то сигнал считается обработанным, а если `false`, то наоборот.

Мы применяем эту функцию и в дочернем, и в родительском процессе. В дочернем во время `CTRL+BREAK` мы симулируем контролируемое завершение процесса. Остальные сигналы просто выводят соответствующее сообщение об их получении.

В качестве эксперимента добавили такой же обработчик в родительскую программу

### 2. GenerateConsoleCtrlEvent(int, int)

С помощью этой функции можно генерировать события, считываемые функцией выше. Первый параметр - код команды, второй - `id` группы процессов, которой мы отправляем это событие. При этом стоит учесть следующее:

    При создании нового процесса необходимо задать флаги. Флаг, необходимый для создания нового процесса в новой консоли (CREATE_NEW_CONSOLE) противоречит флагу (CREATE_NEW_PROCESS_GROUP), поэтому не получится явно передавать сигнал из процессора в одной консоли в процессор в другой.

Из этой ситуации есть выход (костыль). Функция `SendSignal` отправляет сигнал, переподключаясь на другую консоль, таким обраом получается сигнал "в себя". Подробнее можно посмотреть в этой функции, там присутствуют комментарии от ее автора.

#### На счёт `sleep()`

Не смотря на то, что мы ставим пустой хендлер на все сигналы, если не поставить sleep, родительская консоль умрет вместе с дочерней. Практические опыты показали, что **достаточно минимальной задержки в 1мс**

### Вывод

Таким образом, была разобрана работа с сигналами средствами WinApi. Оказалось, что хоть эта функция и присутствует, но простота ее использования  оставляет желать лучшего.