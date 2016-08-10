//
// Created by root on 16-7-26.
//

#include "DataStructure.h"

Camera::Camera(std::string camera_id, std::string group, std::string public_key)
        : camera_id_(camera_id), group_(group), public_key_(public_key) {
}

Client::Client(std::string client_id_, std::string group_, std::string public_key)
        : client_id_(client_id_), group_(group_), public_key_(public_key) {

}

Key::Key(Camera &camera, unsigned char *key_value) {
    this->camera_ = camera;
    this->key_value_ = reinterpret_cast<char *>(key_value);
    auto time_c = time(NULL);
    this->generated_time_ = time_c;
    srand((unsigned int) time_c);
    this->key_id_ = int2str(rand());
}

std::string int2str(const int &int_tmp) {
    std::stringstream stream;
    stream << int_tmp;
    std::string str_tmp;
    stream >> str_tmp;
    return str_tmp;
}
