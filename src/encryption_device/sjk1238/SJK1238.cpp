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
/*
#include "server_authen.cpp"
*/

namespace encryption_device {

    SJK1238::~SJK1238() {
        if (p_ses_handle_) {
            auto status = SDF_CloseSession(p_ses_handle_);  //关闭会话句柄
            if (status) {
                std::stringstream ss;
                ss << "Hardware:: Device closed failed";
                ss << "Error code: 0x" << std::hex << status;
                _exit(status);
            }
            DLOG(INFO) << "Hardware: Close the session";
        }
        if (p_dev_handle_) {
            auto status = SDF_CloseDevice(p_dev_handle_);  //关闭设备句柄
            if (status) {
                std::stringstream ss;
                ss << "Hardware:: Device closed failed";
                ss << "Error code 0x" << std::hex << status;
                _exit(status);
            }
            LOG(INFO) << "Hardware:: Device closed successfully";
        }
    }

    void SJK1238::OpenDevice() {
        auto status = SDF_OpenDevice(&p_dev_handle_);  //打开设备句柄
        if (status) {
            std::stringstream ss;
            ss << "Hardware:: Device opened failed";
            ss << "Error code: 0x" << std::hex << status;
            throw std::runtime_error(ss.str());
        }
        LOG(INFO) << "Hardware:: Device opened successfully";

        status = SDF_OpenSession(p_dev_handle_,
                                 &p_ses_handle_);  //打开会话句柄
        if (status) {
            std::stringstream ss;
            ss << "Hardware:: Can't open a session";
            ss << "Error code: 0x" << std::hex << status;
            throw std::runtime_error(ss.str());
        }
        DLOG(INFO) << "Hardware:: Open a session";
    }

// SJK1238 API
// SDF_GenerateRandom()
    KeyValueType SJK1238::GenerateKey() {
        std::size_t length = Key::kKeyValueLen;
        unsigned char *key_unc = new unsigned char[length];
        std::fill_n(key_unc, length, 0);
        DLOG(INFO) << "Hardware: Open a session";
        // 随机产生指定长度的随机数, 置给key
        auto status = SDF_GenerateRandom(
                p_ses_handle_,
                (unsigned int) length,
                key_unc
        );
        if (status) {
            std::stringstream ss;
            ss << "Hardware:: Can't generate a random key. ";
            ss << "Error code: 0x" << std::hex << status;
            throw std::runtime_error(ss.str());
        }
        DLOG(INFO) << "Hardware:: Generate a random key";
        KeyValueType key;
        for (std::size_t i = 0; i < length; i++) {
            key[i] = key_unc[i];
        }
        delete key_unc;
        return key;
    }

// SJK1238 API
// SDF_Encrypt()
    KeyValueEncType SJK1238::KeyEncryption(
            KeyValueType &key) {
        unsigned int length = Key::kKeyValueLen;
        unsigned int length_out;
        unsigned char puc_iv[16] = {0};
        //密钥的unsigned char* 形式
        unsigned char *key_unc = new unsigned char[rsa_len_];
        unsigned char *key_unc_encrypted = new unsigned char[rsa_len_];
        std::fill_n(key_unc, rsa_len_, 1);
        std::fill_n(key_unc_encrypted, rsa_len_, 0);
        // copy && 取模变换
        for (std::size_t i = 0; i < length; i++) {
            key_unc[i] = key[i];
            key_unc[i + length] = (unsigned char) (key_unc[i] / 128);
            key_unc[i] = (unsigned char) (key_unc[i] % 128);
        }

        auto status = SDF_InternalPublicKeyOperation_RSA(
                p_ses_handle_,
                1,
                SGD_RSA_ENC,
                key_unc,
                rsa_len_,
                key_unc_encrypted,
                &length_out
        );

        if (status) {
            std::stringstream ss;
            ss << "Hardware:: Can't encrypt the key. ";
            ss << "Error code: 0x" << std::hex << status;
            throw std::runtime_error(ss.str());
        }
        DLOG(INFO) << "Hardware:: Encrypt the key using the master key";
        KeyValueEncType key_encrypted;
        for (std::size_t i = 0; i < rsa_len_; i++) {
            key_encrypted[i] = key_unc_encrypted[i];
        }
        delete key_unc;
        delete key_unc_encrypted;
        return key_encrypted;
    }

