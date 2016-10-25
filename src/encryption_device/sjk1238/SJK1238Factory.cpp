//
// Created by Hanzeil on 16-8-15.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//
// The description of this file is in the header file.
//



#include "SJK1238Factory.h"

namespace encryption_device {

    SJK1238Factory::~SJK1238Factory() {
    }

    std::shared_ptr<EncryptionDeviceProductInterface> SJK1238Factory::CreateProduct() {
        LOG(INFO) << "Hardware:: sjk1238 card selected";
        return std::make_shared<SJK1238>();
    }
}
