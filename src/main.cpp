//
// Created by Hanzeil on 16-8-15.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//
// 本文件是KeyManagement可执行程序的入口函数

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/exceptions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <iostream>
#include "config.h"
#include "database/DBFactoryInterface.h"
#include "encryption_device/EncryptionDeviceFactoryInterface.h"
#include "tcp/server.h"
#include <boost/asio.hpp>

#ifdef MYSQL

#include "database/mysql/MysqlFactory.h"

#endif

#ifdef SJK_1238

#include "encryption_device/sjk1238/SJK1238Factory.h"

#endif

#ifdef SIMULATION

#include "encryption_device/simulation/SimulationFactory.h"

#endif

using namespace std;

#define MAX_BLOCK_LEN 100

int main(int argc, char *argv[]) {
    try {
        // Check command line arguments.
        if (argc != 4) {
            std::cerr << "Usage: http_server <address> <port> <doc_root>\n";
            std::cerr << "  For IPv4, try:\n";
            std::cerr << "    receiver 0.0.0.0 80 .\n";
            std::cerr << "  For IPv6, try:\n";
            std::cerr << "    receiver 0::0 80 .\n";
            return 1;
        }

        // Initialise the server.
        http::server::server s(argv[1], argv[2], argv[3]);

        // Run the server until stopped.
        s.run();
    }
    catch (std::exception &e) {
        std::cerr << "exception: " << e.what() << "\n";
    }
    /*
    namespace logging = boost::log;
    logging::core::get()->set_filter(
            logging::trivial::severity >= logging::trivial::info);

    unsigned char *key = NULL;
    unsigned char *key_encrypted = NULL;

#ifdef SJK_1238
    EncrpytionDeviceFactoryInterface *hFactory = new SJK1238Factory();
    EncryptionDeviceProductInterface *hardware = hFactory->CreateProduct();
#endif

#ifdef SIMULATION
    EncrpytionDeviceFactoryInterface *hFactory = new SimulationFactory();
    EncryptionDeviceProductInterface *hardware = hFactory->CreateProduct();
#endif

    if (hardware->OpenDevice()) {
        key = hardware->GenerateKey(16);
        for (auto i = 0; i < 16; i++) {
            std::cout << (int) key[i] << " ";
        }
        std::cout << std::endl;
        key_encrypted = hardware->KeyEncryption(key, 16);
    }
    Key k(key_encrypted, 16);
    Key k3;
    k3 = k;
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
     */
    return 0;
}