//
// Created by Hanzeil on 16-8-15.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//
// 生产SJK1238产品的具体工厂
//

#ifndef KEYMANAGEMENT_SJK1238FACTORY_H
#define KEYMANAGEMENT_SJK1238FACTORY_H

#include "../EncryptionDeviceFactoryInterface.h"
#include "../EncryptionDeviceProductInterface.h"
#include "SJK1238.h"
#include <boost/log/trivial.hpp>

class SJK1238Factory : public EncrpytionDeviceFactoryInterface {
public:

    SJK1238Factory() = default;

    ~SJK1238Factory();

    EncryptionDeviceProductInterface *CreateProduct();
};


#endif //KEYMANAGEMENT_SJK1238FACTORY_H
