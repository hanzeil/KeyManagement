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

namespace tcp {
    ThreadTask::ThreadTask(boost::asio::io_service &io_service)
            : io_service_(io_service) {
    }

    void ThreadTask::Run() {

#ifdef MYSQL
        dbfactory_ = std::make_shared<database::MysqlFactory>();
#endif
        db_ = dbfactory_->CreateProduct();
        try {
            db_->Connect("keymanagement", "keymanagement");
        }
        catch (std::runtime_error e) {
            BOOST_LOG_TRIVIAL(fatal) << e.what();
            _exit(111);
        }

#ifdef SJK_1238
        hFactory_ = std::make_shared<encryption_device::SJK1238Factory>();
#endif

#ifdef SIMULATION
        hFactory_ = std::make_shared<encryption_device::SimulationFactory>();
#endif
        hardware_ = hFactory_->CreateProduct();

        try {
            hardware_->OpenDevice();
            auto master_key_encrypted = db_->GetMasterKey();
            hardware_->ImportMasterKey(master_key_encrypted);
        }
        catch (std::runtime_error e) {
            BOOST_LOG_TRIVIAL(fatal) << e.what();
            _exit(5);
        }


        io_service_.run();
    }
}
