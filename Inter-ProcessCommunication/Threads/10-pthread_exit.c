// Using pthread_exit() to return a value from the thread function and 
// exit the main function. 

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define N_THREADS 10

void* routine() {
int value = rand()%6 + 1;
    // To get results between 1 and 6.
    int* result = malloc(sizeof(int));
    *result = value;
    sleep(2);
    printf("Thread result = %d\n", value);
    pthread_exit((void*)result);
    printf("Line after pthread_exit() in threaded function.\n");

}

int main(int argc, char *argv[]) {
    pthread_t th;
    int* ret;
    srand(time(NULL));
    if (pthread_create(&th, NULL, routine, NULL) != 0) {
        return 1;
    }

    pthread_exit(0);
    // If we want to terminate the main process but wait for the other threads to finish execution.
    // If we want to start some threads in main thread, leave them running and exit from main.
    
    // These lines below will not execute as pthread_exit() has been called.
    printf("Line of code below pthread_exit(0)\n");
    if (pthread_join(th, (void**)&ret) != 0) {
        return 2;
    }    

    printf("Result = %d\n", *ret);
    free(ret);

    return 0;

}

/*
Sources: 
https://www.youtube.com/watch?v=w_8bFrHUK5Q
*/ 