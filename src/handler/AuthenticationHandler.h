//
// Created by Hanzeil on 16-9-27.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//


#ifndef KEYMANAGEMENT_AUTHENTICATIONHANDLER_H
#define KEYMANAGEMENT_AUTHENTICATIONHANDLER_H

#include <memory>
#include <vector>
#include <string>
#include <random>
#include <array>
#include "../encryption_device/EncryptionDeviceProductInterface.h"
#include "usb_key/Signature.h"

namespace handler {

    class AuthenticationHandler {
    public:

        static const size_t kRandLen = 16;

        static const size_t kRandEncLen = 128;

        AuthenticationHandler(const AuthenticationHandler &) = delete;

        AuthenticationHandler &operator=(const AuthenticationHandler &)= delete;

        AuthenticationHandler(
                std::shared_ptr<encryption_device::EncryptionDeviceProductInterface> hardware);

        bool HandleAuthentication1(std::vector<unsigned char> rand_client,
                                   std::vector<unsigned char> cert_client);

        bool HandleAuthentication2(std::vector<unsigned char> rand_signed_server);

        std::tuple<std::vector<unsigned char>, std::vector<unsigned char>>
        GetAuthentication1();

        std::vector<unsigned char> GetAuthentication2();

    private:
        std::vector<unsigned char> cert_client_;
        std::vector<unsigned char> cert_server_;
        std::vector<unsigned char> rand_client_;
        std::vector<unsigned char> rand_signed_client_;
        std::vector<unsigned char> rand_server_;
        std::vector<unsigned char> rand_signed_server_;
        usb_key::Signature signature_;
        std::shared_ptr<encryption_device::EncryptionDeviceProductInterface> hardware_;
    };

}
#endif //KEYMANAGEMENT_AUTHENTICATIONHANDLER_H
