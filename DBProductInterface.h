//
// Created by root on 12-10-7.
//

#ifndef KEYMANAGEMENT_DB_PRODUCT_INTERFACE_H
#define KEYMANAGEMENT_DB_PRODUCT_INTERFACE_H

#include <string>
#include "DataStructure.h"


class DBProductInterface {
public:
    explicit DBProductInterface() = default;

    virtual ~DBProductInterface();

    virtual bool connect(std::string username, std::string password)=0;

    virtual bool insert_key(Key k)=0;

};


#endif //KEYMANAGEMENT_DB_PRODUCT_INTERFACE_H
