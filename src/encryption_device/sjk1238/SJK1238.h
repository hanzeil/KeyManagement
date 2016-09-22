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

#include "../EncryptionDeviceFactoryInterface.h"
#include <swsds.h>
#include <boost/log/trivial.hpp>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>


namespace encryption_device {

// 此类继承并实现了EncryptionDeviceProductInterface接口
// 通过密码卡SJK1238和相关驱动以及API接口实现
// sample usage:
// EncryptionDeviceProductInterface *device=new SJK1238();
// device.OpenDevice();
// auto key = device.GenerateKey(16);
// delete device;
    class SJK1238 : public EncryptionDeviceProductInterface {
    public:

        SJK1238() = default;

        //拷贝构造函数
        //阻止拷贝
        SJK1238(const SJK1238 &) = delete;

        //拷贝赋值函数
        //阻止拷贝
        SJK1238 &operator=(const SJK1238 &)= delete;

        ~SJK1238();

        //打开SJK1238设备
        //如果打开成功，返回true
        void OpenDevice();

        //随机产生一个unsigned char* 类型的密钥，并返回
        //密钥的空间由该函数产生，需要调用者管理
        //如果产生失败，返回NULL
        KeyValueType GenerateKey();

        //给定一个密钥key和密钥长度length, 用主密钥将密钥加密并返回
        //加密结果为unsigned char *,大小与加密前的长度相同，
        //加密后的密钥的空间由该函数产生，需要调用者管理
        //如果加密失败，返回NULL
        KeyValueType KeyEncryption(
                KeyValueType &);

        //给定一个密钥key和密钥长度length, 用主密钥将密钥解密并返回
        //解密结果为unsigned char *,大小与解密前的长度相同，
        //解密后的密钥的空间由该函数产生，需要调用者管理
        //如果解密失败，返回NULL
        KeyValueType KeyDecryption(
                KeyValueType &);

        MasterKey GenerateMasterKeyWithKEK();

        void ImportMasterKey(MasterKey master_key_encrypted);

    private:

        //SJK1238设备句柄，用法见SJK1238 API文档
        void *p_dev_handle_ = nullptr;

        void *p_ses_handle_ = nullptr;

        //定义加密算法，可以选择SM1或者SM4两种对称算法
        unsigned int ui_alg_id_ = SGD_SMS4_CBC; //加密算法

        void *master_key_handle_ = nullptr;

    };
}

#endif //KEYMANAGEMENT_SJK1238_H
