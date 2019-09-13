//  Author: Garrett Smith
//  NetID: gas203

//  Sources Consulted
//  1). https://www.youtube.com/watch?v=cNdlrbZSkyQ     "Creating a TCP Server in C++"
//  2). http://beej.us/guide/bgnet/html/single/bgnet.html#simpleserver
//  3). Lecture-3-F2019.pdf     Lecture 3 Slides
//  4). Various manpages

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

using namespace std;


int create_socket() {
    int my_socket = 0;

    my_socket = socket(AF_INET, SOCK_DGRAM, 0);
    return my_socket;
}

sockaddr_in create_server(int my_socket, const int port) {
    struct sockaddr_in server;

    memset((char *) &server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(my_socket, (struct sockaddr *) &server, sizeof(server));

    return server;

}

int get_from_client(char *payload, const int port) {
    int my_socket;
    struct sockaddr_in server;
    struct sockaddr_in client;

    my_socket = create_socket();
    server = create_server(my_socket, port);

    socklen_t clen = sizeof(client);
    recvfrom(my_socket, payload, sizeof(payload), 0, (struct sockaddr *)&client, &clen);

    close(my_socket);

    return 0;
}

int handshake(char *message, const int port) {

    get_from_client(message, port);

    return 0;
}

int main(int, char* argv[]) {
    const int port = stoi(argv[1]);
    const char* handshake_correct = "117";
    char *handshake_actual = NULL;

    handshake(handshake_actual, port);

    if (*handshake_actual == *handshake_correct) {
        cout << "nice";
    } else {
        cout << "not nice";
    }

    return 0;
}
