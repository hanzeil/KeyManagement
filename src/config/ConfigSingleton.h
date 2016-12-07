//
// Created by Hanzeil on 16-12-1.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//
// 该类提供读取配置文件中所有参数的接口
//


#ifndef KEYMANAGEMENT_CONFIGSINGLETON_H
#define KEYMANAGEMENT_CONFIGSINGLETON_H

#include "Config.h"
#include "global_define.h"

namespace config {

    /// 该类使用单例模式保证只有一个实例的存在
    /// 保证只在第一次实例化此类的时候访问配置文件
    /// sample usage:
    /// auto port = ConfigSingleton::GetInstance().port_;
    class ConfigSingleton {
    public:

        /// 拷贝构造函数
        /// 阻止拷贝
        ConfigSingleton(ConfigSingleton &) = delete;

        /// 拷贝赋值函数
        /// 阻止赋值
        ConfigSingleton &operator=(ConfigSingleton &)= delete;

        /// 获取实例
        static ConfigSingleton &GetInstance();

        std::string address_;
        std::string port_;
        std::size_t port_int_;
        std::size_t num_threads_;
        std::string log_server_port_;
        std::size_t log_server_port_int_;
        std::size_t log_rotation_size_;
        std::size_t log_max_files_;
        std::size_t log_scan_duration_;
        std::string db_url_;
        std::string db_port_;
        std::size_t db_port_int_;
        std::string db_user_name_;
        std::string db_user_password_;
    private:
        ConfigSingleton();

    };

}

#endif //KEYMANAGEMENT_CONFIGSINGLETON_H
