//
// Created by root on 16-11-24.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//
// 通信数据包格式
//
#ifndef KEYMANAGEMENT_TEST_CLIENT_DATA_PACKET_H
#define KEYMANAGEMENT_TEST_CLIENT_DATA_PACKET_H


struct DataPacket {
    uint32_t flag;
    unsigned char rand[16];
    uint32_t len;
    uint32_t point_ignored;
};

#endif //KEYMANAGEMENT_TEST_CLIENT_DATA_PACKET_H
