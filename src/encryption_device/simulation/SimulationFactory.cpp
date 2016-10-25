//
// Created by Hanzeil on 16-8-15.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//
// The description of this file is in the header file.
//

#include "SimulationFactory.h"

namespace encryption_device {

    SimulationFactory::~SimulationFactory() {

    };


    std::shared_ptr<EncryptionDeviceProductInterface> SimulationFactory::CreateProduct() {
        LOG(INFO) << "Hardware:: Simulation selected";
        return std::make_shared<Simulation>();
    };
}
