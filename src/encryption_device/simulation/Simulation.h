//
// Created by Hanzeil on 16-8-15.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//

#ifndef KEYMANAGEMENT_SIMULATION_H
#define KEYMANAGEMENT_SIMULATION_H

#include <iostream>
#include <fstream>
#include "../EncryptionDeviceProductInterface.h"
#include <boost/log/trivial.hpp>
#include <random>
#include <openssl/aes.h>
#include <openssl/err.h>


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
    bool OpenDevice();

    //随机产生一个unsigned char* 类型的密钥，并返回
    //密钥的空间由该函数产生，需要调用者管理
    //如果产生失败，返回NULL
    unsigned char *GenerateKey(unsigned int length);

    //给定一个密钥key和密钥长度length, 用主密钥将密钥加密
    //加密结果为unsigned char *,大小与加密前的长度相同，
    //加密后的密钥的空间由该函数产生，需要调用者管理
    //如果加密失败，返回NULL
    unsigned char *KeyEncryption(unsigned char *key, unsigned int length);

    //给定一个密钥key和密钥长度length, 用主密钥将密钥解密
    //解密结果为unsigned char *,大小与解密前的长度相同，
    //解密后的密钥的空间由该函数产生，需要调用者管理
    //如果解密失败，返回NULL
    unsigned char *KeyDecryption(unsigned char *key_encrypted, unsigned int length);

private:
    //调试时暂时用来获取主密钥的函数
    //主密钥的空间由该函数产生，需要调用者管理
    unsigned char *GetMasterKey();

    //处理openssl出现的错误
    void HandleErrors();

    //模拟设备是否打开
    bool device_status = false;
};


#endif //KEYMANAGEMENT_SIMULATION_H
