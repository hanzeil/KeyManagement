//
// Created by root on 16-8-9.
//

#include "SJK1238Factory.h"

SJK1238Factory::~SJK1238Factory() {
}

HardwareProductInterface *SJK1238Factory:: createProduct() {
    BOOST_LOG_TRIVIAL(info) << "Database: Mysql selected";
    return new SJK1238();
}
