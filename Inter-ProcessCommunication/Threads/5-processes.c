/*
Program to show the difference between threads and processes.
In both programs, we modify a global variable and try to acces it using another thread/process.
Threads share the same global variable but processes do not. Same value will be shown in threads but different value will be shown in processes.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main() {
    int x = 2;
    int pid = fork();
    // On  success, the PID of the child process is returned in the parent, and 0 is returned in the child.  
    // On failure, -1 is returned in the parent, no child process is created, and errno is set appropriately.
    if (pid == -1) {
        return 1; //Fork unsuccessful.
    }

    if (pid == 0) {
        x++;
        // Increment in the child process.
    }
    sleep(2);
    printf("x = %d\n", x);
    printf("Fork id = %d\n", pid);
    printf("Process id = %d.\n", getpid());
    printf("Process parent id = %d.\n", getppid());

    if (pid != 0) {
        wait(NULL);
        // wait() suspends execution of the calling thread until one of its child terminates. 
    }

    return 0;
}