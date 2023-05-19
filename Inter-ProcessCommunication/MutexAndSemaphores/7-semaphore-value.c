/*
Program to display the current value of a semaphore variable.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>

#define N_THREADS 4

sem_t sem;

void* routine(void* args) {
    int index = *(int*)args;
    int sem_val; 
    // Variable to store the value of the semaphore.
    sem_wait(&sem);
    //sleep(index+1);
    // Above line added to get the outpue in sequence of threads' increasing order.
    printf("Thread (%d) has entered critical seciton.\n", index);
    sem_getvalue(&sem, &sem_val);
    // Function to get the current value of the semaphore.
    // The value may change by the time sem_val has been set.
    // sem_getvalue() is not free of race conditions. 
    // For eg., by the time the value is returned to us, another thread may 
    // have changed the value of the semaphore, hence its not updated.
    printf("Current value of the semaphore after wait: %d\n", sem_val);
    sem_post(&sem);
    printf("Thread (%d) has left critical section.\n", index);
    sem_getvalue(&sem, &sem_val);
    printf("Current value of the semaphore after post: %d\n", sem_val);
    free(args);
    // Free memory that was allocated to store and pass the thread id to the function. 
}

int main(int argc, char* argv[]) {
    pthread_t th[N_THREADS];
    sem_init(&sem, 0, 4);
    // Initialize the semaphore with a value of 4.
    for(int i=0; i<N_THREADS; i++) {
        int *a = malloc(sizeof(int));
        *a = i;
        if (pthread_create(&th[i], NULL, routine, a) != 0) {
            perror("Failed to create thread.\n");
        }
    }

    for(int i=0; i<N_THREADS; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread.\n");
        }
    }

    sem_destroy(&sem);

    return 0;
}

/*
Source: https://www.youtube.com/watch?v=0ZlrB6rrUhA
*/