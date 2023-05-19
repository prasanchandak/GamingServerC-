// Problem: Create 10 threads, each taking a unique prime from the primes array 
// and print it on the screen.

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define N_THREADS 10

int primes[N_THREADS] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

void* routine(void* arg) {
    // sleep(1);
    int *res = (int*)arg;
    printf("Number: %d\n", *res);
    return (void*)res;
}

int main(int argc, char *argv[]) {
    pthread_t th[N_THREADS];
    for (int i=0; i<N_THREADS; i++) {
        printf("Creating thread: %d\n", i);
        pthread_create(th+i, NULL, routine, (void*)(primes+i));
    }
    for (int i=0; i<N_THREADS; i++) {
        int* ret = NULL;
        pthread_join(*(th+i), (void**)&ret);
        printf("Thread: %d returned %d\n", i, *(int*)ret);
    }
    
    return 0;
}

/*
Sources: 
https://www.youtube.com/watch?v=HDohXvS6UIk
https://www.youtube.com/watch?v=Adtrk3PREQI
*/ 