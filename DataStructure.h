//
// Created by root on 16-7-26.
//

#ifndef KEYMANAGEMENT_DATA_STRUCTURE_H
#define KEYMANAGEMENT_DATA_STRUCTURE_H

#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <uuid/uuid.h>

struct Camera {
    Camera() = default;

    Camera(std::string camera_id, std::string group, std::string public_key);

    std::string camera_id_;
    std::string group_;
    std::string public_key_;
};

struct Client {
    Client(std::string client_id_, std::string group_, std::string public_key);

    std::string client_id_;
    std::string group_;
    std::string public_key_;
};

class Key {
public:
    Key() = default;

    Key(unsigned char *key_id,
        unsigned char *key_value, unsigned int key_value_len,
        std::time_t generated_time);

    void generate_key(unsigned char *key_value, unsigned int length);

    unsigned char *key_id_ = nullptr;
    unsigned char *key_value_ = nullptr;
    unsigned int key_value_len_ = 0;
    std::time_t generated_time_ = 0;
private:
    std::string int2str(const int &int_tmp);

    unsigned char *generateKeyId();
};


#endif //KEYMANAGEMENT_DATA_STRUCTURE_H
