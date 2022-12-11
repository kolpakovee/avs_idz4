#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>


sem_t semaphore;
pthread_mutex_t forks[5];
int COUNT;

int getRandomTime(int a, int b) {
    return rand() % b + a;
}

void *eat(void *argc) {
    int phNumber = *(int *) argc;
    int t;
    for (int i = 0; i < COUNT; i++) {
        sem_wait(&semaphore);

        pthread_mutex_lock(&forks[phNumber]);
        pthread_mutex_lock(&forks[(phNumber + 1) % 5]);

        t = getRandomTime(1, 10);
        printf("Philosopher(%d) is eating with forks %d and %d (%d seconds)\n", phNumber, phNumber, (phNumber + 1) % 5, t);
        sleep(t);

        printf("Philosopher(%d) finished his meal and put down forks %d and %d\n", phNumber, phNumber, (phNumber + 1) % 5);

        pthread_mutex_unlock(&forks[phNumber]);
        pthread_mutex_unlock(&forks[(phNumber + 1) % 5]);

        sem_post(&semaphore);

        t = getRandomTime(1, 10);
        printf("Philosopher(%d) is thinking (%d seconds)\n", phNumber, t);
        sleep(t);
    }
}

int main() {
    scanf("%d", &COUNT);

    srand(time(nullptr));
    for (auto & fork : forks) {
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

    for (auto & fork : forks) {
        pthread_mutex_destroy(&fork);
    }
    return 0;
}