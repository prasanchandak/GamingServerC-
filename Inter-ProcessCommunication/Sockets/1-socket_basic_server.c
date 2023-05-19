/*
Server-side program to demonstrate socket programming.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#define PORT 7117

int main(int argc, char* argv[]) {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    // sockaddr_in is a structure describing an internet socket address.
    int opt = 1;
    socklen_t addrlen = sizeof(address);
    char buffer[1024] = {0}; // Initializing buffer to 0.
    char *msg = "Server hello";

    //printf("EXIT_FAILURE: %d\n", EXIT_FAILURE);
    //printf("EXIT_SUCCESS: %d\n", EXIT_SUCCESS);

    // Creating a socket file descriptor.
    // socket() function returns file descrioptor on success and -1 on error.
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        
        perror("Socket creation failed.");
        exit(EXIT_FAILURE);
        // EXIT_FAILURE has value of 1, which indicates a failure in execution.
        // EXIT_SUCCESS has value of 0, which indicates a successful execution.
    }

    // 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        // Function to set socket options. It helps deal with the problem of address already in use.
        perror("Error during setsockopt."); 
        exit(EXIT_FAILURE);       
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Binding Failed.");
    }

    if (listen(server_fd, 3) < 0) {
        perror("Listen Failed");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen)) < 0) {
        perror("Accept Failed");
        exit(EXIT_FAILURE);
    }

    valread = read(new_socket, buffer, 1024);
    printf("Buffer: %s\n", buffer);
    send(new_socket, msg, strlen(msg), 0);
    printf("Sent hello message to client.\n");

    // Closing the socket created after accept. 
    close(new_socket);

    // Closing the listening socket.
    shutdown(server_fd, SHUT_RDWR);

    return 0;
}

/*
Source: https://www.geeksforgeeks.org/socket-programming-cc/
*/
