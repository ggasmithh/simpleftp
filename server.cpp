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
//  8). https://stackoverflow.com/questions/14753423/sprintf-convert-int-to-char

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <stdexcept>
#include <time.h>
#include <stdio.h>

#define MAX_BUFFER_SIZE 8

using namespace std;


int main(int, char* argv[]) {
    const char handshake_correct[MAX_BUFFER_SIZE] = "117";

    char *handshake_actual;
    int handshake_port;
    int transaction_port;
    int sockfd;
    struct sockaddr_in server;
    struct sockaddr_in client;
    char buffer[MAX_BUFFER_SIZE];
    char payload[MAX_BUFFER_SIZE];
 
    istringstream(argv[1]) >> handshake_port;

    // THIS IS UDP, CORRECT THIS LATER
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // Clear out our buffer
    memset((char *)&buffer, 0, sizeof(buffer));

    // Set up the server object
    memset((char *)&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(handshake_port);
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(sockfd, (struct sockaddr *)&server, sizeof(server));

    socklen_t clen = sizeof(client);
    
    recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client, &clen);

    // buffer will be the handshake message from the client at this point
    if (strcmp(buffer, handshake_correct) == 0) {
        srand(time(NULL));
        transaction_port = 1024 + (rand() % static_cast<int>(65535 - 1024 + 1));
        sprintf(payload, "%d", transaction_port);

        // DEBUG MESSAGE
        cout << "Server Says: Transaction port: " << transaction_port << endl;
        // Send the transaction port message back to the client
        socklen_t clen = sizeof(client);

        // DEBUG MESSAGE
        cout << "Sending payload '" << payload << "' to this port: " << client.sin_port << " At this address: " << client.sin_addr.s_addr << " on socket " << socket << endl;
        sendto(sockfd, payload, sizeof(payload), 0, (struct sockaddr *)&client, clen);
    } else {
        throw runtime_error("Handshake failed.");
    }

    close(sockfd);

    return 0;
}
