// Creating multiple threads using a for loop.
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define OPTIMISED 
// macro that ensures the optimised code runs. If it is not defines, the unoptimised code runs.
#define N_THREADS 16
// define the number of threads that have to be used

int counter = 0;
pthread_mutex_t mutex;

void* routine() {
    for(int i=0; i<10000000; i++) {
        pthread_mutex_lock(&mutex);
        counter++;
        pthread_mutex_unlock(&mutex);
    }
}

int main(int argc, char* argv[]) {
    pthread_t th[N_THREADS]; // Array of thread identifiers.
    pthread_mutex_init(&mutex, NULL);
    // Function initializes mutex with default values.

    #ifndef OPTIMISED
    // In this method, the thread is created and then system waits for it to
    // finish its execution because of pthread_join(). Therefore, pthread_create and pthread_join()
    // should not be called in the same loop. This will have sequential execution instead of concurrent execution.
    for(int i=0; i<N_THREADS; i++) {
        if (pthread_create(th+i, NULL, routine, NULL) != 0) {
            perror("Failed to create thread.\n");
            return 1;
        }
        if (pthread_join(th[i], NULL) != 0) {
            return 2;
        }
    }
    #endif

    #ifdef OPTIMISED
    // Creating different loops for create and join for concurrency.
    for(int i=0; i<N_THREADS; i++) {
        if (pthread_create(th+i, NULL, routine, NULL) != 0) {
            perror("Failed to create thread.\n");
            return 1;
        }
    }
    for(int i=0; i<N_THREADS; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            return 2;
        }
    }
    #endif

    pthread_mutex_destroy(&mutex);
    // Destroy memory allocated to the mutex variable.
    printf("Value of counter: %d\n", counter);
    return 0;
}