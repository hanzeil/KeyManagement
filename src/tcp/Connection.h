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

namespace tcp {

    class ConnectionManager;

/// Represents a single connection from a client.
    class Connection
            : public std::enable_shared_from_this<Connection> {
    public:
        Connection(const Connection &) = delete;

        Connection &operator=(const Connection &) = delete;

        /// Construct a connection with the given socket.
        explicit Connection(boost::asio::io_service &io_service,
                            ConnectionManager &manager);

        /// Start the first asynchronous operation for the connection.
        void Start(std::shared_ptr<ThreadTask> task);

        /// Stop all asynchronous operations associated with the connection.
        void Stop();


        /// Socket for the connection.
        boost::asio::ip::tcp::socket socket_;

    private:
        /// Perform an asynchronous read operation.
        void DoRead();

        /// Perform an asynchronous write operation.
        void DoWrite();

        /// Reset Request and Reply object
        void Reset();

        /// The manager for this connection.
        ConnectionManager &connection_manager_;

        /// The handler used to process the incoming request.
        RequestHandler request_handler_;

        /// Buffer for incoming data.
        std::array<unsigned char, 8192> buffer_;

        /// The incoming request.
        Request request_;

        /// The parser for the incoming request.
        RequestParser request_parser_;

        /// The reply to be sent back to the client.
        Reply reply_;

    };

    typedef std::shared_ptr<Connection> ConnectionPtr;

} // namespace tcp

#endif //KEYMANAGEMENT_CONNECTION_H
