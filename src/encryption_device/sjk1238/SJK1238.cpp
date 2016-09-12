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
    auto status = SDF_CloseDevice(p_dev_handle_);  //关闭设备句柄
    if (status) {
        BOOST_LOG_TRIVIAL(error) << "Error code: 0x" << std::hex << status;
        BOOST_LOG_TRIVIAL(error) << "Hardware: Can't close the device";
    }
    BOOST_LOG_TRIVIAL(info) << "Hardware: Close the device";
}

bool SJK1238::OpenDevice() {
    auto status = SDF_OpenDevice(&p_dev_handle_);  //打开设备句柄
    if (status) {
        BOOST_LOG_TRIVIAL(error) << "Error code: 0x" << std::hex << status;
        BOOST_LOG_TRIVIAL(error) << "Hardware: Can't open the device";
        return false;
    }
    BOOST_LOG_TRIVIAL(info) << "Hardware: Open device";
    return true;
}

// SJK1238 API
// SDF_GenerateRandom()
KeyValueType SJK1238::GenerateKey(std::size_t length) {
    // 每次SJK1238的操作都需要开启一个会话，伴随一个会话句柄
    void *p_ses_handle = NULL;  //会话句柄
    unsigned char *key_unc = new unsigned char[length];
    std::fill_n(key_unc, length, 0);
    auto status = SDF_OpenSession(p_dev_handle_,
                                  &p_ses_handle);  //打开会话句柄
    if (status) {
        BOOST_LOG_TRIVIAL(error) << "Error code: 0x" << std::hex << status;
        BOOST_LOG_TRIVIAL(error) << "Hardware: Can't open a session.";
    }
    BOOST_LOG_TRIVIAL(debug) << "Hardware: Open a session";
    // 随机产生指定长度的随机数, 置给key
    status = SDF_GenerateRandom(p_ses_handle, (unsigned int) length, key_unc);
    if (status) {
        BOOST_LOG_TRIVIAL(error) << "Error code: 0x" << std::hex << status;
        BOOST_LOG_TRIVIAL(error) << "Hardware: Can't generate a random key";
    }
    BOOST_LOG_TRIVIAL(info) << "Hardware: Generate a random key";
    status = SDF_CloseSession(p_ses_handle);  //关闭会话句柄
    if (status) {
        BOOST_LOG_TRIVIAL(error) << "Error code: 0x" << std::hex << status;
        BOOST_LOG_TRIVIAL(error) << "Hardware: Can't close the session";
    }
    BOOST_LOG_TRIVIAL(debug) << "Hardware: Close the session";
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
    void *p_ses_handle = NULL;  //会话句柄
    void *p_key_handle = NULL;  //主密钥句柄
    unsigned char puc_iv[16] = {0};
    unsigned char *master_key = GetMasterKey();  // 读取主密钥
    //密钥的unsigned char* 形式
    unsigned char *key_unc = new unsigned char[length];
    for (std::size_t i = 0; i < length; i++) {
        key_unc[i] = key[i];
    }
    unsigned char *key_unc_encrypted = new unsigned char[length];
    std::fill_n(key_unc_encrypted, length, 0);
    auto status = SDF_OpenSession(p_dev_handle_, &p_ses_handle);//打开会话句柄
    if (status) {
        BOOST_LOG_TRIVIAL(error) << "Error code: 0x" << std::hex << status;
        BOOST_LOG_TRIVIAL(error) << "Hardware: Can't open a session.";
    }
    BOOST_LOG_TRIVIAL(debug) << "Hardware: Open a session";
    if (master_key == NULL) {
    }
    status = SDF_ImportKey(p_ses_handle, master_key, 16,
                           &p_key_handle);  //导入明文主密钥
    if (status) {
        BOOST_LOG_TRIVIAL(error) << "Error code: 0x" << std::hex << status;
        BOOST_LOG_TRIVIAL(error) << "Hardware: Can't import the master key.";
    }
    BOOST_LOG_TRIVIAL(info) << "Hardware: Import the master key";
    status = SDF_Encrypt(p_ses_handle, p_key_handle, ui_alg_id_,
                         puc_iv, key_unc, length, key_unc_encrypted,
                         &length);  //调用对称加密接口进行加密运算
    if (status) {
        BOOST_LOG_TRIVIAL(error) << "Error code: 0x" << std::hex << status;
        BOOST_LOG_TRIVIAL(error) << "Hardware: Can't encrypt the key";
    }
    BOOST_LOG_TRIVIAL(info) << "Hardware: Encrypt the key using the master key";
    status = SDF_DestroyKey(p_ses_handle, p_key_handle);  //销毁会话密钥
    if (status) {
        BOOST_LOG_TRIVIAL(error) << "Error code: 0x" << std::hex << status;
        BOOST_LOG_TRIVIAL(error) << "Hardware: Can't destroy the key handle";
    }
    status = SDF_CloseSession(p_ses_handle);  //关闭会话句柄
    if (status) {
        BOOST_LOG_TRIVIAL(error) << "Error code: 0x" << std::hex << status;
        BOOST_LOG_TRIVIAL(error) << "Hardware: Can't close the session";
    }
    BOOST_LOG_TRIVIAL(debug) << "Hardware: Close the session";
    KeyValueType key_encrypted;
    for (std::size_t i = 0; i < length; i++) {
        key_encrypted[i] = key_unc_encrypted[i];
    }
    delete master_key;
    delete key_unc_encrypted;
    delete key_unc;
    return key_encrypted;
}

