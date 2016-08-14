#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <chrono>
#include "config.h"
#include "database/DBFactoryInterface.h"
#include "database/DBProductInterface.h"
#include "hardware/HardwareFactoryInterface.h"
#include "hardware/HardwareProductInterface.h"

#ifdef MYSQL

#include "database/mysql/MysqlFactory.h"

#endif

#ifdef SJK1238

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

#ifdef SJK1238
    HardwareFactoryInterface *hFactory = new SJK1238Factory();
    HardwareProductInterface *hardware = hFactory->createProduct();
#endif

#ifdef SIMULATION
    HardwareFactoryInterface *hFactory = new SimulationFactory();
    HardwareProductInterface *hardware = hFactory->createProduct();
#endif

    if (hardware->openDevice()) {
        key = hardware->generateKey(16);
        /*
        for (auto i = 0; i < 16; i++) {
            std::cout << (int) key[i] << " ";
        }
         */
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
    return 0;
}