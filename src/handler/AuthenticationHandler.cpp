//
// Created by Hanzeil on 16-9-27.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//
// The description of this file is in the header file.
//


#include "AuthenticationHandler.h"

namespace handler {

    AuthenticationHandler::AuthenticationHandler(
            std::shared_ptr<encryption_device::EncryptionDeviceProductInterface> hardware)
            : hardware_(hardware) {

    }

    bool AuthenticationHandler::HandleAuthentication1(
            std::vector<unsigned char> rand_client,
            std::vector<unsigned char> cert_client) {
        cert_client_ = cert_client;
        rand_client_ = rand_client;
        // 使用私钥将rand_client_签名
        // 暂时不需要
        rand_signed_client_ = rand_client_;
        return true;
    }

    bool AuthenticationHandler::HandleAuthentication2(
            std::vector<unsigned char> rand_signed_server) {
        rand_signed_server_ = rand_signed_server;
        // 验签rand_signed_server
        return true;
        return hardware_->VerifySignedData(
                cert_client_,
                rand_server_,
                rand_signed_server);
    }


    std::tuple<std::vector<unsigned char>, std::vector<unsigned char>>
    AuthenticationHandler::GetAuthentication1() {
        std::random_device rd;
        for (std::size_t i = 0; i < kRandLen; i++) {
            rand_server_.push_back((unsigned char) rd());
        }
        // 模拟证书
        cert_server_.push_back(0);
        return std::make_tuple(rand_server_, cert_server_);
    }

    std::vector<unsigned char> AuthenticationHandler::GetAuthentication2() {
        return rand_signed_client_;
    }

}