//
// Created by Hanzeil on 16-10-31.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//
// 测试客户端
//

#include "../config/ConfigSingleton.h"
#include "Signature.h"
#include "Reply.h"
#include "DataPacket.h"
#include <experimental/filesystem>
#include <string>
#include <iostream>
#include <random>
#include <boost/asio.hpp>

namespace fs = std::experimental::filesystem;

void TestCreateKey(boost::asio::io_service &service, boost::asio::ip::tcp::endpoint &ep);

void TestGetKeyById(boost::asio::io_service &service, boost::asio::ip::tcp::endpoint &ep);

/// 将数据封装成通信数据包
template<typename InputIterator>
inline DataPacket MakeDataPacket(uint32_t flag,
                                 InputIterator rand_begin,
                                 InputIterator rand_end,
                                 uint32_t len);

int main(int argc, char *argv[]) {

    //config
    std::string user_config_path;
    std::string global_config_path;

#ifdef NDEBUG
    user_config_path = std::string(getenv("HOME")) + "/." + PROJECT_NAME + "/";
    global_config_path = std::string("/etc/") + PROJECT_NAME + "/";
#else
    user_config_path = std::string(PROJECT_DIR) + "/config/";
    global_config_path = user_config_path;
#endif

    std::string config_path;
    fs::path fs_user_config_path, fs_global_config_path;
    fs_user_config_path.append(user_config_path + "keymanagement.conf");
    fs_global_config_path.append(global_config_path + "keymanagement.conf");
    if (fs::exists(fs_user_config_path)) {
        config_path = user_config_path + "keymanagement.conf";
    } else if (fs::exists(fs_global_config_path)) {
        config_path = global_config_path + "keymanagement.conf";
    } else {
        std::cout << "NO CONFIG FILE" << std::endl;
        return 1;
    }

    std::size_t portno = config::ConfigSingleton::GetInstance().port_int_;
    std::string address = config::ConfigSingleton::GetInstance().address_;

    boost::asio::io_service service;
    boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(address),
                                      (unsigned short) portno);
    TestCreateKey(service, ep);
    TestGetKeyById(service, ep);
}

void TestCreateKey(boost::asio::io_service &service, boost::asio::ip::tcp::endpoint &ep) {
    boost::asio::ip::tcp::socket sock(service);
    sock.connect(ep);
    Reply reply;
    Signature signature;
    std::array<unsigned char, 8192> buffer;
    // 获取证书
    auto cert = signature.GetSignCert();
    std::vector<unsigned char> rand_client;
    // 客户端随机数
    std::random_device rd;
    for (std::size_t i = 0; i < 16; i++) {
        rand_client.push_back((unsigned char) rd());
    }
    auto data_packet_1 = MakeDataPacket(
            0xaa000000,
            rand_client.cbegin(),
            rand_client.cend(),
            (uint32_t) cert.size());
    reply.ToContent(data_packet_1);
    reply.ToContent(cert.cbegin(),
                    cert.cend());
    sock.write_some(reply.ToBuffers());

    std::size_t len = sock.read_some(boost::asio::buffer(buffer));
    sock.close();
}

void TestGetKeyById(boost::asio::io_service &service, boost::asio::ip::tcp::endpoint &ep) {
    boost::asio::ip::tcp::socket sock(service);
    sock.connect(ep);
    sock.close();
}

/// 将数据封装成通信数据包
template<typename InputIterator>
inline DataPacket MakeDataPacket(uint32_t flag,
                                 InputIterator rand_begin,
                                 InputIterator rand_end,
                                 uint32_t len) {

    DataPacket data_packet;
    data_packet.flag = flag;
    for (auto i = 0; i < 16 && rand_begin != rand_end; i++) {
        data_packet.rand[i] = *rand_begin++;
    }
    data_packet.len = len;
    data_packet.point_ignored;
    return data_packet;
}