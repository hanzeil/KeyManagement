//
// Created by Hanzeil on 16-8-15.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//
// 数据库工厂接口，可以生产多种数据库接口，目前支持Mysql
//

#ifndef KEYMANAGEMENT_DB_FACTORY_INTERFACE_H
#define KEYMANAGEMENT_DB_FACTORY_INTERFACE_H

#include <string>
#include "DBProductInterface.h"
#include "mysql/MysqlDB.h"
#include <boost/log/trivial.hpp>

namespace database {

    class DBFactoryInterface {
    public:
        DBFactoryInterface() = default;

        virtual ~DBFactoryInterface();

        virtual std::shared_ptr<DBProductInterface> CreateProduct()=0;
    };

}


#endif //KEYMANAGEMENT_DB_FACTORY_INTERFACE_H
