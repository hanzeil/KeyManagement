//
// Created by Hanzeil on 16-8-15.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//
// 生产Mysql产品的具体工厂
//

#ifndef KEYMANAGEMENT_MYSQLFACTORY_H
#define KEYMANAGEMENT_MYSQLFACTORY_H

#include "../DBFactoryInterface.h"

namespace database {

    class MysqlFactory : public DBFactoryInterface {
    public:
        MysqlFactory() = default;

        ~MysqlFactory();

        DBProductInterface *CreateProduct();
    };
}

#endif //KEYMANAGEMENT_MYSQLFACTORY_H
