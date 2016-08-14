//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//

#ifndef KEYMANAGEMENT_DATA_STRUCTURE_H
#define KEYMANAGEMENT_DATA_STRUCTURE_H

#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <uuid/uuid.h>

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
