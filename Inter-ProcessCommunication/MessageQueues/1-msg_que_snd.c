#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

// Represents max_size of the data that can be written in the message queue 
#define MAX_TEXT 50

/*
Messages in message queue must have a defined structure.
Message structures constraints: must be smaller that the system limit,
it must start with a long int.
*/
struct my_msg {
    long int msg_type; // 
    char text[MAX_TEXT]; // Store the data to be written in the message queue.
    // The message will first be written in the buffer, and then in text variable 
    // and then will be written in the message queue using msgsnd function.
};

int main() {
    int running = 1;
    int msgid;
    struct my_msg data;
    char buffer[50];
    msgid = msgget((key_t)12345,0666|IPC_CREAT);
    // Create a message queue, key will be 12345 and permissions will be read and write for all users, group members etc.
    // IPC_CREAT means create msg queue, if queue already exists, has no impact. 
    if (msgid == -1) {
        fprintf(stderr, "Error in creating a message queue.\n");
    }

    while(running) {
        printf("Input some text: ");
        fgets(buffer, 50, stdin); 
        // Store user input in the buffer.
        data.msg_type = 1;
        strcpy(data.text, buffer);
        // Copy text from buffer into data.text variable.
        
        /* 
        Write message in the message queue.
        0 is the control parameter, it mentions what will happen if the queue is full or 
        the limit is reached. 0 means that the sending process will suspend waiting for space to become available in the queue.
        1 means that the process will return without sending the message.
        If 0 is used, the program stops and suspends. So, to prevent that, IPC_NOWAIT is used.
        */
        int msg_ret = msgsnd(msgid, (void*)&data, MAX_TEXT, 0);
        
        if (msg_ret == -1) {
            printf("Error no: %d\n", errno);
            printf("Error message: %s", strerror(errno));
            fprintf(stderr, "Message not sent.\n");
        }

        else if(msg_ret == 0) {
            printf("Message sent successfully.\n");
        }        

        //printf("After msgsend.\n");
        /*
        If the first 3 letters of the buffer start with "end", the program will stop the message queue.
        */
        if (strncmp(buffer, "end", 3) == 0) {
            running = 0; // or break could also be used.
        }

    }

    return 0;
}