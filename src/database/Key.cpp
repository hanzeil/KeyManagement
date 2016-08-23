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

Key::Key(const Key &key)
        : key_value_len_(key.key_value_len_),
          generated_time_(key.generated_time_) {
    // 分配一份key_value的空间
    key_value_ = new unsigned char[key.key_value_len_];
    std::memcpy((char *) key_value_,
                (char *) key.key_value_,
                key.key_value_len_);
    // 分配一份key_id的空间
    key_id_ = new unsigned char[Key::kKeyIdLen];
    std::memcpy((char *) key_id_,
                (char *) key.key_id_,
                Key::kKeyIdLen);
}

//使用copy & swap技术，将左侧运算对象和右侧运算对象的一个副本进行交换
Key &Key::operator=(Key key) {
    swap(*this, key);
    return *this;
}

Key::Key(unsigned char *key_id, unsigned char *key_value,
         size_t key_value_len,
         std::time_t generated_time)
        : key_id_(key_id), key_value_(key_value),
          key_value_len_(key_value_len),
          generated_time_(generated_time) {

};


Key::Key(unsigned char *key_value, size_t key_value_len)
        : key_value_(key_value), key_value_len_(key_value_len) {
    auto time_c = time(nullptr);
    key_id_ = GenerateKeyId();
    generated_time_ = time_c;
}

Key::~Key() {
    delete key_id_;
    delete key_value_;
}

// 通过uuid产生唯一id，uuid的大小为16个字节
unsigned char *Key::GenerateKeyId() {
    unsigned char *uu = new unsigned char[kKeyIdLen];
    uuid_generate(uu);
    return uu;
}

inline void swap(Key &key1, Key &key2) {
    using std::swap;
    swap(key1.key_id_, key2.key_id_);
    swap(key1.key_value_, key2.key_value_);
    swap(key1.key_value_len_, key2.key_value_len_);
    swap(key1.generated_time_, key2.generated_time_);
}
