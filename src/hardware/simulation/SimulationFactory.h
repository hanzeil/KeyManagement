//
// Created by hanzeil on 16-8-12.
//

#ifndef KEYMANAGEMENT_SIMULATIONFACTORY_H
#define KEYMANAGEMENT_SIMULATIONFACTORY_H

#include "../HardwareFactoryInterface.h"
#include "Simulation.h"
#include <boost/log/trivial.hpp>

class SimulationFactory : public HardwareFactoryInterface {
public:

    explicit SimulationFactory() = default;

    ~SimulationFactory();

    HardwareProductInterface *createProduct();
};


#endif //KEYMANAGEMENT_SIMULATIONFACTORY_H
