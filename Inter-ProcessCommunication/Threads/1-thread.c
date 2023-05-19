// Basic program to implement threading.
// Program that alternately prints My Turn and Your turn infinitely.

#include <stdio.h>
#include <unistd.h> // for sleep
#include <pthread.h>

// Function that prints my turn after every second.
// Thread funciton need to return a void* and need to take a void* as an argument.
void* myturn(void* arg) {
    while(1) {
        sleep(1);
        printf("My Turn.\n");
    }
}

// Function that prints your turn after avery 2 seconds.
void yourturn() {
    while(1) {
        sleep(2);
        printf("Your Turn.\n");
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
    /* int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);
    - thread refere to the pointer to the thread that has to be created.
    - pthread_attr_t refers to the thread attributes.
    - *(*start_routine) refers to the pointer to the function that has to be executed using the thread.
    - void *arg refers to the arguments to be passed to the running funcition.
    Returns 0 on SUCCESS and an error value/number on failure.
    */

    yourturn();


    return 0;
}

