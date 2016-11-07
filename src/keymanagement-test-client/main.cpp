#include "../config/Config.h"
#include "global_define.h"
#include <experimental/filesystem>
#include <sys/types.h>
#include <netdb.h>
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


namespace fs = std::experimental::filesystem;

int main(int argc, char *argv[]) {
    DataPack1 data_pack1;
    DataPack2 data_pack2;
    ssize_t n;


    //config
    std::string user_config_path;
    std::string global_config_path;

#ifdef NDEBUG
    user_config_path = std::string(getenv("HOME")) + "/." + PROJECT_NAME + "/";
    global_config_path = std::string("/etc/") + PROJECT_NAME + "/";
#else
    user_config_path = std::string(PROJECT_DIR) + "/config/";
    global_config_path = user_config_path;
#endif

    std::string config_path;
    fs::path fs_user_config_path, fs_global_config_path;
    fs_user_config_path.append(user_config_path + "keymanagement.conf");
    fs_global_config_path.append(global_config_path + "keymanagement.conf");
    if (fs::exists(fs_user_config_path)) {
        config_path = user_config_path + "keymanagement.conf";
    } else if (fs::exists(fs_global_config_path)) {
        config_path = global_config_path + "keymanagement.conf";
    } else {
        perror("NO CONFIG FILE");
        return 1;
    }

    Config config_settings(config_path);

    uint16_t portno = config_settings.Read<uint16_t>("PORT");

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    auto server = gethostbyname("127.0.0.1");
    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *) server->h_addr,
          (char *) &serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");
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

    buffer[0] = 0;
    n = write(sockfd, buffer, 1);

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
