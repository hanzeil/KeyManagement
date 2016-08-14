//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
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
