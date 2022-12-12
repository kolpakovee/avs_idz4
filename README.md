# avs_idz4
Здравствуйте, дорогие проверяющие! 

Это ИДЗ №4 по АВС студента БПИ215 Колпакова Егора Евгеньевича. По мнению автора оно выполнено на 9 баллов.

Мой вариант 5 представлен ниже:

![v6](https://github.com/kolpakovee/avs_idz4/blob/main/images/task.png)

Чтобы было немного понятнее я решил сделать небольшую визуализацию (такую нигде не найти)
Философы пронумерованы от 0 до 4, так же, как и вилки. Дальнейшие детели реализации раскроются в отчёте.

![v6](https://github.com/kolpakovee/avs_idz4/blob/main/images/picture.png)

---
**Формат ввода:**
- Программа запускается через командую строку и реализован ввод из консоли, командной строки или файла.
- Входной параметр - кол-во раз, которое покушает каждый философ
- Формат ввода зависит от кол-ва аргументов командной строки:

argv == 1 -> консольный ввод входного параметра (в данном примере ввели единицу)
```
./a.out
1
```
argv == 2 -> входной параметр вводится из командной строки (в данном пример вводится двойка)
```
./a.out 2
```
argv == 3 -> ввод из файла (нужно указать имена входного и выходного файлов)
```
./a.out test.in out.txt
```

*ВАЖНО, что вводимое число должно быть натуальным (>0)*

---
**Формат вывода.**
Выводится 3 * 5 * <входной параметр> строк о том, что делют философы:
```
Philosopher(0) is thinking (4 seconds)
Philosopher(4) is eating with forks 4 and 0 (8 seconds)
Philosopher(1) is eating with forks 1 and 2 (10 seconds)
Philosopher(4) finished his meal and put down forks 4 and 0
Philosopher(4) is thinking (7 seconds)
...
```
---
## 4 балла
![v6](https://github.com/kolpakovee/avs_idz4/blob/main/images/4.png)
- Условие задачи приведино
- Моя модель параллельных вычислений - итеративный параллелизм, так как я использую для реализации 5 потоков, каждый из которых содержит циклы. Потоки программы, описываются итеративными функциями и работают совместно над решением одной задачи. 
- Входные данные описывают кол-во итераций, которое выполнит каждый из потоков (философов). В одной итерации выполняется "приём пищи" и "процесс мышления". Каждый из процессов идёт случайное время, поэтому каждый запуск - уникальное поведение. Впрочем, это можно увидеть по тестовым файлам далее в отчёте
- Реализовано консольное приложение с использованием мьютексов и семафоров. Семафоры нужны для того, чтобы каждый из философов не взял по одной вилке (например, левой) и они сидели и не кушали. Поэтому семафор создаётся со значением 4, таким образом одновременно кушать могут лишь два философа. Теперь о мьютексах: так как философы борятся за совместный ресурс (вилки), два философа не должны одновременно есть одно вилкой, поэтому как только философ берёт вилку - ресурс блокируется для других философов
- Ввод данных реализован из консоли/командной строки/файла
## 5 баллов
![v6](https://github.com/kolpakovee/avs_idz4/blob/main/images/5.png)
- Комментарии добавлены в программу. Можно увидеть в файле main.cpp
- Сущности программы: философы (5 штук), вилки (5 штук). Каждый из филосфов может или думать, или кушать. Когда один из философов потоков подумал, он ждёт, когда рядом с ним освободяться обе вилки (освободяться другими философами). Как только они освобождаются, он их берёт и начинает кушать какое-то время, таким образом его "соседи"(философы слева и справа от него) не смогут кушать в это время и будут ждать. В задаче есть две глобальные проблемы: состояние системы, в котором каждый философ взял вилку слева и ждёт, когда вилка справа освободится, и состояние системы, в котором некоторые философы получают доступ к своим вилкам существенно чаще, чем остальные философы. Бороться с ними нужно с помощью приоритезации и исключения взаимной блокировки. 
- Поведение сущностей: филомофы(потоки) сидят за столом, два "несоседних" философа берут по две вилки и начинают есть. Остальные ждут, пока эти вилки освободятся. Как только у какого-то из философов появляется возможность кушать (то есть кто-то из первых двух поел и полоижл вилки), он сразу начинает есть. В свою очередь философ, который уже поел и положил вилки садится думать и ждать, пока освободятся вилки. Так каждый философ будет делать столько раз, сколько захочет пользователь (потому что пользователь вводит кол-во итераций)
## 6 баллов
![v6](https://github.com/kolpakovee/avs_idz4/blob/main/images/6.png)
- В программе есть 5 философов (потоков)
```
pthread_t thread[5];
// Это массив философов
int philosophers[5] = {0, 1, 2, 3, 4};

// Запускаем потоки
for (int i = 0; i < 5; ++i) {
    pthread_create(&thread[i], nullptr, eat, (void *) &philosophers[i]);
}
```
- Так же есть 5 вилок, которые представляют собой мьютексы
```
pthread_mutex_t forks[5];
```
- Каждый поток выполняет функцию eat, она достаточно объёмная, поэтому приведу её здесь частями и опишу поведение потоков
```
void *eat(void *argc)
```
- Как я уже говорил ранее, глобально у нас есть две проблемы при параллельном выполнении программы: состояние системы, в котором каждый философ взял вилку слева и ждёт, когда вилка справа освободится, и состояние системы, в котором некоторые философы получают доступ к своим вилкам существенно чаще, чем остальные философы. Так же следует добавить сюда борьбу за совместные ресурсы (вилки). Теперь опишу, как я с ними боролся.
- Проблема 1 + Проблема 2(взаимная блокировка и один ест чаще). Для исключения этих я использую семафор. Семафор создаётся со значением 4, чтобы исключить ситуацию когда один философ ест чаще и все философы взяли по одной вилке
```
sem_t semaphore;
sem_init(&semaphore, 0, 4);
sem_destroy(&semaphore);
```
- Как только философ берёт вилки, я уменьшаю значение семафора на 2
```
sem_wait(&semaphore);
sem_wait(&semaphore);
```
- Как тольео философ кладёт вилки, я увеличиваю значение семафора обратно
```
sem_post(&semaphore);
sem_post(&semaphore);
```
- Проблема 3(борьба за совместные ресурсы). Для решения этой проблемы я использовал мьютексы, а именно целых 5 штук
```
pthread_mutex_t forks[5];

for (auto &fork: forks) {
    pthread_mutex_init(&fork, nullptr);
}

for (auto &fork: forks) {
    pthread_mutex_destroy(&fork);
}
```
- Как только философ берёт вилку я блокирую мьютекс и освобождаю его, как только он положил её обратно
```
pthread_mutex_lock(&forks[phNumber]);
pthread_mutex_lock(&forks[(phNumber + 1) % 5]);

pthread_mutex_unlock(&forks[phNumber]);
pthread_mutex_unlock(&forks[(phNumber + 1) % 5]);
```
- Таким образом сразу несколько философов не могут использовать одну вилку. Проблема совместного использования ресурсов решена.
- Ввод из командной строки реализован
```
if (argc == 1) {
    // ввод с консоли
    scanf("%d", &COUNT);
} else if (argc == 2) {
    // ввод с командной строки
    COUNT = atoi(argv[1]);
} else if (argc == 3) {
    // ввод / вывод из файла (в файл)
    FILE *input_file = fopen(argv[1], "r");
    fscanf(input_file, "%d", &COUNT);
    output_file = fopen(argv[2], "w");
} else {
    // В случае, если некорректное число аргументов
    printf("Invalid input!");
    return 0;
}
```
## 7 баллов
![v6](https://github.com/kolpakovee/avs_idz4/blob/main/images/7.png)
- Реализован ввод из файла и вывод в файл
```
// ввод
FILE *input_file = fopen(argv[1], "r");
fscanf(input_file, "%d", &COUNT);
output_file = fopen(argv[2], "w");

// вывод
fprintf(output_file, "Philosopher(%d) is eating with forks %d and %d (%d seconds)\n", phNumber, phNumber, (phNumber + 1) % 5, t);
fprintf(output_file, "Philosopher(%d) finished his meal and put down forks %d and %d\n", phNumber, phNumber, (phNumber + 1) % 5);
fprintf(output_file, "Philosopher(%d) is thinking (%d seconds)\n", phNumber, t);
```
- Входные и выходные данные с тестами в папке tests
- Результаты работы программы выводятся на экран и записываются в файл
```
// Печатаем в консоль, что философ начинает есть
printf("Philosopher(%d) is eating with forks %d and %d (%d seconds)\n", phNumber, phNumber, (phNumber + 1) % 5, t);
// Если пользователь вводил имя файла -> печатаем в файл тоже
if (output_file != nullptr) {
    fprintf(output_file, "Philosopher(%d) is eating with forks %d and %d (%d seconds)\n", phNumber, phNumber, (phNumber + 1) % 5, t);
}
// Имитируем процесс "поедания"
sleep(t);

// Печатаем в консоль, что философ положил вилки
printf("Philosopher(%d) finished his meal and put down forks %d and %d\n", phNumber, phNumber, (phNumber + 1) % 5);
// Если пользователь вводил имя файла -> печатаем в файл тоже
if (output_file != nullptr) {
    fprintf(output_file, "Philosopher(%d) finished his meal and put down forks %d and %d\n", phNumber, phNumber, (phNumber + 1) % 5);
}
printf("Philosopher(%d) is thinking (%d seconds)\n", phNumber, t);
// Если пользователь вводил имя файла -> печатаем в файл тоже
if (output_file != nullptr) {
    fprintf(output_file, "Philosopher(%d) is thinking (%d seconds)\n", phNumber, t);
}
```
- Расширен ввод данных из командной строки
## 8 баллов
![v6](https://github.com/kolpakovee/avs_idz4/blob/main/images/8.png)
- Добавлена генерация случайных чисел для времени "еды" и "размышлений" философов
- ПРИВЕСТИ ВХОДНЫЕ И ВЫХОДНЫЕ ДАННЫЕ С ТЕСТАМИ
- Расширен ввод данных из командной строки
## 9 баллов
![v6](https://github.com/kolpakovee/avs_idz4/blob/main/images/9.png)
- ПРОВЕСТИ ИССЛЕДОВАНИЕ ПОВЕДЕНИЯ ПРОГРАММЫ, ЕСЛИ НЕ ИСПОЛЬЗОВАТЬ МЬЮТЕКСЫ + СЕМАФОРЫ, ТОЛЬКО МЬЮТЕКСЫ, ТОЛЬКО СЕМАФОРЫ)
- ОПИСАТЬ НЕКОРРЕКТНЫЕ СИТУАЦИИ
