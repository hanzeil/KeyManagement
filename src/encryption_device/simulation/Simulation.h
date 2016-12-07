//
// Created by Hanzeil on 16-8-15.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//

#ifndef KEYMANAGEMENT_SIMULATION_H
#define KEYMANAGEMENT_SIMULATION_H

#include "global_define.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include "../EncryptionDeviceProductInterface.h"
#include <glog/logging.h>
#include <random>
#include <openssl/aes.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>


namespace encryption_device {

// 此类继承并实现了EncryptionDeviceProductInterface接口
// 通过openssl/aes加密库实现
// 加密／解密算法固定，为aes.
// sample usage:
// EncryptionDeviceProductInterface *device=new Simulation();
// device.OpenDevice();
// auto key = device.GenerateKey(16);
// delete device;
    class Simulation : public EncryptionDeviceProductInterface {
    public:
        explicit Simulation() = default;

        /// 拷贝构造函数
        /// 阻止拷贝
        Simulation(const Simulation &) = delete;

        /// 拷贝赋值函数
        /// 阻止拷贝
        Simulation &operator=(const Simulation &)= delete;

        ~Simulation();

        /// 打开硬件设备
        /// 打开失败会抛出异常
        void OpenDevice();

        /// 随机产生一个KeyValueType类型的密钥，并返回
        /// 如果产生失败，会抛出异常
        KeyValueType GenerateKey();

        /// 给定一个密钥key, 用主密钥将密钥加密
        /// 如果加密失败，会抛出异常
        KeyValueEncType KeyEncryption(KeyValueType &key);

        /// 给定一个密钥key, 用主密钥将密钥解密
        /// 如果解密失败，会抛出异常
        KeyValueType KeyDecryption(KeyValueEncType &key);

        /// 验签操作，cert存放证书，data存放待验签数据，singed_data属于签名值
        /// 如果验签失败，会抛出异常
        bool VerifySignedData(
                const std::vector<unsigned char> &cert,
                const std::vector<unsigned char> &data,
                const std::vector<unsigned char> &signed_data);

    private:

        /// 主密钥的公钥
        RSA *master_key_pub_ = nullptr;

        /// 主密钥的私钥
        RSA *master_key_pri_ = nullptr;

        /// 处理openssl出现的错误
        void HandleErrors();

        /// 模拟设备是否打开
        bool device_status = false;

        /// RSA密钥长度
        const unsigned int rsa_len_ = Key::kKeyValueEncLen;
    };

}


#endif //KEYMANAGEMENT_SIMULATION_H
