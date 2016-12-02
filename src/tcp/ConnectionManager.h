//
// Created by Hanzeil on 16-9-4.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//


#ifndef KEYMANAGEMENT_CONNECTIONMANAGER_H
#define KEYMANAGEMENT_CONNECTIONMANAGER_H


#include <unordered_set>
#include "Connection.h"

namespace tcp {

/// Manages open connections so that they may be cleanly stopped when the server
/// needs to shut down.
    class ConnectionManager {
    public:
        ConnectionManager(const ConnectionManager &) = delete;

        ConnectionManager &operator=(const ConnectionManager &) = delete;

        /// Construct a connection manager.
        ConnectionManager();

        /// Add the specified connection to the manager and start it.
        void Start(ConnectionPtr c, std::shared_ptr<ThreadTask> task);

        /// Stop the specified connection.
        void Stop(ConnectionPtr c);

        /// Stop all connections.
        void StopAll();

    private:
        /// The managed connections.
        std::unordered_set<ConnectionPtr> connections_;
    };

} // namespace tcp

#endif //KEYMANAGEMENT_ConnectionManager_H
