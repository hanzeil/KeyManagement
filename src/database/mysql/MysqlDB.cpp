//
// Created by Hanzeil on 16-8-15.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//
// The description of this file is in the header file.
//

#include "MysqlDB.h"

namespace database {

    MysqlDB::~MysqlDB() {
        con_->close();
        delete con_;
        DLOG(INFO) << "Database:: Mysql closed successfully";
    }

// 连接到Mysql后，使用数据库_db_name_
    void MysqlDB::Connect(std::string url, std::string port,
                          std::string username, std::string password) {
        static auto driver_ = sql::mysql::get_mysql_driver_instance();
        try {
            auto host_name = std::string("tcp://") + url + ":" + port;
            con_ = driver_->connect(host_name, username, password);
            DLOG(INFO) << "Database:: Mysql connected successfully";
        }
        catch (std::runtime_error e) {
            throw std::runtime_error(std::string("Database:: ")
                                     + e.what());
        }
    }

    void MysqlDB::OpenDatabase() {
        try {
            con_->setSchema(db_name_);
            DLOG(INFO) << "Database:: Open database " << db_name_;
        }
        catch (std::runtime_error e) {
            throw std::runtime_error(std::string("Database:: ")
                                     + e.what());
        }
    }

    void MysqlDB::Init() {
        try {
            auto stmt = con_->createStatement();
            stmt->execute("DROP DATABASE IF EXISTS "
                          + db_name_);
            stmt->execute("CREATE DATABASE " + db_name_);
            con_->setSchema(db_name_);
            stmt->execute("DROP TABLE IF EXISTS "
                          + key_table_name_);
            std::string sql = std::string()
                              + "(key_id BINARY(16) PRIMARY KEY,"
                              + "key_value BINARY(128),"
                              + "generated_time INT)";
            stmt->execute("CREATE TABLE "
                          + key_table_name_
                          + sql);
            DLOG(INFO) << "Database:: Create a table named "
                       << key_table_name_;

            delete stmt;
        }
        catch (std::runtime_error e) {
            throw std::runtime_error(std::string("Database:: ")
                                     + e.what());
        }
    }

    void MysqlDB::CreateUser(std::string user, std::string password) {
        try {
            auto stmt = con_->createStatement();
            std::string sql = std::string()
                              + "create user '"
                              + user
                              + "'@localhost identified by '"
                              + password
                              + "'";
            stmt->execute(sql);
            stmt->execute("flush privileges");
            // 赋予权限
            sql = std::string()
                  + "grant all privileges on "
                  + db_name_
                  + ".* to "
                  + user
                  + "@localhost identified by '"
                  + password
                  + "'";
            stmt->execute(sql);
            delete stmt;
        }
        catch (std::runtime_error e) {
            throw std::runtime_error(std::string("Database:: ")
                                     + e.what());
        }
    }

// 将Key中的key_id和key_value转换为std::string
// 方便执行SQL语句
    void MysqlDB::InsertKey(Key &key) {
        try {
            auto p_stmt = con_->prepareStatement("INSERT INTO "
                                                 + this->key_table_name_
                                                 + " VALUES (?,?,?)");
            std::string key_id(key.key_id_.cbegin(),
                               key.key_id_.cend());
            std::string key_value_enc(key.key_value_enc_.cbegin(),
                                      key.key_value_enc_.cend());
            p_stmt->setString(1, key_id);
            p_stmt->setString(2, key_value_enc);
            // 为了减少计算，暂定直接保存time_t格式的时间戳
            p_stmt->setInt(3, (int) key.generated_time_);
            //p_stmt->setString(3, unixTime2MysqlTime(k.generated_time_));
            p_stmt->execute();
            delete p_stmt;
        }
        catch (std::runtime_error e) {
            throw std::runtime_error(std::string("Database:: ")
                                     + e.what());
        }
        DLOG(INFO) << "Database:: Insert key";
    }

    Key MysqlDB::GetKey(KeyIdType key_id) {
        try {
            auto p_stmt = con_->prepareStatement("SELECT * FROM " + this->key_table_name_ + " WHERE key_id=?");
            std::string key_id_str(key_id.cbegin(),
                                   key_id.cend());
            p_stmt->setString(1, key_id_str);
            auto res = p_stmt->executeQuery();
            res->afterLast();
            delete p_stmt;
            // 如果有查询结果
            if (res->previous()) {
                auto key_value_enc_str = res->getString("key_value");
                std::time_t generated_time = res->getInt("generated_time");
                // 将std::string类型的key_value转为std::array
                KeyValueEncType key_value_enc_arr;
                for (std::size_t i = 0; i < Key::kKeyValueEncLen; i++) {
                    key_value_enc_arr[i] = (unsigned char) key_value_enc_str[i];
                }

                // new Key
                Key key(std::move(key_id), std::move(key_value_enc_arr),
                        generated_time);
                DLOG(INFO) << "Database:: Get key from database by key id";
                delete res;
                return key;
            } else {
                throw std::runtime_error("Database: Empty Set");
            }
        } catch (std::runtime_error e) {
            throw std::runtime_error(std::string("Database:: ")
                                     + e.what());
        }

    }

    void MysqlDB::DeleteKey(unsigned char *key_id) {
        try {
            auto p_stmt = con_->prepareStatement("DELETE FROM " + this->key_table_name_ + " WHERE key_id=?");
            // tmp为了将unsigned char 转换为std::string
            char *tmp = reinterpret_cast<char *>(key_id);
            std::string key_id_str(tmp, tmp + Key::kKeyIdLen);
            p_stmt->setString(1, key_id_str);
            p_stmt->execute();
            delete p_stmt;
        }
        catch (std::runtime_error e) {
            throw std::runtime_error(std::string("Database:: ")
                                     + e.what());
        }
        DLOG(INFO) << "Database:: Delete a line from database by key id";
    }

// 将time_t转换为tm格式，然后用strftime打印在字符串中
    std::string MysqlDB::UnixTime2MysqlTime(time_t unix_timestamp) {
        std::string result;
        struct tm t_tm = *localtime(&unix_timestamp);
        char s[20] = {0};
        std::strftime(s, sizeof(s), "%Y/%m/%d %H:%M:%S", &t_tm);
        result = s;
        return result;
    }

// 抽取mysql_time每个位置的数据，
// 转换为int后保存在tm中，
// 最后转为time_t返回
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

}