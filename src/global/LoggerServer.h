//
// Created by Hanzeil on 16-10-31.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//
// 日志服务器类，用于程序运行时动态修改日志的部分设置(filter)
//


#ifndef KEYMANAGEMENT_LOGGERSERVER_H
#define KEYMANAGEMENT_LOGGERSERVER_H


#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/date_time.hpp>
#include <string>
#include "Logger.h"
#include "../config/ConfigSingleton.h"

/// 该服务在一个子线程中运行
/// 负责接收客户端发来的请求, 从而修改日志属性
/// 另外设置了定时器，定时器的时间在配置文件中给出
/// 定时器的作用是定时检测日志文件的个数是否超过上限
/// Sample usage:
/// LoggerServer ls(address, log_server_port);
/// ls.Run();
class LoggerServer {
    typedef std::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;
public:
    LoggerServer(const LoggerServer &) = delete;

    LoggerServer &operator=(const LoggerServer &) = delete;

    /// Construct the server to listen on the specified TCP address and port, and
    /// serve up files from the given directory.
    explicit LoggerServer(const std::string &address, const std::string &port);

    /// Run the server's io_service loop.
    void Run();

private:
    /// Perform an asynchronous accept operation.
    void DoAccept();

    /// Wait for a request to stop the server.
    void DoAwaitStop();

    void DoRead(socket_ptr socket);

    void DoDeadlineTimer();

    /// The io_service used to perform asynchronous operations.
    boost::asio::io_service io_service_;

    /// The signal_set is used to register for process termination notifications.
    boost::asio::signal_set signals_;

    /// Acceptor used to listen for incoming connections.
    boost::asio::ip::tcp::acceptor acceptor_;

    boost::asio::deadline_timer deadline_timer_;

    std::size_t log_rotation_size_;

    std::size_t log_scan_duration_;

    std::size_t log_max_files_;

    std::string user_config_path_;

};


#endif //KEYMANAGEMENT_LOGGERSERVER_H
