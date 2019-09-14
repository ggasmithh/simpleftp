//  Author: Garrett Smith
//  NetID: gas203

//  Sources Consulted
//  1). https://www.youtube.com/watch?v=cNdlrbZSkyQ     "Creating a TCP Server in C++"
//  2). http://beej.us/guide/bgnet/html/single/bgnet.html#simpleserver
//  3). Lecture-3-F2019.pdf     Lecture 3 Slides
//  4). Various manpages
//  5). https://www.cprogramming.com/tutorial/lesson14.html commandline argument parsing
//  6). https://stackoverflow.com/questions/8480640/how-to-throw-a-c-exception Argument handling
//  7). https://stackoverflow.com/questions/5008804/generating-random-integer-from-a-range

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <sstream>
#include <stdlib.h>

using namespace std;

int MAXBUFFERLENGTH = 4;

int create_socket() {
    int my_socket = 0;

    my_socket = socket(AF_INET, SOCK_DGRAM, 0);

    return my_socket;
}

sockaddr_in create_server(int my_socket, int port) {
    struct sockaddr_in server;

    memset((char *) &server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(my_socket, (struct sockaddr *) &server, sizeof(server));

    return server;

}

char* get_from_client(int port) {
    int my_socket;
    struct sockaddr_in server;
    struct sockaddr_in client;
    char buffer[MAXBUFFERLENGTH];

    my_socket = create_socket();
    server = create_server(my_socket, port);
    socklen_t clen = sizeof(client);
    recvfrom(my_socket, buffer, sizeof(buffer), 0, (struct sockaddr *)&client, &clen);
    close(my_socket);

    return buffer;
}

int get_transaction_port() {
    return 1024 + (rand() % static_cast<int>(65535 - 1024 + 1));
}

int handshake(int port) {

    const char* handshake_correct = "117";
    char *handshake_actual;

    handshake_actual = get_from_client(port);

    if (*handshake_actual == *handshake_correct) {
        cout << endl << "nice" << endl;
    } else {
        cout << endl << "not nice" << endl;
    }

    return 0;
}

int main(int, char* argv[]) {
    int port;
 
    istringstream(argv[1]) >> port;

    handshake(port);

    return 0;
}
