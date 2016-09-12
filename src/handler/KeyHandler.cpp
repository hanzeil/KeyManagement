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


    KeyHandler::KeyHandler() {

#ifdef SJK_1238
        hFactory_ = std::make_shared<SJK1238Factory>();
#endif

#ifdef SIMULATION
        hFactory_ = std::make_shared<Simulation>();
#endif
        hardware_ = hFactory_->CreateProduct();

#ifdef MYSQL
        dbfactory_ = std::make_shared<database::MysqlFactory>();
#endif
        db_ = dbfactory_->CreateProduct();

        if (!hardware_->OpenDevice()) {
            throw ("error");
        }

        db_->Connect("keymanagement", "keymanagement");
    }

    Key KeyHandler::CreateKey() {

        auto key_value = hardware_->GenerateKey(Key::kKeyValueLen);
        Key key(key_value);
        auto key_encrypted = key;
        key.key_value_ = hardware_->KeyEncryption(key_value);
        db_->InsertKey(key_encrypted);
        return key;
    }

    Key KeyHandler::FindKeyByID(KeyIdType key_id) {
    }

}
