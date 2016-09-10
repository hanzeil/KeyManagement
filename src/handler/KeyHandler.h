//
// Created by Hanzeil on 16-9-6.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//


#ifndef KEYMANAGEMENT_KEYHANDLER_H
#define KEYMANAGEMENT_KEYHANDLER_H

#include "config.h"
#include "../Key.h"
#include "../database/DBFactoryInterface.h"
#include "../encryption_device/EncryptionDeviceFactoryInterface.h"
#include <boost/log/trivial.hpp>

#ifdef MYSQL

#include "../database/mysql/MysqlFactory.h"

#endif

#ifdef SJK_1238

#include "../encryption_device/sjk1238/SJK1238Factory.h"

#endif

#ifdef SIMULATION

#include "../encryption_device/simulation/SimulationFactory.h"

#endif
namespace handler {

    class KeyHandler {
    public:
        KeyHandler(const KeyHandler &) = delete;

        KeyHandler &operator=(const KeyHandler &)= delete;

        KeyHandler();

        Key CreateKey();

        Key FindKeyByID(KeyIdType key_id);
    };

}

#endif //KEYMANAGEMENT_KEYHANDLER_H
