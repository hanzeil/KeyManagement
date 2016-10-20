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
        delete master_key_pri_;
        delete master_key_pub_;
        BOOST_LOG_TRIVIAL(info) << "Hardware: Close the device";
    }

    void Simulation::OpenDevice() {
        auto pub_file = fopen("/home/hanzeil/coding/ClionProjects/KeyManagement/MasterKey.pub", "r");
        auto pri_file = fopen("/home/hanzeil/coding/ClionProjects/KeyManagement/MasterKey", "r");
        master_key_pub_ = PEM_read_RSA_PUBKEY(pub_file, nullptr, nullptr, nullptr);
        master_key_pri_ = PEM_read_RSAPrivateKey(pri_file, nullptr, nullptr, nullptr);
        BOOST_LOG_TRIVIAL(info) << "Hardware: Read the master key";
        if (master_key_pub_ != nullptr && master_key_pri_ != nullptr) {
            device_status = true;
            BOOST_LOG_TRIVIAL(info) << "Hardware: Open device";
        } else {
            device_status = false;
            BOOST_LOG_TRIVIAL(error) << "Hardware: Open device failed";
        }
        delete pub_file;
        delete pri_file;

    }

// random 标准库
// random_device 真随机数
    KeyValueType Simulation::GenerateKey() {
        std::size_t length = Key::kKeyValueLen;
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
    KeyValueEncType Simulation::KeyEncryption(KeyValueType &key) {
        std::size_t length = Key::kKeyValueLen;
        unsigned char *key_unc = new unsigned char[rsa_len_];
        unsigned char *key_unc_encrypted = new unsigned char[rsa_len_];
        // copy && 取模变换
        for (std::size_t i = 0; i < length; i++) {
            key_unc[i] = key[i];
            key_unc[i + length] = (unsigned char) (key_unc[i] / 128);
            key_unc[i] = (unsigned char) (key_unc[i] % 128);
        }
        if (!device_status) {
            std::stringstream ss;
            ss << "Hardware: Device is not opened yet";
            throw std::runtime_error(ss.str());
        }

        auto status = RSA_public_encrypt(rsa_len_, key_unc, key_unc_encrypted, master_key_pub_, RSA_NO_PADDING);

        BOOST_LOG_TRIVIAL(info) << "Hardware: Encrypt the key using the master key";
        KeyValueEncType key_encrypted;
        for (std::size_t i = 0; i < rsa_len_; i++) {
            key_encrypted[i] = key_unc_encrypted[i];
        }
        delete key_unc;
        delete key_unc_encrypted;
        return key_encrypted;
    }

// Openssl crypto API
// AES_set_decrypt_key
// AES_cbc_encrypt
    KeyValueType Simulation::KeyDecryption(KeyValueEncType &key_encrypted) {
        std::size_t length = Key::kKeyValueLen;
        unsigned char puc_iv[AES_BLOCK_SIZE] = {0};
        unsigned char *key_unc = new unsigned char[rsa_len_];
        unsigned char *key_unc_encrypted = new unsigned char[rsa_len_];
        for (std::size_t i = 0; i < rsa_len_; i++) {
            key_unc_encrypted[i] = key_encrypted[i];
        }
        if (!device_status) {
            std::stringstream ss;
            ss << "Hardware: Device is not opened yet.";
            throw std::runtime_error(ss.str());
        }

        auto len = RSA_size(master_key_pri_);
        auto status = RSA_private_decrypt(rsa_len_, key_unc_encrypted, key_unc, master_key_pri_, RSA_NO_PADDING);
        BOOST_LOG_TRIVIAL(info) << "Hardware: Decrypt the key using the master key";
        KeyValueType key;
        // copy && 取模变换
        for (std::size_t i = 0; i < length; i++) {
            key[i] = key_unc[i];
            key[i] = (unsigned char) (key_unc[i + length] * 128 + key[i]);
        }
        delete key_unc_encrypted;
        delete key_unc;
        return key;
    }

    void Simulation::HandleErrors() {
        unsigned long errCode = ERR_get_error();
        if (errCode) {
            std::stringstream ss;
            char *err = ERR_error_string(errCode, NULL);
            ss << "Hardware: " << err;
            throw std::runtime_error(ss.str());
        }
    }
}
