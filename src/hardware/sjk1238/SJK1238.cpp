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

SJK1238::~SJK1238() {
    auto status = SDF_CloseDevice(p_dev_handle_);//关闭设备句柄
    if (status) {
        BOOST_LOG_TRIVIAL(error) << "Error code: 0x" << std::hex << status;
        BOOST_LOG_TRIVIAL(error) << "Hardware: Can't close the device";
    }
    BOOST_LOG_TRIVIAL(info) << "Hardware: Close the device";
}

bool SJK1238::OpenDevice() {
    auto status = SDF_OpenDevice(&p_dev_handle_);//打开设备句柄
    if (status) {
        BOOST_LOG_TRIVIAL(error) << "Error code: 0x" << std::hex << status;
        BOOST_LOG_TRIVIAL(error) << "Hardware: Can't open the device";
        return false;
    }
    BOOST_LOG_TRIVIAL(info) << "Hardware: Open device";
    return true;
}

unsigned char *SJK1238::GenerateKey(unsigned int length) {
    void *p_ses_handle = NULL;//会话句柄
    unsigned char *key = new unsigned char[length];
    std::fill_n(key, length, 0);
    auto status = SDF_OpenSession(p_dev_handle_, &p_ses_handle);//打开会话句柄
    if (status) {
        BOOST_LOG_TRIVIAL(error) << "Error code: 0x" << std::hex << status;
        BOOST_LOG_TRIVIAL(error) << "Hardware: Can't open a session.";
        return NULL;
    }
    BOOST_LOG_TRIVIAL(debug) << "Hardware: Open a session";
    status = SDF_GenerateRandom(p_ses_handle, length, key);
    if (status) {
        BOOST_LOG_TRIVIAL(error) << "Error code: 0x" << std::hex << status;
        BOOST_LOG_TRIVIAL(error) << "Hardware: Can't generate a random key";
        return NULL;
    }
    BOOST_LOG_TRIVIAL(info) << "Hardware: Generate a random key";
    status = SDF_CloseSession(p_ses_handle);//关闭会话句柄
    if (status) {
        BOOST_LOG_TRIVIAL(error) << "Error code: 0x" << std::hex << status;
        BOOST_LOG_TRIVIAL(error) << "Hardware: Can't close the session";
        return NULL;
    }
    BOOST_LOG_TRIVIAL(debug) << "Hardware: Close the session";
    return key;
}

unsigned char *SJK1238::KeyEncryption(unsigned char *key, unsigned int length) {
    void *p_ses_handle = NULL;//会话句柄
    void *p_key_handle = NULL; //主密钥句柄
    unsigned char puc_iv[16] = {0};
    unsigned char *master_key = GetMasterKey();
    unsigned char *key_encrypted = new unsigned char[length];
    std::fill_n(key_encrypted, length, 0);
    if (key == NULL) {
        return NULL;
    }
    auto status = SDF_OpenSession(p_dev_handle_, &p_ses_handle);//打开会话句柄
    if (status) {
        BOOST_LOG_TRIVIAL(error) << "Error code: 0x" << std::hex << status;
        BOOST_LOG_TRIVIAL(error) << "Hardware: Can't open a session.";
        return NULL;
    }
    BOOST_LOG_TRIVIAL(debug) << "Hardware: Open a session";
    /*读取主密钥*/
    if (master_key == NULL) {
        return NULL;
    }
    status = SDF_ImportKey(p_ses_handle, master_key, 16, &p_key_handle);//导入明文会话密钥
    if (status) {
        BOOST_LOG_TRIVIAL(error) << "Error code: 0x" << std::hex << status;
        BOOST_LOG_TRIVIAL(error) << "Hardware: Can't import the master key.";
        return NULL;
    }
    BOOST_LOG_TRIVIAL(info) << "Hardware: Import the master key";
    status = SDF_Encrypt(p_ses_handle, p_key_handle, ui_alg_id_,
                         puc_iv, key, length, key_encrypted, &length);//调用对称加密接口进行加密运算
    if (status) {
        BOOST_LOG_TRIVIAL(error) << "Error code: 0x" << std::hex << status;
        BOOST_LOG_TRIVIAL(error) << "Hardware: Can't encrypt the key";
        return NULL;
    }
    BOOST_LOG_TRIVIAL(info) << "Hardware: Encrypt the key using the master key";
    status = SDF_DestroyKey(p_ses_handle, p_key_handle);//销毁会话密钥
    if (status) {
        BOOST_LOG_TRIVIAL(error) << "Error code: 0x" << std::hex << status;
        BOOST_LOG_TRIVIAL(error) << "Hardware: Can't destroy the key handle";
        return NULL;
    }
    status = SDF_CloseSession(p_ses_handle);//关闭会话句柄
    if (status) {
        BOOST_LOG_TRIVIAL(error) << "Error code: 0x" << std::hex << status;
        BOOST_LOG_TRIVIAL(error) << "Hardware: Can't close the session";
        return NULL;
    }
    BOOST_LOG_TRIVIAL(debug) << "Hardware: Close the session";
    delete master_key;
    return key_encrypted;
}

