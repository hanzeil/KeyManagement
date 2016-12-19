//
// Created by Hanzeil on 16-8-15.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//
// 生产Simulation产品的具体工厂
//

#ifndef KEYMANAGEMENT_SIMULATIONFACTORY_H
#define KEYMANAGEMENT_SIMULATIONFACTORY_H

#include "../EncryptionDeviceFactoryInterface.h"
#include "Simulation.h"
#include <glog/logging.h>

namespace encryption_device {

    class SimulationFactory : public EncrpytionDeviceFactoryInterface {
    public:

        SimulationFactory() = default;

        ~SimulationFactory();

        std::shared_ptr<EncryptionDeviceProductInterface> CreateProduct();
    };
}


#endif //KEYMANAGEMENT_SIMULATIONFACTORY_H
