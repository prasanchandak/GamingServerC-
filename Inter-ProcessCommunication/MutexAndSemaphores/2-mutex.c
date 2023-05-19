#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int count = 0;
pthread_mutex_t mutex;

void* routine() {
    for(int i=0; i<1000000; i++) {
        pthread_mutex_lock(&mutex);
        count++;
        pthread_mutex_unlock(&mutex);
    }
}

int main(int argc, char* argv[]) {
    pthread_t p1, p2;
    pthread_mutex_init(&mutex, NULL);
    // Initializes mutex with default values.
    if (pthread_create(&p1, NULL, routine, NULL)) {
        return 1;
    }
    if (pthread_create(&p2, NULL, routine, NULL)) {
        return 2;
    }
    if (pthread_join(p1, NULL)) {
        return 3;
    }
    if (pthread_join(p2, NULL)) {
        return 4;
    }
    pthread_mutex_destroy(&mutex);
    // Destroy memory allocated to the mutex variable.
    return 0;
}
