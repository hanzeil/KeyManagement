//
// Created by Hanzeil on 16-10-31.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//

#include <iostream>
#include <array>
#include <boost/asio.hpp>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <loglevel [0/1/2/3]>\n", argv[0]);
        fprintf(stderr, "Example: keymanagement-loglevel 1\n", argv[0]);
        return 1;
    }
    std::array<char, 1> buffer;
    buffer[0] = argv[1][0];
    if (buffer[0] != '0' && buffer[0] != '1' && buffer[0] != '2' && buffer[0] != '3') {
        fprintf(stderr, "Log level error. Limit [0/1/2/3]");
        return 1;
    }

    boost::asio::io_service service;
    boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string("127.0.0.1"), 6091);
    boost::asio::ip::tcp::socket sock(service);
    sock.connect(ep);
    sock.write_some(boost::asio::buffer(buffer));
    return 0;
}
