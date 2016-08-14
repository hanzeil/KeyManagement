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

    bool openDevice();

    unsigned char *generateKey(unsigned int length);

    unsigned char *keyEncryption(unsigned char *key, unsigned int length);

    unsigned char *keyDecryption(unsigned char *key_encrypted, unsigned int length);

private:
    unsigned char *getMasterKey();

    void handleErrors();

    bool device_status = false;
};


#endif //KEYMANAGEMENT_SIMULATION_H
