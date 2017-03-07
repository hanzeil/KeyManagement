//
// Created by Hanzeil on 16-8-15.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//
// SJK1238产品类，实现了密钥与Mysql数据库之间的操作
//

#ifndef KEYMANAGEMENT_SJK1238_H
#define KEYMANAGEMENT_SJK1238_H

#include "GetPublicKey.h"
#include "../EncryptionDeviceFactoryInterface.h"
#include <swsds.h>
#include <glog/logging.h>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>


namespace encryption_device {

// 此类继承并实现了EncryptionDeviceProductInterface接口
// 通过密码卡SJK1238和相关驱动以及API接口实现
// sample usage:
// auto device=std::make_shared<encryption_device::SJK1238>();
// device.OpenDevice();
// auto key = device.GenerateKey(16);
// delete device;
    class SJK1238 : public EncryptionDeviceProductInterface {
    public:

        SJK1238() = default;

        /// 拷贝构造函数
        /// 阻止拷贝
        SJK1238(const SJK1238 &) = delete;

        /// 拷贝赋值函数
        /// 阻止拷贝
        SJK1238 &operator=(const SJK1238 &)= delete;

        ~SJK1238();

        /// 打开硬件设备
        /// 打开失败会抛出异常
        void OpenDevice();

        /// 随机产生一个KeyValueType类型的密钥，并返回
        /// 如果产生失败，会抛出异常
        KeyValueType GenerateKey();

        /// 给定一个密钥key, 用主密钥将密钥加密
        /// 如果加密失败，会抛出异常
        KeyValueEncType KeyEncryption(
                KeyValueType &);

        /// 给定一个密钥key, 用主密钥将密钥解密
        /// 如果解密失败，会抛出异常
        KeyValueType KeyDecryption(
                KeyValueEncType &);

        /// 验签操作，cert存放证书，data存放待验签数据，singed_data属于签名值
        /// 如果验签失败，会抛出异常
        void VerifySignedData(
                const std::vector<unsigned char> &cert,
                const std::vector<unsigned char> &data,
                const std::vector<unsigned char> &signed_data);


    private:

        /// SJK1238设备句柄，用法见SJK1238 API文档
        void *p_dev_handle_ = nullptr;

        /// SJK1238会话句柄
        void *p_ses_handle_ = nullptr;


        struct ECCPUBLICKEYBLOB {
            u_int32_t bit_len;
            unsigned char x[64];
            unsigned char y[64];
        };
        struct ECCSIGNATUREBLOB {
            unsigned char r[64];
            unsigned char s[64];
        };
    };
}

#endif //KEYMANAGEMENT_SJK1238_H
