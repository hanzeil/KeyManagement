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

        try {
            Config config_settings;

            config_settings.ReadFile(config_settings.GetConifgPath(CONFIG_FILE_NAME));

            auto db_url = config_settings.Read<std::string>("DATABASE_URL");
            auto db_port = config_settings.Read<std::string>("DATABASE_PORT");
            auto db_user = config_settings.Read<std::string>("DATABASE_USER_NAME");
            auto db_password = config_settings.Read<std::string>("DATABASE_PASSWORD");
#ifdef MYSQL
            dbfactory_ = std::make_shared<database::MysqlFactory>();
            LOG(INFO) << "Database:: Mysql selected";
#endif
            db_ = dbfactory_->CreateProduct();
            db_->Connect(db_url, db_port, db_user, db_password);
            db_->OpenDatabase();
        }
        catch (std::runtime_error e) {
            LOG(FATAL) << e.what();
        }

#ifdef SJK_1238
        hFactory_ = std::make_shared<encryption_device::SJK1238Factory>();
        LOG(INFO) << "Hardware:: SJK_1238 cryptographic card selected";
#endif

#ifdef SIMULATION
        hFactory_ = std::make_shared<encryption_device::SimulationFactory>();
        LOG(INFO) << "Hardware:: Software simulation selected";
#endif
        hardware_ = hFactory_->CreateProduct();

        try {
            hardware_->OpenDevice();
        }
        catch (std::runtime_error e) {
            LOG(FATAL) << e.what();
        }


        io_service_.run();
    }
}
