//
// Created by root on 16-8-9.
//

#ifndef KEYMANAGEMENT_HARDWAREPRODUCTINTERFACE_H
#define KEYMANAGEMENT_HARDWAREPRODUCTINTERFACE_H

class HardwareProductInterface {
public:
    explicit HardwareProductInterface() = default;

    virtual ~HardwareProductInterface();

    virtual bool startDevice()=0;

    virtual unsigned char *generateKey(unsigned int length)=0;
};


#endif //KEYMANAGEMENT_HARDWAREPRODUCTINTERFACE_H
