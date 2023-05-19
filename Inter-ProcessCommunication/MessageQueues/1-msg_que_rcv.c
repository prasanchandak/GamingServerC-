#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

struct my_msg {
    long int msg_type;
    char text[BUFSIZ];
};

int main() {

    int running = 1;
    int msgid;
    struct my_msg data;
    long int msg_to_rec=0;
    msgid = msgget((key_t)12345, 0666|IPC_CREAT);

    while(running) {
        msgrcv(msgid, (void*)&data, BUFSIZ, msg_to_rec, 0);
        printf("Data received: %s\n", data.text);
        if (strncmp(data.text, "end", 3) == 0) {
            running = 0;
        }
    }

    /*
    Message control function. To delete the message queue. 
    */
    msgctl(msgid, IPC_RMID, 0);

    return 0;
}