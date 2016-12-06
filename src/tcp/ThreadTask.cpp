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

#ifdef SJK_1238
        hFactory_ = std::make_shared<encryption_device::SJK1238Factory>();
        DLOG(INFO) << "Hardware:: SJK_1238 cryptographic card selected";
#endif

#ifdef SIMULATION
        hFactory_ = std::make_shared<encryption_device::SimulationFactory>();
        DLOG(INFO) << "Hardware:: Software simulation selected";
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
