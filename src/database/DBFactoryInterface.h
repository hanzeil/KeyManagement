//
// Created by root on 12-10-7.
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

    virtual DBProductInterface *createProduct()=0;
};




#endif //KEYMANAGEMENT_DB_FACTORY_INTERFACE_H
