//
// Created by Hanzeil on 16-8-15.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//
// The description of this file is in the header file.
//

#include "Key.h"

Key::Key(unsigned char *key_id, unsigned char *key_value,
         size_t key_value_len,
         std::time_t generated_time)
        : key_id_(key_id), key_value_(key_value),
          key_value_len_(key_value_len),
          generated_time_(generated_time) {

};

Key::~Key() {
    delete key_id_;
    delete key_value_;
}

Key::Key(unsigned char *key_value, size_t key_value_len)
        : key_value_(key_value), key_value_len_(key_value_len) {
    auto time_c = time(nullptr);
    key_id_ = GenerateKeyId();
    generated_time_ = time_c;
}

// 通过uuid产生唯一id，uuid的大小为16个字节
unsigned char *Key::GenerateKeyId() {
    unsigned char *uu = new unsigned char[kKeyIdLen];
    uuid_generate(uu);
    return uu;
}