    KeyValueType SJK1238::KeyDecryption(
            KeyValueEncType &key_encrypted) {
        unsigned int length = Key::kKeyValueLen;
        unsigned int length_out;
        //密钥的unsigned char* 形式
        unsigned char *key_unc_encrypted = new unsigned char[rsa_len_];
        unsigned char *key_unc = new unsigned char[rsa_len_];
        std::fill_n(key_unc, rsa_len_, 0);
        std::fill_n(key_unc_encrypted, rsa_len_, 0);
        for (std::size_t i = 0; i < rsa_len_; i++) {
            key_unc_encrypted[i] = key_encrypted[i];
        }
        DLOG(INFO) << "Hardware: Open a session";

        auto status = SDF_InternalPrivateKeyOperation_RSA(
                p_ses_handle_,
                1,
                SGD_RSA_ENC,
                key_unc_encrypted,
                rsa_len_,
                key_unc,
                &length_out
        );

        if (status) {
            std::stringstream ss;
            ss << "Hardware:: Can't decrypt the key. ";
            ss << "Error code: 0x" << std::hex << status;
            throw std::runtime_error(ss.str());
        }
        DLOG(INFO) << "Hardware:: Decrypt the key using the master key";
        KeyValueType key;
        // copy && 取模变换
        for (std::size_t i = 0; i < length; i++) {
            key[i] = key_unc[i];
            key[i] = (unsigned char) (key_unc[i + length] * 128 + key[i]);
        }
        delete key_unc;
        delete key_unc_encrypted;
        return key;
    }

    bool
    SJK1238::VerifySignedData(std::vector<unsigned char> &cert,
                              std::vector<unsigned char> &data,
                              std::vector<unsigned char> &signed_data) {

        // 证书的指针形式
        auto cert_point = new unsigned char[cert.size()];
        for (std::size_t i = 0; i < cert.size(); i++) {
            cert_point[i] = cert[i];
        }
        // 数据的指针形式
        // debug用的测试数据
        unsigned char data_debug[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
        auto data_point = new unsigned char[data.size()];
        for (std::size_t i = 0; i < data.size(); i++) {
            data_point[i] = data[i];
        }
        // 签名数据的skf标准格式
        ECCSIGNATUREBLOB signed_data_skf;
        for (std::size_t i = 0; i < signed_data.size(); i++) {
            ((unsigned char *) &signed_data_skf)[i] =
                    signed_data[i];
        }
        // 证书的公钥 skf标准格式
        ECCPUBLICKEYBLOB public_key_skf;
        //parce_cer(cert_point, (int) cert.size(),
        //          (unsigned char *) &public_key_skf);

        // 将签名数据的skf标准格式转换为sdf标准格式
        ECCSignature signed_data_sdf;
        for (std::size_t i = 0; i < 32; i++) {
            signed_data_sdf.r[i] = signed_data_skf.r[i + 32];
        }
        for (std::size_t i = 0; i < 32; i++) {
            signed_data_sdf.s[i] = signed_data_skf.s[i + 32];
        }
        // 将证书公钥的skf标准格式转换为sdf标准格式
        ECCrefPublicKey_st public_key_sdf;
        for (std::size_t i = 0; i < 32; i++) {
            public_key_sdf.x[i] = public_key_skf.x[i + 32];
        }
        for (std::size_t i = 0; i < 32; i++) {
            public_key_sdf.y[i] = public_key_skf.y[i + 32];
        }
        public_key_sdf.bits = public_key_skf.bit_len;
        int result = SDF_ExternalVerify_ECC(p_ses_handle_,
                                            SGD_SM2_1,
                                            &public_key_sdf,
                                            data_debug,
                                            data.size(),
                                            &signed_data_sdf);
        delete cert_point;
        delete data_point;
        return true;
    }
}

