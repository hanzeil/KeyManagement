#include <sys/types.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>

enum {
    max_length = 1024
};

void dostuff(int); /* function prototype */
void error(std::string msg) {
    std::cout << msg << std::endl;
}

int main(int argc, char *argv[]) {
    int sockfd, portno;
    ssize_t n;

    struct sockaddr_in serv_addr;
    struct hostent *server;

    if (argc < 3) {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *) server->h_addr,
          (char *) &serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");
    printf("Please enter the message: ");
    char buffer[256] = {'a'};
    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0)
        error("ERROR writing to socket");
    bzero(buffer, 256);
    n = read(sockfd, buffer, 255);
    if (n < 0)
        error("ERROR reading from socket");
    for (size_t i=0;i<n;i++){
        std::cout<<(int)buffer[i]<<" ";
    }
    std::cout<<std::endl;
    char key_id[16];
    memcpy(key_id, buffer, 16);
    buffer[0] = 'b';
    buffer[1] = 0x00;
    buffer[2] = 0x10;
    memcpy(buffer + 3, key_id, 16);
    n = write(sockfd, buffer, 19);
    std::cout << n << std::endl;
    if (n < 0)
        error("ERROR writing to socket");
    bzero(buffer, 256);
    n = read(sockfd, buffer, 255);
    if (n < 0)
        error("ERROR reading from socket");
    for (size_t i=0;i<n;i++){
        std::cout<<(int)buffer[i]<<" ";
    }
    std::cout<<std::endl;
    return 0;
}
