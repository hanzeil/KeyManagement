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
#include "Key.h"

// 抽象类
// sample usage:
// DBProductInterface *db=new MysqlDB()
class DBProductInterface {
public:
    explicit DBProductInterface() = default;

    virtual ~DBProductInterface();

    //连接数据库
    // 连接成功则返回true
    virtual bool Connect(std::string username, std::string password)=0;

    // 新建Key表
    // 如果新建成功则返回true
    virtual bool InitTable()=0;

    //插入一个Key到数据库
    //插入成功则返回true
    virtual bool InsertKey(Key &k)=0;

    //根据Key id从数据库查找key并以Key类型的指针返回.
    //如果查找失败，返回NULL
    virtual Key *GetKey(unsigned char *key_id)=0;

};


#endif //KEYMANAGEMENT_DB_PRODUCT_INTERFACE_H
