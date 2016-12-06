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

    void Reply::ToContent(DataPacket &data_packet) {
        auto begin = (unsigned char *) &data_packet;
        auto end = begin + sizeof(data_packet);
        content += std::string(begin, end);

    }

    boost::asio::const_buffers_1 Reply::ToBuffers() {
        return boost::asio::buffer(content);
    }

    void Reply::ErrorContent() {
        Reset();
        for (size_t i = 0; i < sizeof(DataPacket); i++) {
            content += (char)0;
        }
    }
}
