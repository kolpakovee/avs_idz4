#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <pthread.h>
#include <semaphore.h>
#include <string>
#include <unistd.h>

// семафор для ограничения доступа философов одновременно ко всем вилкам
sem_t semaphore;
// массив мьютексов, которые являются "вилками" (нужен, чтобы несколько философов не деражли одновременно одну вилку)
pthread_mutex_t forks[5];
// COUNT нужен для задания кол-ва циклов
static int COUNT;
// FILE на случай, если пользователь захочет вывести в файл
static FILE *output_file = nullptr;

// Функция для генерации времени для процесса "думать" и "есть"
int getRandomTime(int a, int b) {
    return rand() % b + a;
}

// Функция, имитирующая деятельность философа
void *eat(void *argc) {
    // Получение аргумента - число циклов
    int phNumber = *(int *) argc;
    int t;
    std::string s;
    // COUNT раз философ поест и подумает
    for (int i = 0; i < COUNT; i++) {
        // Уменьшаем семафор на 2, чтобы не было ситуации, когда все философы взяли по одной вилке
        sem_wait(&semaphore);
        sem_wait(&semaphore);

        // Блокируем вилки рядом с философом, чтобы два философа не брали одну и ту же вилку одновременно
        pthread_mutex_lock(&forks[phNumber]);
        pthread_mutex_lock(&forks[(phNumber + 1) % 5]);

        // Генерируем рандомное время
        t = getRandomTime(1, 10);

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

        // Так как философ доел, то можно освободить вилки
        pthread_mutex_unlock(&forks[phNumber]);
        pthread_mutex_unlock(&forks[(phNumber + 1) % 5]);

        // Так как положил вилки - увеличим семафор на 2 обратно
        sem_post(&semaphore);
        sem_post(&semaphore);

        // Генерируем рандомное время
        t = getRandomTime(1, 10);

        // Печатаем в консоль, что философ думает (он же уже поел)
        printf("Philosopher(%d) is thinking (%d seconds)\n", phNumber, t);
        // Если пользователь вводил имя файла -> печатаем в файл тоже
        if (output_file != nullptr) {
            fprintf(output_file, "Philosopher(%d) is thinking (%d seconds)\n", phNumber, t);
        }
        // Имитируем процесс "размышления"
        sleep(t);
    }
}

int main(int argc, char **argv) {
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

    srand(time(nullptr));

    // Инициализируем мьютексы
    for (auto &fork: forks) {
        pthread_mutex_init(&fork, nullptr);
    }

    // Инициализируем семафор
    sem_init(&semaphore, 0, 4);

    pthread_t thread[5];

    // Это массив философов
    int philosophers[5] = {0, 1, 2, 3, 4};

    // Запускаем потоки
    for (int i = 0; i < 5; ++i) {
        pthread_create(&thread[i], nullptr, eat, (void *) &philosophers[i]);
    }

    // Ждём выполнения всех потоков
    for (auto &i: thread) {
        pthread_join(i, nullptr);
    }

    // Разрушаем семафор
    sem_destroy(&semaphore);

    // Разрушаем мьютексы
    for (auto &fork: forks) {
        pthread_mutex_destroy(&fork);
    }

    // Закрываем поток
    if (output_file != nullptr) {
        fclose(output_file);
    }
    return 0;
}