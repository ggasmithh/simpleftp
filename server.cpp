//  Author: Garrett Smith
//  NetID: gas203

//  Sources Consulted
//  1). https://www.youtube.com/watch?v=cNdlrbZSkyQ     "Creating a TCP Server in C++"
//  2). http://beej.us/guide/bgnet/html/single/bgnet.html#simpleserver
//  3). Lecture-3-F2019.pdf     Lecture 3 Slides
//  4). Various manpages, i.e. https://linux.die.net/man/3/listen
//  5). https://www.cprogramming.com/tutorial/lesson14.html commandline argument parsing
//  6). https://stackoverflow.com/questions/8480640/how-to-throw-a-c-exception Argument handling
//  7). https://stackoverflow.com/questions/5008804/generating-random-integer-from-a-range
//  8). https://stackoverflow.com/questions/14753423/sprintf-convert-int-to-char
//  9). https://stackoverflow.com/questions/10513196/c-convert-character-array-to-uppercase-no-mfc
//  10). https://stackoverflow.com/questions/15388041/how-to-write-stdstring-to-file

#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <stdexcept>
#include <time.h>
#include <stdio.h>
#include <fstream>
#include <cctype>

#define MAX_BUFFER_SIZE 8

using namespace std;


int main(int, char* argv[]) {
    const char handshake_correct[MAX_BUFFER_SIZE] = "117";
    const char transaction_end[MAX_BUFFER_SIZE] = "1234567";
    char *handshake_actual;
    int handshake_port;
    int transaction_port;
    int sockfd;
    int acc_sockfd;
    struct sockaddr_in server;
    struct sockaddr_in client;
    char buffer[MAX_BUFFER_SIZE];
    char payload[MAX_BUFFER_SIZE];
    char small_payload[4];
    string complete_payload = "";
    char *is_end;
 
    istringstream(argv[1]) >> handshake_port;

    // TCP Socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Set up the server object
    memset((char *)&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(handshake_port);
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(sockfd, (struct sockaddr *)&server, sizeof(server));

    socklen_t clen = sizeof(client);

    listen(sockfd, SOMAXCONN);

    acc_sockfd = accept(sockfd, (sockaddr*)&client, &clen);
    
    close(sockfd);

    // Clear out our buffer
    memset((char *)&buffer, 0, sizeof(buffer));

    recv(acc_sockfd, buffer, sizeof(buffer), 0);

    // buffer will be the handshake message from the client at this point
    if (strcmp(buffer, handshake_correct) == 0) {
        srand(time(NULL));
        transaction_port = 1024 + (rand() % static_cast<int>(65535 - 1024 + 1));
        sprintf(payload, "%d", transaction_port);

        cout << "Handshake detected. Selected random port " << transaction_port << endl;

        //// DEBUG MESSAGE
        //cout << "Sending payload '" << payload << "' to this port: " << client.sin_port << " At this address: " << client.sin_addr.s_addr << " on socket " << socket << endl;
        send(acc_sockfd, payload, sizeof(payload), 0);
    } else {
        throw runtime_error("Handshake failed.");
    }

    // END STAGE 1 - HANDSHAKE
    close(acc_sockfd);

    //  
    //  BEGIN STAGE 2 - TRANSACTION/TRANSMISSION
    //

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
 
    // Set up the server object
    memset((char *)&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(transaction_port);
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(sockfd, (struct sockaddr *)&server, sizeof(server));

    while (1) {
        // Clear out our buffer
        memset((char *)&buffer, 0, sizeof(buffer));
        memset((char *)&payload, 0, sizeof(payload));
        recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client, &clen);
        
        if(strcmp(buffer, transaction_end) == 0) {
            break;
        } else {
            complete_payload += buffer;

            for (int i = 0; i < 4; i++) {
                payload[i] = toupper(buffer[i]);
            }

            sendto(sockfd, payload, sizeof(payload), 0, (struct sockaddr *)&client, clen);
        }
    } 
    
    close(sockfd);

    // Write file
    ofstream output("dataReceived.txt", ios_base::trunc);
    output << complete_payload;
    output.close();

    return 0;
}

