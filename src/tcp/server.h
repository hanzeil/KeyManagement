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
#include <boost/lexical_cast.hpp>
#include <string>
#include "connection.h"
#include "connection_manager.h"
#include "RequestHandler.h"

namespace http {
    namespace server {

/// The top-level class of the HTTP server.
        class server {
        public:
            server(const server &) = delete;

            server &operator=(const server &) = delete;

            /// Construct the server to listen on the specified TCP address and port, and
            /// serve up files from the given directory.
            explicit server(const std::string &address, const std::string &port,
                            const std::string &doc_root, size_t thread_pool_size);

            /// Run the server's io_service loop.
            void run();

        private:
            /// Perform an asynchronous accept operation.
            void do_accept();

            /// Wait for a request to stop the server.
            void do_await_stop();

            /// The io_service used to perform asynchronous operations.
            boost::asio::io_service io_service_;

            /// The signal_set is used to register for process termination notifications.
            boost::asio::signal_set signals_;

            /// Acceptor used to listen for incoming connections.
            boost::asio::ip::tcp::acceptor acceptor_;

            /// The connection manager which owns all live connections.
            connection_manager connection_manager_;

            /// The next connection.
            connection_ptr new_connection_;

            /// The handler for all incoming requests.
            RequestHandler request_handler_;

            /// the number of threads
            size_t thread_pool_size_;
        };

    } // namespace server
} // namespace http


#endif //KEYMANAGEMENT_SERVER_H
