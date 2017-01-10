//
// Created by Hanzeil on 16-8-15.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//
// 该文件描述的是密钥的数据结构和相关操作
//

#ifndef KEYMANAGEMENT_DATABASE_KEY_H
#define KEYMANAGEMENT_DATABASE_KEY_H

#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <utility>
#include <uuid/uuid.h>
#include <cstring>


/// 类Key是密钥的数据结构和相关操作。支持以下几种操作：
/// 从给定的所有必须属性构造一个Key的对象
/// 给定key_value和其长度构造一个Key对象（key_id随机生成）
/// 随机生成一个唯一的key_value
/// Sample usage:
/// Key k(key_value);
class Key {
public:


    /// Key_id的长度，由于使用UUID，所以默认就是16个字节
    static const size_t kKeyIdLen = 16;

    /// Key_value的长度，对称密钥的默认长度，16个字节
    static const size_t kKeyValueLen = 16;

    /// 加密后的密文长度
    static const size_t kKeyValueEncLen = 128;

    typedef std::array<unsigned char, Key::kKeyValueLen> KeyValueType;
    typedef std::array<unsigned char, Key::kKeyIdLen> KeyIdType;
    typedef std::array<unsigned char, Key::kKeyValueEncLen> KeyValueEncType;

    Key() = default;

    /// 通过指定Key的明文(unsigned char *)来构造Key
    Key(unsigned char *key_id,
        unsigned char *key_value,
        std::time_t generated_time);

    /// 通过指定Key的明文(std::array)来构造Key
    Key(KeyIdType key_id,
        KeyValueType key_value,
        std::time_t
        generated_time);

    /// 通过指定Key的密文(std::array)来构造Key
    Key(KeyIdType key_id,
        KeyValueEncType key_value_enc,
        std::time_t
        generated_time);

    /// 给定key_value(unsigned char)构造一个Key对象（key_id随机生成）
    Key(unsigned char *key_value);

    /// 给定key_value(std::array)构造一个Key对象（key_id随机生成）
    Key(KeyValueType key_value);

    /// 每个key的唯一标识
    KeyIdType key_id_;

    /// key_value
    KeyValueType key_value_;

    /// key_value_enc
    KeyValueEncType key_value_enc_;

    /// 密钥生成的时间
    std::time_t generated_time_ = 0;
private:

    /// 随机生成一个永不重复Key id的函数，并返回
    /// 函数分配了unsigned char[kKeyIdLen]的空间，需要调用者释放
    /// Key id的长度由类成员常量kKeyIdLen决定
    void GenerateKeyId();

    /// key_id unsigned char* -> std::array
    void key_id_to_array(unsigned char *,
                         KeyIdType &);

    /// key_value unsigned char* -> std::array
    void key_value_to_array(unsigned char *,
                            KeyValueType &);
};

typedef std::array<unsigned char, Key::kKeyValueLen> KeyValueType;
typedef std::array<unsigned char, Key::kKeyIdLen> KeyIdType;
typedef std::array<unsigned char, Key::kKeyValueEncLen> KeyValueEncType;
#endif //KEYMANAGEMENT_DATABASE_KEY_H
