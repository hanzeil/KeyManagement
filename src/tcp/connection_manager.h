//
// Created by Hanzeil on 16-9-4.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//


#ifndef KEYMANAGEMENT_CONNECTION_MANAGER_H
#define KEYMANAGEMENT_CONNECTION_MANAGER_H


#include <set>
#include "connection.h"

namespace http {
    namespace server {

/// Manages open connections so that they may be cleanly stopped when the server
/// needs to shut down.
        class connection_manager
        {
        public:
            connection_manager(const connection_manager&) = delete;
            connection_manager& operator=(const connection_manager&) = delete;

            /// Construct a connection manager.
            connection_manager();

            /// Add the specified connection to the manager and start it.
            void start(connection_ptr c);

            /// Stop the specified connection.
            void stop(connection_ptr c);

            /// Stop all connections.
            void stop_all();

        private:
            /// The managed connections.
            std::set<connection_ptr> connections_;
        };

    } // namespace server
} // namespace http


#endif //KEYMANAGEMENT_CONNECTION_MANAGER_H
