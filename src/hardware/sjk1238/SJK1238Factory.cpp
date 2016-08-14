//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//



#include "SJK1238Factory.h"

SJK1238Factory::~SJK1238Factory() {
}

HardwareProductInterface *SJK1238Factory::createProduct() {
    BOOST_LOG_TRIVIAL(info) << "Hardware: sjk1238 card selected";
    return new SJK1238();
}
