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
#include "../HardwareProductInterface.h"
#include <boost/log/trivial.hpp>
#include <random>
#include <openssl/aes.h>
#include <openssl/err.h>


class Simulation : public HardwareProductInterface {
public:
    explicit Simulation() = default;

    ~Simulation();

    bool OpenDevice();

    unsigned char *GenerateKey(unsigned int length);

    unsigned char *KeyEncryption(unsigned char *key, unsigned int length);

    unsigned char *KeyDecryption(unsigned char *key_encrypted, unsigned int length);

private:
    unsigned char *GetMasterKey();

    void HandleErrors();

    bool device_status = false;
};


#endif //KEYMANAGEMENT_SIMULATION_H
