/*
Implementation of pthread_mutex_trylock().
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#define N_THREADS 10

pthread_mutex_t stove_mutex;
int stove_fuel = 100;

void* routine(void* args) {
    pthread_mutex_lock(&stove_fuel);
    usleep(5000000);
    int fuel_needed = (rand()%20);
    if (stove_fuel - fuel_needed < 0) {
        printf("No more fuel.\n");
    }
    stove_fuel -= (rand()%20);
    printf("Fuel left: %d", stove_fuel);
    pthread_mutex_unlock(&stove_fuel);
}

int main(int argc, char* argv[]) {
    pthread_t th[10];
    for (int i=0; i<N_THREADS; i++) {
        if (pthreas_create(&th[i], NULL, routine, NULL) != 0) {
            perror("Failed to create thread.");
        }
    }

    for (int i=0; i<N_THREADS; i++) {
        if (pthreas_join(th[i], NULL) != 0) {
            perror("Failed to create thread.");
        }
    }

    pthread_mutex_destroy(&stove_mutex);

    return 0;

}
