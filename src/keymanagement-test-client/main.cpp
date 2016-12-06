#include "../config/ConfigSingleton.h"
#include "global_define.h"
#include <experimental/filesystem>
#include <sys/types.h>
#include <netdb.h>
#include <string>
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
    uint32_t flag = 0xaa000000;
    char rand[16] = "123456789abcdef";
    uint32_t length = 717;
    uint32_t data;
};

struct DataPack2 {
    uint32_t flag = 0xaabbcc00;
    char rand[16] = {0};
    uint16_t length = 128;
    uint32_t data;
};

struct Request1 {
    uint32_t flag = 0xaaaabbbb;
    char rand[16] = {0};
    uint32_t length = 1;
    uint32_t data;
};

struct Request2 {
    uint32_t flag = 0xaaaabbbb;
    char rand[16] = {(char) 0x46, (char) 0x0b, (char) 0x91,
                     (char) 0x38, (char) 0x36, (char) 0x30,
                     (char) 0x41, (char) 0x7f, (char) 0xb9,
                     (char) 0x99, (char) 0x0c, (char) 0x40,
                     (char) 0x2d, (char) 0x72, (char) 0x18,
                     (char) 0x6d};
    uint32_t length = 2;
    uint32_t data;
};

struct ECCPUBLICKEYBLOB {
    u_int32_t bit_len;
    unsigned char x[64];
    unsigned char y[64];
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

    uint16_t portno = (uint16_t) config::ConfigSingleton::GetInstance().port_int_;
    std::string address = config::ConfigSingleton::GetInstance().address_;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    auto server = gethostbyname(address.c_str());
    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *) server->h_addr,
          (char *) &serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");
    unsigned char buffer[8196] = {0};

    //Authentation1
    std::string path = std::string(PROJECT_DIR) +
                       "/src/keymanagement-test-client/cert.cer";
    FILE *file = fopen(path.c_str(), "rb");
    auto data = new unsigned char[data_pack1.length];
    fread(data, data_pack1.length, 1, file);
    fclose(file);
    memcpy(buffer, &data_pack1, sizeof(data_pack1));
    for (int i = 0; i < data_pack1.length; i++) {
        buffer[i + sizeof(data_pack1)] = data[i];
    }
    n = write(sockfd, buffer, sizeof(data_pack1) + data_pack1.length);
    if (n < 0)
        error("ERROR writing to socket");
    bzero(buffer, 8196);
    n = read(sockfd, buffer, 8195);
    if (n < 0)
        error("ERROR reading from socket");
    for (size_t i = 0; i < n; i++) {
        std::cout << std::hex << (int) buffer[i] << " ";
    }
    std::cout << std::endl;

    //Authentation2
    path = std::string(PROJECT_DIR) +
           "/src/keymanagement-test-client/data.signed";
    file = fopen(path.c_str(), "rb");
    data = new unsigned char[data_pack2.length];
    memcpy(buffer, &data_pack2, sizeof(data_pack2));
    fread(data, data_pack2.length, 1, file);
    fclose(file);
    for (int i = 0; i < data_pack2.length; i++) {
        buffer[i + sizeof(data_pack2)] = data[i];
    }
    n = write(sockfd, buffer, sizeof(data_pack2) + data_pack2.length);
    if (n < 0)
        error("ERROR writing to socket");
    bzero(buffer, 8196);
    n = read(sockfd, buffer, 8195);
    if (n < 0)
        error("ERROR reading from socket");
    for (size_t i = 0; i < n; i++) {
        std::cout << (int) buffer[i] << " ";
    }
    std::cout << std::endl;

    //Request1
    Request1 request1;
    n = write(sockfd, &request1, sizeof(request1));
    if (n < 0)
        error("ERROR writing to socket");
    bzero(buffer, 8196);
    n = read(sockfd, buffer, 8195);
    if (n < 0)
        error("ERROR reading from socket");

    printf("ID:\n");
    for (size_t i = 4; i < 4 + 16; i++) {
        printf("%0x ", buffer[i]);
    }
    printf("\n");

    for (size_t i = 28; i < n; i++) {
        printf("%0x ", buffer[i]);
    }
    std::cout << std::endl;

    /*
    //Request2
    Request2 request2;
    //memcpy(request2.rand, buffer + 4, 16);
    n = write(sockfd, &request2, sizeof(request2));
    if (n < 0)
        error("ERROR writing to socket");
    bzero(buffer, 8196);
    n = read(sockfd, buffer, 8195);
    if (n < 0)
        error("ERROR reading from socket");
    for (size_t i = 28; i < n; i++) {
        std::cout << (int) buffer[i] << " ";
    }
    std::cout << std::endl;
    */
    return 0;
}
