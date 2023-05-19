#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <netdb.h>


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

    if (server == NULL) {
        fprintf(stderr, "ERROR No such host.\n");
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    // The bzero() function erases the data in the n bytes of the memory starting at the location 
    // pointed to by s, by writing zeros (bytes containing '\0') to that area.
    serv_addr.sin_family = AF_INET;
    bcopy((char*) server->h_addr_list[0], (char*) &serv_addr.sin_addr.s_addr, server->h_length);
    // #define h_addr h_addr_list[0] /* for backward compatibility */
    // h_addr_list[0] needs to be spicified instead of h_addr for backwards compatibility reasons.
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        error("Connection Failed");
    }

    bzero(buffer, 255);
    
    FILE *f;
    int words = 0;

    close(sockfd);
    return 0;
}