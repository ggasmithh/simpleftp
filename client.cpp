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

sockaddr_in get_server() {

    struct hostent *s;
    s = gethostbyname(argv[1]);

    struct sockaddr_in server;
    memset((char *) &server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(7755);
    bcopy((char *)s->h_addr, (char *)&server.sin_addr.s_addr, s->h_length);

    return server;
}

int send_to_server(char *payload) {

    my_socket = create_socket();

    struct sockaddr_in server = get_server();

    socklen_t slen = sizeof(server);
    sendto(my_socket, payload, sizeof(payload), 0, (struct sockaddr *)&server, slen);

    close(my_socket);

    return 0;
}

int main(int argc, char *argv[]) {

    char payload[512] = "TESTPAYLOAD";
    send_to_server(payload);
    
    return 0;
}