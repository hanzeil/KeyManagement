//
// Created by root on 16-11-24.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//
// 通信数据包格式
//
#ifndef KEYMANAGEMENT_DATA_PACKET_H_H
#define KEYMANAGEMENT_DATA_PACKET_H_H

namespace tcp {

    struct DataPacket {
        uint32_t flag;
        unsigned char rand[16];
        uint32_t len;
        uint32_t point_ignored;
    };
}

#endif //KEYMANAGEMENT_DATA_PACKET_H_H
