/*
Program to check if there is an error when a mutex is unlocked without being locked repeatedly.
In default mutex, unlocked can be called without locking.
It cannot be done in errorcheck mutex.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t def_mutex; // Default mutex/.
pthread_mutex_t errchk_mutex; //Mutex for error checking.
pthread_mutexattr_t attr;
//pthread_mutex_t mutex;
size_t const shared_var = 0;

int main(void) {
  int result;

  int id = fork();

  if (id != 0) {

    // For parent process

    if ((result = pthread_mutex_init(&def_mutex, NULL)) != 0) {
        printf("Unable to initialize a mutex.\n");
    }
    // Default mutex
    while (1) {
        if ((result = pthread_mutex_unlock(&def_mutex)) != 0) {
            printf("Default Mutex cannot be unlocked without locking.\n");
            break;
        }
    }
    printf("Parent process terminated.\n");
  }

    if (id ==0) {

        // Errorcheck mutex 
        if ((result = pthread_mutexattr_init(&attr)) != 0) {
            /* Handle Error */
        }
        if ((result = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK_NP)) != 0) {
            /* Handle Error */
        }
        if ((result = pthread_mutex_init(&errchk_mutex, &attr)) != 0) {
            /* Handle Error */
        }
        //   if ((result = pthread_mutex_lock(&errchk_mutex)) != 0) {
        //     /* Handle Error */
        //   }
        
        /* Critical Region*/
        while(1) {
            // This will terminate just after one iteration.
            if ((result = pthread_mutex_unlock(&errchk_mutex)) != 0) {
                /* Handle Error */
                printf("Errorcheck Mutex cannot be unlocked without locking.\n");
                break;
            }
        }
        printf("Child process terminated.\n");
    }
 
  return 0;
}

/*
Source: https://wiki.sei.cmu.edu/confluence/display/c/POS04-C.+Avoid+using+PTHREAD_MUTEX_NORMAL+type+mutex+locks
*/