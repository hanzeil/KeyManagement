//
// Created by Hanzeil on 16-9-4.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//


#include "connection_manager.h"

namespace http {
    namespace server {

        connection_manager::connection_manager() {
        }

        void connection_manager::start(connection_ptr c) {
            connections_.insert(c);
            c->start();
        }

        void connection_manager::stop(connection_ptr c) {
            connections_.erase(c);
            c->stop();
        }

        void connection_manager::stop_all() {
            for (auto c: connections_)
                c->stop();
            connections_.clear();
        }

    } // namespace server
} // namespace http