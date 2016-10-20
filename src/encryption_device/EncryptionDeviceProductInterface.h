//
// Created by Hanzeil on 16-8-15.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//
// 加密设备产品的接口，负责定义加密硬件的统一操作
//

#ifndef KEYMANAGEMENT_ENCRYPTION_DEVICE_PRODUCT_INTERFACE_H
#define KEYMANAGEMENT_ENCRYPTION_DEVICE_PRODUCT_INTERFACE_H

#include <iostream>
#include <array>
#include "../Key.h"

namespace encryption_device {

    class EncryptionDeviceProductInterface {
    public:
        EncryptionDeviceProductInterface() = default;

        virtual ~EncryptionDeviceProductInterface();

        //打开硬件设备
        virtual void OpenDevice()=0;

        //随机产生一个unsigned char* 类型的密钥，并返回
        //如果产生失败，返回NULL
        virtual KeyValueType GenerateKey()=0;

        //给定一个密钥key和密钥长度length, 用主密钥将密钥加密
        //如果加密失败，返回NULL
        virtual KeyValueEncType KeyEncryption(
                KeyValueType &)=0;

        //给定一个密钥key和密钥长度length, 用主密钥将密钥解密
        //如果解密失败，返回NULL
        virtual KeyValueType KeyDecryption(
                KeyValueEncType &)=0;
    };
}

#endif //KEYMANAGEMENT_ENCRYPTION_DEVICE_PRODUCT_INTERFACE_H

