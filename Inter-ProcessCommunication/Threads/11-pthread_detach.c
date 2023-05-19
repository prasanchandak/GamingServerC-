#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define N_THREADS 2

void* routine(void* args) {
    sleep(1);
    printf("Thread finished successfully.\n");
}

int main(int argc, char *argv[]) {

    pthread_t th[N_THREADS];

    pthread_t det_th[N_THREADS]; // New threads 
    
    // Parameters to make a thread detachable from creation.
    pthread_attr_t detached_thread; // Attribute of a thread.
    pthread_attr_init(&detached_thread);
    pthread_attr_setdetachstate(&detached_thread, PTHREAD_CREATE_DETACHED);
    
    // Default threads.
    for (int i=0; i<N_THREADS; i++) {
        if (pthread_create(th+i, NULL, routine, NULL) != 0) {
            perror("Failed to create thread.");
        }
        pthread_detach(th[i]);
        // Detaches the thread from the main thread.
        // A detached thread is not joinable i.e., we cannot call pthread_join() on it.
    }

    // Detached threads.
    for (int i=0; i<N_THREADS; i++) {
        if (pthread_create(det_th+i, &detached_thread, routine, NULL) != 0) {
            // Pass the address of our pthread_attr_t state in the second argument.
            perror("Failed to create thread.");
        }
    }
    /*
    The threads are detached therefore, we cannot join them.
    for (int i=0; i<N_THREADS; i++) {
        if (pthread_join(*(th+i), NULL) != 0) {
            perror("Failed to join thread.");
        }
    }
    */

    pthread_attr_destroy(&detached_thread);
    // Destroy the pthread_attr instance.
    
    pthread_exit(0);
    // Used to wait for all the detached threads to finish executing and exit from main.
}

/*
Source:
https://www.youtube.com/watch?v=-i8Kzuwr4T4
*/