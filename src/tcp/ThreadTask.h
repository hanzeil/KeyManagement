//
// Created by Hanzeil on 16-9-13.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//


#ifndef KEYMANAGEMENT_MYIOSERVICE_H
#define KEYMANAGEMENT_MYIOSERVICE_H

#include "config.h"
#include <boost/asio.hpp>
#include <cstdlib>
#include "../database/DBFactoryInterface.h"
#include "../encryption_device/EncryptionDeviceFactoryInterface.h"

#ifdef MYSQL

#include "../database/mysql/MysqlFactory.h"

#endif

#ifdef SJK_1238

#include "../encryption_device/sjk1238/SJK1238Factory.h"

#endif

#ifdef SIMULATION

#include "../encryption_device/simulation/SimulationFactory.h"

#endif

namespace tcp {

    class ThreadTask {
    public:

        ThreadTask(boost::asio::io_service &io_service);

        void Run();

        std::shared_ptr<database::DBProductInterface> db_;

        std::shared_ptr<encryption_device::EncryptionDeviceProductInterface> hardware_;

    private:

        std::shared_ptr<database::DBFactoryInterface> dbfactory_;

        std::shared_ptr<encryption_device::EncrpytionDeviceFactoryInterface> hFactory_;

        boost::asio::io_service &io_service_;
    };

}

#endif //KEYMANAGEMENT_MYIOSERVICE_H
