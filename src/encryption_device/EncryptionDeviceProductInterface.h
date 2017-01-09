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
#include "../global/Key.h"

namespace encryption_device {

    class EncryptionDeviceProductInterface {
    public:
        EncryptionDeviceProductInterface() = default;

        virtual ~EncryptionDeviceProductInterface();

        /// 打开硬件设备
        /// 打开失败会抛出异常
        virtual void OpenDevice()=0;

        /// 随机产生一个KeyValueType类型的密钥，并返回
        /// 如果产生失败，会抛出异常
        virtual KeyValueType GenerateKey()=0;

        /// 给定一个密钥key, 用主密钥将密钥加密
        /// 如果加密失败，会抛出异常
        virtual KeyValueEncType KeyEncryption(
                KeyValueType &)=0;

        /// 给定一个密钥key, 用主密钥将密钥解密
        /// 如果解密失败，会抛出异常
        virtual KeyValueType KeyDecryption(
                KeyValueEncType &)=0;

        /// 验签操作，cert存放证书，data存放待验签数据，singed_data属于签名值
        /// 如果验签失败，会抛出异常
        virtual void VerifySignedData(
                const std::vector<unsigned char> &cert,
                const std::vector<unsigned char> &data,
                const std::vector<unsigned char> &signed_data)=0;
    };
}

#endif //KEYMANAGEMENT_ENCRYPTION_DEVICE_PRODUCT_INTERFACE_H

