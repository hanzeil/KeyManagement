//
// Created by Hanzeil on 16-8-15.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//
// The description of this file is in the header file.
//

#include "Simulation.h"

Simulation::~Simulation() {
    device_status = false;
    BOOST_LOG_TRIVIAL(info) << "Hardware: Close the device";
}

bool Simulation::OpenDevice() {
    device_status = true;
    BOOST_LOG_TRIVIAL(info) << "Hardware: Open device";
    return true;
}

// random 标准库
// random_device 真随机数
unsigned char *Simulation::GenerateKey(unsigned int length) {
    if (!device_status) {
        BOOST_LOG_TRIVIAL(error) << "Hardware: Device is not opened yet";
        return NULL;
    }
    unsigned char *key = new unsigned char[length];
    std::random_device rd;
    for (auto i = 0; i < length; i++) {
        key[i] = (unsigned char) rd();
    }
    BOOST_LOG_TRIVIAL(info) << "Hardware: Generate a random key";
    return key;
}

// Openssl crypto API
// AES_set_encrypt_key
// AES_cbc_encrypt
unsigned char *Simulation::KeyEncryption(unsigned char *key, size_t length) {
    unsigned char puc_iv[AES_BLOCK_SIZE] = {0};
    auto master_key = GetMasterKey(); //获取主密钥
    AES_KEY aes_encrpyt_key;
    if (!device_status) {
        BOOST_LOG_TRIVIAL(error) << "Hardware: Device is not opened yet";
        return NULL;
    }
    auto status = AES_set_encrypt_key(master_key, 128,
                                      &aes_encrpyt_key);  //得到加密主密钥
    if (status) {
        HandleErrors();
        return NULL;
    }
    unsigned char *key_encrypted = new unsigned char[length];
    AES_cbc_encrypt(key, key_encrypted, length, &aes_encrpyt_key,
                    puc_iv, AES_ENCRYPT);  //用主密钥加密
    BOOST_LOG_TRIVIAL(info) << "Hardware: Encrypt the key using the master key";
    delete master_key;
    return key_encrypted;
}

// Openssl crypto API
// AES_set_decrypt_key
// AES_cbc_encrypt
unsigned char *Simulation::KeyDecryption(unsigned char *key_encrypted, size_t length) {
    unsigned char puc_iv[AES_BLOCK_SIZE] = {0};
    auto master_key = GetMasterKey(); //获取主密钥
    AES_KEY aes_decrpyt_key;
    if (!device_status) {
        BOOST_LOG_TRIVIAL(error) << "Hardware: Device is not opened yet";
        return NULL;
    }
    auto status = AES_set_decrypt_key(master_key, 128,
                                      &aes_decrpyt_key);  //得到加密主密钥
    if (status) {
        HandleErrors();
        return NULL;
    }
    unsigned char *key = new unsigned char[length];
    AES_cbc_encrypt(key_encrypted, key, length, &aes_decrpyt_key,
                    puc_iv, AES_DECRYPT);  //用主密钥解密
    BOOST_LOG_TRIVIAL(info) << "Hardware: Decrypt the key using the master key";
    delete master_key;
    return key;

}

unsigned char *Simulation::GetMasterKey() {
    unsigned char *master_key = new unsigned char[16];
    std::fill_n(master_key, 16, 0);
    std::ifstream f(
            "/root/ClionProjects/KeyManagement/MasterKey", std::ifstream::binary);
    f.read((char *) master_key, 16);
    f.close();
    BOOST_LOG_TRIVIAL(info) << "Hardware: Read the master key";
    return master_key;
}

void Simulation::HandleErrors() {
    unsigned long errCode = ERR_get_error();
    while (errCode) {
        char *err = ERR_error_string(errCode, NULL);
        BOOST_LOG_TRIVIAL(error) << "Hardware: " << err;
        errCode = ERR_get_error();
    }
}
