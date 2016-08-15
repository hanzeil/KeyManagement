//
// Created by Hanzeil on 16-8-15.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//

#ifndef KEYMANAGEMENT_DB_FACTORY_INTERFACE_H
#define KEYMANAGEMENT_DB_FACTORY_INTERFACE_H

#include <string>
#include "DBProductInterface.h"
#include "mysql/MysqlDB.h"
#include <boost/log/trivial.hpp>

class DBFactoryInterface {
public:
    explicit DBFactoryInterface() = default;

    virtual ~DBFactoryInterface();

    virtual DBProductInterface *CreateProduct()=0;
};




#endif //KEYMANAGEMENT_DB_FACTORY_INTERFACE_H
