//  Author: Garrett Smith
//  NetID: gas203

//  Sources Consulted
//  1). ./server.cpp
//  2). Lecture-3-F2019.pdf     Lecture 3 Slides
//  3). Various manpages
//  4). https://www.cprogramming.com/tutorial/lesson14.html commandline argument parsing
//  5). https://stackoverflow.com/questions/19985095/read-and-write-file-in-chunks-of-bytes-in-c

#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <netdb.h>
#include <sstream>
#include <stdexcept>
#include <fstream>

#define MAX_BUFFER_SIZE 8
#define MAX_FILE_BUFFER_SIZE 4

using namespace std;

int main(int, char *argv[]) {
    char handshake_message[MAX_BUFFER_SIZE] = "117";
    const char transaction_end[MAX_BUFFER_SIZE] = "1234567";
    const char *hostname = argv[1];
    struct hostent *s;
    struct sockaddr_in server;
    struct sockaddr_in response_server;
    int handshake_port;
    int transaction_port;
    char buffer[MAX_BUFFER_SIZE];
    char small_buffer[MAX_FILE_BUFFER_SIZE];
    char payload[MAX_BUFFER_SIZE];
    char file_buffer[MAX_FILE_BUFFER_SIZE];
    int sockfd;
        
    istringstream(argv[2]) >> handshake_port;

    // TCP Socket 
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Get the server's details
    s = gethostbyname(hostname);
    memset((char *) &server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(handshake_port);
    bcopy((char *)s->h_addr, (char *)&server.sin_addr.s_addr, s->h_length);

    // Connect to the server
    socklen_t slen = sizeof(server);
    connect(sockfd, (struct sockaddr *)&server, slen);

    // Send the handshake message
    send(sockfd, handshake_message, sizeof(handshake_message), 0);

    // // Prepare for response
    memset((char *) &buffer, 0, sizeof(buffer));

    recv(sockfd, buffer, sizeof(buffer), 0);

    istringstream(buffer) >> transaction_port;

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

    // Set up a response server
    memset((char *) &response_server, 0, sizeof(response_server));
    response_server.sin_family = AF_INET;
    response_server.sin_port = htons(transaction_port);
    response_server.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(sockfd, (struct sockaddr *)&response_server, sizeof(response_server));
    socklen_t rslen = sizeof(response_server);

    ifstream text_file(argv[3]);
    if (text_file) {
        while (text_file.read((char *)file_buffer, MAX_FILE_BUFFER_SIZE)) {
            memset((char *) &small_buffer, 0, sizeof(small_buffer));
            sendto(sockfd, file_buffer, sizeof(file_buffer), 0, (struct sockaddr *)&server, slen);
            recvfrom(sockfd, small_buffer, sizeof(small_buffer), 0, (struct sockaddr *)&response_server, &rslen);
            //cout << small_buffer << endl;
            printf(small_buffer);
            printf("\n");
        }
    } else {
        throw runtime_error("Input file does not exist."); 
    }
    sendto(sockfd, transaction_end, sizeof(transaction_end), 0, (struct sockaddr *)&server, slen);

    close(sockfd);

    return 0;
}
