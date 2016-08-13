//
// Created by root on 16-8-9.
//

#ifndef KEYMANAGEMENT_MYSQLFACTORY_H
#define KEYMANAGEMENT_MYSQLFACTORY_H

#include "../DBFactoryInterface.h"

class MysqlFactory : public DBFactoryInterface {
public:
    explicit MysqlFactory() = default;

    ~MysqlFactory();

    DBProductInterface *createProduct();
};


#endif //KEYMANAGEMENT_MYSQLFACTORY_H
