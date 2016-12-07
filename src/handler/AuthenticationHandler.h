//
// Created by Hanzeil on 16-9-27.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//
// 负责处理与客户端的认证
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

    /// 服务器与客户端的双向认证处理
    /// 包括：产生服务端随机数、签名、验签等操作
    class AuthenticationHandler {
    public:

        /// 随机数的长度
        static const size_t kRandLen = 16;

        /// 拷贝构造函数
        /// 阻止拷贝
        AuthenticationHandler(const AuthenticationHandler &) = delete;

        /// 拷贝赋值函数
        /// 阻止拷贝
        AuthenticationHandler &operator=(const AuthenticationHandler &)= delete;

        /// 构造函数
        /// 参数是硬件的接口
        AuthenticationHandler(
                std::shared_ptr<encryption_device::EncryptionDeviceProductInterface> hardware);

        /// 处理客户端发来的第一次认证请求
        bool HandleAuthentication1(const std::vector<unsigned char> &rand_client,
                                   const std::vector<unsigned char> &cert_client);

        /// 处理客户端发来的第二次认证请求
        bool HandleAuthentication2(const std::vector<unsigned char> &rand_signed_server);

        /// 产生服务端第一次认证回复
        std::tuple<std::vector<unsigned char>, std::vector<unsigned char>>
        GetAuthentication1();

        /// 产生服务端第二次认证回复
        std::vector<unsigned char> GetAuthentication2();

    private:
        /// 客户端证书
        std::vector<unsigned char> cert_client_;
        /// 服务端证书
        std::vector<unsigned char> cert_server_;
        /// 客户端随机数
        std::vector<unsigned char> rand_client_;
        /// 客户端随机数的签名数据
        std::vector<unsigned char> rand_signed_client_;
        /// 服务端随机数
        std::vector<unsigned char> rand_server_;
        /// 服务端随机数的签名数据
        std::vector<unsigned char> rand_signed_server_;
<<<<<<< HEAD
        usb_key::Signature signature_;
=======
        /// 硬件模块的接口
>>>>>>> master
        std::shared_ptr<encryption_device::EncryptionDeviceProductInterface> hardware_;
    };

}
#endif //KEYMANAGEMENT_AUTHENTICATIONHANDLER_H
