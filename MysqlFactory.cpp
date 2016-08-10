//
// Created by root on 16-8-9.
//

#include "MysqlFactory.h"

MysqlFactory::~MysqlFactory() {
}

DBProductInterface* MysqlFactory::createProduct() {
    BOOST_LOG_TRIVIAL(info) << "Database: Mysql selected";
    return new MysqlDB();
}
