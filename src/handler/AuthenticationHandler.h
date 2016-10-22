//
// Created by Hanzeil on 16-9-27.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//


#ifndef KEYMANAGEMENT_AUTHENTICATIONHANDLER_H
#define KEYMANAGEMENT_AUTHENTICATIONHANDLER_H

#include <vector>
#include <string>
#include <random>
#include <array>

namespace handler {

    class AuthenticationHandler {
    public:

        static const size_t kRandLen = 16;

        AuthenticationHandler(const AuthenticationHandler &) = delete;

        AuthenticationHandler &operator=(const AuthenticationHandler &)= delete;

        AuthenticationHandler();

        bool HandleAuthentication1(std::vector<unsigned char> rand_client,
                                   std::vector<unsigned char> cert_client);

        bool HandleAuthentication2(std::vector<unsigned char> rand_server_en);

        std::string GetAuthentication1();

        std::string GetAuthentication2();

    private:
        std::vector<unsigned char> cert_client_;
        std::vector<unsigned char> cert_server_;
        std::vector<unsigned char> rand_client_;
        std::vector<unsigned char> rand_client_en_;
        std::vector<unsigned char> rand_server_;
        std::vector<unsigned char> rand_server_en_;

        struct DataPack1 {
            unsigned char method[2] = {'a', '1'};
            std::array<unsigned char, kRandLen> rand;
            uint16_t length;
        } data_pack1_;

        struct DataPack2 {
            unsigned char method[2] = {'a', '2'};
            std::array<unsigned char, kRandLen> rand_en;
        } data_pack2_;
    };

}
#endif //KEYMANAGEMENT_AUTHENTICATIONHANDLER_H
