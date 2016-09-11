//
// Created by Hanzeil on 16-9-5.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//


#ifndef KEYMANAGEMENT_REPLY_H
#define KEYMANAGEMENT_REPLY_H

#include <string>
#include <boost/asio.hpp>

namespace http {
    namespace server {


        struct Reply {
            std::string method;

            /// The content to be sent in the reply.
            boost::asio::const_buffer content;

            /// Convert the reply into buffer.
            boost::asio::const_buffer to_buffer();
        };
    }
}
#endif //KEYMANAGEMENT_REPLY_H
