#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* roll_dice() {
    int value = rand()%6 + 1;
    // To get results between 1 and 6.
    int* result = malloc(sizeof(int));
    /* 
    We cannot return the address of result as result itself is a local varible
    that contains the address of  the first byte of memory allocated for an int on the heap.
    */
    *result = value;
    printf("Value = %d\n", value);
    return (void*)result;
    // We have returned the address of the value on the heap.
    // This address will be cast to void**. 
    // We can cast it back to int* and then dereference it to check the returned value.
}

int main(int argc, char* argv[]) {
    int* res;
    // int* whose address we need to pass to pthread_join() to get the return value.
    srand(time(NULL));
    pthread_t th;
    if (pthread_create(&th, NULL, roll_dice, NULL) != 0) {
        return 1;
    }
    if (pthread_join(th, (void**) &res) != 0) {
        // get the returned value stored in the int* res.
        return 2;
    }

    printf("The value returned by the thread function is: %d\n", *(int*)res);
    // res was cast to void** type so we first cast it back to its type int* and then dereference it to get the result.
    

    return 0;
}