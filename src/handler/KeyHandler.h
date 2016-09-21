//
// Created by Hanzeil on 16-9-6.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//


#ifndef KEYMANAGEMENT_KEYHANDLER_H
#define KEYMANAGEMENT_KEYHANDLER_H

#include <memory>
#include "../Key.h"
#include "../database/DBProductInterface.h"
#include "../encryption_device/EncryptionDeviceProductInterface.h"
#include <boost/log/trivial.hpp>

namespace handler {

    class KeyHandler {
    public:
        KeyHandler(const KeyHandler &) = delete;

        KeyHandler &operator=(const KeyHandler &)= delete;

        KeyHandler(std::shared_ptr<database::DBProductInterface> db,
                   std::shared_ptr<encryption_device::EncryptionDeviceProductInterface> hardware);

        Key CreateKey();

        Key FindKeyByID(KeyIdType key_id);

    private:
        std::shared_ptr<encryption_device::EncryptionDeviceProductInterface> hardware_;
        std::shared_ptr<database::DBProductInterface> db_;
    };

}

#endif //KEYMANAGEMENT_KEYHANDLER_H
