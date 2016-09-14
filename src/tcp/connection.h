//
// Created by Hanzeil on 16-9-4.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//


#ifndef KEYMANAGEMENT_CONNECTION_H
#define KEYMANAGEMENT_CONNECTION_H

#include <iostream>
#include <array>
#include <memory>
#include <boost/asio.hpp>
#include "Reply.h"
#include "Request.h"
#include "RequestHandler.h"
#include "RequestParser.h"

namespace http {
    namespace server {

        class connection_manager;

/// Represents a single connection from a client.
        class connection
                : public std::enable_shared_from_this<connection> {
        public:
            connection(const connection &) = delete;

            connection &operator=(const connection &) = delete;

            /// Construct a connection with the given socket.
            explicit connection(boost::asio::io_service &io_service,
                                connection_manager &manager, RequestHandler &handler);

            /// Start the first asynchronous operation for the connection.
            void start();

            /// Stop all asynchronous operations associated with the connection.
            void stop();

            /// Socket for the connection.
            boost::asio::ip::tcp::socket socket_;

        private:
            /// Perform an asynchronous read operation.
            void do_read();

            /// Perform an asynchronous write operation.
            void do_write();

            /// Reset Request and Reply object
            void Reset();

            /// The manager for this connection.
            connection_manager &connection_manager_;

            /// The handler used to process the incoming request.
            RequestHandler &request_handler_;

            /// Buffer for incoming data.
            std::array<unsigned char, 8192> buffer_;

            /// The incoming request.
            Request request_;

            /// The parser for the incoming request.
            RequestParser request_parser_;

            /// The reply to be sent back to the client.
            Reply reply_;

        };

        typedef std::shared_ptr<connection> connection_ptr;

    } // namespace server
} // namespace http

#endif //KEYMANAGEMENT_CONNECTION_H
