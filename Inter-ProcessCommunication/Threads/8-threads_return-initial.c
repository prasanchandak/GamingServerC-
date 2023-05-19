#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* roll_dice() {
    int value = rand()%6 + 1;
    // To get results between 1 and 6.
    printf("Value = %d\n", value);
    printf("(void*)value: %p\n", (void*)value);
    printf("&value: %p\n", &value);
    printf("(void*)&value: %p\n", (void*)&value);

    return (void*)value;
    // Will lead to a warning on compilation. 
    // Cast to pointer from integer of different size.
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

    /*
    printf("The value returned by the thread function is: %d\n", *(res));
    This line will lead to a Segmentation fault as result does not have a valid address.
    It contains the return value of the threaded function.
    So, when we dereference it, the program tries to read memory that does not belong to it.
    We have returned an int value cast as a (void*) as is the convention.
    So, when the value is in int*, IT IS NOT AN ADDRESS BUT THE INT RESULT ITSELF.
    */
    printf("The value returned by the thread function is: %d\n", (int)res);
    // Cast the result back to int, i.e., the original form of the value.
    // This will lead to a compiler warning.
    // Cast from pointer to integer of different size.

    return 0;
}