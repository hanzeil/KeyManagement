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

struct Key {
    Key(Camera &camera, unsigned char *key_value);

    Camera camera_;
    std::string key_value_;
    std::string key_id_;
    std::time_t generated_time_;
};

std::string int2str(const int &int_tmp);

#endif //KEYMANAGEMENT_DATA_STRUCTURE_H
