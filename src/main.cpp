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
#include "config/ConfigSingleton.h"

using namespace std;

int main() {

    // 注释和代码优化

    try {

        // 从配置文件中读取日志服务端地址、端口 以及 服务端地址、端口 、线程数
        std::string port = config::ConfigSingleton::GetInstance().port_;
        std::string address = config::ConfigSingleton::GetInstance().address_;
        std::size_t num_threads = config::ConfigSingleton::GetInstance().num_threads_;
        std::string log_server_port = config::ConfigSingleton::GetInstance().log_server_port_;

        // 初始化log server
        LoggerServer ls(address, log_server_port);
        ls.Run();

        // 初始化密钥管理server
        tcp::Server s(address, port, num_threads);
        s.Run();
    }
    catch (std::exception &e) {
        LOG(FATAL) << e.what() << "\n";
    }
    return 0;
}