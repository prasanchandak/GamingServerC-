#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define N_THREADS 2

int *rtn_test = NULL;
int *ret_val = NULL;
pthread_mutex_t mutex;

void* routine(void* args) {
    pthread_mutex_lock(&mutex);
    *rtn_test += 100;
    pthread_mutex_unlock(&mutex);
    printf("rtn test address: %p\n", rtn_test);
    return (void*)rtn_test;
}

int main(int argc, char* argv[]) {
    pthread_t th[N_THREADS];
    rtn_test = malloc(sizeof(int)); // Variable to test the return value of the thread.
    int j =0;
    rtn_test = &j;
    ret_val = (int*) malloc(sizeof(int*) * N_THREADS);
    pthread_mutex_init(&mutex, NULL);

    for (int i=0; i<N_THREADS; i++) {
        if (pthread_create(&th[i], NULL, routine, NULL) != 0) {
            perror("Failed to create thread.");
        }
    }
    pthread_cancel(th[1]);
    for (int i=0; i<N_THREADS; i++) {
        if (pthread_join(th[i], (void**)(ret_val[i])) != 0) {
            // Here, we write ret_val+i as we have to give the ADDRESS where we need to write the memory returned by thread.
            // ret_val[i] will not work as it dereferences *(ret_val+i) and there will be a type error.
            // &ret_val[i] will also work.
            perror("Failed to create thread.");
        }
    }

    for (int i=0; i<N_THREADS; i++) {
        printf("Return value of thread(%d) = %d\n", i, *(ret_val+i));
        // Here, again we have to provide the address of the value from where we have to read the value.
    }

    printf("Value of rtn_test = %d\n", *rtn_test);

    pthread_mutex_destroy(&mutex);

    free(rtn_test);
    free(ret_val);

    return 0;

}

