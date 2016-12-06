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


    KeyHandler::KeyHandler(std::shared_ptr<encryption_device::EncryptionDeviceProductInterface> hardware)
            : hardware_(hardware) {
        auto db_url = config::ConfigSingleton::GetInstance().db_url_;
        auto db_port = config::ConfigSingleton::GetInstance().db_port_;
        auto db_user = config::ConfigSingleton::GetInstance().db_user_name_;
        auto db_password = config::ConfigSingleton::GetInstance().db_user_password_;
#ifdef MYSQL
        auto db_factory = std::make_shared<database::MysqlFactory>();
        LOG(INFO) << "Database:: Mysql selected";
#endif
        db_ = db_factory->CreateProduct();
        db_->Connect(db_url, db_port, db_user, db_password);
        db_->OpenDatabase();
    }

    Key KeyHandler::CreateKey() {
        try {
            auto key_value = hardware_->GenerateKey();
            Key key(key_value);
            key.key_value_enc_ = hardware_->KeyEncryption(key_value);
            db_->InsertKey(key);
            LOG(INFO) << "Created a key and encrypt it and insert this key-encrypted into the Database";
            return key;
        }
        catch (std::runtime_error e) {
            throw;
        }
    }

    Key KeyHandler::FindKeyByID(KeyIdType key_id) {
        try {
            auto key = db_->GetKey(key_id);
            key.key_value_ = hardware_->KeyDecryption(key.key_value_enc_);
            LOG(INFO) << "Find a key-encrypted by key-id and decrypt it";
            return key;
        }
        catch (std::runtime_error e) {
            throw;
        }
    }

}
