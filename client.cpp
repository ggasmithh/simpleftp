//  Author: Garrett Smith
//  NetID: gas203

//  Sources Consulted
//  1). ./server.cpp
//  2). Lecture-3-F2019.pdf     Lecture 3 Slides
//  3). Various manpages
//  4). https://www.cprogramming.com/tutorial/lesson14.html commandline argument parsing

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <netdb.h>
#include <sstream>
#include <stdexcept>

#define MAX_BUFFER_SIZE 8

using namespace std;

int main(int, char *argv[]) {
    char handshake_message[MAX_BUFFER_SIZE] = "117";
    const char *hostname = argv[1];
    struct hostent *s;
    struct sockaddr_in server;
    struct sockaddr_in response_server;
    int handshake_port;
    int transaction_port;
    char buffer[MAX_BUFFER_SIZE];
    char payload[MAX_BUFFER_SIZE];
    int sockfd;
        
    istringstream(argv[2]) >> handshake_port;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // Get the server's details
    s = gethostbyname(hostname);
    memset((char *) &server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(handshake_port);
    bcopy((char *)s->h_addr, (char *)&server.sin_addr.s_addr, s->h_length);

    // Send the handshake message
    socklen_t slen = sizeof(server);
    sendto(sockfd, handshake_message, sizeof(handshake_message), 0, (struct sockaddr *)&server, slen);

    // Create a "response "server"???"  (find a better name for this)
    memset((char *) &response_server, 0, sizeof(response_server));
    response_server.sin_family = AF_INET;
    response_server.sin_port = htons(handshake_port);
    response_server.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(sockfd, (struct sockaddr *)&response_server, sizeof(response_server));

    // // Prepare for response
    memset((char *) &buffer, 0, sizeof(buffer));
    socklen_t rslen = sizeof(response_server);

    //// DEBUG MESSAGE
    //cout << "Waiting for payload on socket " << sockfd << endl;

    recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&response_server, &rslen);

    istringstream(buffer) >> transaction_port;

    //// DEBUG MESSAGE
    //cout << "Client Says: Transaction port: " << buffer << endl;

    // END STAGE 1 - HANDSHAKE
    close(sockfd);

    //
    //  BEGIN STAGE 2 - TRANSACTION/TRANSMISSION
    //

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // Get the server's details
    memset((char *) &server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(transaction_port);
    bcopy((char *)s->h_addr, (char *)&server.sin_addr.s_addr, s->h_length);
    slen = sizeof(server);
 
    for(int i = 0; i < 4; i++) {
        sendto(sockfd, "test\n", sizeof("test\n"), 0, (struct sockaddr *)&server, slen);
    };
    
    close(sockfd);

    return 0;
}
