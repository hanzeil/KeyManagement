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

    AuthenticationHandler::AuthenticationHandler() {

    }

    bool AuthenticationHandler::HandleAuthentication1(
            std::vector<unsigned char> rand_client,
            std::vector<unsigned char> cert_client) {
        cert_client_ = cert_client;
        rand_client_ = rand_client;
        // 解析证书
        // 加密rand_client_
        rand_client_en_ = rand_client_;
        return true;
    }

    bool AuthenticationHandler::HandleAuthentication2(
            std::vector<unsigned char> rand_server_en) {
        rand_server_en_ = rand_server_en;
        // 解密rand_server_en
        // 与rand_server对比
        return true;
    }

    std::string AuthenticationHandler::GetAuthentication1() {
        std::random_device rd;
        for (std::size_t i = 0; i < kRandLen; i++) {
            rand_server_.push_back((unsigned char) rd());
            data_pack1_.rand[i] = rand_server_.back();
        }
        // 模拟证书
        data_pack1_.length = 50;
        std::string result;
        for (std::size_t i = 0; i < sizeof(data_pack1_); i++) {
            result.push_back(*((unsigned char *) &data_pack1_ + i));
        }
        std::string cert(50, 0);
        result += cert;
        return result;
    }

    std::string AuthenticationHandler::GetAuthentication2() {
        for (std::size_t i = 0; i < kRandLen; i++) {
            data_pack2_.rand_en[i] = rand_client_en_[i];
        }
        std::string result;
        for (std::size_t i = 0; i < sizeof(data_pack2_); i++) {
            result.push_back(*((unsigned char *) &data_pack2_ + i));
        }
        return result;
    }

}