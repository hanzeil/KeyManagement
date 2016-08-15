//
// Created by Hanzeil on 16-8-15.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//
// The description of this file is in the header file.
//

#include <stdlib.h>
#include "MysqlDB.h"

MysqlDB::~MysqlDB() {
    delete con_;
    BOOST_LOG_TRIVIAL(info) << "Database: Mysql closed";
}

bool MysqlDB::Connect(std::string username, std::string password) {
    driver_ = sql::mysql::get_mysql_driver_instance();
    try {
        con_ = driver_->connect("tcp://127.0.0.1:3306", username, password);
        BOOST_LOG_TRIVIAL(info) << "Database: Connect Mysql";
        con_->setSchema(db_name_);
        BOOST_LOG_TRIVIAL(info) << "Database: database selected";
    }
    catch (std::runtime_error e) {
        BOOST_LOG_TRIVIAL(error) << "Database: " << e.what();
        return false;
    }
    return true;
}

bool MysqlDB::InsertKey(Key &k) {
    try {
        auto p_stmt = con_->prepareStatement("INSERT INTO " + this->key_table_name_ + " VALUES (?,?,?,?)");
        char *tmp = reinterpret_cast<char *>(k.key_id_);
        std::string key_id(tmp, tmp + Key::kKeyIdLen);
        tmp = reinterpret_cast<char *>(k.key_value_);
        std::string key_value(tmp, tmp + k.key_value_len_);
        p_stmt->setString(1, key_id);
        p_stmt->setString(2, key_value);
        p_stmt->setInt(3, k.key_value_len_);
        p_stmt->setInt(4, k.generated_time_);
        //p_stmt->setString(4, unixTime2MysqlTime(k.generated_time_));
        p_stmt->execute();
        delete p_stmt;
    }
    catch (std::runtime_error e) {
        BOOST_LOG_TRIVIAL(error) << "Database: " << e.what();
        return false;
    }
    BOOST_LOG_TRIVIAL(info) << "Database: Insert key";
    return true;
}

Key *MysqlDB::GetKey(unsigned char *key_id) {
    Key *k = nullptr;
    try {
        auto p_stmt = con_->prepareStatement("SELECT * FROM " + this->key_table_name_ + " WHERE key_id=?");
        char *tmp = reinterpret_cast<char *>(key_id);
        std::string key_id_str(tmp, tmp + Key::kKeyIdLen);
        p_stmt->setString(1, key_id_str);
        auto res = p_stmt->executeQuery();
        res->afterLast();
        std::string key_value_str;
        std::time_t generated_time;
        unsigned int key_value_len = 0;
        if (res->previous()) {
            key_value_str = res->getString("key_value");
            key_value_len = (unsigned int) res->getInt("key_value_len");
            generated_time = res->getInt("generated_time");
            unsigned char *key_value_cpy = new unsigned char[key_value_len];
            std::memcpy((char *) key_value_cpy,
                        key_value_str.c_str(),
                        key_value_len);
            unsigned char *key_id_cpy = new unsigned char[Key::kKeyIdLen];
            std::memcpy((char *) key_id_cpy,
                        (char *) key_id,
                        Key::kKeyIdLen);
            k = new Key(key_id_cpy, key_value_cpy,
                        key_value_len, generated_time);
        }
        else {
            BOOST_LOG_TRIVIAL(error) << "Database: No query result";
            return nullptr;
        }
    }

    catch (std::runtime_error e) {
        BOOST_LOG_TRIVIAL(error) << "Database: " << e.what();
        return nullptr;
    }
    BOOST_LOG_TRIVIAL(info) << "Database: Get key from database by key id";
    return k;

}

std::string MysqlDB::UnixTime2MysqlTime(time_t unix_timestamp) {
    std::string result;
    struct tm t_tm = *localtime(&unix_timestamp);
    char s[20] = {0};
    std::strftime(s, sizeof(s), "%Y/%m/%d %H:%M:%S", &t_tm);
    result = s;
    return result;
}

std::time_t MysqlDB::MysqlTime2UnixTime(std::string mysql_time) {
    char *tmp = const_cast<char *>(mysql_time.c_str());
    tmp[4] = 0;
    tmp[7] = 0;
    tmp[10] = 0;
    tmp[13] = 0;
    tmp[16] = 0;
    tmp[19] = 0;
    struct tm t_tm;
    t_tm.tm_year = std::atoi(tmp) - 1900;
    t_tm.tm_mon = std::atoi(tmp + 5) - 1;
    t_tm.tm_mday = std::atoi(tmp + 8);
    t_tm.tm_hour = std::atoi(tmp + 11);
    t_tm.tm_min = std::atoi(tmp + 14);
    t_tm.tm_sec = std::atoi(tmp + 17);
    time_t t = std::mktime(&t_tm);
    return t;
}