KeyValueType SJK1238::KeyDecryption(
        KeyValueType &key_encrypted) {
    unsigned int length = Key::kKeyValueLen;
    void *p_ses_handle = NULL;  //会话句柄
    void *p_key_handle = NULL;  //主密钥句柄
    unsigned char puc_iv[16] = {0};
    unsigned char *masterKey = GetMasterKey();  // 读取主密钥
    //密钥的unsigned char* 形式
    unsigned char *key_unc_encrypted = new unsigned char[length];
    for (std::size_t i = 0; i < length; i++) {
        key_unc_encrypted[i] = key_encrypted[i];
    }
    unsigned char *key_unc = new unsigned char[length];
    std::fill_n(key_unc, length, 0);
    if (masterKey == NULL) {
    }
    auto status = SDF_OpenSession(p_dev_handle_,
                                  &p_ses_handle);  //打开会话句柄
    if (status) {
        BOOST_LOG_TRIVIAL(error) << "Error code: 0x" << std::hex << status;
        BOOST_LOG_TRIVIAL(error) << "Hardware: Can't open a session.";
    }
    BOOST_LOG_TRIVIAL(debug) << "Hardware: Open a session";
    status = SDF_ImportKey(p_ses_handle, masterKey, 16,
                           &p_key_handle);  //导入明文主密钥
    if (status) {
        BOOST_LOG_TRIVIAL(error) << "Error code: 0x" << std::hex << status;
        BOOST_LOG_TRIVIAL(error) << "Hardware: Can't import the master key.";
    }
    BOOST_LOG_TRIVIAL(info) << "Hardware: Import the master key";
    status = SDF_Decrypt(p_ses_handle, p_key_handle, ui_alg_id_,
                         puc_iv, key_unc_encrypted, length,
                         key_unc, &length);  //调用对称加密接口进行解密运算
    if (status) {
        BOOST_LOG_TRIVIAL(error) << "Error code: 0x" << std::hex << status;
        BOOST_LOG_TRIVIAL(error) << "Hardware: Can't decrypt the key";
    }
    BOOST_LOG_TRIVIAL(info) << "Hardware: Decrypt the key using the master key";
    status = SDF_DestroyKey(p_ses_handle,
                            p_key_handle);  //销毁会话密钥
    if (status) {
        BOOST_LOG_TRIVIAL(error) << "Error code: 0x" << std::hex << status;
        BOOST_LOG_TRIVIAL(error) << "Hardware: Can't destroy the key handle";
    }
    status = SDF_CloseSession(p_ses_handle);  //关闭会话句柄
    if (status) {
        BOOST_LOG_TRIVIAL(error) << "Error code: 0x" << std::hex << status;
        BOOST_LOG_TRIVIAL(error) << "Hardware: Can't close the session";
    }
    BOOST_LOG_TRIVIAL(debug) << "Hardware: Close the session";
    KeyValueType key;
    for (std::size_t i = 0; i < length; i++) {
        key[i] = key_unc[i];
    }
    delete masterKey;
    delete key_unc;
    delete key_unc_encrypted;
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

