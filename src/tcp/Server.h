//
// Created by Hanzeil on 16-9-4.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//


#ifndef KEYMANAGEMENT_SERVER_H
#define KEYMANAGEMENT_SERVER_H

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <string>
#include <map>
#include "Connection.h"
#include "RequestHandler.h"
#include "ThreadTask.h"

namespace tcp {

    class Server {
    public:
        Server(const Server &) = delete;

        Server &operator=(const Server &) = delete;

        /// Construct the server to listen on the specified TCP address and port, and
        /// serve up files from the given directory.
        explicit Server(const std::string &address, const std::string &port,
                        size_t thread_pool_size);

        /// Run the server's io_service loop.
        void Run();

    private:
        /// Perform an asynchronous accept operation.
        void DoAccept();

        /// Wait for a request to stop the server.
        void DoAwaitStop();

        /// The io_service used to perform asynchronous operations.
        boost::asio::io_service io_service_;

        /// The signal_set is used to register for process termination notifications.
        boost::asio::signal_set signals_;

        /// Acceptor used to listen for incoming connections.
        boost::asio::ip::tcp::acceptor acceptor_;

        /// The next connection.
        ConnectionPtr new_connection_;

        /// the number of threads
        std::size_t thread_pool_size_;

        /// 每个线程均要执行的任务指针
        std::map<boost::thread::id, std::shared_ptr<ThreadTask>> thread_tasks_;

    };

} // namespace tcp


#endif //KEYMANAGEMENT_SERVER_H
