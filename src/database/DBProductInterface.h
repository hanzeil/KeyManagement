//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//

#ifndef KEYMANAGEMENT_DB_PRODUCT_INTERFACE_H
#define KEYMANAGEMENT_DB_PRODUCT_INTERFACE_H

#include <string>
#include "Key.h"


class DBProductInterface {
public:
    explicit DBProductInterface() = default;

    virtual ~DBProductInterface();

    virtual bool connect(std::string username, std::string password)=0;

    virtual bool insertKey(Key k)=0;

    virtual Key *getKey(unsigned char *key_id)=0;

};


#endif //KEYMANAGEMENT_DB_PRODUCT_INTERFACE_H
