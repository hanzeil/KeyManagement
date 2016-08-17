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

SimulationFactory::~SimulationFactory() {

};


EncryptionDeviceProductInterface *SimulationFactory::CreateProduct() {
    BOOST_LOG_TRIVIAL(info) << "Hardware: Simulation selected";
    return new Simulation();
};