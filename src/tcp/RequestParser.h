//
// Created by Hanzeil on 16-9-5.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//
// 接收请求流后的数据流的解析
//


#ifndef KEYMANAGEMENT_REQUESTPARSER_H
#define KEYMANAGEMENT_REQUESTPARSER_H

#include <iostream>
#include <cstring>
#include "DataPacket.h"
#include <tuple>
#include "Request.h"

namespace tcp {

    struct Request;

/// Parser for incoming requests.
    class RequestParser {
    public:
        /// Construct ready to parse the request method.
        RequestParser();

        /// Result of parse.
        enum ResultType {
            good, bad
        };

        /// Parse some data. The enum return value is good when a complete request has
        /// been parsed, bad if the data is invalid.
        /// The InputIterator return value indicates how much of the input
        /// has been consumed.
        template<typename InputIterator>
        std::tuple<ResultType, InputIterator> Parse(Request &req,
                                                    InputIterator begin,
                                                    InputIterator end) {

            std::size_t len = end - begin;
            DataPacket data_packet;
            if (len < sizeof(data_packet)) {
                return std::make_tuple(bad, begin);
            }
            std::memcpy(&data_packet, begin, sizeof(data_packet));
            //判断请求包的类型
            if (data_packet.flag != 0xaaaabbbb) {
                if (len - sizeof(data_packet) != data_packet.len ||
                    data_packet.len == 0) {
                    return std::make_tuple(bad, begin);
                }
                for (std::size_t i = sizeof(data_packet); i < len; i++) {
                    req.data.push_back(begin[i]);
                }
            }
            if (data_packet.flag == 0xaa000000) {
                req.method = "Authentication1";
            } else if (data_packet.flag == 0xaabbcc00) {
                req.method = "Authentication2";
            } else if (data_packet.flag == 0xaaaabbbb) {
                if (data_packet.len == 1) {
                    req.method = "CreateKey";
                } else if (data_packet.len == 2) {
                    req.method = "FindKeyByID";
                } else {
                    return std::make_tuple(bad, begin);
                }
            } else {
                return std::make_tuple(bad, begin);
            }
            req.rand = std::vector<unsigned char>
                    (data_packet.rand, data_packet.rand + 16);
            req.length = data_packet.len;
            return std::make_tuple(good, begin);
        };

    };
}
#endif //KEYMANAGEMENT_REQUESTPARSER_H