unsigned char *SJK1238::KeyDecryption(unsigned char *key_encrypted, unsigned int length) {
    void *p_ses_handle = NULL;//会话句柄
    void *p_key_handle = NULL; //主密钥句柄
    unsigned char puc_iv[16] = {0};
    unsigned char *masterKey = GetMasterKey();
    unsigned char *key = new unsigned char[length];
    std::fill_n(key, length, 0);
    if (masterKey == NULL) {
        return NULL;
    }
    auto status = SDF_OpenSession(p_dev_handle_, &p_ses_handle);//打开会话句柄
    if (status) {
        BOOST_LOG_TRIVIAL(error) << "Error code: 0x" << std::hex << status;
        BOOST_LOG_TRIVIAL(error) << "Hardware: Can't open a session.";
        return NULL;
    }
    BOOST_LOG_TRIVIAL(debug) << "Hardware: Open a session";
    status = SDF_ImportKey(p_ses_handle, masterKey, 16, &p_key_handle);//导入明文会话密钥
    if (status) {
        BOOST_LOG_TRIVIAL(error) << "Error code: 0x" << std::hex << status;
        BOOST_LOG_TRIVIAL(error) << "Hardware: Can't import the master key.";
        return NULL;
    }
    BOOST_LOG_TRIVIAL(info) << "Hardware: Import the master key";
    status = SDF_Decrypt(p_ses_handle, p_key_handle, ui_alg_id_, puc_iv,
                         key_encrypted, length, key, &length);
    if (status) {
        BOOST_LOG_TRIVIAL(error) << "Error code: 0x" << std::hex << status;
        BOOST_LOG_TRIVIAL(error) << "Hardware: Can't decrypt the key";
        return NULL;
    }
    BOOST_LOG_TRIVIAL(info) << "Hardware: Decrypt the key using the master key";
    status = SDF_DestroyKey(p_ses_handle, p_key_handle);//销毁会话密钥
    if (status) {
        BOOST_LOG_TRIVIAL(error) << "Error code: 0x" << std::hex << status;
        BOOST_LOG_TRIVIAL(error) << "Hardware: Can't destroy the key handle";
        return NULL;
    }
    status = SDF_CloseSession(p_ses_handle);//关闭会话句柄
    if (status) {
        BOOST_LOG_TRIVIAL(error) << "Error code: 0x" << std::hex << status;
        BOOST_LOG_TRIVIAL(error) << "Hardware: Can't close the session";
        return NULL;
    }
    BOOST_LOG_TRIVIAL(debug) << "Hardware: Close the session";
    delete masterKey;
    return key;
}

unsigned char *SJK1238::GetMasterKey() {
    unsigned char *master_key = new unsigned char[16];
    std::fill_n(master_key, 16, 0);
    std::ifstream f(
            "/root/ClionProjects/KeyManagement/MasterKey", std::ifstream::binary);
    f.read((char *) master_key, 16);
    f.close();
    BOOST_LOG_TRIVIAL(info) << "Hardware: Read the master key";
    return master_key;
}

