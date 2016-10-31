//
// Created by Hanzeil on 16-10-31.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
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

class LoggerServer {
    typedef std::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;
public:
    LoggerServer(const LoggerServer &) = delete;

    LoggerServer &operator=(const LoggerServer &) = delete;

    /// Construct the server to listen on the specified TCP address and port, and
    /// serve up files from the given directory.
    explicit LoggerServer(const std::string &address, const std::string &port, std::string user_config_path);

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


    std::string user_config_path_;

};


#endif //KEYMANAGEMENT_LOGGERSERVER_H
