//
// Created by Hanzeil on 16-9-13.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//
// 每个线程均需要开启的任务，包括数据库模块的接口和硬件模块的接口
//


#ifndef KEYMANAGEMENT_MYIOSERVICE_H
#define KEYMANAGEMENT_MYIOSERVICE_H

#include "global_define.h"
#include <glog/logging.h>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <cstdlib>
#include "../database/DBFactoryInterface.h"
#include "../encryption_device/EncryptionDeviceFactoryInterface.h"
#include "../config/Config.h"

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

        /// 运行io_service
        void Run();

        /// 加密硬件的接口
        std::shared_ptr<encryption_device::EncryptionDeviceProductInterface> hardware_;

    private:

        std::shared_ptr<encryption_device::EncrpytionDeviceFactoryInterface> hFactory_;

        boost::asio::io_service &io_service_;


    };

}

#endif //KEYMANAGEMENT_MYIOSERVICE_H
