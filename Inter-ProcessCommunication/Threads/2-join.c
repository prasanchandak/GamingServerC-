/* 
Need for join.
If the threaded function needs to run longer than the 
non-threaded function, we need to join the thread otherwise,
the program terminates (main() finishes) as soon as yourturn() completes before the execution of the threaded
myturn() has executed fully.
*/

#include <stdio.h>
#include <unistd.h> // for sleep
#include <pthread.h>

// #define EXE_ERR_CODE
// This variable when defined, will execute the code that leads to buggy behaviour.

// Function that prints my turn after every second.
// Thread funciton need to return a void* and need to take a void* as an argument.
void* myturn(void* arg) {
    for (int i=1; i<=10; i++) {
        sleep(1);
        printf("My Turn. Iteration: %d\n", i);
    }
}

// Function that prints your turn after avery 2 seconds.
void yourturn() {
    for(int i=1; i<=2; i++) {
        sleep(2);
        printf("Your Turn. Iteration: %d\n", i);
    }
}

int main() {
    /*
    myturn();
    The below function will never execute this way. We need threads to do that.
    yourturn();
    */
    pthread_t newthread; // Declare a new pthread_t variable. It is of unsigned long type. Thread identifier.

    pthread_create(&newthread, NULL, myturn, NULL);
    yourturn();

    #ifndef EXE_ERR_CODE
    // Code that solves the problem using join
    pthread_join(newthread, NULL);
    // The pthread_join() function waits for the thread specified by thread to terminate.  
    // If that thread has already terminated, then pthread_join() returns immediately.
    #endif

    return 0;
}

