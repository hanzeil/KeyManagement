//
// Created by Hanzeil on 16-8-15.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//
// 本文件是KeyManagement可执行程序的入口函数

#include <cstdlib>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <experimental/filesystem>
#include "global/Logger.h"
#include "global/LoggerServer.h"
#include "Config.h"
#include "tcp/Server.h"

using namespace std;

namespace fs = std::experimental::filesystem;

#define MAX_BLOCK_LEN 100

int main() {
    // need to do
    // add config parameter, such as log-scan time, log-server port
    // config as a module, for others executable program
    // forbid remote connect for log-server port

    std::string user_config_path = std::string(getenv("HOME")) + "/." + PROJECT_NAME + "/";
    std::string global_config_path = std::string("/etc/") + PROJECT_NAME + "/";

    std::string config_path;
    fs::path fs_user_config_path, fs_global_config_path;
    fs_user_config_path.append(user_config_path + "keymanagement.conf");
    fs_global_config_path.append(global_config_path + "keymanagement.conf");
    if (fs::exists(fs_user_config_path)) {
        config_path = user_config_path + "keymanagement.conf";
    } else if (fs::exists(fs_global_config_path)) {
        config_path = global_config_path + "keymanagement.conf";
    } else {
        LOG(FATAL) << "NO CONFIG FILE";
    }

    Config config_settings(config_path);

    std::string port = config_settings.Read<std::string>("PORT");
    std::string address = config_settings.Read<std::string>("ADDRESS");
    std::string threads = config_settings.Read<std::string>("THREADS");
    std::size_t log_rotation_size = config_settings.Read<std::size_t>("LOG_ROTATION_SIZE");
    std::size_t log_max_files = config_settings.Read<std::size_t>("LOG_MAX_FILES");

    try {
        // Initialise the log server
        LoggerServer ls(address, "6091", user_config_path);
        Logger::GetInstance().SetLogRotationSize(log_rotation_size);
        Logger::GetInstance().SetLogMaxFiles(log_max_files);
        ls.Run();

        // Initialise the tcp server.
        size_t num_threads = boost::lexical_cast<std::size_t>(threads);
        tcp::Server s(address, port, num_threads);

        // Run the server until stopped.
        s.Run();
    }
    catch (std::exception &e) {
        std::cerr << "exception: " << e.what() << "\n";
    }
    return 0;
}