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
         std::time_t generated_time)
        : generated_time_(generated_time) {
    key_id_to_array(key_id, key_id_);
    key_value_to_array(key_value, key_value_);
}

Key::Key(KeyIdType key_id,
         KeyValueType key_value,
         std::time_t generated_time)
        : key_id_(std::move(key_id)),
          key_value_(std::move(key_value)),
          generated_time_(generated_time) {
}

Key::Key(unsigned char *key_value)
        : generated_time_(time(nullptr)) {
    key_value_to_array(key_value, key_value_);
    GenerateKeyId();
}

Key::Key(KeyValueType key_value)
        : key_value_(std::move(key_value)),
          generated_time_(time(nullptr)) {
    GenerateKeyId();
}

// 通过uuid产生唯一id，uuid的大小为16个字节
void Key::GenerateKeyId() {
    unsigned char *uu = new unsigned char[kKeyIdLen];
    uuid_generate(uu);
    key_id_to_array(uu, key_id_);
    delete uu;
}

void Key::key_id_to_array(unsigned char *unc,
                          KeyIdType &arr) {
    for (size_t i = 0; i < kKeyIdLen; i++) {
        arr[i] = unc[i];
    }
}

void Key::key_value_to_array(unsigned char *unc,
                             KeyValueType &arr) {
    for (size_t i = 0; i < kKeyValueLen; i++) {
        arr[i] = unc[i];
    }
}
