//
// Created by Hanzeil on 16-8-15.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//
// 该文件描述的是密钥（被加密）的数据结构和相关操作
//

#ifndef KEYMANAGEMENT_KEY_H
#define KEYMANAGEMENT_KEY_H

#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <uuid/uuid.h>
#include <cstring>

// 类Key是密钥的数据结构和相关操作。支持以下几种操作：
// 从给定的所有必须属性构造一个Key的对象
// 给定key_value和其长度构造一个Key对象（key_id随机生成）
// 随机生成一个唯一的key_value
// Sample usage:
// Key k("key_sample", key_len);
class Key {
public:
    //Key_id的长度，由于使用UUID，所以默认就是16个字节
    static const size_t kKeyIdLen = 16;

    //拷贝构造函数
    //key_id和Key_value需要重新分配空间
    Key(const Key &);

    //拷贝赋值函数
    //采用copy & swap技术
    Key &operator=(Key);


    Key() = default;

    // 通过指定Key需要的所有属性来构造Key
    Key(unsigned char *key_id,
        unsigned char *key_value, size_t key_value_len,
        std::time_t generated_time);

    // 给定key_value和其长度构造一个Key对象（key_id随机生成）
    Key(unsigned char *key_value, size_t key_value_len);

    ~Key();

    // 每个key的唯一标识， 默认NULL
    unsigned char *key_id_ = nullptr;
    unsigned char *key_value_ = nullptr;
    size_t key_value_len_ = 0;
    std::time_t generated_time_ = 0;
private:
    // 随机生成一个永不重复Key id的函数，并返回
    // 函数分配了unsigned char[kKeyIdLen]的空间，需要调用者释放
    // Key id的长度由类成员常量kKeyIdLen决定
    unsigned char *GenerateKeyId();

    friend void swap(Key &, Key &);
};

inline void swap(Key &key1, Key &key2);

#endif //KEYMANAGEMENT_KEY_H
