//
// Created by root on 16-8-9.
//

#ifndef KEYMANAGEMENT_SJK1238FACTORY_H
#define KEYMANAGEMENT_SJK1238FACTORY_H

#include "HardwareFactoryInterface.h"
#include "HardwareProductInterface.h"
#include "SJK1238.h"
#include <boost/log/trivial.hpp>

class SJK1238Factory : public HardwareFactoryInterface {
public:

    explicit SJK1238Factory() = default;

    ~SJK1238Factory();

    HardwareProductInterface *createProduct();
};


#endif //KEYMANAGEMENT_SJK1238FACTORY_H
