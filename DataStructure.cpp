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

Key::Key(unsigned char *key_id, unsigned char *key_value,
         unsigned int key_value_len,
         std::time_t generated_time)
        : key_id_(key_id), key_value_(key_value),
          key_value_len_(key_value_len),
          generated_time_(generated_time) {

};

void Key::generate_key(unsigned char *key_value, unsigned int length) {
    auto time_c = time(nullptr);
    key_id_ = generateKeyId();
    key_value_ = key_value;
    key_value_len_ = length;
    generated_time_ = time_c;
}

std::string Key::int2str(const int &int_tmp) {
    std::stringstream stream;
    stream << int_tmp;
    std::string str_tmp;
    stream >> str_tmp;
    return str_tmp;
}

unsigned char *Key::generateKeyId() {
    unsigned char *uu = new unsigned char[16];
    uuid_generate(uu);
    return uu;
}

