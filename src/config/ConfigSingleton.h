//
// Created by Hanzeil on 16-12-1.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//


#ifndef KEYMANAGEMENT_CONFIGSINGLETON_H
#define KEYMANAGEMENT_CONFIGSINGLETON_H

#include "Config.h"
#include "global_define.h"

namespace config {

    class ConfigSingleton {
    public:
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

        ConfigSingleton(ConfigSingleton &) = default;

        ConfigSingleton &operator=(ConfigSingleton &)= default;

        Config config_settings_;

    };

}

#endif //KEYMANAGEMENT_CONFIGSINGLETON_H
