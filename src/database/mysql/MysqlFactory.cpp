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

    std::shared_ptr<DBProductInterface> MysqlFactory::CreateProduct() {
        LOG(INFO) << "Database:: sjk1238 card selected";
        return std::make_shared<MysqlDB>();
    }
}
