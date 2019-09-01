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

int main() {

    char payload[512];

    int my_socket = 0;
    my_socket = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in server;
    memset((char *) &server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(7755);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(my_socket, (struct sockaddr *) &server, sizeof(server));

    struct sockaddr_in client;
    socklen_t clen = sizeof(client);
    recvfrom(my_socket, payload, sizeof(payload), 0, (struct sockaddr *)&client, &clen);

    close(my_socket);

    cout << payload;

    return 0;
}