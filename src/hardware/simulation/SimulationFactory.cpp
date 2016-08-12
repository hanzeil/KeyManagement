//
// Created by hanzeil on 16-8-12.
//

#include "SimulationFactory.h"

SimulationFactory::~SimulationFactory() {

};


HardwareProductInterface *SimulationFactory::createProduct() {
    BOOST_LOG_TRIVIAL(info) << "Hardware: Simulation selected";
    return new Simulation();
};
