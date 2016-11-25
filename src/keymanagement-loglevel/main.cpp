//
// Created by Hanzeil on 16-10-31.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//

#include "global_define.h"
#include <experimental/filesystem>
#include <iostream>
#include "../config/Config.h"
#include <boost/asio.hpp>

namespace fs = std::experimental::filesystem;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <loglevel [0/1/2/3]>\n", argv[0]);
        fprintf(stderr, "Example: keymanagement-loglevel 1\n");
        return 1;
    }

    std::string user_config_path;
    std::string global_config_path;

#ifdef NDEBUG
    user_config_path = std::string(getenv("HOME")) + "/." + PROJECT_NAME + "/";
    global_config_path = std::string("/etc/") + PROJECT_NAME + "/";
#else
    user_config_path = std::string(PROJECT_DIR) + "/config/";
    global_config_path = user_config_path;
#endif

    std::string config_path;
    fs::path fs_user_config_path, fs_global_config_path;
    fs_user_config_path.append(user_config_path + "keymanagement.conf");
    fs_global_config_path.append(global_config_path + "keymanagement.conf");
    if (fs::exists(fs_user_config_path)) {
        config_path = user_config_path + "keymanagement.conf";
    } else if (fs::exists(fs_global_config_path)) {
        config_path = global_config_path + "keymanagement.conf";
    } else {
        perror("NO CONFIG FILE");
        return 1;
    }

    Config config_settings(config_path);

    std::size_t log_server_port = config_settings.Read<std::size_t>("LOG_SERVER_PORT");

    std::array<char, 1> buffer;
    buffer[0] = argv[1][0];
    if (buffer[0] != '0' && buffer[0] != '1' && buffer[0] != '2' && buffer[0] != '3') {
        fprintf(stderr, "Log level error. Limit [0/1/2/3]");
        return 1;
    }

    boost::asio::io_service service;
    boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string("127.0.0.1"),
                                      (unsigned short) log_server_port);
    boost::asio::ip::tcp::socket sock(service);
    sock.connect(ep);
    sock.write_some(boost::asio::buffer(buffer));
    return 0;
}
