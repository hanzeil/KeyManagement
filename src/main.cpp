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
#include <glog/logging.h>
#include "global/Logger.h"
#include "global/LoggerServer.h"
#include "tcp/Server.h"

using namespace std;

int main() {

    // 注释和代码优化

    try {

        // 从配置文件中读取日志服务端地址、端口 以及 服务端地址、端口 、线程数
        Config config_settings;

        config_settings.ReadFile(config_settings.GetConifgPath(CONFIG_FILE_NAME));

        std::string port = config_settings.Read<std::string>("PORT");
        std::string address = config_settings.Read<std::string>("ADDRESS");
        std::string threads = config_settings.Read<std::string>("THREADS");
        std::string log_server_port = config_settings.Read<std::string>("LOG_SERVER_PORT");

        // 初始化log server
        LoggerServer ls(address, log_server_port);
        ls.Run();

        // 初始化密钥管理server
        size_t num_threads = boost::lexical_cast<std::size_t>(threads);
        tcp::Server s(address, port, num_threads);

        // Run the server until stopped.
        s.Run();
    }
    catch (std::exception &e) {
        LOG(FATAL) << e.what() << "\n";
    }
    return 0;
}