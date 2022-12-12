#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <pthread.h>
#include <semaphore.h>
#include <string>
#include <unistd.h>


sem_t semaphore;
pthread_mutex_t forks[5];
static int COUNT;
static FILE *output_file = nullptr;

int getRandomTime(int a, int b) {
    return rand() % b + a;
}

void *eat(void *argc) {
    int phNumber = *(int *) argc;
    int t;
    std::string s;
    for (int i = 0; i < COUNT; i++) {
        sem_wait(&semaphore);

        pthread_mutex_lock(&forks[phNumber]);
        pthread_mutex_lock(&forks[(phNumber + 1) % 5]);

        t = getRandomTime(1, 10);

        printf("Philosopher(%d) is eating with forks %d and %d (%d seconds)\n", phNumber, phNumber, (phNumber + 1) % 5, t);
        if (output_file != nullptr) {
            fprintf(output_file, "Philosopher(%d) is eating with forks %d and %d (%d seconds)\n", phNumber, phNumber, (phNumber + 1) % 5, t);
        }
        sleep(t);

        printf("Philosopher(%d) finished his meal and put down forks %d and %d\n", phNumber, phNumber, (phNumber + 1) % 5);
        if (output_file != nullptr) {
            fprintf(output_file, "Philosopher(%d) finished his meal and put down forks %d and %d\n", phNumber, phNumber, (phNumber + 1) % 5);
        }

        pthread_mutex_unlock(&forks[phNumber]);
        pthread_mutex_unlock(&forks[(phNumber + 1) % 5]);

        sem_post(&semaphore);

        t = getRandomTime(1, 10);

        printf("Philosopher(%d) is thinking (%d seconds)\n", phNumber, t);
        if (output_file != nullptr) {
            fprintf(output_file, "Philosopher(%d) is thinking (%d seconds)\n", phNumber, t);
        }
        sleep(t);
    }
}

int main(int argc, char **argv) {
    if (argc == 1) {
        scanf("%d", &COUNT);
    } else if (argc == 2) {
        COUNT = atoi(argv[1]);
    } else if (argc == 3) {
        FILE *input_file = fopen(argv[1], "r");
        fscanf(input_file, "%d", &COUNT);
        printf("%d\n", COUNT);
        output_file = fopen(argv[2], "w");
    } else {
        printf("Invalid input!");
        return 0;
    }

    srand(time(nullptr));

    for (auto &fork: forks) {
        pthread_mutex_init(&fork, nullptr);
    }

    sem_init(&semaphore, 0, 4);

    pthread_t thread[5];

    int philosophers[5] = {0, 1, 2, 3, 4};

    for (int i = 0; i < 5; ++i) {
        pthread_create(&thread[i], nullptr, eat, (void *) &philosophers[i]);
    }

    for (auto &i: thread) {
        pthread_join(i, nullptr);
    }

    sem_destroy(&semaphore);

    for (auto &fork: forks) {
        pthread_mutex_destroy(&fork);
    }

    if (output_file != nullptr) {
        fclose(output_file);
    }
}