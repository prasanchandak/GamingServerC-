/*
Program to simulate a login queue using semaphores.
Multiple threads try to enter the critical section and semaphores are used
to limit those that can access the critical seciton at the same time.
This kind of code can be used when a server can only serve a limited number of clients.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#define N_THREADS 16

sem_t semaphore; // Create a semaphore variable.

void* routine(void* args) {
    printf("(%d) Waiting for the Login Queue.\n", *(int*)args);
    sem_wait(&semaphore);
    sleep(rand()%5+1);
    // Sleep for a random amount of time between 1 to 5 seconds.
    printf("(%d) Logged In.\n", *(int*)args);
    printf("(%d) Logged Out.\n", *(int*)args);
    sem_post(&semaphore);
    free(args); 
    // Freeing the heap storage allocated in the for loop where thread was initialized.
}

int main(int argc, char* argv[]) {
    pthread_t th[N_THREADS];
    srand(time(NULL));
    sem_init(&semaphore, 0, 12);
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

/*
Source:
https://www.youtube.com/watch?v=Q79uEdKNVGY
*/