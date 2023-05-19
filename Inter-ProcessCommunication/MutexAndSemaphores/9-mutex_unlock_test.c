/*
Program to check if a mutex locked bu one thread can be unlocked by another thread.
This program will run for sometime but will crash abruptly AT SOME POINT.
This is because mutex is supposed to be unlocked by a thread that has locked it.
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#define N_THREADS 4
// sem_t sem_fuel;
pthread_mutex_t mutex_fuel;
pthread_mutexattr_t attr;
int *fuel;


void* routine1(void* args) {
    // while(1) {
        pthread_mutex_lock(&mutex_fuel);
        *fuel += 50;
        printf("Fuel = %d\n", *fuel);
        // pthread_mutex_unlock(&mutex_fuel);
        // pthread_mutex_unlock();
    // }
}

void* routine2(void* args) {
    // while(1) {
        pthread_mutex_unlock(&mutex_fuel);
        *fuel -= 50;
        printf("Fuel = %d\n", *fuel);
        // usleep(5000);
    // }
}

int main(int argc, char* argv[]) {
    pthread_t th[N_THREADS];
    fuel = malloc(sizeof(int));
    *fuel = 50;
    pthread_mutex_init(&mutex_fuel, NULL);
    // sem_init(&sem_fuel, 0, 1);
    // Binary semaphore. We set the initial value to 0 and then the main() thread will call sem_wait().
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK_NP);
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
    // sem_destroy(&sem_fuel);

    return 0;
}