#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#define N_THREADS 4

sem_t semaphore; // Create a semaphore variable.

void* routine(void* args) {
    sem_wait(&semaphore);
    sleep(1);
    printf("Thread: %d\n", *(int*)args);
    sem_post(&semaphore);
    free(args); 
    // Freeing the heap storage allocated in the for loop where thread was initialized.
}

int main(int argc, char* argv[]) {
    pthread_t th[N_THREADS];
    sem_init(&semaphore, 0, 2);\
    /*
    int sem_init(sem_t *sem, int pshared, unsigned int value);
    - pshared specifies whether semaphore is share dbetween processes or threads
    - value referes to the initialized value of the semaphore.
    The value represents the number of threads that can enter the critical section at the same time.
    */
    for (int i=0; i<N_THREADS; i++) {
        int * a = malloc(sizeof(int));
        // variable to pass the value of i to the thread function.
        // Address of i cannot be directly passed as its value keeps changing, so when passed the value
        // might be different that what it originally was.
        *a = i;
        if (pthread_create(&th[i], NULL, &routine, a) != 0) {
            perror("Failed to create thread.");
        }     
    }

    sem_destroy(&semaphore);
    // Destroy the semaphore variable.

    pthread_exit(0);
    // Terminate the main function while keeping the threads active.
    return 0;
}