//
// Created by root on 16-8-9.
//

#ifndef KEYMANAGEMENT_SJK1238_H
#define KEYMANAGEMENT_SJK1238_H

#include "HardwareFactoryInterface.h"
#include <swsds.h>

class SJK1238 : public HardwareProductInterface {
public:

    explicit SJK1238() = default;

    ~SJK1238();

    bool startDevice();

    unsigned char *generateKey(unsigned int length);

private:
    void *pDevHandle = nullptr;//设备句柄
};

#endif //KEYMANAGEMENT_SJK1238_H
