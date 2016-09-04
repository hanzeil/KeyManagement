//
// Created by hanzeil on 16-9-4.
//
// Author hanzeil.
//

#ifndef KEYMANAGEMENT_REQUEST_H
#define KEYMANAGEMENT_REQUEST_H

#endif //KEYMANAGEMENT_REQUEST_H

#include <string>
#include <vector>
#include "header.h"

namespace http {
    namespace server {

/// A request received from a client.
        struct request {
            std::string method;
            std::string uri;
            int http_version_major;
            int http_version_minor;
            std::vector<header> headers;
        };

    } // namespace server
} // namespace http