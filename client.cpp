//  Author: Garrett Smith
//  NetID: gas203

//  Sources Consulted
//  1). ./server.cpp
//  2). Lecture-3-F2019.pdf     Lecture 3 Slides
//  3). Various manpages

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <netdb.h>

using namespace std;

int create_socket() {
    int my_socket = 0;

    my_socket = socket(AF_INET, SOCK_DGRAM, 0);
    return my_socket;
}

sockaddr_in get_server(const char *hostname, const int port) {
    struct hostent *s;
    struct sockaddr_in server;

    s = gethostbyname(hostname);

    memset((char *) &server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    bcopy((char *)s->h_addr, (char *)&server.sin_addr.s_addr, s->h_length);

    return server;
}

int send_to_server(const char *payload, const char *hostname, const int port) {
    int my_socket;
    struct sockaddr_in server;

    my_socket = create_socket();
    server = get_server(hostname, port);

    socklen_t slen = sizeof(server);
    sendto(my_socket, payload, sizeof(payload), 0, (struct sockaddr *)&server, slen);

    close(my_socket);

    return 0;
}

sockaddr_in create_response_server(int my_socket, const int port) {
    struct sockaddr_in server;

    memset((char *) &server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(my_socket, (struct sockaddr *) &server, sizeof(server));

    return server;

}

int get_from_server(char *response, const int port) {

    int my_socket;
    struct sockaddr_in server;
    struct sockaddr_in client;
    
    my_socket = create_socket();
    server = create_response_server(my_socket, port);

    socklen_t clen = sizeof(client);
    recvfrom(my_socket, response, sizeof(response), 0, (struct sockaddr *)&client, &clen);

    close(my_socket);

    return 0;
}

int handshake(const char *message, const char *hostname, const int port) {
    //char *response;

    send_to_server(message, hostname, port);
    //get_from_server(response);

    return 0;
}

int main(int, char *argv[]) {
    const char *hostname = argv[1];
    const int port = stoi(argv[2]);
    const char *handshake_message = "117";

    handshake(handshake_message, hostname, port);
    
    return 0;
}
