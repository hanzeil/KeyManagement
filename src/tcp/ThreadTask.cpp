//
// Created by Hanzeil on 16-9-13.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//
// The description of this file is in the header file.
//


#include "ThreadTask.h"

namespace http {
    namespace server {
        ThreadTask::ThreadTask(boost::asio::io_service &io_service)
                : io_service_(io_service) {
        }

        void ThreadTask::run() {

#ifdef MYSQL
            dbfactory_ = std::make_shared<database::MysqlFactory>();
#endif
            db_ = dbfactory_->CreateProduct();
            db_->Connect("keymanagement", "keymanagement");

#ifdef SJK_1238
            hFactory_ = std::make_shared<SJK1238Factory>();
#endif

#ifdef SIMULATION
            hFactory_ = std::make_shared<SimulationFactory>();
#endif
            hardware_ = hFactory_->CreateProduct();

            if (!hardware_->OpenDevice()) {
                throw ("error");
            }

            io_service_.run();
        }
    }
}