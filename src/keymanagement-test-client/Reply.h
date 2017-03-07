//
// Created by Hanzeil on 16-9-5.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//
// 回复的处理类
//


#ifndef KEYMANAGEMENT_TEST_CLIENT_REPLY_H
#define KEYMANAGEMENT_TEST_CLIENT_REPLY_H

#include <iostream>
#include <string>
#include <vector>
#include <boost/asio.hpp>
#include "../global/Key.h"
#include "DataPacket.h"


class Reply {
public:
    /// The content to be sent in the reply.
    std::string content;

    void Reset();

    /// Convert the reply into buffer.
    boost::asio::const_buffers_1 ToBuffers();

    /// Conver the DataPacket reply into string
    void ToContent(DataPacket &);

    /// Conver the InputIterator reply into string
    template<typename InputIterator>
    void ToContent(InputIterator begin,
                   InputIterator end) {
        content += std::string(begin, end);
    }


    /// Conver the error reply into string
    void ErrorContent();

};

#endif //KEYMANAGEMENT_TEST_CLIENT_REPLY_H
