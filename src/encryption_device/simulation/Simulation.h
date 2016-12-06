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

        //拷贝构造函数
        //阻止拷贝
        Simulation(const Simulation &) = delete;

        //拷贝赋值函数
        //阻止拷贝
        Simulation &operator=(const Simulation &)= delete;

        ~Simulation();

        //打开Simulation设备
        //如果打开成功，返回true
        void OpenDevice();

        //随机产生一个unsigned char* 类型的密钥，并返回
        //密钥的空间由该函数产生，需要调用者管理
        //如果产生失败，返回NULL
        KeyValueType GenerateKey();

        //给定一个密钥key和密钥长度length, 用主密钥将密钥加密
        //加密结果为unsigned char *,大小与加密前的长度相同，
        //加密后的密钥的空间由该函数产生，需要调用者管理
        //如果加密失败，返回NULL
        KeyValueEncType KeyEncryption(KeyValueType &key);

        //给定一个密钥key和密钥长度length, 用主密钥将密钥解密
        //解密结果为unsigned char *,大小与解密前的长度相同，
        //解密后的密钥的空间由该函数产生，需要调用者管理
        //如果解密失败，返回NULL
        KeyValueType KeyDecryption(KeyValueEncType &key);

        bool VerifySignedData(
                std::vector<unsigned char> &cert,
                std::vector<unsigned char> &data,
                std::vector<unsigned char> &signed_data);
    private:

        RSA *master_key_pub_ = nullptr;

        RSA *master_key_pri_ = nullptr;

        //处理openssl出现的错误
        void HandleErrors();

        //模拟设备是否打开
        bool device_status = false;

        // RSA密钥长度
        const unsigned int rsa_len_ = Key::kKeyValueEncLen;
    };

}


#endif //KEYMANAGEMENT_SIMULATION_H
