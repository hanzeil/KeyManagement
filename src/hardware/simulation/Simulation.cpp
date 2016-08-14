//
// Created by hanzeil on 16-8-12.
//

#include "Simulation.h"

Simulation::~Simulation() {
    device_status = false;
    BOOST_LOG_TRIVIAL(info) << "Hardware: Close the device";
}

bool Simulation::openDevice() {
    device_status = true;
    BOOST_LOG_TRIVIAL(info) << "Hardware: Open device";
    return true;
}


unsigned char *Simulation::generateKey(unsigned int length) {
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

unsigned char *Simulation::keyEncryption(unsigned char *key, unsigned int length) {
    unsigned char puc_iv[AES_BLOCK_SIZE] = {0};
    auto master_key = getMasterKey();
    AES_KEY aes_encrpyt_key;
    if (!device_status) {
        BOOST_LOG_TRIVIAL(error) << "Hardware: Device is not opened yet";
        return NULL;
    }
    auto status = AES_set_encrypt_key(master_key, 128, &aes_encrpyt_key);
    if (status) {
        handleErrors();
        return NULL;
    }
    unsigned char *key_encrypted = new unsigned char[length];
    AES_cbc_encrypt(key, key_encrypted, length, &aes_encrpyt_key,
                    puc_iv, AES_ENCRYPT);
    BOOST_LOG_TRIVIAL(info) << "Hardware: Encrypt the key using the master key";
    return key_encrypted;
}

unsigned char *Simulation::keyDecryption(unsigned char *key_encrypted, unsigned int length) {
    unsigned char puc_iv[AES_BLOCK_SIZE] = {0};
    auto master_key = getMasterKey();
    AES_KEY aes_decrpyt_key;
    if (!device_status) {
        BOOST_LOG_TRIVIAL(error) << "Hardware: Device is not opened yet";
        return NULL;
    }
    auto status = AES_set_decrypt_key(master_key, 128, &aes_decrpyt_key);
    if (status) {
        handleErrors();
        return NULL;
    }
    unsigned char *key = new unsigned char[length];
    AES_cbc_encrypt(key_encrypted, key, length, &aes_decrpyt_key,
                    puc_iv, AES_DECRYPT);
    BOOST_LOG_TRIVIAL(info) << "Hardware: Decrypt the key using the master key";
    return key;

}

unsigned char *Simulation::getMasterKey() {
    if (!device_status) {
        BOOST_LOG_TRIVIAL(error) << "Hardware: Device is not opened yet";
        return NULL;
    }
    char *buffer = new char[16];
    std::ifstream f(
            "../../../MasterKey", std::ifstream::binary);
    f.read(buffer, 16);
    f.close();
    unsigned char *master_key = reinterpret_cast<unsigned char *>(buffer);
    BOOST_LOG_TRIVIAL(info) << "Hardware: Read the master key";
    return master_key;
}

void Simulation::handleErrors() {
    unsigned long errCode;
    while (errCode = ERR_get_error()) {
        char *err = ERR_error_string(errCode, NULL);
        BOOST_LOG_TRIVIAL(error) << "Hardware: " << err;
    }
}
