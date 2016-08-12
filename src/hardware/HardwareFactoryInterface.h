//
// Created by root on 16-8-9.
//

#ifndef KEYMANAGEMENT_HARDWAREFACTORYINTERFACE_H
#define KEYMANAGEMENT_HARDWAREFACTORYINTERFACE_H

#include "HardwareProductInterface.h"

class HardwareFactoryInterface {
public:
    explicit HardwareFactoryInterface() = default;

    virtual ~HardwareFactoryInterface();

    virtual HardwareProductInterface *createProduct()=0;
};


#endif //KEYMANAGEMENT_HARDWAREFACTORYINTERFACE_H
