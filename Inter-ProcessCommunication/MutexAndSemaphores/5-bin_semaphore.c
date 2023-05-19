/* 
Program to implement binary semaphores.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#define N_THREADS 4

sem_t sem_fuel;
pthread_mutex_t mutex_fuel;

int *fuel;

void* routine(void* args) {
    sem_wait(&sem_fuel);
    *fuel += 50;
    printf("Fuel = %d\n", *fuel);
    sem_post(&sem_fuel);
}

int main(int argc, char* argv[]) {
    pthread_t th[N_THREADS];
    fuel = malloc(sizeof(int));
    *fuel = 50;
    pthread_mutex_init(&mutex_fuel, NULL);
    sem_init(&sem_fuel, 0, 1);
    // Binary semaphore. We set the initial value to 0 and then the main() thread will call sem_wait().
    for (int i=0; i<N_THREADS; i++) {
        if (pthread_create(&th[i], NULL, &routine, NULL) != 0) {
            perror("Failed to create a thread.");
        }
    }
    
    
    for (int i=0; i<N_THREADS; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to create a thread.");
        }
    }

    // sem_wait(&sem_fuel);
    // This makes the main thread wait until the value of semaphore changes after threaded function.
    printf("Deallocating fuel.\n");
    free(fuel);

    pthread_mutex_destroy(&mutex_fuel);
    sem_destroy(&sem_fuel);

    return 0;
}

/*
Source: https://www.youtube.com/watch?v=YpD-vsj5Jbk
*/