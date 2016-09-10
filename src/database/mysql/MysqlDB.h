//
// Created by Hanzeil on 16-8-15.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//
// Mysql产品类，实现了密钥与Mysql数据库之间的操作
//

#ifndef KEYMANAGEMENT_MYSQL_DB_H
#define KEYMANAGEMENT_MYSQL_DB_H

#include "../DBProductInterface.h"
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/exception.h>
#include <cppconn/driver.h>
#include <stdexcept>
#include <boost/log/trivial.hpp>
#include <string>
#include <array>
#include <cstring>
#include <sstream>
#include <time.h>
#include <cstdlib>

namespace database {

// 此类继承并实现了DBProductInterface接口
// 通过mysql-connector-cpp-1.7.0库实现
// sample usage:
// DBProductInterface *db=new MysqlDB()
// db->Connect(user,password);
// db->InsertKey(k);
// delete db;
    class MysqlDB : public DBProductInterface {
    public:
        MysqlDB() = default;

        //拷贝构造函数
        //阻止拷贝
        MysqlDB(const MysqlDB &) = delete;

        //拷贝赋值函数
        //阻止赋值
        MysqlDB &operator=(MysqlDB &)= delete;

        ~MysqlDB();

        // 连接mysql
        // 连接成功则返回true
        bool Connect(std::string username, std::string password);

        // 新建Key表
        // 如果新建成功则返回true
        bool InitTable();

        //插入一个Key到Mysql
        //插入成功则返回true
        bool InsertKey(Key &key);

        //根据Key id从Mysql查找key并以Key类型的指针返回.
        //Key类型和其中的key_value, key_id的空间均在该函数中分配
        //参数key_id的空间没有释放，需要调用者继续管理
        //如果查找失败，返回NULL
        Key GetKey(KeyIdType key_id);

        //根据Key id从Mysql删除该字段.
        //参数key_id的空间没有释放，需要调用者继续管理
        //如果删除成功，返回true
        bool DeleteKey(unsigned char *key_id);

    private:
        //将time_t格式的时间戳转换为Mysql的DateTime类型的时间格式
        //返回格式为 %Y/%m/%d %H:%M:%S
        static std::string UnixTime2MysqlTime(time_t unix_timestamp);

        //将Mysql的DateTime类型的时间格式转换为time_t格式的时间戳
        static std::time_t MysqlTime2UnixTime(std::string mysql_time);

        //mysql_driver
        //指针不需要释放
        //默认为NULL
        sql::mysql::MySQL_Driver *driver_ = nullptr;

        //driver_->connector()返回的连接对象
        //默认为NULL
        sql::Connection *con_ = nullptr;

        //数据所在的数据库名称
        std::string db_name_ = "symmetric_key";

        //key所在的数据表名称
        std::string key_table_name_ = "tb_key";

    };

}

#endif //KEYMANAGEMENT_MYSQL_DB_H
