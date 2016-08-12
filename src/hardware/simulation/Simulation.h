//
// Created by hanzeil on 16-8-12.
//

#ifndef KEYMANAGEMENT_SIMULATION_H
#define KEYMANAGEMENT_SIMULATION_H

#include "../HardwareProductInterface.h"


class Simulation : public HardwareProductInterface {
public:
    explicit Simulation() = default;

    ~Simulation();

    bool openDevice();

    unsigned char *generateKey(unsigned int length);

    unsigned char *keyEncryption(unsigned char *key, unsigned int length);

    unsigned char *keyDecryption(unsigned char *keyEncrypted, unsigned int length);

private:
    unsigned char *getMasterKey();
};


#endif //KEYMANAGEMENT_SIMULATION_H
