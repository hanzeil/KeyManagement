//
// Created by Hanzeil on 16-8-15.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//

#ifndef KEYMANAGEMENT_HARDWAREPRODUCTINTERFACE_H
#define KEYMANAGEMENT_HARDWAREPRODUCTINTERFACE_H

class HardwareProductInterface {
public:
    explicit HardwareProductInterface() = default;

    virtual ~HardwareProductInterface();

    virtual bool OpenDevice()=0;

    virtual unsigned char *GenerateKey(unsigned int length)=0;

    virtual unsigned char *KeyEncryption(unsigned char *key, unsigned int length)=0;

    virtual unsigned char *KeyDecryption(unsigned char *key_encrypted, unsigned int length)=0;

};


#endif //KEYMANAGEMENT_HARDWAREPRODUCTINTERFACE_H
