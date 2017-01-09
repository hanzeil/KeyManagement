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

    void AuthenticationHandler::HandleAuthentication1(
            const std::vector<unsigned char> &rand_client,
            const std::vector<unsigned char> &cert_client) {
        cert_client_ = cert_client;
        rand_client_ = rand_client;
        // 服务器端随机数
        std::random_device rd;
        for (std::size_t i = 0; i < kRandLen; i++) {
            rand_server_.push_back((unsigned char) rd());
        }
        // 模拟证书
        cert_server_.push_back(0);
        // 如果证书无效
        if (false) {
            std::stringstream ss;
            ss << "Authentication:: Certificate of the client is invalid";
            throw std::runtime_error(ss.str());
        }
    }

    void AuthenticationHandler::HandleAuthentication2(
            const std::vector<unsigned char> &rand_signed_server) {
        rand_signed_server_ = rand_signed_server;
        try {
            // 填充16个字节的0
            auto zeros = decltype(rand_server_)(16, 0);
            rand_server_.insert(rand_server_.end(),
                                zeros.cbegin(),
                                zeros.cend());
            // 验证签名
            hardware_->VerifySignedData(
                    cert_client_,
                    rand_server_,
                    rand_signed_server);
        }
        catch (std::runtime_error e) {
            throw;
        }
        // 使用私钥将rand_client_签名
        try {
            rand_signed_client_ = rand_client_;
        }
        catch (std::runtime_error e) {
            // 如果签名失败
            throw;
        }
    }


    std::tuple<std::vector<unsigned char>, std::vector<unsigned char>>
    AuthenticationHandler::GetAuthentication1() {
        // 返回服务器随机数以及服务器证书
        return std::make_tuple(rand_server_, cert_server_);
    }

    std::vector<unsigned char> AuthenticationHandler::GetAuthentication2() {
        // 返回服务器签名的客户端随机数
        return rand_signed_client_;
    }

}