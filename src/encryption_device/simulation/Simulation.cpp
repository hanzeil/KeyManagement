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

namespace encryption_device {

    Simulation::~Simulation() {
        device_status = false;
        BOOST_LOG_TRIVIAL(info) << "Hardware: Close the device";
    }

    void Simulation::OpenDevice() {
        device_status = true;
        BOOST_LOG_TRIVIAL(info) << "Hardware: Open device";
        return true;
    }

// random 标准库
// random_device 真随机数
    KeyValueType Simulation::GenerateKey(std::size_t length) {
        if (!device_status) {
            std::stringstream ss;
            ss << "Hardware: Device is not opened yet";
            throw std::runtime_error(ss.str());
        }
        unsigned char *key_unc = new unsigned char[length];
        std::random_device rd;
        for (auto i = 0; i < length; i++) {
            key_unc[i] = (unsigned char) rd();
        }
        BOOST_LOG_TRIVIAL(info) << "Hardware: Generate a random key";
        KeyValueType key;
        for (std::size_t i = 0; i < Key::kKeyValueLen; i++) {
            key[i] = key_unc[i];
        }
        delete key_unc;
        return key;
    }

// Openssl crypto API
// AES_set_encrypt_key
// AES_cbc_encrypt
    KeyValueType Simulation::KeyEncryption(KeyValueType &key) {
        std::size_t length = Key::kKeyValueLen;
        unsigned char puc_iv[AES_BLOCK_SIZE] = {0};
        unsigned char *key_unc = new unsigned char[length];
        for (std::size_t i = 0; i < Key::kKeyIdLen; i++) {
            key_unc[i] = key[i];
        }
        auto master_key = GetMasterKey(); //获取主密钥
        AES_KEY aes_encrpyt_key;
        if (!device_status) {
            std::stringstream ss;
            ss << "Hardware: Device is not opened yet";
            throw std::runtime_error(ss.str());
        }
        auto status = AES_set_encrypt_key(master_key, 128,
                                          &aes_encrpyt_key);  //得到加密主密钥
        if (status) {
            HandleErrors();
        }
        unsigned char *key_unc_encrypted = new unsigned char[length];
        AES_cbc_encrypt(key_unc, key_unc_encrypted, length, &aes_encrpyt_key,
                        puc_iv, AES_ENCRYPT);  //用主密钥加密
        BOOST_LOG_TRIVIAL(info) << "Hardware: Encrypt the key using the master key";
        KeyValueType key_encrypted;
        for (std::size_t i = 0; i < Key::kKeyValueLen; i++) {
            key_encrypted[i] = key_unc_encrypted[i];
        }
        delete key_unc;
        delete master_key;
        delete key_unc_encrypted;
        return key_encrypted;
    }

// Openssl crypto API
// AES_set_decrypt_key
// AES_cbc_encrypt
    KeyValueType Simulation::KeyDecryption(KeyValueType &key_encrypted) {
        std::size_t length = Key::kKeyValueLen;
        unsigned char puc_iv[AES_BLOCK_SIZE] = {0};
        auto master_key = GetMasterKey(); //获取主密钥
        unsigned char *key_unc_encrypted = new unsigned char[length];
        for (std::size_t i = 0; i < Key::kKeyIdLen; i++) {
            key_unc_encrypted[i] = key_encrypted[i];
        }
        AES_KEY aes_decrpyt_key;
        if (!device_status) {
            std::stringstream ss;
            ss << "Hardware: Device is not opened yet.";
            throw std::runtime_error(ss.str());
        }
        auto status = AES_set_decrypt_key(master_key, 128,
                                          &aes_decrpyt_key);  //得到加密主密钥
        if (status) {
            HandleErrors();
        }
        unsigned char *key_unc = new unsigned char[length];
        AES_cbc_encrypt(key_unc_encrypted, key_unc, length, &aes_decrpyt_key,
                        puc_iv, AES_DECRYPT);  //用主密钥解密
        BOOST_LOG_TRIVIAL(info) << "Hardware: Decrypt the key using the master key";
        KeyValueType key;
        for (std::size_t i = 0; i < Key::kKeyValueLen; i++) {
            key[i] = key_unc[i];
        }
        delete master_key;
        delete key_unc_encrypted;
        delete key_unc;
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
            std::stringstream ss;
            char *err = ERR_error_string(errCode, NULL);
            errCode = ERR_get_error();
            ss << "Hardware: " << err;
            throw std::runtime_error(ss.str());
        }
    }
}
