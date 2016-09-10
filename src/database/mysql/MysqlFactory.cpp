//
// Created by Hanzeil on 16-8-15.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//
// The description of this file is in the header file.
//

#include "MysqlFactory.h"

namespace database {

    MysqlFactory::~MysqlFactory() {
    }

    DBProductInterface *MysqlFactory::CreateProduct() {
        BOOST_LOG_TRIVIAL(info) << "Database: Mysql selected";
        return new MysqlDB();
    }
}
