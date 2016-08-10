//
// Created by root on 16-8-9.
//

#include "SJK1238.h"

SJK1238::~SJK1238() {
}

bool SJK1238::startDevice() {
    auto iRet = SDF_OpenDevice(&pDevHandle);//打开设备句柄
    if (iRet)
        return false;
}

unsigned char *SJK1238::generateKey(unsigned int length) {
    unsigned char *key = new unsigned char[length];
}

