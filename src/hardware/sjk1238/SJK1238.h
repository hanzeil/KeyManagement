//
// Created by Hanzeil on 16-8-15.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//
// The description of this file is in the header file.

#ifndef KEYMANAGEMENT_SJK1238_H
#define KEYMANAGEMENT_SJK1238_H

#include "../HardwareFactoryInterface.h"
#include <swsds.h>
#include <boost/log/trivial.hpp>
#include <algorithm>
#include <fstream>
#include <iostream>

class SJK1238 : public HardwareProductInterface {
public:

    explicit SJK1238() = default;

    ~SJK1238();

    bool OpenDevice();

    unsigned char *GenerateKey(unsigned int length);

    unsigned char *KeyEncryption(unsigned char *key, unsigned int length);

    unsigned char *KeyDecryption(unsigned char *key_encrypted, unsigned int length);

private:
    unsigned char *GetMasterKey();

    void *p_dev_handle_ = nullptr;//设备句柄
    unsigned int ui_alg_id_ = SGD_SMS4_CBC; //加密算法
};

#endif //KEYMANAGEMENT_SJK1238_H
