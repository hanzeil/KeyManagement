//
// Created by Hanzeil on 16-8-15.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//
// The description of this file is in the header file.
//

#include "SJK1238.h"

namespace encryption_device {

    SJK1238::~SJK1238() {
        if (p_ses_handle_) {
            auto status = SDF_CloseSession(p_ses_handle_);  //关闭会话句柄
            if (status) {
                std::stringstream ss;
                ss << "Hardware: Can't close the session. ";
                ss << "Error code: 0x" << std::hex << status;
                _exit(status);
            }
            BOOST_LOG_TRIVIAL(debug) << "Hardware: Close the session";
        }
        if (p_dev_handle_) {
            auto status = SDF_CloseDevice(p_dev_handle_);  //关闭设备句柄
            if (status) {
                std::stringstream ss;
                ss << "Hardware: Can't close the device. ";
                ss << "Error code 0x" << std::hex << status;
                _exit(status);
            }
            BOOST_LOG_TRIVIAL(info) << "Hardware: Close the device";
        }
    }

    void SJK1238::OpenDevice() {
        auto status = SDF_OpenDevice(&p_dev_handle_);  //打开设备句柄
        if (status) {
            std::stringstream ss;
            ss << "Hardware: Can't open the device. ";
            ss << "Error code: 0x" << std::hex << status;
            throw std::runtime_error(ss.str());
        }
        BOOST_LOG_TRIVIAL(info) << "Hardware: Open device";

        status = SDF_OpenSession(p_dev_handle_,
                                 &p_ses_handle_);  //打开会话句柄
        if (status) {
            std::stringstream ss;
            ss << "Hardware: Can't open a session. ";
            ss << "Error code: 0x" << std::hex << status;
            throw std::runtime_error(ss.str());
        }
        BOOST_LOG_TRIVIAL(info) << "Hardware: Open a session";
    }

// SJK1238 API
// SDF_GenerateRandom()
    KeyValueType SJK1238::GenerateKey() {
        std::size_t length = Key::kKeyValueLen;
        unsigned char *key_unc = new unsigned char[length];
        std::fill_n(key_unc, length, 0);
        BOOST_LOG_TRIVIAL(debug) << "Hardware: Open a session";
        // 随机产生指定长度的随机数, 置给key
        auto status = SDF_GenerateRandom(
                p_ses_handle_,
                (unsigned int) length,
                key_unc
        );
        if (status) {
            std::stringstream ss;
            ss << "Hardware: Can't generate a random key. ";
            ss << "Error code: 0x" << std::hex << status;
            throw std::runtime_error(ss.str());
        }
        BOOST_LOG_TRIVIAL(info) << "Hardware: Generate a random key";
        KeyValueType key;
        for (std::size_t i = 0; i < length; i++) {
            key[i] = key_unc[i];
        }
        delete key_unc;
        return key;
    }

// SJK1238 API
// SDF_Encrypt()
    KeyValueType SJK1238::KeyEncryption(
            KeyValueType &key) {
        unsigned int length = Key::kKeyValueLen;
        unsigned char puc_iv[16] = {0};
        //密钥的unsigned char* 形式
        unsigned char *key_unc = new unsigned char[length];
        for (std::size_t i = 0; i < length; i++) {
            key_unc[i] = key[i];
        }
        unsigned char *key_unc_encrypted = new unsigned char[length];
        std::fill_n(key_unc_encrypted, length, 0);
        auto status = SDF_Encrypt(
                p_ses_handle_,
                master_key_handle_,
                ui_alg_id_,
                puc_iv,
                key_unc,
                length,
                key_unc_encrypted,
                &length);  //调用对称加密接口进行加密运算
        if (status) {
            std::stringstream ss;
            ss << "Hardware: Can't encrypt the key. ";
            ss << "Error code: 0x" << std::hex << status;
            throw std::runtime_error(ss.str());
        }
        BOOST_LOG_TRIVIAL(info) << "Hardware: Encrypt the key using the master key";
        KeyValueType key_encrypted;
        for (std::size_t i = 0; i < length; i++) {
            key_encrypted[i] = key_unc_encrypted[i];
        }
        delete key_unc_encrypted;
        delete key_unc;
        return key_encrypted;
    }

    KeyValueType SJK1238::KeyDecryption(
            KeyValueType &key_encrypted) {
        unsigned int length = Key::kKeyValueLen;
        unsigned char puc_iv[16] = {0};
        //密钥的unsigned char* 形式
        unsigned char *key_unc_encrypted = new unsigned char[length];
        for (std::size_t i = 0; i < length; i++) {
            key_unc_encrypted[i] = key_encrypted[i];
        }
        unsigned char *key_unc = new unsigned char[length];
        std::fill_n(key_unc, length, 0);
        BOOST_LOG_TRIVIAL(debug) << "Hardware: Open a session";
        auto status = SDF_Decrypt(
                p_ses_handle_,
                master_key_handle_,
                ui_alg_id_,
                puc_iv,
                key_unc_encrypted,
                length,
                key_unc,
                &length
        );  //调用对称加密接口进行解密运算
        if (status) {
            std::stringstream ss;
            ss << "Hardware: Can't decrypt the key. ";
            ss << "Error code: 0x" << std::hex << status;
            throw std::runtime_error(ss.str());
        }
        BOOST_LOG_TRIVIAL(info) << "Hardware: Decrypt the key using the master key";
        KeyValueType key;
        for (std::size_t i = 0; i < length; i++) {
            key[i] = key_unc[i];
        }
        delete key_unc;
        delete key_unc_encrypted;
        return key;
    }

    MasterKey SJK1238::GenerateMasterKeyWithKEK() {
        unsigned int length = Key::kKeyValueLen;
        unsigned char *master_key_unc = new unsigned char[length];
        std::fill_n(master_key_unc, length, 0);
        void *p_key_handle = nullptr;
        auto status = SDF_GenerateKeyWithKEK
                (
                        p_ses_handle_,
                        length,
                        SGD_SMS4_ECB,
                        1,
                        master_key_unc,
                        &length,
                        &p_key_handle
                );
        if (status) {
            std::stringstream ss;
            ss << "Hardware: Can't generate the master key. ";
            ss << "Error code: 0x" << std::hex << status;
            throw std::runtime_error(ss.str());
        }
        MasterKey master_key;
        for (std::size_t i = 0; i < length; i++) {
            master_key[i] = master_key_unc[i];
        }
        delete master_key_unc;
        return master_key;
    }

    void SJK1238::ImportMasterKey(MasterKey master_key_encrypted) {
        unsigned int length = Key::kKeyValueLen;
        unsigned char *master_key_encrypted_unc = new unsigned char[length];
        for (std::size_t i = 0; i < length; i++) {
            master_key_encrypted_unc[i] = master_key_encrypted[i];
        }
        auto status = SDF_ImportKeyWithKEK(
                p_ses_handle_,
                SGD_SMS4_ECB,
                1,
                master_key_encrypted_unc,
                length,
                &master_key_handle_
        );
        master_key_handle_ = (void *) (
                (char *) master_key_handle_ + 1
        );
        if (status) {
            std::stringstream ss;
            ss << "Hardware: Can't import the master key. ";
            ss << "Error code: 0x" << std::hex << status;
            throw std::runtime_error(ss.str());
        }
        BOOST_LOG_TRIVIAL(info) << "Hardware: Import the master key";
        delete master_key_encrypted_unc;
    }
}

