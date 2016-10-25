//
// Created by Hanzeil on 16-9-5.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//


#ifndef KEYMANAGEMENT_REPLY_H
#define KEYMANAGEMENT_REPLY_H

#include <iostream>
#include <string>
#include <vector>
#include <boost/asio.hpp>
#include "../global/Key.h"

namespace tcp {


    struct Reply {
        /// The content to be sent in the reply.
        std::string content;

        void Reset();

        /// Convert the reply into buffer.
        boost::asio::const_buffers_1 ToBuffers();

        /// Conver the Key reply into string
        void ToContent(Key &);

        /// Conver the string reply into string
        void ToContent(std::string &);

        /// Conver the error reply into string
        void ErrorContent();

    };
}
#endif //KEYMANAGEMENT_REPLY_H
