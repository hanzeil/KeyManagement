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
#include <boost/thread/thread.hpp>

namespace fs = std::experimental::filesystem;

std::tuple<std::vector<unsigned char>,
        std::vector<unsigned char>>
TestCreateKey(boost::asio::io_service &service, boost::asio::ip::tcp::endpoint &ep);

std::vector<unsigned char>
TestGetKeyById(boost::asio::io_service &service,
               boost::asio::ip::tcp::endpoint &ep,
               std::vector<unsigned char> &key_id);

void Authentication(boost::asio::ip::tcp::socket &sock);

/// 将数据封装成通信数据包
template<typename InputIterator>
inline DataPacket MakeDataPacket(uint32_t flag,
                                 InputIterator rand_begin,
                                 InputIterator rand_end,
                                 uint32_t len);

std::tuple<std::vector<unsigned char>,
        std::vector<unsigned char>>
ParseDataPacket(
        std::array<unsigned char, 8192> &array);

int main(int argc, char *argv[]) {
    int test_times = 1, test_interval = 0;
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " <test times [int]> <test interval(ms) [int]>" << std::endl;
        std::cout << "Example: " << argv[0] << " 10 1000" << std::endl;
        std::cout << "Default: " << argv[0] << " 1 0" << std::endl;
    }
    else {
        test_times = std::atoi(argv[1]);
        test_interval = std::atoi(argv[2]);
    }
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
    for (std::size_t i = 0; i < test_times; i++) {

        std::vector<unsigned char> key_id;
        std::vector<unsigned char> key;
        std::tie(key_id, key) = TestCreateKey(service, ep);
        std::cout << "CreateKey Test OK" << std::endl;
        std::cout << "key_id: ";
        for (auto item : key_id) {
            std::cout << std::hex << (int) item << " ";
        }
        std::cout << std::endl;
        std::cout << "key: ";
        for (auto item : key) {
            std::cout << std::hex << (int) item << " ";
        }
        std::cout << std::endl;
        TestGetKeyById(service, ep, key_id);
        std::cout << "GetKeyByID Test OK" << std::endl;
        std::cout << "key: ";
        for (auto item : key) {
            std::cout << std::hex << (int) item << " ";
        }
        std::cout << std::endl;
        usleep(test_interval * 1000);
    }
}

std::tuple<std::vector<unsigned char>,
        std::vector<unsigned char>>
TestCreateKey(boost::asio::io_service &service, boost::asio::ip::tcp::endpoint &ep) {
    boost::asio::ip::tcp::socket sock(service);
    sock.connect(ep);
    Authentication(sock);
    std::vector<unsigned char> vector_ignored(16);
    Reply reply;
    std::array<unsigned char, 8192> buffer;
    try {
        auto send = MakeDataPacket(
                0xaaaabbbb,
                vector_ignored.cbegin(),
                vector_ignored.cend(),
                1);
        reply.ToContent(send);
        sock.write_some(reply.ToBuffers());
        auto len = sock.read_some(boost::asio::buffer(buffer));
    }
    catch (std::runtime_error e) {
        std::cout << "The third "
                  << e.what()
                  << " error"
                  << std::endl;
        throw;
    }
    std::vector<unsigned char> key_id;
    std::vector<unsigned char> key;
    std::tie(key_id, key) =
            ParseDataPacket(buffer);
    sock.close();
    return std::make_tuple(key_id, key);
}

std::vector<unsigned char>
TestGetKeyById(boost::asio::io_service &service,
               boost::asio::ip::tcp::endpoint &ep,
               std::vector<unsigned char> &key_id) {
    boost::asio::ip::tcp::socket sock(service);
    sock.connect(ep);
    Authentication(sock);
    std::vector<unsigned char> vector_ignored(16);
    Reply reply;
    std::array<unsigned char, 8192> buffer;
    try {
        auto send = MakeDataPacket(
                0xaaaabbbb,
                key_id.cbegin(),
                key_id.cend(),
                2);
        reply.ToContent(send);
        sock.write_some(reply.ToBuffers());
        auto len = sock.read_some(boost::asio::buffer(buffer));
    }
    catch (std::runtime_error e) {
        std::cout << "The third "
                  << e.what()
                  << " error"
                  << std::endl;
        throw;
    }
    std::vector<unsigned char> key;
    std::tie(key_id, key) =
            ParseDataPacket(buffer);
    sock.close();
    return key;
}

void Authentication(boost::asio::ip::tcp::socket &sock) {
    Reply reply;
    Signature signature;
    std::size_t len;
    std::array<unsigned char, 8192> buffer;
    std::vector<unsigned char> rand_client;
    std::vector<unsigned char> rand_server;
    std::vector<unsigned char> cert_server;
    std::vector<unsigned char> cert_client;
    std::vector<unsigned char> signed_data_client;
    // 获取证书
    cert_client = signature.GetSignCert();
    // 客户端随机数
    std::random_device rd;
    for (std::size_t i = 0; i < 16; i++) {
        rand_client.push_back((unsigned char) rd());
    }
    try {

        // 第一次发送
        auto send_1 = MakeDataPacket(
                0xaa000000,
                rand_client.cbegin(),
                rand_client.cend(),
                (uint32_t) cert_client.size());
        reply.ToContent(send_1);
        reply.ToContent(cert_client.cbegin(),
                        cert_client.cend());
        sock.write_some(reply.ToBuffers());
        // 第一次接收
        len = sock.read_some(boost::asio::buffer(buffer));
        std::tie(rand_server, cert_server) =
                ParseDataPacket(buffer);
    }
    catch (std::runtime_error e) {
        std::cout << "The first "
                  << e.what()
                  << " error"
                  << std::endl;
        throw;
    }
    try {

        // 签名服务端的随机数
        signed_data_client = signature.SignData(rand_server);
        // 第二次发送
        reply.Reset();
        auto send_2 = MakeDataPacket(
                0xaabbcc00,
                rand_client.cbegin(),
                rand_client.cend(),
                (uint32_t) signed_data_client.size());
        reply.ToContent(send_2);
        reply.ToContent(signed_data_client.cbegin(),
                        signed_data_client.cend());
        sock.write_some(reply.ToBuffers());
        // 第二次接收
        sock.read_some(boost::asio::buffer(buffer));
    }
    catch (std::runtime_error e) {
        std::cout << "The second "
                  << e.what()
                  << " error"
                  << std::endl;
        throw;
    }
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

std::tuple<std::vector<unsigned char>,
        std::vector<unsigned char>>
ParseDataPacket(
        std::array<unsigned char, 8192> &array) {
    DataPacket data_packet;
    auto point = (unsigned char *) &data_packet;
    for (auto i = 0; i < sizeof(DataPacket); i++) {
        *point++ = array[i];
    }
    std::vector<unsigned char> result_1(16);
    std::vector<unsigned char> result_2(data_packet.len);
    for (auto i = 0; i < 16; i++) {
        result_1[i] = data_packet.rand[i];
    }
    for (auto i = 0; i < data_packet.len; i++) {
        result_2[i] = array[sizeof(DataPacket) + i];
    }
    return std::make_tuple(result_1, result_2);

};