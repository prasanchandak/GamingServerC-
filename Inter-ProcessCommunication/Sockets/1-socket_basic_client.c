#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#define PORT 7117

int main(int argc, char* argv[]) {
    int status, valread, client_fd;
    struct sockaddr_in serv_addr;
    char *msg = "Client hello";
    char buffer[1024] = {0};

    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed.");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    // serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Convert IPv4 and IPV6 addresses from text to binary form.
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Address not supported");
        return -1;
    }

    if ((status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
        perror("Connect failed");
        return -1;
    }

    send(client_fd, msg, strlen(msg), 0);
    printf("Sent hello message to server.\n");
    valread = read(client_fd, buffer, 1024);
    printf("Buffer: %s\n", buffer);

    close(client_fd);

    return 0;
}

/*
Source: https://www.geeksforgeeks.org/socket-programming-cc/
*/
