//
// Created by Hanzeil on 16-9-6.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//
// The description of this file is in the header file.
//


#include "KeyHandler.h"

namespace handler {


    KeyHandler::KeyHandler(std::shared_ptr<database::DBProductInterface> db,
                           std::shared_ptr<encryption_device::EncryptionDeviceProductInterface> hardware)
            : db_(db),
              hardware_(hardware) {
    }

    Key KeyHandler::CreateKey() {
        try {
            auto key_value = hardware_->GenerateKey();
            Key key(key_value);
            auto key_encrypted = key;
            key_encrypted.key_value_ = hardware_->KeyEncryption(key_value);
            db_->InsertKey(key_encrypted);
            return key;
        }
        catch (std::runtime_error e) {
            throw;
        }
    }

    Key KeyHandler::FindKeyByID(KeyIdType key_id) {
        try {
            auto key = db_->GetKey(key_id);
            key.key_value_ = hardware_->KeyDecryption(key.key_value_);
            return key;
        }
        catch (std::runtime_error e) {
            throw;
        }
    }

}
