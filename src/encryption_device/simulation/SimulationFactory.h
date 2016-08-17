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
#include <boost/log/trivial.hpp>

class SimulationFactory : public EncrpytionDeviceFactoryInterface {
public:

    explicit SimulationFactory() = default;

    ~SimulationFactory();

    EncryptionDeviceProductInterface *CreateProduct();
};


#endif //KEYMANAGEMENT_SIMULATIONFACTORY_H
