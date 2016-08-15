//
// Created by Hanzeil on 16-8-15.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//
// 本文件是KeyManagement可执行程序的入口函数



#include <iostream>
#include "config.h"
#include "database/DBFactoryInterface.h"
#include "hardware/HardwareFactoryInterface.h"

#ifdef MYSQL

#include "database/mysql/MysqlFactory.h"

#endif

#ifdef SJK_1238

#include "hardware/sjk1238/SJK1238Factory.h"

#endif

#ifdef SIMULATION

#include "hardware/simulation/SimulationFactory.h"

#endif

using namespace std;

#define MAX_BLOCK_LEN 100

int main() {

    unsigned char *key = NULL;
    unsigned char *key_encrypted = NULL;

#ifdef SJK_1238
    HardwareFactoryInterface *hFactory = new SJK1238Factory();
    HardwareProductInterface *hardware = hFactory->CreateProduct();
#endif

#ifdef SIMULATION
    HardwareFactoryInterface *hFactory = new SimulationFactory();
    HardwareProductInterface *hardware = hFactory->CreateProduct();
#endif

    if (hardware->OpenDevice()) {
        key = hardware->GenerateKey(16);
        for (auto i = 0; i < 16; i++) {
            std::cout << (int) key[i] << " ";
        }
        key_encrypted = hardware->KeyEncryption(key, 16);
    }
    Key k(key_encrypted, 16);
    DBFactoryInterface *factory = new MysqlFactory();
    DBProductInterface *db = factory->CreateProduct();
    db->Connect("keymanagement", "keymanagement");
    db->InsertKey(k);
    Key *k2 = db->GetKey(k.key_id_);
    unsigned char *key2 = hardware->KeyDecryption(k2->key_value_, k2->key_value_len_);
    for (auto i = 0; i < 16; i++) {
        std::cout << (int) key2[i] << " ";
    }
    delete k2;
    return 0;
}