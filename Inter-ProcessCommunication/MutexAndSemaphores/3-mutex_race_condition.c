// Program to use threads to count upto the number BIG (a billion).
// 4 macros are defined to compile the code for 4 different outputs to demonstrate threading. 

#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#define BIG 1000000000UL

// #define NO_THREAD
// #define THREAD_RACE
// #define MUTEX_LOCK
#define MUTEX_FAST

uint32_t counter = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

#ifdef NO_THREAD
void count_to_big() {
    for(uint32_t i=0; i<BIG; i++) {
        counter++;
    }
}
#endif

#if !defined(NO_THREAD) && !defined(MUTEX_FAST)
void* count_to_big(void *arg) {
    for(uint32_t i=0; i<BIG; i++) {
        #ifndef MUTEX_LOCK
        // Both threads read the same value at the same time and this leads to loss of some operations.
        // counter has different value for every execution.
        // This code is NOT thread-safe. This bug is called a race condition.
        counter++;
        #endif

        #ifdef MUTEX_LOCK
        // Code without race condition but where each thread acquires a mutex lock after every loop iteration.
        pthread_mutex_lock(&lock);
        counter++;
        pthread_mutex_unlock(&lock);
        #endif
    }
    return NULL;
}
#endif

#if !defined(NO_THREAD) && defined(MUTEX_FAST)
// This code makes the threaded code faster by only acquiring the lock once for each thread and instead updating the value in a local variable.
void* count_to_big(void *arg) {
    uint32_t local_counter = 0;
    for(uint32_t i=0; i<BIG; i++) {
        local_counter++;
    }
    pthread_mutex_lock(&lock);
    counter += local_counter;
    pthread_mutex_unlock(&lock);
    return NULL;
}
#endif

int main() {
    pthread_t t;

    #ifdef NO_THREAD
    // Simple approach without the use of threads.
    // counter should be 2 billion in the end.
    count_to_big();
    count_to_big();
    #endif

    #ifndef NO_THREAD
    pthread_create(&t, NULL, count_to_big, NULL); // Creates a separate thread.
    count_to_big(NULL); 
    pthread_join(t, NULL); // Waits for the thread t to finish.
    #endif

    printf("Done. Counter = %u\n", counter);
}
