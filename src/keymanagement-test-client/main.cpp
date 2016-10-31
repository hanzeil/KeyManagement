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

struct DataPack1 {
    char method[2] = {'a', '1'};
    char rand[16] = "123456789abcdef";
    uint16_t length = 1600;
    char data[1600] = "0";
};

struct DataPack2 {
    char method[2] = {'a', '2'};
    uint16_t length = 128;
    char rand[128] = "123456789abcdef";
};

struct Request1 {
    char method[2] = {'c', 0};
};

struct Request2 {
    char method[2] = {'f', 0};
    uint16_t length = 16;
    unsigned char key_id[16];
};

int main(int argc, char *argv[]) {
    DataPack1 data_pack1;
    DataPack2 data_pack2;
    int sockfd, portno;
    ssize_t n;

    struct sockaddr_in serv_addr;
    struct hostent *server;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <address> <port>\n", argv[0]);
        fprintf(stderr, "Example: keymanagement-test-client 127.0.0.1 6090\n", argv[0]);
        return 1;
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
    char buffer[256] = {'c'};

    //Authentation1
    n = write(sockfd, &data_pack1, sizeof(data_pack1));
    if (n < 0)
        error("ERROR writing to socket");
    bzero(buffer, 256);
    n = read(sockfd, buffer, 255);
    if (n < 0)
        error("ERROR reading from socket");
    for (size_t i = 0; i < n; i++) {
        std::cout << (int) (unsigned char) buffer[i] << " ";
    }
    std::cout << std::endl;

    //Authentation2
    n = write(sockfd, &data_pack2, sizeof(data_pack2));
    if (n < 0)
        error("ERROR writing to socket");
    bzero(buffer, 256);
    n = read(sockfd, buffer, 255);
    if (n < 0)
        error("ERROR reading from socket");
    for (size_t i = 0; i < n; i++) {
        std::cout << (int) (unsigned char) buffer[i] << " ";
    }
    std::cout << std::endl;

    //Request1
    Request1 request1;
    n = write(sockfd, &request1, sizeof(request1));
    if (n < 0)
        error("ERROR writing to socket");
    bzero(buffer, 256);
    n = read(sockfd, buffer, 255);
    if (n < 0)
        error("ERROR reading from socket");
    for (size_t i = 16; i < n; i++) {
        std::cout << (int) (unsigned char) buffer[i] << " ";
    }
    std::cout << std::endl;

    //Request2
    Request2 request2;
    memcpy(request2.key_id, buffer, 16);
    n = write(sockfd, &request2, sizeof(request2));
    if (n < 0)
        error("ERROR writing to socket");
    bzero(buffer, 256);
    n = read(sockfd, buffer, 255);
    if (n < 0)
        error("ERROR reading from socket");
    for (size_t i = 16; i < n; i++) {
        std::cout << (int) (unsigned char) buffer[i] << " ";
    }
    std::cout << std::endl;
    return 0;
}
