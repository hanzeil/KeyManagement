//
// Created by hanzeil on 16-9-4.
//
// Author hanzeil.
//

#ifndef KEYMANAGEMENT_HEADER_H
#define KEYMANAGEMENT_HEADER_H

#include <string>

namespace http {
    namespace server {

        struct header {
            std::string name;
            std::string value;
        };

    } // namespace server
} // namespace http

#endif //KEYMANAGEMENT_HEADER_H
