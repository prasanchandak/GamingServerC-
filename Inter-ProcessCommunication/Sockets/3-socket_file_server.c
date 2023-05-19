#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>


void error(const char* msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char* argv[]) {

    int sockfd, newsockfd, portno;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    struct hostent *server;
    char buffer[255];
    int n;

    if (argc < 3) {
        fprintf(stderr, "ERROR, no port provided.\n");
        exit(1);
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        error("ERROR opening socket.");
    }

    server = gethostbyname(argv[1]);

    bzero((char *) &serv_addr, sizeof(serv_addr));
    // The bzero() function erases the data in the n bytes of the memory starting at the location 
    // pointed to by s, by writing zeros (bytes containing '\0') to that area.

    FILE *f;
    int words = 0;

    close(sockfd);
    return 0;
}