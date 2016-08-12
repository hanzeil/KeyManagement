//
// Created by root on 16-8-9.
//

#ifndef KEYMANAGEMENT_SJK1238_H
#define KEYMANAGEMENT_SJK1238_H

#include "HardwareFactoryInterface.h"
#include <swsds.h>
#include <boost/log/trivial.hpp>
#include <algorithm>
#include <fstream>
#include <iostream>

class SJK1238 : public HardwareProductInterface {
public:

    explicit SJK1238() = default;

    ~SJK1238();

    bool openDevice();

    unsigned char *generateKey(unsigned int length);

    unsigned char *keyEncryption(unsigned char *key, unsigned int length);

    unsigned char *keyDecryption(unsigned char *key_encrypted, unsigned int length);

private:
    unsigned char *getMasterKey();

    void *p_dev_handle_ = nullptr;//设备句柄
    unsigned int ui_alg_id_ = SGD_SMS4_ECB; //加密算法
};

#endif //KEYMANAGEMENT_SJK1238_H
