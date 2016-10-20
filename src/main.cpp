//
// Created by Hanzeil on 16-8-15.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//
// 本文件是KeyManagement可执行程序的入口函数

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/exceptions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <iostream>
#include "tcp/Server.h"
#include <boost/asio.hpp>

#include "database/DBFactoryInterface.h"
#include "encryption_device/EncryptionDeviceFactoryInterface.h"

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

void init() {

#ifdef MYSQL
    auto dbfactory = std::make_shared<database::MysqlFactory>();
#endif
    auto db = dbfactory->CreateProduct();

    db->Connect("keymanagement", "keymanagement");
    db->InitTable();

}

int main(int argc, char *argv[]) {
    namespace logging = boost::log;
    logging::core::get()->set_filter(
            logging::trivial::severity >= logging::trivial::info);
    try {
        init();
        // Check command line arguments.
        if (argc != 4) {
            std::cerr << "Usage: Server <address> <port>\n";
            std::cerr << "    receiver 0.0.0.0 80 1\n";
            return 1;
        }

        // Initialise the server.
        size_t num_threads = boost::lexical_cast<std::size_t>(argv[3]);
        tcp::Server s(argv[1], argv[2], num_threads);

        // Run the server until stopped.
        s.Run();
    }
    catch (std::exception &e) {
        std::cerr << "exception: " << e.what() << "\n";
    }
    return 0;
}