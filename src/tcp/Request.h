//
// Created by Hanzeil on 16-9-5.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//


#ifndef KEYMANAGEMENT_REQUEST_H
#define KEYMANAGEMENT_REQUEST_H

#include <string>
#include <vector>

namespace http {
    namespace server {

/// A request received from a client.
        struct Request {
            std::string method;
            size_t length;
            std::vector<unsigned char> data;

            void Reset();
        };

    } // namespace server
}

#endif //KEYMANAGEMENT_REQUEST_H
