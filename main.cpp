#include <iostream>
#include <fstream>
#include <iomanip>
#include <swsds.h>
#include <cstring>
#include "DBFactoryInterface.h"
#include "MysqlFactory.h"
#include "DBProductInterface.h"
#include "HardwareFactoryInterface.h"
#include "SJK1238Factory.h"
#include "HardwareProductInterface.h"
#include <chrono>

using namespace std;

#define MAX_BLOCK_LEN 100

int main() {
    unsigned char *key = NULL;
    unsigned char *key_encrypted = NULL;
    HardwareFactoryInterface *hFactory = new SJK1238Factory();
    HardwareProductInterface *hardware = hFactory->createProduct();
    if (hardware->openDevice()) {
        key = hardware->generateKey(16);
        for (auto i=0;i<16;i++){
            std::cout << (int)key[i] << " ";
        }
        key_encrypted = hardware->keyEncryption(key, 16);
    }
    Key k;
    k.generate_key(key_encrypted, 16);
    DBFactoryInterface *factory = new MysqlFactory();
    DBProductInterface *db = factory->createProduct();
    db->connect("keymanagement", "keymanagement");
    db->insertKey(k);
    Key *k2 = db->getKey(k.key_id_);
    unsigned char *key2 = hardware->keyDecryption(k2->key_value_, k2->key_value_len_);
    for (auto i=0;i<16;i++){
        std::cout << (int)key2[i] << " ";
    }
    return 0;
}