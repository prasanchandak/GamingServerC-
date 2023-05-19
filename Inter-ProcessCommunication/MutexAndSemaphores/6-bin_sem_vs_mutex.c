/* 
Program to illustrate the differences between binary semaphores and mutex.
Here, 2 macros have been defined - USE_SEM and USE_MUTEX. Comment each alternately to
check the functioning of the code using either mutex or semaphores.
Semaphore can be put in wait by one process and post by another.
Mutex is SUPPOSED TO be unlocked by the thread/process that has locked it.
If another thread/process unlcks it, it results in undefined behaviour. 
Using mutex in this program will cause the program to abort at some point during execution.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#define N_THREADS 4
// #define USE_SEM
#define USE_MUTEX

sem_t sem_fuel;
pthread_mutex_t mutex_fuel;

int *fuel;

#ifdef USE_MUTEX
void* routine1(void* args) {
    while(1) {
        pthread_mutex_lock(&mutex_fuel);
        *fuel += 50;
        printf("Fuel = %d\n", *fuel);
        // pthread_mutex_unlock(&mutex_fuel);
    }
}

void* routine2(void* args) {
    while(1) {
        pthread_mutex_unlock(&mutex_fuel);
        usleep(5000);
    }
}
#endif

#ifdef USE_SEM
void* routine1(void* args) {
    while(1) {
        sem_wait(&sem_fuel);
        *fuel += 50;
        printf("Fuel = %d\n", *fuel);
    }
}

void* routine2(void* args) {
    while(1) {
        sem_post(&sem_fuel);
        usleep(5000);
    }
}
#endif

int main(int argc, char* argv[]) {
    pthread_t th[N_THREADS];
    fuel = malloc(sizeof(int));
    *fuel = 50;
    pthread_mutex_init(&mutex_fuel, NULL);
    sem_init(&sem_fuel, 0, 1);
    // Binary semaphore. We set the initial value to 0 and then the main() thread will call sem_wait().
    for (int i=0; i<N_THREADS; i++) {
        if (i%2 == 0) {
            if (pthread_create(&th[i], NULL, routine1, NULL) != 0) {
                perror("Failed to create a thread.");
            }
        }
        else {
            if (pthread_create(&th[i], NULL, routine2, NULL) != 0) {
                perror("Failed to create a thread.");
            }
        }
    }
    for (int i=0; i<N_THREADS; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread.");
        }
    }

    pthread_mutex_destroy(&mutex_fuel);
    sem_destroy(&sem_fuel);

    return 0;
}

/*
Source: https://www.youtube.com/watch?v=ZpRN3XQY4AA
*/