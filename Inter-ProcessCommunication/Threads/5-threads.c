/*
Program to show the difference between threads and processes.
In both programs, we modify a global variable and try to acces it using another thread/process.
Threads share the same global variable but processes do not. Same value will be shown in threads but different value will be shown in processes.
*/

#include <stdio.h>
#include <stdlib.h>
/*
Includes the definition of EXIT_FAILURE - 1 and EXIT_SUCCESS 0
Includes the declaration of exit(), malloc(), calloc(), realloc(), atoi(), abort().
Check abort() which causes an abnormal process termination.
*/
#include <unistd.h>
#include <pthread.h>

int x = 2;
// Variable to demonstrate sharing of variables among threads.

void* process_id() {
    printf("Process id = %d\n", getpid());
    // Same process id is returned for both threads.
}

// Function for thread 1.
void* routine1() {
    x++; 
    // increment the value in the first thread.
    sleep(2);
    // sleep() is used to give time to the other thread to execute.
    // if sleep() is not used, both threads may print different values if the increment thread runs a little later after the first thread has completed execution.
    printf("x = %d\n", x);
}

// Function for thread 2.
void* routine2() {
    sleep(2);
    printf("x = %d\n", x);
}

int main(int argc, char* argv[]) {
    pthread_t t1, t2;

    if (pthread_create(&t2, NULL, routine2, NULL)) {
        return 2;
    }
    // if sleep() is not used and t2 runs before t1, the threads print different values.
    if (pthread_create(&t1, NULL, routine1, NULL)) {
        return 1;
    }

    // Joining the threads.
    if (pthread_join(t1, NULL)) {
        return 3;
    }
    if (pthread_join(t2, NULL)) {
        return 4;
    }

    return 0;
}