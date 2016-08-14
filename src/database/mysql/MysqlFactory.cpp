//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//

#include "MysqlFactory.h"

MysqlFactory::~MysqlFactory() {
}

DBProductInterface* MysqlFactory::createProduct() {
    BOOST_LOG_TRIVIAL(info) << "Database: Mysql selected";
    return new MysqlDB();
}
