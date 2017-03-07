//
// Created by Hanzeil on 16-8-15.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//
// 数据库产品的接口，负责定义数据库的统一操作
//

#ifndef KEYMANAGEMENT_DB_PRODUCT_INTERFACE_H
#define KEYMANAGEMENT_DB_PRODUCT_INTERFACE_H

#include <string>
#include "../global/Key.h"

namespace database {

// 抽象类
// sample usage:
// DBProductInterface *db=new MysqlDB()
    class DBProductInterface {
    public:
        DBProductInterface() = default;

        virtual ~DBProductInterface();

        /// 连接DBMS
        /// 连接失败会抛出异常
        virtual void Connect(std::string url, std::string port,
                             std::string username, std::string password)=0;

        /// 打开名为db_name_的数据库
        /// 打开失败会抛出异常
        virtual void OpenDatabase()=0;

        /// 新建Key表
        /// 新建失败会抛出异常
        virtual void Init()=0;

        /// 创建一个具有访问数据库db_name_所有权限的用户
        /// 创建失败会抛出异常
        virtual void CreateUser(std::string user, std::string password)=0;

        /// 插入一个Key到数据库
        /// 插入失败会抛出异常
        virtual void InsertKey(Key &key)=0;

        /// 根据Key id从Mysql查找key并以Key类型的指针返回.
        /// 查找失败会抛出异常
        virtual Key GetKey(KeyValueType key_id)=0;

        /// 根据Key id从Mysql删除该字段.
        /// 删除失败会抛出异常
        virtual void DeleteKey(unsigned char *key_id)=0;

    };

}

#endif //KEYMANAGEMENT_DB_PRODUCT_INTERFACE_H
