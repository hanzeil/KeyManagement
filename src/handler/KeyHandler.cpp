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

    }

    Key KeyHandler::CreateKey() {

#ifdef SJK_1238
        EncrpytionDeviceFactoryInterface *hFactory = new SJK1238Factory();
#endif

#ifdef SIMULATION
        EncrpytionDeviceFactoryInterface *hFactory = new SimulationFactory();
#endif

        EncryptionDeviceProductInterface *hardware = hFactory->CreateProduct();

        if (!hardware->OpenDevice()) {
            delete hFactory;
            delete hardware;
            throw ("error");
        }
        auto key_value = hardware->GenerateKey(Key::kKeyValueLen);
        Key key(key_value);
        auto key_encrypted = key;
        key.key_value_ = hardware->KeyEncryption(key_value);

#ifdef MYSQL
        database::DBFactoryInterface *dbfactory = new database::MysqlFactory();
#endif

        database::DBProductInterface *db = dbfactory->CreateProduct();
        db->Connect("keymanagement", "keymanagement");
        db->InsertKey(key_encrypted);
        delete hFactory;
        delete hardware;
        delete dbfactory;
        delete db;
        return std::move(key);
    }

    Key KeyHandler::FindKeyByID(KeyIdType key_id) {
    }

}
