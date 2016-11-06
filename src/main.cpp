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


char getch() {
    /*#include <unistd.h>   //_getch*/
    /*#include <termios.h>  //_getch*/
    char buf = 0;
    struct termios old = {0};
    fflush(stdout);
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    printf("*");
    return buf;
}

int main() {

    // Config 的异常处理
    // 注释和代码优化

    try {

        Config config_settings;

        config_settings.ReadFile(config_settings.GetConifgPath(CONFIG_FILE_NAME));

        std::string port = config_settings.Read<std::string>("PORT");
        std::string address = config_settings.Read<std::string>("ADDRESS");
        std::string threads = config_settings.Read<std::string>("THREADS");
        std::string log_server_port = config_settings.Read<std::string>("LOG_SERVER_PORT");

        // Initialise the log server
        LoggerServer ls(address, log_server_port);
        ls.Run();

        // Initialise the tcp server.
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