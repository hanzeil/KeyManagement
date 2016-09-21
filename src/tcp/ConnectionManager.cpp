//
// Created by Hanzeil on 16-9-4.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//


#include "ConnectionManager.h"

namespace tcp {

    ConnectionManager::ConnectionManager() {
    }

    void ConnectionManager::Start(ConnectionPtr c) {
        connections_.insert(c);
        c->Start();
    }

    void ConnectionManager::Stop(ConnectionPtr c) {
        connections_.erase(c);
        c->Stop();
    }

    void ConnectionManager::StopAll() {
        for (auto c: connections_)
            c->Stop();
        connections_.clear();
    }

} // namespace tcp
