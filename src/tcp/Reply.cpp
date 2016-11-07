//
// Created by Hanzeil on 16-9-5.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//
// The description of this file is in the header file.
//


#include "Reply.h"

namespace tcp {
    void Reply::Reset() {
        content.clear();
    }

    void Reply::ToContent(Key &key) {
        content += std::string(key.key_id_.cbegin(),
                               key.key_id_.cend());
        content += std::string(key.key_value_.cbegin(),
                               key.key_value_.cend());

    }

    void Reply::ToContent(std::string &str) {
        content += str;
    }

    boost::asio::const_buffers_1 Reply::ToBuffers() {
        return boost::asio::buffer(content);
    }

    void Reply::ErrorContent() {
        content = "0";
    }
}
