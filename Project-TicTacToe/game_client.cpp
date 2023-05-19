#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>

#define DEBUG

using namespace std;

void error(const char *msg) {
    perror(msg);
    cout << "Either the server shut down or the other player disconnected.\nGame over.\n";
    
    exit(0);
}

void recv_msg(int sockfd, char * msg) {
    memset(msg, 0, 4);
    int n = read(sockfd, msg, 3);
    
    if (n < 0 || n != 3)
     error("ERROR reading message from server socket.");

    #ifdef DEBUG
    cout << "[DEBUG] Received message: "<< msg << "\n";
    #endif
}

int recv_int(int sockfd) {
    int msg = 0;
    int n = read(sockfd, &msg, sizeof(int));
    
    if (n < 0 || n != sizeof(int)) 
        error("ERROR reading int from server socket");
    
    #ifdef DEBUG
    cout << "[DEBUG] Received int: " << msg << "\n";
    #endif

    return msg;
}

void write_server_int(int sockfd, int msg) {
    int n = write(sockfd, &msg, sizeof(int));
    if (n < 0)
        error("ERROR writing int to server socket");
    
    #ifdef DEBUG
    cout << "[DEBUG] Wrote int to server: " << msg << "\n";
    #endif
    
}

int connect_to_server(char * hostname, int portno) {
    struct sockaddr_in serv_addr;
    struct hostent *server;
 
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
    if (sockfd < 0) 
        error("ERROR opening socket for server.");
	
    server = gethostbyname(hostname);
    
	
    if (server == NULL) {
        cerr << "ERROR, no such host\n";
        exit(0);
    }
	
	memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno); 
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
	// cout << "Connect Error: " << errno << "\n";
	   error("ERROR connecting to server");
    } 

    #ifdef DEBUG
    cout << "[DEBUG] Connected to server.\n";
    #endif

    return sockfd;
}

void draw_board(char board[][3]) {
    cout << board[0][0] << " | " << board[0][1] << " | " << board[0][2] << "\n";
    cout << "-----------\n";
    cout << board[1][0] << " | " << board[1][1] << " | " << board[1][2] << "\n";
    cout << "-----------\n";
    cout << board[2][0] << " | " << board[2][1] << " | " << board[2][2] << "\n";
}

void take_turn(int sockfd) {

    char buffer[10];
    
    while (1) { 
        cout << "Enter 0-8 to make a move, or 9 for number of active players: ";
	    fgets(buffer, 10, stdin);
	    int move = buffer[0] - '0';
        if (move <= 9 && move >= 0){
            cout << "\n";
            write_server_int(sockfd, move);   
            break;
        }
        else
            cout << "\nInvalid input. Try again.\n";
    }
}

void get_update(int sockfd, char board[][3]) {
    
    int player_id = recv_int(sockfd);
    int move = recv_int(sockfd);
    board[move/3][move%3] = player_id ? 'X' : 'O';    
}


int main(int argc, char *argv[]) {

    string port, ip_of_server;
    
    if (argc < 3) {
        if (argc == 1) {
            port = "5157";
            ip_of_server = "127.0.0.1";
            #ifdef DEBUG
            cout << "[DEBUG] Using default loopback ip: 127.0.0.1.\n[DEBUG] Using default port: 5157.\n";
            #endif
        } 
        else if (argc == 2) {
            int flag = 0;
            for(int i=0; argv[1][i]!='\0'; i++) {
                if (argv[1][i] == '.') {
                    flag = 1;
                    cout << "\n";
                    break;
                }
            }
            if (flag == 0) {
                ip_of_server = "127.0.0.1";
                port = argv[1];
                
                #ifdef DEBUG
                cout << "[DEBUG] Using default loopback ip: 127.0.0.1.\n";
                #endif
            }
            else if (flag == 1) {
                port = "5157";
                ip_of_server = argv[1];
                
                #ifdef DEBUG
                cout << "[DEBUG] Using default port: 5157.\n";
                #endif
            }
        } 
    }

    else {
        ip_of_server = argv[1];
        port = argv[2];
    }

    int sockfd = connect_to_server(ip_of_server.data(), atoi(port.c_str()));
    // ip_of_server.data() returns a non-constant char * which is required by connect_to-server() function.
    // port.c_str() is used as atoi() expects a const char *.
   
    int id = recv_int(sockfd);

    #ifdef DEBUG
    cout << "[DEBUG] Client ID: " << id << "\n";
    #endif 

    char msg[4];
    char board[3][3] = { {' ', ' ', ' '}, 
                         {' ', ' ', ' '}, 
                         {' ', ' ', ' '} };

    cout << "Tic-Tac-Toe\n------------\n";

    do {
        recv_msg(sockfd, msg);
        if (!strcmp(msg, "HLD"))
            cout << "Waiting for a second player...\n";
    } while ( strcmp(msg, "SRT") );

    /* The game has begun. */
    cout << "Game on!\n";

    // printf("Your are %c's\n", id ? 'X' : 'O');
    cout << "You are " << (id ? 'X' : 'O') << "'s. \n";
    // Ternary expression inside a print statement . If id is 1, return X otherwise return O.

    draw_board(board);

    while(1) {
        recv_msg(sockfd, msg);

        if (!strcmp(msg, "TRN")) { 
	        cout << "Your move...\n";
	        take_turn(sockfd);
        }
        else if (!strcmp(msg, "INV")) { 
            cout << "That position has already been played. Try again.\n"; 
        }
        else if (!strcmp(msg, "CNT")) { 
            int num_players = recv_int(sockfd);
            cout << "There are currently " << num_players << " active players.\n"; 
        }
        else if (!strcmp(msg, "UPD")) { 
            get_update(sockfd, board);
            draw_board(board);
        }
        else if (!strcmp(msg, "WAT")) { 
            cout << "Waiting for other players move...\n";
        }
        else if (!strcmp(msg, "WIN")) { 
            cout << "You win!\n";
            break;
        }
        else if (!strcmp(msg, "LSE")) { 
            cout << "You lost.\n";
            break;
        }
        else if (!strcmp(msg, "DRW")) { 
            cout << "Draw.\n";
            break;
        }
        else 
            error("Unknown message.");
    }
    
    cout << "Game over.\n";
    close(sockfd);
    return 0;
}
